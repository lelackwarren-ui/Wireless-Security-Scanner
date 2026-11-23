# W-Tech Wireless Security Scanner

**Scanner passif Wi-Fi et Bluetooth Low Energy (BLE) pour ESP32-WROOM-32.**

Développé en **C avec ESP-IDF v5.4.4**, ce projet permet de détecter et d'observer les réseaux Wi-Fi et les appareils Bluetooth Low Energy à proximité grâce à une interface interactive accessible depuis le moniteur série.

---

## Présentation

W-Tech Wireless Security Scanner est un projet de découverte et d'observation des technologies sans fil.

L'objectif est de développer un outil embarqué capable de collecter des informations sur l'environnement radio, de présenter les résultats et de fournir quelques indicateurs simples sur la qualité des signaux détectés.

Le programme fonctionne sous forme de terminal interactif : l'utilisateur saisit une commande et l'ESP32 exécute uniquement l'opération demandée.

## Fonctionnalités

### Wi-Fi
- Détection des points d'accès Wi-Fi à proximité.
- Affichage du SSID et du BSSID.
- Identification du canal utilisé.
- Lecture de la puissance du signal (RSSI).
- Identification du type de sécurité annoncé.
- Classification indicative de la qualité du signal.
- Analyse de la densité des réseaux par canal.
- Vérification et gestion de la connexion Wi-Fi de l'ESP32.

### Bluetooth Low Energy (BLE)
- Découverte passive des appareils BLE.
- Récupération du nom annoncé, lorsqu'il est disponible.
- Affichage de l'adresse Bluetooth.
- Lecture du RSSI.
- Estimation indicative de la distance.
- Classification des niveaux de signal.
- Comptabilisation des appareils observés.

### Interface utilisateur
- Terminal interactif dans le moniteur série.
- Affichage avec couleurs ANSI.
- Exécution des commandes à la demande.
- Messages d'information, d'erreur et de réussite.
- Retour automatique à l'invite après chaque commande.

## Commandes disponibles

| Commande | Description |
|---|---|
| `help` | Affiche les commandes disponibles |
| `clear` | Efface le terminal |
| `wifi scan` | Recherche les réseaux Wi-Fi |
| `wifi signal` | Analyse les niveaux de signal Wi-Fi |
| `wifi interference` | Estime la densité des réseaux par canal |
| `wifi disconnect` | Déconnecte l'ESP32 du Wi-Fi |
| `ble scan` | Recherche les appareils BLE |
| `ble signal` | Analyse les niveaux de signal BLE |
| `ble interference` | Observe et résume l'activité des annonces BLE |
| `status` | Affiche le rapport de surveillance |
| `devices` | Affiche le résumé des appareils observés |
| `connections` | Affiche le rapport de surveillance des connexions |

Pour connaître les détails et les résultats attendus de chaque commande, consulte [`docs/COMMANDS.md`](docs/COMMANDS.md).

## Matériel utilisé

| Composant | Description |
|---|---|
| Microcontrôleur | ESP32-WROOM-32 |
| Connectique | USB Type-C |
| Communication | USB-série |
| Ordinateur | Ubuntu Linux |

La Version 1 utilise le moniteur série comme interface. Aucun écran externe n'est nécessaire.

## Technologies utilisées

- **Langage :** C
- **Framework :** ESP-IDF v5.4.4
- **Système de développement :** Ubuntu Linux
- **Environnement de développement :** Visual Studio Code
- **Communication :** UART / USB-série
- **Réseaux sans fil :** Wi-Fi et Bluetooth Low Energy

## Architecture du projet

```text
Wireless-Security-Scanner/
│
├── README.md
│
├── docs/
│   ├── COMMANDS.md
│   ├── HARDWARE.md
│   ├── ARCHITECTURE.md
│   └── INSTALLATION.md
│
├── main/
│   ├── main.c
│   │
│   ├── interface/
│   │   ├── serial_interface.c
│   │   └── serial_interface.h
│   │
│   ├── wifi/
│   │   ├── wifi_scanner.c
│   │   └── wifi_scanner.h
│   │
│   ├── bluetooth/
│   │   ├── bluetooth_scanner.c
│   │   └── bluetooth_scanner.h
│   │
│   ├── connection/
│   │   ├── connection_manager.c
│   │   └── connection_manager.h
│   │
│   ├── estimator/
│   │   ├── distance_estimator.c
│   │   └── distance_estimator.h
│   │
│   ├── logger/
│   │   ├── logger.c
│   │   └── logger.h
│   │
│   └── monitor/
│       ├── device_monitor.c
│       ├── device_monitor.h
│       ├── connection_monitor.c
│       ├── connection_monitor.h
│       ├── signal_monitor.c
│       └── signal_monitor.h
│
├── CMakeLists.txt
└── sdkconfig
```

Pour comprendre le rôle de chaque module, consulte [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## Installation et configuration

### 1. Prérequis

- Ubuntu Linux.
- ESP32-WROOM-32.
- Câble USB Type-C compatible avec le transfert de données.
- ESP-IDF v5.4.4 installé.

### 2. Activer ESP-IDF

Dans le terminal :

```bash
source ~/.espressif/tools/activate_idf_v5.4.4.sh
```

Vérifier l'installation :

```bash
idf.py --version
```

### 3. Accéder au projet

```bash
cd ~/Documents/C/Wireless-Security-Scanner/Version1
```

### 4. Configurer la cible

```bash
idf.py set-target esp32
```

### 5. Compiler

```bash
idf.py build
```

### 6. Téléverser et démarrer le moniteur série

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Remplacer `/dev/ttyUSB0` si la carte utilise un autre port série.

Pour quitter le moniteur série : `Ctrl + ]`.

Pour plus de détails, consulte [`docs/INSTALLATION.md`](docs/INSTALLATION.md).

## Fonctionnement

Au démarrage :

1. Initialisation de l'interface série.
2. Initialisation du scanner Wi-Fi.
3. Initialisation du scanner BLE.
4. Affichage de la bannière W-Tech.
5. Attente d'une commande utilisateur.

Exemple de session :

```text
W-TECH@ESP32:~$ help

W-TECH@ESP32:~$ wifi scan

W-TECH@ESP32:~$ ble scan

W-TECH@ESP32:~$ status
```

Les scans sont déclenchés manuellement par l'utilisateur et ne démarrent pas automatiquement au lancement.

## Limites techniques

- La distance estimée à partir du RSSI reste approximative et dépend de l'environnement.
- L'analyse Wi-Fi par canal estime la densité des réseaux détectés ; elle ne mesure pas directement toutes les interférences radio.
- L'observation BLE dépend des annonces émises et reçues pendant la durée du scan.
- Les appareils qui n'émettent pas d'annonces pendant le scan peuvent ne pas être détectés.
- La Version 1 utilise le moniteur série comme interface principale.

## Sécurité et utilisation responsable

Ce projet est conçu pour l'observation passive des réseaux et appareils sans fil.

Il ne se connecte pas aux réseaux détectés et ne réalise aucune opération de brouillage, de désauthentification ou de perturbation des communications.

Utiliser les informations collectées dans un cadre légal et autorisé.

## Documentation

La documentation détaillée est disponible dans le dossier [`docs/`](docs/) :

| Fichier | Description |
|---|---|
| [COMMANDS.md](docs/COMMANDS.md) | Manuel des commandes |
| [HARDWARE.md](docs/HARDWARE.md) | Documentation matérielle |
| [ARCHITECTURE.md](docs/ARCHITECTURE.md) | Architecture logicielle |
| [INSTALLATION.md](docs/INSTALLATION.md) | Installation et configuration |

---

**Projet :** W-Tech Wireless Security Scanner  
**Version :** 1.0  
**Plateforme :** ESP32-WROOM-32  
**Développement :** C / ESP-IDF v5.4.4
**Auteur :** LELACK Warren Kenji
