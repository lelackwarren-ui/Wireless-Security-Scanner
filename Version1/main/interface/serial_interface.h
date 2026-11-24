#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <stddef.h>

/* Couleurs ANSI */
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

/* Initialisation */
void serial_interface_init(void);

/* Affichage */
void serial_print_banner(void);
void serial_print_help(void);

void serial_print_status(const char *message);
void serial_print_error(const char *message);
void serial_print_success(const char *message);

void serial_print_wifi_header(void);

void serial_print_wifi_network(
    const char *ssid,
    const char *bssid,
    int channel,
    int rssi,
    const char *security,
    float distance_m
);

void serial_print_ble_device(
    const char *name,
    const char *address,
    int rssi,
    float distance_m
);

/* Gestion du terminal */
void serial_read_command(char *buffer, size_t buffer_size);
void serial_clear(void);

#endif