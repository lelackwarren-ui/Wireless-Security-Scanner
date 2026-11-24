# Wireless Security Scanner — Architecture logicielle

**Version :** 1.0  
**Langage :** C  
**Framework :** ESP-IDF  
**Cible :** ESP32-WROOM-32

## 1. Vue d'ensemble

Le programme est organisé en modules. Chaque module possède une responsabilité précise : interface série, scan Wi-Fi, scan BLE, estimation de distance, surveillance et journalisation.

L'objectif est de garder `main.c` comme point d'entrée et coordinateur du programme, sans y placer toute l'implémentation des fonctionnalités.

## 2. Organisation des fichiers

Arborescence de référence correspondant à l'organisation du projet :

```text
Wireless-Security-Scanner/
├── README.md
├── CMakeLists.txt
├── sdkconfig
├── docs/
│   ├── COMMANDS.md
│   ├── HARDWARE.md
│   ├── ARCHITECTURE.md
│   └── INSTALLATION.md
└── main/
    ├── CMakeLists.txt
    ├── main.c
    ├── interface/
    │   ├── serial_interface.c
    │   └── serial_interface.h
    ├── wifi/
    │   ├── wifi_scanner.c
    │   └── wifi_scanner.h
    ├── bluetooth/
    │   ├── bluetooth_scanner.c
    │   └── bluetooth_scanner.h
    ├── connection/
    │   ├── connection_manager.c
    │   └── connection_manager.h
    ├── estimator/
    │   ├── distance_estimator.c
    │   └── distance_estimator.h
    ├── logger/
    │   ├── logger.c
    │   └── logger.h
    └── monitor/
        ├── device_monitor.c
        ├── device_monitor.h
        ├── connection_monitor.c
        ├── connection_monitor.h
        ├── signal_monitor.c
        └── signal_monitor.h
```

L'arborescence réelle peut évoluer. Les fichiers listés doivent correspondre aux fichiers effectivement présents dans le dépôt.

## 3. Rôle des modules

| Module | Responsabilité |
|---|---|
| `main.c` | Initialise les modules, affiche le terminal, lit les commandes et les distribue |
| `interface/serial_interface` | Configure la liaison série, affiche les messages et lit les commandes utilisateur |
| `wifi/wifi_scanner` | Initialise le Wi-Fi et réalise les scans des réseaux accessibles |
| `bluetooth/bluetooth_scanner` | Initialise le contrôleur BLE et observe les annonces BLE |
| `connection/connection_manager` | Regroupe les fonctions de gestion de connexion réellement implémentées |
| `estimator/distance_estimator` | Estime une distance indicative à partir du RSSI |
| `logger/logger` | Centralise les messages de journalisation |
| `monitor/device_monitor` | Comptabilise ou résume les appareils observés |
| `monitor/connection_monitor` | Produit les rapports d'état des connexions |
| `monitor/signal_monitor` | Enregistre ou présente les informations de signal |

## 4. Cycle de démarrage

Au démarrage, `app_main()` suit le déroulement général ci-dessous :

1. Déclarer le tampon destiné à recevoir une commande.
2. Initialiser l'interface série.
3. Effacer l'affichage et présenter la bannière.
4. Initialiser le scanner Wi-Fi et mémoriser le résultat de l'initialisation.
5. Initialiser le scanner BLE et mémoriser le résultat de l'initialisation.
6. Afficher une indication invitant l'utilisateur à saisir `help`.
7. Entrer dans la boucle principale du terminal.

Les scans ne sont pas lancés automatiquement pendant cette phase.

## 5. Cycle de traitement d'une commande

```text
Démarrage
   |
Initialisation des modules
   |
Affichage de la bannière
   |
Affichage de l'invite W-TECH@ESP32:~$
   |
Lecture de la commande
   |
Commande reconnue ?
   |                 |
  Oui               Non
   |                 |
Exécuter la         Afficher une
fonction associée   erreur
   |                 |
Afficher le résultat/
le message d'erreur
   |
Retour à l'invite
   |
Attendre la commande suivante
```

Le traitement est conçu pour que les erreurs normales d'une commande ne ferment pas le terminal. Une erreur d'initialisation d'un module doit rendre indisponibles les commandes qui dépendent de ce module, sans empêcher nécessairement l'utilisation des autres fonctions.

## 6. Fonctionnement Wi-Fi

Le module Wi-Fi initialise la pile réseau et le pilote Wi-Fi nécessaires au scan. Lorsqu'une commande Wi-Fi est saisie :

1. `main.c` vérifie que l'initialisation Wi-Fi a réussi.
2. Le scanner lance une recherche des points d'accès.
3. Les résultats sont récupérés et stockés dans les structures prévues.
4. Les informations sont affichées ou analysées selon la commande demandée.
5. Le contrôle revient au terminal.

L'analyse de densité par canal est une estimation basée sur le nombre de réseaux détectés. Elle ne constitue pas une mesure radio complète des interférences.

## 7. Fonctionnement BLE

Le module BLE initialise le contrôleur Bluetooth et la pile Bluedroid/GAP utilisée par le projet.

Le lancement du scan configure les paramètres puis demande le démarrage du scan à la pile BLE. Les résultats arrivent de manière asynchrone dans la fonction de rappel GAP (*callback*).

La callback traite notamment :
- la fin de configuration des paramètres de scan ;
- la réception d'un résultat d'annonce ;
- la fin du scan.

Les données reçues peuvent être affichées ou utilisées pour mettre à jour les compteurs de signal et les statistiques du moniteur.

## 8. Communication entre les modules

`main.c` appelle les fonctions exposées par les fichiers d'en-tête (`.h`). Les détails internes restent dans les fichiers source (`.c`).

Exemple conceptuel :

```text
main.c
  ├── serial_interface
  │     ├── afficher la bannière
  │     ├── lire la commande
  │     └── afficher les résultats
  ├── wifi_scanner
  │     └── rechercher les réseaux Wi-Fi
  ├── bluetooth_scanner
  │     └── observer les annonces BLE
  ├── device_monitor
  ├── connection_monitor
  └── logger
```

Un fichier `.h` déclare les fonctions et types accessibles aux autres modules. Le fichier `.c` contient leur implémentation.

## 9. Compilation ESP-IDF

Le fichier `main/CMakeLists.txt` doit déclarer les fichiers source et les répertoires d'en-têtes réellement utilisés. Exemple de référence à adapter si l'arborescence change :

```cmake
idf_component_register(
    SRCS
        "main.c"
        "interface/serial_interface.c"
        "wifi/wifi_scanner.c"
        "bluetooth/bluetooth_scanner.c"
        "connection/connection_manager.c"
        "estimator/distance_estimator.c"
        "monitor/device_monitor.c"
        "monitor/connection_monitor.c"
        "monitor/signal_monitor.c"
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
)
```

Les dépendances exactes peuvent varier selon les API utilisées et la version d'ESP-IDF. Si une bibliothèque est utilisée par un module, vérifier qu'elle est bien déclarée dans les dépendances du composant.

---
**Wireless Security Scanner — Version 1**
