idf_component_register(
    SRCS
        "main.c"

        "wifi/wifi_scanner.c"

        "bluetooth/bluetooth_scanner.c"

        "connection/connection_manager.c"

        "estimator/distance_estimator.c"

        "interface/serial_interface.c"

        "logger/logger.c"

        "monitor/device_monitor.c"
        "monitor/signal_monitor.c"
        "monitor/connection_monitor.c"

    INCLUDE_DIRS
        "."
        "wifi"
        "bluetooth"
        "connection"
        "estimator"
        "interface"
        "logger"
        "monitor"

    REQUIRES
        esp_wifi
        esp_event
        esp_netif
        nvs_flash
        bt
)