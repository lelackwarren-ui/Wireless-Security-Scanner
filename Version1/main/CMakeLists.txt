idf_component_register(
    SRCS
        "main.c"

        # Interface série
        "interface/serial_interface.c"

        # Scanner Wi-Fi
        "wifi/wifi_scanner.c"

        # Scanner Bluetooth
        "bluetooth/bluetooth_scanner.c"

        # Gestion des connexions
        "connection/connection_manager.c"

        # Estimation de distance
        "estimator/distance_estimator.c"

        # Moniteurs
        "monitor/device_monitor.c"
        "monitor/connection_monitor.c"
        "monitor/signal_monitor.c"

        # Logger
        "logger/logger.c"

    INCLUDE_DIRS
        "."
        "interface"
        "wifi"
        "bluetooth"
        "connection"
        "estimator"
        "monitor"
        "logger"

    PRIV_REQUIRES
    esp_driver_uart
    esp_wifi
    bt
    nvs_flash
    vfs
)