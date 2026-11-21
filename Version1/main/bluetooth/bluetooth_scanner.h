#ifndef BLUETOOTH_SCANNER_H
#define BLUETOOTH_SCANNER_H

#include "esp_err.h"

esp_err_t bluetooth_scanner_init(void);
esp_err_t bluetooth_scanner_start_scan(void);

#endif