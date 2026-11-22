#include "wifi_scanner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "distance_estimator.h"
#include "serial_interface.h"
#include "device_monitor.h"
#include "signal_monitor.h"
#include "logger.h"

#define WIFI_MAX_NETWORKS 30

static wifi_network_t networks[WIFI_MAX_NETWORKS];
static int network_count = 0;

static const char *wifi_auth_name(wifi_auth_mode_t auth)
{
    switch (auth) {
        case WIFI_AUTH_OPEN: return "Ouvert";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/3";
        default: return "Inconnue";
    }
}

esp_err_t wifi_scanner_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) return err;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    err = esp_wifi_init(&cfg);
    if (err != ESP_OK) return err;

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) return err;

    err = esp_wifi_start();
    if (err != ESP_OK) return err;

    LOG_INFO("Scanner Wi-Fi initialisé.");
    return ESP_OK;
}

esp_err_t wifi_scanner_scan(void)
{
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_PASSIVE
    };

    serial_print_status("Début du scan Wi-Fi...");
    esp_err_t err = esp_wifi_scan_start(&scan_config, true);
    if (err != ESP_OK) {
        LOG_ERROR("Échec du scan Wi-Fi : %s", esp_err_to_name(err));
        return err;
    }

       uint16_t count = 0;
    err = esp_wifi_scan_get_ap_num(&count);

    if (err != ESP_OK) {
        return err;
    }

    if (count == 0) {
        network_count = 0;
        serial_print_status("Aucun réseau Wi-Fi détecté.");
        return ESP_OK;
    }

    uint16_t limit = count > WIFI_MAX_NETWORKS
                   ? WIFI_MAX_NETWORKS
                   : count;

    wifi_ap_record_t *records = calloc(limit, sizeof(wifi_ap_record_t));

    if (records == NULL) {
        return ESP_ERR_NO_MEM;
    }

    uint16_t found = limit;
    err = esp_wifi_scan_get_ap_records(&found, records);

    if (err == ESP_OK) {

        network_count = found;

        serial_print_wifi_header();

        for (uint16_t i = 0; i < found; i++) {

            char bssid[18];

            snprintf(
                bssid,
                sizeof(bssid),
                MACSTR,
                MAC2STR(records[i].bssid)
            );

            const char *ssid = (const char *)records[i].ssid;

            snprintf(
                networks[i].ssid,
                sizeof(networks[i].ssid),
                "%s",
                ssid[0] ? ssid : "<SSID caché>"
            );

            snprintf(
                networks[i].bssid,
                sizeof(networks[i].bssid),
                "%s",
                bssid
            );

            networks[i].id = i;
            networks[i].channel = records[i].primary;
            networks[i].rssi = records[i].rssi;

            snprintf(
                networks[i].security,
                sizeof(networks[i].security),
                "%s",
                wifi_auth_name(records[i].authmode)
            );

            networks[i].distance =
                distance_estimate_m(records[i].rssi);

            serial_print_wifi_network(
                networks[i].ssid,
                networks[i].bssid,
                networks[i].channel,
                networks[i].rssi,
                networks[i].security,
                networks[i].distance
            );

            signal_monitor_report(
                "Wi-Fi",
                networks[i].ssid,
                networks[i].rssi
            );

            device_monitor_add_wifi();
        }
    }

    free(records);
    return err;
}


/* Nombre de réseaux détectés */
int wifi_scanner_get_count(void)
{
    return network_count;
}


/* Récupérer un réseau par son identifiant */
const wifi_network_t *wifi_scanner_get_network(int id)
{
    if (id < 0 || id >= network_count)
    {
        return NULL;
    }

    return &networks[id];
}


/* Afficher tous les réseaux enregistrés */
void wifi_scanner_print_networks(void)
{
    if (network_count == 0)
    {
        serial_print_status("Aucun réseau enregistré.");
        return;
    }

    serial_print_wifi_header();

    for (int i = 0; i < network_count; i++)
    {
        wifi_network_t *network = &networks[i];

        serial_print_wifi_network(
            network->ssid,
            network->bssid,
            network->channel,
            network->rssi,
            network->security,
            network->distance
        );
    }
}


/* Afficher les détails d'un réseau */
void wifi_scanner_print_details(int id)
{
    const wifi_network_t *network = wifi_scanner_get_network(id);

    if (network == NULL)
    {
        serial_print_error("Identifiant Wi-Fi invalide.");
        return;
    }

    printf("\n========== DETAILS WI-FI ==========\n");
    printf("ID       : %d\n", network->id);
    printf("SSID     : %s\n", network->ssid);
    printf("BSSID    : %s\n", network->bssid);
    printf("Canal    : %d\n", network->channel);
    printf("RSSI     : %d dBm\n", network->rssi);
    printf("Sécurité : %s\n", network->security);
    printf("Distance : %.2f m\n", network->distance);
    printf("===================================\n\n");
}


/* Déconnecter l'ESP32 de son réseau Wi-Fi */
esp_err_t wifi_scanner_disconnect(void)
{
    esp_err_t err = esp_wifi_disconnect();

    if (err == ESP_OK)
    {
        serial_print_success("ESP32 déconnecté du Wi-Fi.");
    }
    else
    {
        serial_print_error("Impossible de déconnecter le Wi-Fi.");
    }

    return err;
}

void wifi_scanner_print_signal(void)
{
    if (network_count == 0) {
        serial_print_status("Aucun résultat. Lancez un scan Wi-Fi.");
        return;
    }

    printf("\n" COLOR_CYAN COLOR_BOLD);
    printf("========== WIFI SIGNAL ANALYSIS ==========\n");
    printf(COLOR_RESET);

    for (int i = 0; i < network_count; i++) {

        const wifi_network_t *network = &networks[i];

        const char *color;

        if (network->rssi >= -50)
            color = COLOR_GREEN;
        else if (network->rssi >= -70)
            color = COLOR_YELLOW;
        else
            color = COLOR_RED;

        printf(
            "[%02d] %-25.25s | Canal: %2d | %s%d dBm%s\n",
            network->id,
            network->ssid,
            network->channel,
            color,
            network->rssi,
            COLOR_RESET
        );
    }

    printf("==========================================\n");
}

void wifi_scanner_analyze_interference(void)
{
    if (network_count == 0) {
        serial_print_status("Aucun résultat. Lancez un scan Wi-Fi.");
        return;
    }

    int channel_count[14] = {0};

    for (int i = 0; i < network_count; i++) {
        int channel = networks[i].channel;

        if (channel >= 1 && channel <= 13) {
            channel_count[channel]++;
        }
    }

    printf("\n" COLOR_CYAN COLOR_BOLD);
    printf("======= WIFI CHANNEL ANALYSIS =======\n");
    printf(COLOR_RESET);

    for (int channel = 1; channel <= 13; channel++) {

        if (channel_count[channel] == 0)
            continue;

        printf("Canal %2d : %d réseau(x) détecté(s) ",
               channel, channel_count[channel]);

        if (channel_count[channel] >= 4) {
            printf(COLOR_RED "[DENSE]\n" COLOR_RESET);
        }
        else if (channel_count[channel] >= 2) {
            printf(COLOR_YELLOW "[MODÉRÉ]\n" COLOR_RESET);
        }
        else {
            printf(COLOR_GREEN "[FAIBLE]\n" COLOR_RESET);
        }
    }

    printf("\n");
    printf(COLOR_YELLOW
           "Indicateur basé sur les réseaux détectés, "
           "pas une mesure directe des interférences radio.\n"
           COLOR_RESET);

    printf("=====================================\n");
}