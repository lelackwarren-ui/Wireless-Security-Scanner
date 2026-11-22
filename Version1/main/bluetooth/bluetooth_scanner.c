#include "bluetooth_scanner.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_device.h"

#include "distance_estimator.h"
#include "serial_interface.h"
#include "device_monitor.h"
#include "signal_monitor.h"
#include "logger.h"

#define BLE_SCAN_SECONDS 10

typedef enum {
    BLE_OUTPUT_NORMAL,
    BLE_OUTPUT_SIGNAL,
    BLE_OUTPUT_INTERFERENCE
} ble_output_mode_t;

static ble_output_mode_t output_mode = BLE_OUTPUT_NORMAL;

static int ble_total_devices = 0;
static int ble_strong_signals = 0;
static int ble_medium_signals = 0;
static int ble_weak_signals = 0;


/* Convertir une adresse BLE en texte */
static void format_address(
    const uint8_t *bda,
    char *out,
    size_t size
)
{
    snprintf(
        out,
        size,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        bda[0], bda[1], bda[2],
        bda[3], bda[4], bda[5]
    );
}


/* Callback principal Bluetooth */
static void ble_gap_callback(
    esp_gap_ble_cb_event_t event,
    esp_ble_gap_cb_param_t *param
)
{
    switch (event) {

        /* Les paramètres du scan sont configurés */
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:

            if (param->scan_param_cmpl.status == ESP_BT_STATUS_SUCCESS) {

                esp_err_t err = esp_ble_gap_start_scanning(
                    BLE_SCAN_SECONDS
                );

                if (err != ESP_OK) {
                    LOG_ERROR(
                        "Impossible de démarrer le scan BLE : %s",
                        esp_err_to_name(err)
                    );

                    output_mode = BLE_OUTPUT_NORMAL;
                }

            } else {

                LOG_ERROR("Configuration du scan BLE échouée.");

                output_mode = BLE_OUTPUT_NORMAL;
            }

            break;


        /* Réception des résultats du scan */
        case ESP_GAP_BLE_SCAN_RESULT_EVT:

            if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {

                int rssi = param->scan_rst.rssi;

                ble_total_devices++;

                if (rssi >= -60) {
                    ble_strong_signals++;
                }
                else if (rssi >= -80) {
                    ble_medium_signals++;
                }
                else {
                    ble_weak_signals++;
                }

                char address[18];
                char name[ESP_BLE_ADV_DATA_LEN_MAX + 1] = {0};

                format_address(
                    param->scan_rst.bda,
                    address,
                    sizeof(address)
                );

                uint8_t name_len = 0;

                uint8_t *name_data = esp_ble_resolve_adv_data(
                    param->scan_rst.ble_adv,
                    ESP_BLE_AD_TYPE_NAME_CMPL,
                    &name_len
                );

                if (name_data == NULL) {

                    name_data = esp_ble_resolve_adv_data(
                        param->scan_rst.ble_adv,
                        ESP_BLE_AD_TYPE_NAME_SHORT,
                        &name_len
                    );
                }

                if (name_data != NULL && name_len > 0) {

                    size_t copy_len = name_len < sizeof(name) - 1
                                    ? name_len
                                    : sizeof(name) - 1;

                    memcpy(name, name_data, copy_len);

                } else {

                    snprintf(
                        name,
                        sizeof(name),
                        "Appareil sans nom"
                    );
                }

                /* Affichage individuel sauf en mode interférences */
                if (output_mode != BLE_OUTPUT_INTERFERENCE) {

                    serial_print_ble_device(
                        name,
                        address,
                        rssi,
                        distance_estimate_m(rssi)
                    );
                }

                signal_monitor_report("BLE", name, rssi);

                device_monitor_add_ble();
            }

            /* Fin du scan */
            else if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT) {

                serial_print_success("Scan BLE terminé.");

                if (output_mode == BLE_OUTPUT_INTERFERENCE) {

                    printf("\n" COLOR_CYAN COLOR_BOLD);
                    printf("======= BLE RADIO OBSERVATION =======\n");
                    printf(COLOR_RESET);

                    printf(
                        "Appareils observés : %d\n",
                        ble_total_devices
                    );

                    printf(
                        COLOR_GREEN "Signaux forts   : %d\n" COLOR_RESET,
                        ble_strong_signals
                    );

                    printf(
                        COLOR_YELLOW "Signaux moyens  : %d\n" COLOR_RESET,
                        ble_medium_signals
                    );

                    printf(
                        COLOR_RED "Signaux faibles  : %d\n" COLOR_RESET,
                        ble_weak_signals
                    );

                    printf(
                        COLOR_YELLOW
                        "\nObservation des annonces BLE uniquement. "
                        "Ce résultat ne mesure pas directement les interférences.\n"
                        COLOR_RESET
                    );

                    printf("=====================================\n");
                }

                output_mode = BLE_OUTPUT_NORMAL;
            }

            break;


        default:
            break;
    }
}


/* Initialisation Bluetooth */
esp_err_t bluetooth_scanner_init(void)
{
    esp_err_t err;

    err = esp_bt_controller_mem_release(
        ESP_BT_MODE_CLASSIC_BT
    );

    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    esp_bt_controller_config_t bt_cfg =
        BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    err = esp_bt_controller_init(&bt_cfg);
    if (err != ESP_OK) {
        return err;
    }

    err = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (err != ESP_OK) {
        return err;
    }

    err = esp_bluedroid_init();
    if (err != ESP_OK) {
        return err;
    }

    err = esp_bluedroid_enable();
    if (err != ESP_OK) {
        return err;
    }

    err = esp_ble_gap_register_callback(ble_gap_callback);
    if (err != ESP_OK) {
        return err;
    }

    LOG_INFO("Scanner BLE initialisé.");

    return ESP_OK;
}


/* Fonction commune de démarrage */
static esp_err_t bluetooth_start_scan_with_mode(
    ble_output_mode_t mode
)
{
    ble_total_devices = 0;
    ble_strong_signals = 0;
    ble_medium_signals = 0;
    ble_weak_signals = 0;

    output_mode = mode;

    esp_ble_scan_params_t scan_params = {
        .scan_type = BLE_SCAN_TYPE_PASSIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
    };

    serial_print_status("Configuration du scan BLE passif...");

    esp_err_t err = esp_ble_gap_set_scan_params(
        &scan_params
    );

    if (err != ESP_OK) {
        output_mode = BLE_OUTPUT_NORMAL;
    }

    return err;
}


/* Scan BLE classique */
esp_err_t bluetooth_scanner_start_scan(void)
{
    return bluetooth_start_scan_with_mode(
        BLE_OUTPUT_NORMAL
    );
}


/* Analyse des niveaux de signal */
esp_err_t bluetooth_scanner_start_signal_scan(void)
{
    return bluetooth_start_scan_with_mode(
        BLE_OUTPUT_SIGNAL
    );
}


/* Observation de l'activité radio */
esp_err_t bluetooth_scanner_start_interference_scan(void)
{
    return bluetooth_start_scan_with_mode(
        BLE_OUTPUT_INTERFERENCE
    );
}