#include "serial_interface.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "freertos/FreeRTOS.h"

/* =====================================================
   INITIALISATION UART
   ===================================================== */

void serial_interface_init(void)
{
    esp_err_t result = uart_driver_install(
        UART_NUM_0,
        256,
        0,
        0,
        NULL,
        0
    );

    /*
     * ESP_ERR_INVALID_STATE signifie que le pilote
     * est déjà installé.
     */
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE)
    {
        printf("Erreur initialisation UART : %s\n",
               esp_err_to_name(result));
        return;
    }

    /* Utiliser le pilote UART pour les entrées/sorties */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);
}

/* =====================================================
   BANNIERE
   ===================================================== */

void serial_print_banner(void)
{
    printf("\n");

    printf(COLOR_CYAN COLOR_BOLD);

    printf("============================================\n");
    printf("       W-TECH WIRELESS SECURITY SCANNER     \n");
    printf("                 VERSION 1                  \n");
    printf("============================================\n");

    printf(COLOR_RESET);

    printf(COLOR_GREEN "SYSTEM STATUS : READY\n" COLOR_RESET);

    printf("HARDWARE : ESP32-WROOM-32\n");
    printf("INTERFACE : SERIAL TERMINAL\n");
    printf("MODE : PASSIVE SCANNING\n\n");
}

/* =====================================================
   AIDE
   ===================================================== */

void serial_print_help(void)
{
    printf("\n");

    printf(COLOR_CYAN COLOR_BOLD);
    printf("============== COMMAND CENTER ==============\n");
    printf(COLOR_RESET);

    printf("\nSYSTEM\n");

    printf(COLOR_GREEN "help" COLOR_RESET);
    printf("               : Afficher les commandes\n");

    printf(COLOR_GREEN "clear" COLOR_RESET);
    printf("              : Nettoyer le terminal\n");

    printf(COLOR_GREEN "version" COLOR_RESET);
    printf("            : Version du programme\n");

    printf(COLOR_GREEN "status" COLOR_RESET);
    printf("             : Etat du systeme\n");

    printf("\nWIFI\n");

    printf(COLOR_GREEN "wifi scan" COLOR_RESET);
    printf("         : Scanner les reseaux Wi-Fi\n");

    printf(COLOR_GREEN "wifi disconnect" COLOR_RESET);
    printf("  : Deconnecter l'ESP32 du Wi-Fi\n");

    printf("\nBLUETOOTH\n");

    printf(COLOR_GREEN "ble scan" COLOR_RESET);
    printf("          : Scanner les appareils BLE\n");

    printf("\nMONITORING\n");

    printf(COLOR_GREEN "devices" COLOR_RESET);
    printf("            : Afficher les appareils detectes\n");

    printf("\n");

    printf(COLOR_YELLOW
           "Les autres commandes seront ajoutees progressivement.\n"
           COLOR_RESET);

    printf(COLOR_CYAN
           "============================================\n"
           COLOR_RESET);
}

/* =====================================================
   MESSAGES
   ===================================================== */

void serial_print_status(const char *message)
{
    printf(COLOR_CYAN "[STATUS] " COLOR_RESET "%s\n", message);
}

void serial_print_error(const char *message)
{
    printf(COLOR_RED "[ERROR] " COLOR_RESET "%s\n", message);
}

void serial_print_success(const char *message)
{
    printf(COLOR_GREEN "[SUCCESS] " COLOR_RESET "%s\n", message);
}

/* =====================================================
   WIFI AFFICHAGE
   ===================================================== */

void serial_print_wifi_header(void)
{
    printf("\n");

    printf(COLOR_BLUE COLOR_BOLD);
    printf("=============== WIFI SCANNER ===============\n");
    printf(COLOR_RESET);

    printf("%-30s %-17s CH RSSI  SECURITY\n",
           "SSID", "BSSID");

    printf("---------------------------------------------------------------\n");
}

void serial_print_wifi_network(
    const char *ssid,
    const char *bssid,
    int channel,
    int rssi,
    const char *security,
    float distance_m)
{
    const char *signal_color;

    if (rssi >= -50)
        signal_color = COLOR_GREEN;
    else if (rssi >= -70)
        signal_color = COLOR_YELLOW;
    else
        signal_color = COLOR_RED;

    printf("%-30.30s %-17s %2d ",
           ssid, bssid, channel);

    printf("%s%4d%s ",
           signal_color, rssi, COLOR_RESET);

    printf("%-10s %.2f m\n",
           security, distance_m);
}

/* =====================================================
   BLUETOOTH AFFICHAGE
   ===================================================== */

void serial_print_ble_device(
    const char *name,
    const char *address,
    int rssi,
    float distance_m)
{
    const char *signal_color;

    if (rssi >= -50)
        signal_color = COLOR_GREEN;
    else if (rssi >= -70)
        signal_color = COLOR_YELLOW;
    else
        signal_color = COLOR_RED;

    printf(COLOR_MAGENTA "[BLE] " COLOR_RESET);

    printf("%-24.24s | %s | ",
           name, address);

    printf("%s%d dBm%s | %.2f m\n",
           signal_color, rssi, COLOR_RESET, distance_m);
}

/* =====================================================
   LECTURE DES COMMANDES
   ===================================================== */

void serial_read_command(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 2)
    {
        return;
    }

    static bool previous_was_cr = false;

    size_t index = 0;
    uint8_t character;

    memset(buffer, 0, buffer_size);

    while (1)
    {
        int received = uart_read_bytes(
            UART_NUM_0,
            &character,
            1,
            pdMS_TO_TICKS(100)
        );

        if (received <= 0)
        {
            continue;
        }

        /* Ignorer le LF après un CR */
        if (character == '\n' && previous_was_cr)
        {
            previous_was_cr = false;
            continue;
        }

        /* Entrée */
        if (character == '\r' || character == '\n')
        {
            previous_was_cr = (character == '\r');

            buffer[index] = '\0';

            printf("\n");
            fflush(stdout);

            return;
        }

        previous_was_cr = false;

        /* Retour arrière */
        if (character == 8 || character == 127)
        {
            if (index > 0)
            {
                index--;
                buffer[index] = '\0';

                printf("\b \b");
                fflush(stdout);
            }

            continue;
        }

        /* Caractères imprimables */
        if (character >= 32 && character <= 126)
        {
            if (index < buffer_size - 1)
            {
                buffer[index++] = (char)character;

                putchar(character);
                fflush(stdout);
            }
        }
    }
}

/* =====================================================
   NETTOYAGE TERMINAL
   ===================================================== */

void serial_clear(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}