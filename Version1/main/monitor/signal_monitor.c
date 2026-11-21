#include "signal_monitor.h"
#include <stdio.h>

void signal_monitor_report(const char *technology, const char *name, int rssi)
{
    const char *quality = "Faible";
    if (rssi >= -55) quality = "Excellent";
    else if (rssi >= -67) quality = "Bon";
    else if (rssi >= -75) quality = "Moyen";

    printf("[SIGNAL] %s | %s | %d dBm | %s\n",
           technology, name, rssi, quality);
}