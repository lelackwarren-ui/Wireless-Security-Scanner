#include "bluetooth_scanner.h"
#include <stdio.h>
#include <string.h>
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

static void format_address(const uint8_t *bda, char *out, size_t size)
{
    snprintf(out, size, "%02X:%02X:%02X:%02X:%02X:%02X",
             bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
}

static void ble_gap_callback(esp_gap_ble_cb_event_t event,
                             esp_ble_gap_cb_param_t *param)
{
    if (event == ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT) {
        if (param->scan_param_cmpl.status == ESP_BT_STATUS_SUCCESS) {
            esp_err_t err = esp_ble_gap_start_scanning(BLE_SCAN_SECONDS);
            if (err != ESP_OK) LOG_ERROR("Démarrage BLE impossible : %s", esp_err_to_name(err));
        } else {
            LOG_ERROR("Paramètres de scan BLE refusés.");
        }
    } else if (event == ESP_GAP_BLE_SCAN_RESULT_EVT) {
        if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
            char address[18];
            char name[ESP_BLE_ADV_DATA_LEN_MAX + 1] = {0};
            format_address(param->scan_rst.bda, address, sizeof(address));

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
                size_t copy_len = name_len < sizeof(name) - 1 ? name_len : sizeof(name) - 1;
                memcpy(name, name_data, copy_len);
            } else {
                snprintf(name, sizeof(name), "Appareil sans nom");
            }

            int rssi = param->scan_rst.rssi;
            serial_print_ble_device(name, address, rssi, distance_estimate_m(rssi));
            signal_monitor_report("BLE", name, rssi);
            device_monitor_add_ble();
        } else if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT) {
            serial_print_status("Scan BLE terminé.");
        }
    }
}

esp_err_t bluetooth_scanner_init(void)
{
    esp_err_t err = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt_cfg);
    if (err != ESP_OK) return err;

    err = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (err != ESP_OK) return err;

    err = esp_bluedroid_init();
    if (err != ESP_OK) return err;

    err = esp_bluedroid_enable();
    if (err != ESP_OK) return err;

    err = esp_ble_gap_register_callback(ble_gap_callback);
    if (err != ESP_OK) return err;

    LOG_INFO("Scanner BLE initialisé.");
    return ESP_OK;
}

esp_err_t bluetooth_scanner_start_scan(void)
{
    esp_ble_scan_params_t scan_params = {
        .scan_type = BLE_SCAN_TYPE_PASSIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE
    };

    serial_print_status("Configuration du scan BLE passif...");
    return esp_ble_gap_set_scan_params(&scan_params);
}