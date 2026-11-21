# W-Tech Wireless Security Scanner — Version 1

Scanner passif Wi-Fi et Bluetooth Low Energy pour ESP32-WROOM-32, développé en C avec ESP-IDF v5.4.4.

## Fonctions

- Scan des points d'accès Wi-Fi : SSID, BSSID, canal, RSSI et type de sécurité.
- Découverte BLE passive : nom annoncé, adresse et RSSI.
- Estimation indicative de distance à partir du RSSI.
- Classification simple de la qualité du signal.
- Comptage des observations.
- Vérification de l'état de connexion Wi-Fi.
- Affichage dans le moniteur série.

Le programme ne se connecte pas aux réseaux détectés et n'effectue aucune action de perturbation.

## Matériel

- ESP32-WROOM-32 (module classique).
- Câble USB de données.
- Ordinateur avec Ubuntu et ESP-IDF v5.4.4.

## Compilation

Depuis le dossier du projet, après activation d'ESP-IDF :

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
