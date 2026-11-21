#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "serial_interface.h"
#include "wifi_scanner.h"
#include "bluetooth_scanner.h"
#include "device_monitor.h"
#include "connection_monitor.h"
#include "logger.h"
#include "connection_manager.h"

#define COMMAND_BUFFER_SIZE 64
#define BLE_SCAN_DURATION_MS 12000

static void process_command(const char *command, esp_err_t ble_status)
{
    /* Commande : help */
    if (strcmp(command, "help") == 0) {

        serial_print_help();
    }

    /* Commande : wifi scan */
    else if (strcmp(command, "wifi scan") == 0) {

        serial_print_status("Demarrage du scan Wi-Fi...");

        esp_err_t result = wifi_scanner_scan();

        if (result == ESP_OK) {
            serial_print_success("Scan Wi-Fi termine.");
        }
        else {
            printf(COLOR_RED "Erreur Wi-Fi : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* Commande : ble scan */
    else if (strcmp(command, "ble scan") == 0) {

        if (ble_status != ESP_OK) {
            serial_print_error("Bluetooth n'est pas disponible.");
            return;
        }

        serial_print_status("Demarrage du scan Bluetooth...");

        esp_err_t result = bluetooth_scanner_start_scan();

        if (result == ESP_OK) {

            printf(COLOR_YELLOW
                   "Recherche des appareils pendant 12 secondes...\n"
                   COLOR_RESET);

            vTaskDelay(pdMS_TO_TICKS(BLE_SCAN_DURATION_MS));

            serial_print_success("Scan Bluetooth termine.");
        }
        else {
            printf(COLOR_RED "Erreur BLE : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* Commande : wifi disconnect */
    else if (strcmp(command, "wifi disconnect") == 0) {

        serial_print_status("Demande de deconnexion Wi-Fi...");

        esp_err_t result = wifi_scanner_disconnect();

        if (result == ESP_OK) {
            serial_print_success("ESP32 deconnecte du reseau Wi-Fi.");
        }
        else {
            printf(COLOR_RED "Deconnexion impossible : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* Commande : status */
    else if (strcmp(command, "status") == 0) {

        serial_print_status("Etat de surveillance :");
        connection_monitor_report();
    }

    /* Commande : devices */
    else if (strcmp(command, "devices") == 0) {

        device_monitor_print_summary();
    }

    /* Commande : clear */
    else if (strcmp(command, "clear") == 0) {

        serial_clear();
        serial_print_banner();
    }

    /* Commande inconnue */
    else if (strlen(command) > 0) {

        serial_print_error("Commande inconnue.");
        printf("Tapez " COLOR_YELLOW "help" COLOR_RESET
               " pour afficher les commandes.\n");
    }
}

void app_main(void)
{
    char command[64];

    serial_interface_init();

    serial_print_banner();

    connection_manager_init();

    wifi_scanner_init();
    bluetooth_scanner_init();

    serial_print_help();

    while (1)
{
    memset(command, 0, sizeof(command));

    printf("\n" COLOR_GREEN "W-TECH@ESP32"
           COLOR_RESET ":~$ ");

    serial_read_command(command, sizeof(command));

    if (strlen(command) > 0)
    {
        process_command(command, bluetooth_scanner_start_scan());
    }
}
}