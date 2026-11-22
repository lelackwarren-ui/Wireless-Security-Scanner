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
#define BLE_SCAN_DURATION_MS 11000

static void run_ble_scan(esp_err_t result)
{
    if (result != ESP_OK) {
        printf(COLOR_RED "Erreur BLE : %s\n" COLOR_RESET,
               esp_err_to_name(result));
        return;
    }

    printf(COLOR_YELLOW
           "Observation des appareils pendant 10 secondes...\n"
           COLOR_RESET);

    vTaskDelay(pdMS_TO_TICKS(BLE_SCAN_DURATION_MS));
}

static void process_command(
    const char *command,
    esp_err_t wifi_status,
    esp_err_t ble_status
)
{
    if (strcmp(command, "help") == 0) {
        serial_print_help();
    }

    /* WIFI SCAN */
    else if (strcmp(command, "wifi scan") == 0) {

        if (wifi_status != ESP_OK) {
            serial_print_error("Scanner Wi-Fi indisponible.");
            return;
        }

        serial_print_status("Démarrage du scan Wi-Fi...");

        esp_err_t result = wifi_scanner_scan();

        if (result == ESP_OK) {
            serial_print_success("Scan Wi-Fi terminé.");
        } else {
            printf(COLOR_RED "Erreur Wi-Fi : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* WIFI SIGNAL */
    else if (strcmp(command, "wifi signal") == 0) {

        if (wifi_status != ESP_OK) {
            serial_print_error("Scanner Wi-Fi indisponible.");
            return;
        }

        serial_print_status("Analyse des niveaux de signal Wi-Fi...");

        esp_err_t result = wifi_scanner_scan();

        if (result == ESP_OK) {
            wifi_scanner_print_signal();
        } else {
            serial_print_error("Impossible d'analyser les signaux Wi-Fi.");
        }
    }

    /* WIFI INTERFERENCE */
    else if (strcmp(command, "wifi interference") == 0) {

        if (wifi_status != ESP_OK) {
            serial_print_error("Scanner Wi-Fi indisponible.");
            return;
        }

        serial_print_status("Analyse de la densité des canaux Wi-Fi...");

        esp_err_t result = wifi_scanner_scan();

        if (result == ESP_OK) {
            wifi_scanner_analyze_interference();
        } else {
            serial_print_error("Analyse Wi-Fi impossible.");
        }
    }

    /* BLUETOOTH SCAN */
    else if (strcmp(command, "ble scan") == 0) {

        if (ble_status != ESP_OK) {
            serial_print_error("Bluetooth indisponible.");
            return;
        }

        serial_print_status("Démarrage du scan Bluetooth...");

        esp_err_t result = bluetooth_scanner_start_scan();

        run_ble_scan(result);
    }

    /* BLE SIGNAL */
    else if (strcmp(command, "ble signal") == 0) {

        if (ble_status != ESP_OK) {
            serial_print_error("Bluetooth indisponible.");
            return;
        }

        serial_print_status("Analyse des niveaux de signal BLE...");

        esp_err_t result =
            bluetooth_scanner_start_signal_scan();

        run_ble_scan(result);
    }

    /* BLE INTERFERENCE */
    else if (strcmp(command, "ble interference") == 0) {

        if (ble_status != ESP_OK) {
            serial_print_error("Bluetooth indisponible.");
            return;
        }

        serial_print_status("Observation de l'activité radio BLE...");

        esp_err_t result =
            bluetooth_scanner_start_interference_scan();

        run_ble_scan(result);
    }

    /* WIFI DISCONNECT */
    else if (strcmp(command, "wifi disconnect") == 0) {

        serial_print_status("Demande de déconnexion Wi-Fi...");

        esp_err_t result = wifi_scanner_disconnect();

        if (result == ESP_OK) {
            serial_print_success("ESP32 déconnecté du Wi-Fi.");
        } else {
            printf(COLOR_RED "Déconnexion impossible : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* STATUS */
    else if (strcmp(command, "status") == 0) {

        serial_print_status("État de surveillance :");
        connection_monitor_report();
    }

    /* DEVICES */
    else if (strcmp(command, "devices") == 0) {

        device_monitor_print_summary();
    }

    /* CONNECTIONS */
    else if (strcmp(command, "connections") == 0) {

        serial_print_status("Gestion des connexions.");
        connection_monitor_report();
    }

    /* CLEAR */
    else if (strcmp(command, "clear") == 0) {

        serial_clear();
        serial_print_banner();
    }

    /* UNKNOWN COMMAND */
    else if (strlen(command) > 0) {

        serial_print_error("Commande inconnue.");

        printf("Tapez " COLOR_YELLOW "help" COLOR_RESET
               " pour afficher les commandes.\n");
    }
}

void app_main(void)
{
    char command[COMMAND_BUFFER_SIZE];

    serial_interface_init();

    serial_clear();
    serial_print_banner();

    /* Initialisation Wi-Fi une seule fois */
    esp_err_t wifi_status = wifi_scanner_init();

    if (wifi_status == ESP_OK) {
        serial_print_success("Scanner Wi-Fi initialisé.");
    } else {
        serial_print_error("Initialisation Wi-Fi échouée.");
    }

    /* Initialisation Bluetooth une seule fois */
    esp_err_t ble_status = bluetooth_scanner_init();

    if (ble_status == ESP_OK) {
        serial_print_success("Scanner Bluetooth initialisé.");
    } else {
        serial_print_error("Initialisation Bluetooth échouée.");
    }

    printf("\nTapez " COLOR_YELLOW "help" COLOR_RESET
           " pour afficher les commandes.\n");

    /* TERMINAL PRINCIPAL */
    while (1) {

        printf("\n" COLOR_GREEN
               "W-TECH@ESP32"
               COLOR_RESET ":~$ ");

        fflush(stdout);

        serial_read_command(command, sizeof(command));

        if (strlen(command) > 0) {

            process_command(
                command,
                wifi_status,
                ble_status
            );
        }
    }
}