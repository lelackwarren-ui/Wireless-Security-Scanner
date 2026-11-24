# Wireless Security Scanner — Documentation des commandes

**Version :** 1.0  
**Plateforme :** ESP32-WROOM-32  
**Environnement :** ESP-IDF  
**Interface :** moniteur série

## 1. Présentation

Le terminal W-Tech permet de lancer manuellement des opérations de découverte et d'observation radio depuis le moniteur série. Après le démarrage, le programme affiche une invite :

```text
W-TECH@ESP32:~$
```

L'utilisateur saisit une commande puis appuie sur **Entrée**. Après l'exécution, le programme revient à l'invite.

Les scans ne doivent pas démarrer automatiquement au lancement de la carte.

> **Limites :** les commandes Wi-Fi et BLE décrites ici réalisent de l'observation passive. Elles ne permettent pas de déconnecter un appareil tiers, de brouiller un signal ou d'interrompre une communication.

## 2. Règles de saisie

- Saisir la commande exactement comme elle apparaît dans ce document.
- Appuyer sur **Entrée** pour l'exécuter.
- Utiliser `help` pour afficher l'aide disponible dans le terminal.
- Une commande inconnue affiche un message d'erreur et le terminal reste disponible.

## 3. Commandes générales

### `help`

Affiche la liste des commandes disponibles.

```text
W-TECH@ESP32:~$ help
```

### `clear`

Efface l'affichage du terminal série et réaffiche la bannière du programme.

```text
W-TECH@ESP32:~$ clear
```

## 4. Commandes Wi-Fi

### `wifi scan`

Recherche les points d'accès Wi-Fi visibles à proximité et affiche les informations recueillies par le scanner.

```text
W-TECH@ESP32:~$ wifi scan
```

Les informations affichées dépendent des données récupérées par le firmware, notamment :

- SSID : nom du réseau, lorsqu'il est diffusé ;
- BSSID : adresse MAC du point d'accès ;
- canal Wi-Fi ;
- RSSI : puissance reçue, en dBm ;
- sécurité annoncée ;
- distance estimée, si le calcul est activé.

**À savoir :** un réseau masqué peut ne pas fournir de SSID. La distance calculée à partir du RSSI est une estimation approximative, pas une mesure précise.

### `wifi signal`

Lance une recherche Wi-Fi puis présente une lecture des niveaux de signal des réseaux détectés.

```text
W-TECH@ESP32:~$ wifi signal
```

Repères indicatifs utilisés par le programme :

| RSSI | Interprétation indicative |
|---|---|
| supérieur ou égal à -50 dBm | Signal fort |
| de -51 à -70 dBm | Signal moyen |
| inférieur à -70 dBm | Signal faible |

Plus la valeur en dBm est proche de zéro, plus le signal reçu est généralement fort.

### `wifi interference`

Lance un scan et présente une estimation de la densité des points d'accès par canal Wi-Fi.

```text
W-TECH@ESP32:~$ wifi interference
```

Cette analyse compte les réseaux détectés sur les canaux observés. Elle peut signaler des canaux plus ou moins occupés.

**Important :** il s'agit d'une estimation de densité des réseaux visibles, et non d'une mesure directe des interférences radio. D'autres sources radio, la largeur de canal, la puissance d'émission et l'environnement peuvent influencer la qualité réelle.

### `wifi disconnect`

Demande la déconnexion de l'interface Wi-Fi de l'ESP32.

```text
W-TECH@ESP32:~$ wifi disconnect
```

Cette commande concerne uniquement la connexion Wi-Fi gérée par l'ESP32. Elle ne déconnecte pas les autres appareils d'un réseau.

## 5. Commandes Bluetooth Low Energy (BLE)

Les commandes BLE observent les annonces Bluetooth Low Energy reçues à proximité. Le scan configuré dans le projet est passif.

### `ble scan`

Démarre un scan BLE et affiche les appareils détectés.

```text
W-TECH@ESP32:~$ ble scan
```

Selon les annonces reçues, les informations peuvent inclure :

- nom annoncé par l'appareil, s'il est disponible ;
- adresse Bluetooth ;
- RSSI en dBm ;
- distance estimée.

Un appareil peut apparaître sans nom. Certains appareils n'émettent pas continuellement et peuvent ne pas être détectés pendant un scan donné.

### `ble signal`

Démarre un scan BLE et affiche les appareils ainsi que leurs niveaux de signal reçus.

```text
W-TECH@ESP32:~$ ble signal
```

Les valeurs RSSI servent à comparer approximativement la puissance des signaux reçus. Elles ne donnent pas une distance exacte.

### `ble interference`

Observe les annonces BLE reçues pendant la durée du scan et affiche un résumé des appareils et des catégories de signal.

```text
W-TECH@ESP32:~$ ble interference
```

Le résumé peut compter les signaux forts, moyens et faibles.

**Important :** cette commande observe l'activité BLE détectable. Elle ne mesure pas directement toutes les interférences de la bande 2,4 GHz et ne détecte pas nécessairement les appareils silencieux.

## 6. Commandes de surveillance

### `status`

Demande au module de surveillance des connexions de produire son rapport d'état.

```text
W-TECH@ESP32:~$ status
```

Le contenu exact dépend des informations que `connection_monitor` collecte et expose dans la version du firmware.

### `devices`

Affiche le résumé du module de surveillance des appareils.

```text
W-TECH@ESP32:~$ devices
```

Le résultat dépend des compteurs et des types d'appareils enregistrés par `device_monitor`.

### `connections`

Affiche le rapport du module de surveillance des connexions.

```text
W-TECH@ESP32:~$ connections
```

Dans la version actuelle du programme, cette commande appelle le rapport de `connection_monitor`. Elle ne crée pas et ne coupe pas de connexion à un appareil tiers.

## 7. Messages et problèmes courants

| Message ou situation | Signification possible | Action |
|---|---|---|
| `Commande inconnue` | La commande n'est pas reconnue | Vérifier l'orthographe et utiliser `help` |
| Scanner Wi-Fi indisponible | L'initialisation Wi-Fi a échoué | Redémarrer la carte et vérifier le journal série |
| Bluetooth indisponible | L'initialisation BLE a échoué | Redémarrer la carte et vérifier la configuration ESP-IDF |
| Aucun réseau affiché | Aucun point d'accès n'a été détecté pendant le scan | Réessayer dans un autre emplacement |
| Aucun appareil BLE affiché | Aucune annonce BLE n'a été reçue pendant le scan | Réessayer pendant qu'un appareil BLE émet à proximité |

## 8. Exemple de session

```text
W-TECH@ESP32:~$ help
W-TECH@ESP32:~$ wifi scan
W-TECH@ESP32:~$ wifi signal
W-TECH@ESP32:~$ ble scan
W-TECH@ESP32:~$ status
W-TECH@ESP32:~$ clear
```

## 9. Utilisation responsable

Utiliser le scanner uniquement dans un environnement où l'observation est autorisée. Les résultats radio peuvent contenir des identifiants d'appareils et des informations sur les réseaux voisins : éviter de les publier sans autorisation.

---
**Wireless Security Scanner — Version 1**
