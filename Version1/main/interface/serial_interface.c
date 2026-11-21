#include "serial_interface.h"

#include <stdio.h>
#include <string.h>

void serial_interface_init(void)
{
    /* UART initialisé par ESP-IDF. */
}

void serial_print_banner(void)
{
    printf("\n");

    printf(COLOR_CYAN COLOR_BOLD);
    printf("============================================\n");
    printf("       W-TECH WIRELESS SECURITY SCANNER     \n");
    printf("                 VERSION 1                  \n");
    printf("============================================\n");
    printf(COLOR_RESET);

    printf(COLOR_GREEN);
    printf("SYSTEM STATUS : INITIALIZED\n");
    printf(COLOR_RESET);

    printf("HARDWARE : ESP32-WROOM-32\n");
    printf("INTERFACE : SERIAL MONITOR\n");
    printf("MODE : PASSIVE SCANNING\n\n");
}

void serial_print_help(void)
{
    printf("\n");

    printf(COLOR_CYAN COLOR_BOLD);
    printf("============== COMMAND CENTER ==============\n");
    printf(COLOR_RESET);

    printf(COLOR_GREEN "wifi scan" COLOR_RESET);
    printf("         : Scanner Wi-Fi\n");

    printf(COLOR_GREEN "wifi details <ID>" COLOR_RESET);
    printf(" : Details d'un reseau\n");

    printf(COLOR_GREEN "wifi monitor" COLOR_RESET);
    printf("      : Surveillance Wi-Fi\n");

    printf(COLOR_GREEN "wifi disconnect" COLOR_RESET);
    printf("  : Deconnexion Wi-Fi ESP32\n");

    printf(COLOR_GREEN "ble scan" COLOR_RESET);
    printf("          : Scanner Bluetooth LE\n");

    printf(COLOR_GREEN "ble details <ID>" COLOR_RESET);
    printf("  : Details d'un appareil\n");

    printf(COLOR_GREEN "ble disconnect" COLOR_RESET);
    printf("   : Deconnexion BLE ESP32\n");

    printf(COLOR_GREEN "devices" COLOR_RESET);
    printf("            : Appareils detectes\n");

    printf(COLOR_GREEN "connections" COLOR_RESET);
    printf("        : Connexions gerees\n");

    printf(COLOR_GREEN "status" COLOR_RESET);
    printf("             : Etat du systeme\n");

    printf(COLOR_GREEN "clear" COLOR_RESET);
    printf("              : Nettoyer le terminal\n");

    printf(COLOR_GREEN "help" COLOR_RESET);
    printf("               : Afficher les commandes\n");

    printf(COLOR_CYAN "=============================================\n");
    printf(COLOR_RESET);
}

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

void serial_read_command(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 2) {
        return;
    }

    size_t index = 0;
    int character;

    memset(buffer, 0, buffer_size);

    while (1)
    {
        /* Attendre la réception d'un caractère */
        character = getchar();

        /* Ignorer les caractères invalides */
        if (character == EOF) {
            continue;
        }

        /* Détecter la touche Entrée */
        if (character == '\r' || character == '\n')
        {
            if (index == 0) {
                continue;
            }

            putchar('\n');
            break;
        }

        /* Gérer la touche Retour arrière */
        if (character == 8 || character == 127)
        {
            if (index > 0)
            {
                index--;
                buffer[index] = '\0';
                printf("\b \b");
            }

            continue;
        }

        /* Ajouter le caractère à la commande */
        if (character >= 32 && character <= 126)
        {
            if (index < buffer_size - 1)
            {
                buffer[index++] = (char)character;
                putchar(character);
            }
        }
    }

    buffer[index] = '\0';
}