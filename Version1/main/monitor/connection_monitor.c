#include "connection_monitor.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "serial_interface.h"

void connection_monitor_report(void)
{
    wifi_ap_record_t ap_info = {0};
    esp_err_t result = esp_wifi_sta_get_ap_info(&ap_info);

    if (result == ESP_OK) {
        serial_print_status("Station Wi-Fi connectée à un point d'accès.");
    } else if (result == ESP_ERR_WIFI_NOT_CONNECT) {
        serial_print_status("Aucune connexion Wi-Fi active (mode scan passif).");
    } else {
        serial_print_status("État de connexion Wi-Fi indisponible.");
    }
}