#ifndef DEVICE_MONITOR_H
#define DEVICE_MONITOR_H

void device_monitor_reset(void);
void device_monitor_add_wifi(void);
void device_monitor_add_ble(void);
void device_monitor_print_summary(void);

#endif