#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "esp_err.h"
#include <stdbool.h>

/* Type de fonction utilisée pour déconnecter un périphérique BLE */
typedef esp_err_t (*ble_disconnect_callback_t)(void);

/* Initialisation */
void connection_manager_init(void);

/* Mise à jour de l'état des connexions */
void connection_manager_wifi_connected(void);
void connection_manager_wifi_disconnected(void);

void connection_manager_ble_connected(void);
void connection_manager_ble_disconnected(void);

/* Enregistrement de la fonction de déconnexion BLE */
void connection_manager_set_ble_disconnect_callback(
    ble_disconnect_callback_t callback
);

/* Gestion des déconnexions */
esp_err_t connection_manager_disconnect_wifi(void);
esp_err_t connection_manager_disconnect_ble(void);

/* Affichage */
void connection_manager_print_status(void);
void connection_manager_init(void);

/* Vérification des états */
bool connection_manager_is_wifi_connected(void);
bool connection_manager_is_ble_connected(void);

#endif
