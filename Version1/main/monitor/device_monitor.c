#include "device_monitor.h"
#include <stdio.h>

static unsigned int wifi_count;
static unsigned int ble_count;

void device_monitor_reset(void)
{
    wifi_count = 0;
    ble_count = 0;
}

void device_monitor_add_wifi(void) { wifi_count++; }
void device_monitor_add_ble(void) { ble_count++; }

void device_monitor_print_summary(void)
{
    printf("\n[DEVICES] Réseaux Wi-Fi détectés : %u | Appareils BLE observés : %u\n",
           wifi_count, ble_count);
}