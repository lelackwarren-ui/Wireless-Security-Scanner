#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "serial_interface.h"
#include "wifi_scanner.h"
#include "bluetooth_scanner.h"
#include "device_monitor.h"
#include "connection_monitor.h"

#define COMMAND_BUFFER_SIZE 64
#define BLE_SCAN_DURATION_MS 12000

static bool bluetooth_initialized = false;

/* =====================================================
   INTERPRETATION DES COMMANDES
   ===================================================== */

static void process_command(const char *command)
{
    /* HELP */
    if (strcmp(command, "help") == 0)
    {
        serial_print_help();
    }

    /* CLEAR */
    else if (strcmp(command, "clear") == 0)
    {
        serial_clear();
    }

    /* VERSION */
    else if (strcmp(command, "version") == 0)
    {
        printf("\nW-Tech Wireless Security Scanner\n");
        printf("Version : 1.0\n");
        printf("Hardware : ESP32-WROOM-32\n");
        printf("Firmware : ESP-IDF 5.4.4\n");
    }

    /* WIFI SCAN */
    else if (strcmp(command, "wifi scan") == 0)
    {
        serial_print_status("Demarrage du scan Wi-Fi...");

        esp_err_t result = wifi_scanner_scan();

        if (result == ESP_OK)
        {
            serial_print_success("Scan Wi-Fi termine.");
        }
        else
        {
            printf(COLOR_RED "Erreur Wi-Fi : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* WIFI DISCONNECT */
    else if (strcmp(command, "wifi disconnect") == 0)
    {
        serial_print_status("Deconnexion du reseau Wi-Fi...");

        esp_err_t result = wifi_scanner_disconnect();

        if (result == ESP_OK)
        {
            serial_print_success("ESP32 deconnecte du Wi-Fi.");
        }
        else
        {
            printf(COLOR_RED "Erreur : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* BLE SCAN */
    else if (strcmp(command, "ble scan") == 0)
    {
        /* Initialiser Bluetooth une seule fois */
        if (!bluetooth_initialized)
        {
            serial_print_status("Initialisation Bluetooth...");

            esp_err_t init_result = bluetooth_scanner_init();

            if (init_result != ESP_OK)
            {
                printf(COLOR_RED "Erreur Bluetooth : %s\n" COLOR_RESET,
                       esp_err_to_name(init_result));
                return;
            }

            bluetooth_initialized = true;
            serial_print_success("Bluetooth initialise.");
        }

        serial_print_status("Demarrage du scan BLE...");

        esp_err_t result = bluetooth_scanner_start_scan();

        if (result == ESP_OK)
        {
            printf(COLOR_YELLOW
                   "Recherche des appareils pendant 12 secondes...\n"
                   COLOR_RESET);

            vTaskDelay(pdMS_TO_TICKS(BLE_SCAN_DURATION_MS));

            serial_print_success("Scan Bluetooth termine.");
        }
        else
        {
            printf(COLOR_RED "Erreur BLE : %s\n" COLOR_RESET,
                   esp_err_to_name(result));
        }
    }

    /* STATUS */
    else if (strcmp(command, "status") == 0)
    {
        serial_print_status("Etat du systeme :");
        connection_monitor_report();
    }

    /* DEVICES */
    else if (strcmp(command, "devices") == 0)
    {
        device_monitor_print_summary();
    }

    /* EXIT */
    else if (strcmp(command, "exit") == 0)
    {
        printf(COLOR_YELLOW
               "Un terminal embarque ne peut pas etre ferme.\n"
               COLOR_RESET);
    }

    /* COMMANDE INCONNUE */
    else if (strlen(command) > 0)
    {
        serial_print_error("Commande inconnue.");

        printf("Tapez "
               COLOR_YELLOW "help" COLOR_RESET
               " pour afficher les commandes.\n");
    }
}

/* =====================================================
   PROGRAMME PRINCIPAL
   ===================================================== */

void app_main(void)
{
    char command[COMMAND_BUFFER_SIZE];

    /* Initialisation de l'interface série */
    serial_interface_init();

    /* Affichage initial */
    serial_clear();
    serial_print_banner();

    printf("Tapez 'help' pour afficher les commandes.\n");

    /* Boucle permanente du terminal */
    while (1)
    {
        printf("\n" COLOR_GREEN
               "W-TECH@ESP32" COLOR_RESET ":~$ ");

        fflush(stdout);

        /* Attendre une commande */
        serial_read_command(command, sizeof(command));

        /* Exécuter uniquement si une commande est saisie */
        if (command[0] != '\0')
        {
            process_command(command);
        }
    }
}