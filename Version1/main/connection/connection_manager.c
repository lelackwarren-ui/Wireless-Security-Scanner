#include "connection_manager.h"

#include <stdio.h>

#include "esp_wifi.h"
#include "esp_log.h"

#include "serial_interface.h"

/* Nom du module dans les logs ESP-IDF */
static const char *TAG = "CONNECTION_MANAGER";

/* État des connexions */
static bool wifi_connected = false;
static bool ble_connected = false;

/* Fonction de déconnexion BLE fournie par le module Bluetooth */
static ble_disconnect_callback_t ble_disconnect_callback = NULL;


/* =====================================================
   INITIALISATION
   ===================================================== */

void connection_manager_init(void)
{
    wifi_connected = false;
    ble_connected = false;

    ble_disconnect_callback = NULL;

    ESP_LOGI(TAG, "Connection manager initialized");

    serial_print_success("Connection manager initialized.");
}


/* =====================================================
   MISE À JOUR DE L'ÉTAT WI-FI
   ===================================================== */

void connection_manager_wifi_connected(void)
{
    wifi_connected = true;

    ESP_LOGI(TAG, "ESP32 connected to Wi-Fi");
}


void connection_manager_wifi_disconnected(void)
{
    wifi_connected = false;

    ESP_LOGI(TAG, "ESP32 disconnected from Wi-Fi");
}


/* =====================================================
   MISE À JOUR DE L'ÉTAT BLUETOOTH
   ===================================================== */

void connection_manager_ble_connected(void)
{
    ble_connected = true;

    ESP_LOGI(TAG, "ESP32 connected to a BLE device");
}


void connection_manager_ble_disconnected(void)
{
    ble_connected = false;

    ESP_LOGI(TAG, "ESP32 disconnected from BLE device");
}


/* =====================================================
   ENREGISTREMENT DU CALLBACK BLUETOOTH
   ===================================================== */

void connection_manager_set_ble_disconnect_callback(
    ble_disconnect_callback_t callback)
{
    ble_disconnect_callback = callback;

    ESP_LOGI(TAG, "BLE disconnect callback registered");
}


/* =====================================================
   DÉCONNEXION WI-FI
   ===================================================== */

esp_err_t connection_manager_disconnect_wifi(void)
{
    /*
     * Vérifier l'état réel de la connexion Wi-Fi.
     * ESP_OK signifie que l'ESP32 est connecté à un point d'accès.
     */
    wifi_ap_record_t ap_info;

    esp_err_t result = esp_wifi_sta_get_ap_info(&ap_info);

    if (result != ESP_OK)
    {
        wifi_connected = false;

        serial_print_status("ESP32 is not connected to Wi-Fi.");

        return result;
    }

    /* Déconnecter uniquement l'ESP32 */
    result = esp_wifi_disconnect();

    if (result == ESP_OK)
    {
        wifi_connected = false;

        serial_print_success("ESP32 disconnected from Wi-Fi.");
    }
    else
    {
        serial_print_error("Wi-Fi disconnection failed.");
        ESP_LOGE(TAG, "Wi-Fi disconnect error: %s",
                 esp_err_to_name(result));
    }

    return result;
}


/* =====================================================
   DÉCONNEXION BLUETOOTH
   ===================================================== */

esp_err_t connection_manager_disconnect_ble(void)
{
    /*
     * Vérifier si une connexion BLE existe.
     */
    if (!ble_connected)
    {
        serial_print_status("No active BLE connection.");

        return ESP_ERR_INVALID_STATE;
    }

    /*
     * Vérifier si le module Bluetooth a fourni
     * une fonction capable de déconnecter le périphérique.
     */
    if (ble_disconnect_callback == NULL)
    {
        serial_print_error(
            "BLE disconnection is not configured yet."
        );

        return ESP_ERR_NOT_SUPPORTED;
    }

    /*
     * Exécuter la fonction de déconnexion du module BLE.
     */
    esp_err_t result = ble_disconnect_callback();

    if (result == ESP_OK)
    {
        ble_connected = false;

        serial_print_success("BLE device disconnected.");
    }
    else
    {
        serial_print_error("BLE disconnection failed.");

        ESP_LOGE(TAG, "BLE disconnect error: %s",
                 esp_err_to_name(result));
    }

    return result;
}


/* =====================================================
   VÉRIFICATION DES CONNEXIONS
   ===================================================== */

bool connection_manager_is_wifi_connected(void)
{
    wifi_ap_record_t ap_info;

    /*
     * On interroge directement le pilote Wi-Fi
     * pour éviter de dépendre uniquement d'une variable.
     */
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK)
    {
        wifi_connected = true;
    }
    else
    {
        wifi_connected = false;
    }

    return wifi_connected;
}


bool connection_manager_is_ble_connected(void)
{
    return ble_connected;
}


/* =====================================================
   AFFICHAGE DE L'ÉTAT DES CONNEXIONS
   ===================================================== */

void connection_manager_print_status(void)
{
    bool wifi_status = connection_manager_is_wifi_connected();
    bool ble_status = connection_manager_is_ble_connected();

    printf("\n");
    printf("%s========== CONNECTION STATUS ==========%s\n",
           COLOR_BOLD, COLOR_RESET);

    if (wifi_status)
    {
        printf("%s[CONNECTED]%s Wi-Fi\n",
               COLOR_GREEN, COLOR_RESET);
    }
    else
    {
        printf("%s[DISCONNECTED]%s Wi-Fi\n",
               COLOR_RED, COLOR_RESET);
    }

    if (ble_status)
    {
        printf("%s[CONNECTED]%s Bluetooth LE\n",
               COLOR_GREEN, COLOR_RESET);
    }
    else
    {
        printf("%s[DISCONNECTED]%s Bluetooth LE\n",
               COLOR_RED, COLOR_RESET);
    }

    printf("%s=======================================%s\n\n",
           COLOR_BOLD, COLOR_RESET);
}
