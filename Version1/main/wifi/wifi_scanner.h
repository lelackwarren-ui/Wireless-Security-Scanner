#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include "esp_err.h"
#include "esp_wifi.h"

#define WIFI_MAX_NETWORKS 30

typedef struct
{
    int id;
    char ssid[33];
    char bssid[18];
    int channel;
    int rssi;
    char security[16];
    float distance;
} wifi_network_t;

/* Initialisation et scan */
esp_err_t wifi_scanner_init(void);
esp_err_t wifi_scanner_scan(void);

/* Affichage */
void wifi_scanner_print_networks(void);
void wifi_scanner_print_details(int id);

/* Récupération des réseaux */
int wifi_scanner_get_count(void);
const wifi_network_t *wifi_scanner_get_network(int id);

/* Déconnexion Wi-Fi de l'ESP32 */
esp_err_t wifi_scanner_disconnect(void);

#endif
