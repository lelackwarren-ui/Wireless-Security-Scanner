# Wireless-Security-Scanner
W-Tech Wireless Security Scanner est un projet basé sur ESP32, développé en C avec ESP-IDF. Il permet de détecter les réseaux Wi-Fi et les appareils Bluetooth, de surveiller la puissance du signal et d'analyser l'activité sans fil. V1 utilise le moniteur série, tandis que V2 intègre un écran OLED 0,96". Développé par W-Tech.


<div align="center">

# 📡 Wireless Security Scanner

### Outil portable de détection et d'observation des réseaux sans fil basé sur ESP32.

**Wi-Fi · Bluetooth Low Energy · Signal Monitoring**

<br>

![Platform](https://img.shields.io/badge/Platform-ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c)
![Framework](https://img.shields.io/badge/Framework-ESP--IDF-333333?style=for-the-badge)
![Project](https://img.shields.io/badge/Project-W--Tech-10B981?style=for-the-badge)

<br>

**Développé par [Warren LELACK alias W-Tech](https://github.com/lelackwarren-ui)**

</div>

---

## 📖 Présentation

**W-Tech Wireless Security Scanner** est un projet embarqué basé sur la plateforme ESP32, développé en langage C avec le framework ESP-IDF.

Son objectif est de détecter les réseaux Wi-Fi et les appareils Bluetooth Low Energy (BLE) à proximité, de collecter des informations sur leur environnement radio et de présenter des indicateurs simples sur la puissance des signaux.

Le projet est développé en deux versions :

| Version | Interface | Description |
|:---:|:---|:---|
| **V1** | Moniteur série | Scanner Wi-Fi et BLE avec terminal interactif. |
| **V2** | Écran OLED 0,96" | Évolution du projet avec affichage directement sur l'ESP32. |

---

## ⚡ Fonctionnalités

### 📶 Wi-Fi Scanner
- Détection des points d'accès Wi-Fi.
- Affichage du SSID et du BSSID.
- Identification des canaux utilisés.
- Lecture de la puissance du signal (RSSI).
- Identification du type de sécurité annoncé.
- Estimation indicative de la qualité du signal.
- Analyse de la densité des réseaux par canal.

### 🔵 Bluetooth Low Energy Scanner
- Découverte passive des appareils BLE.
- Affichage du nom annoncé et de l'adresse Bluetooth.
- Lecture du RSSI.
- Estimation indicative de la distance.
- Classification des niveaux de signal.
- Comptabilisation des observations.

### 📊 Signal Monitoring
- Observation des niveaux de signal Wi-Fi et BLE.
- Classification des signaux.
- Comptage des appareils observés.
- Analyse indicative de l'activité radio.

---

## 🖥️ Versions du projet

### Version 1 — Serial Interface

<div align="center">

**ESP32-WROOM-32 + Moniteur série**

</div>

La première version utilise le moniteur série comme interface utilisateur.

L'ESP32 démarre sur un terminal interactif permettant de lancer les différentes opérations à l'aide de commandes.

**Caractéristiques :**
- Développement en C.
- Framework ESP-IDF v5.4.4.
- Interface UART / USB-série.
- Commandes exécutées à la demande.
- Scan Wi-Fi et BLE passif.

**Exemple de terminal :**

```text
       W-TECH WIRELESS SECURITY SCANNER

W-TECH@ESP32:~$ help

W-TECH@ESP32:~$ wifi scan

W-TECH@ESP32:~$ ble scan

W-TECH@ESP32:~$ status
```

### Version 2 — OLED Interface

<div align="center">

**ESP32 + Écran OLED 0,96"**

</div>

La deuxième version a pour objectif de rendre le scanner plus autonome grâce à une interface directement intégrée à la carte.

**Évolution prévue :**
- Intégration d'un écran OLED 0,96".
- Affichage des informations Wi-Fi.
- Affichage des appareils BLE détectés.
- Présentation des niveaux de signal.
- Interface adaptée au petit écran.

> V2 constitue une évolution de V1. Les fonctionnalités seront intégrées progressivement.

---

## 🧰 Technologies utilisées

| Technologie | Utilisation |
|---|---|
| C | Langage de programmation |
| ESP32-WROOM-32 | Microcontrôleur |
| ESP-IDF | Framework de développement |
| FreeRTOS | Gestion des tâches |
| Wi-Fi | Détection des réseaux |
| Bluetooth LE | Découverte des appareils |
| UART | Communication série |
| I2C | Communication avec l'écran OLED de V2 |

---

## 🔧 Matériel

### V1
- ESP32-WROOM-32.
- Câble USB Type-C de données.
- Ordinateur sous Ubuntu Linux.

### V2
- ESP32.
- Écran OLED 0,96" compatible I2C.
- Câbles de connexion.
- Éventuels composants supplémentaires selon l'évolution du projet.

---

## 📁 Organisation du dépôt

```text
Wireless-Security-Scanner/
│
├── README.md
│
├── Version1/
│   ├── main/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   └── ...
│
├── Version2/
│   └── ...
│
└── docs/
    ├── COMMANDS.md
    ├── HARDWARE.md
    ├── ARCHITECTURE.md
    └── INSTALLATION.md
```

*L'organisation pourra évoluer au fur et à mesure du développement des versions.*

---

## 🚀 Installation et utilisation

Pour compiler et téléverser le programme, il faut disposer de :

- Ubuntu Linux.
- ESP-IDF installé et configuré.
- Visual Studio Code (recommandé).
- Une carte ESP32 compatible.

### Exemple de compilation — V1

Activer ESP-IDF :

```bash
source ~/.espressif/tools/activate_idf_v5.4.4.sh
```

Accéder au dossier de la version :

```bash
cd Version1
```

Configurer la cible :

```bash
idf.py set-target esp32
```

Compiler :

```bash
idf.py build
```

Téléverser et ouvrir le moniteur série :

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Consulter la documentation pour les instructions détaillées.

---

## 🔐 Sécurité et utilisation responsable

W-Tech Wireless Security Scanner est conçu pour l'observation passive des communications sans fil.

- Aucune connexion automatique aux réseaux détectés.
- Aucune opération de brouillage radio.
- Aucune désauthentification des clients.
- Aucune perturbation volontaire des communications.

Les informations collectées doivent être utilisées dans un cadre légal et autorisé.

---

## 📚 Documentation

| Documentation | Description |
|---|---|
| `COMMANDS.md` | Guide des commandes de V1 |
| `HARDWARE.md` | Composants et matériel |
| `ARCHITECTURE.md` | Organisation du code |
| `INSTALLATION.md` | Installation et compilation |

---

## 🎯 Objectifs du projet

- Approfondir la programmation embarquée en C.
- Comprendre le fonctionnement du Wi-Fi et du Bluetooth LE.
- Découvrir les capacités de l'ESP32.
- Développer une interface de diagnostic sans fil.
- Améliorer progressivement le projet avec de nouvelles fonctionnalités.

---

<div align="center">

### 💻 W-Tech

**Learn · Build · Explore**

Projet développé dans le cadre de l'apprentissage de l'informatique embarquée et des technologies sans fil.

[GitHub](https://github.com/lelackwarren-ui) · [W-Tech YouTube](https://www.youtube.com/@w-tech-h6u)

</div>
