```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"

#define PROJECT_NAME    "W-TECH WIRELESS SECURITY SCANNER"
#define PROJECT_VERSION "1.0.0"
#define HARDWARE        "ESP32-WROOM-32"
#define DEVELOPER       "W-Tech"

static const char *TAG = "MAIN";

/*
 * Fonction : initialize_nvs()
 * Rôle : Initialiser la mémoire non volatile NVS.
 */
static esp_err_t initialize_nvs(void)
{
    esp_err_t result = nvs_flash_init();

    if (result == ESP_ERR_NVS_NO_FREE_PAGES ||
        result == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS recovery required.");

        ESP_ERROR_CHECK(nvs_flash_erase());

        result = nvs_flash_init();
    }

    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "NVS initialization failed: %s",
                 esp_err_to_name(result));

        return result;
    }

    ESP_LOGI(TAG, "NVS initialized successfully.");

    return ESP_OK;
}

/*
 * Fonction : display_project_info()
 * Rôle : Afficher les informations du projet.
 */
static void display_project_info(void)
{
    printf("\n");
    printf("========================================\n");
    printf("   %s\n", PROJECT_NAME);
    printf("========================================\n");
    printf("Developer : %s\n", DEVELOPER);
    printf("Version   : %s\n", PROJECT_VERSION);
    printf("Hardware  : %s\n", HARDWARE);
    printf("Framework : ESP-IDF\n");
    printf("Interface : Serial Monitor\n");
    printf("========================================\n\n");
}

/*
 * Fonction principale de l'application ESP-IDF.
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Starting application...");

    // PHASE 1 : Initialisation NVS
    if (initialize_nvs() != ESP_OK)
    {
        ESP_LOGE(TAG, "System startup aborted.");
        return;
    }

    // PHASE 2 : Affichage des informations
    display_project_info();

    // PHASE 3 : Confirmation du démarrage
    ESP_LOGI(TAG, "System initialized successfully.");

    printf("W-Tech Wireless Security Scanner is ready!\n");

    // PHASE 4 : Attente des prochaines fonctionnalités
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```