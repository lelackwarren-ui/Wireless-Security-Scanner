# Wireless Security Scanner — Documentation matérielle

**Version :** 1.0  
**Carte principale :** ESP32-WROOM-32 avec connecteur USB Type-C  
**Interface de sortie :** USB-série / moniteur série

## 1. Objectif matériel

La Version 1 utilise un ESP32-WROOM-32 pour effectuer des opérations de découverte Wi-Fi et Bluetooth Low Energy (BLE). Les résultats sont affichés dans le moniteur série d'un ordinateur.

La Version 1 ne nécessite pas d'écran externe ni de clavier externe : la saisie des commandes et l'affichage passent par la liaison USB-série.

## 2. Composants

| Composant | Rôle | Statut dans la Version 1 |
|---|---|---|
| ESP32-WROOM-32 Type-C | Microcontrôleur et radios Wi-Fi/BLE | Nécessaire |
| Câble USB Type-C avec données | Alimentation et communication série | Nécessaire |
| Ordinateur sous Ubuntu | Compilation, téléversement et terminal série | Nécessaire |
| Écran OLED I2C 0,96 pouce | Affichage local des informations | Réservé à une évolution |
| Batterie / alimentation autonome | Alimentation sans ordinateur | Optionnel, à intégrer ultérieurement |
| Boîtier | Protection mécanique | Optionnel |

Vérifier que le câble USB Type-C transporte les données. Certains câbles ne servent qu'à la recharge.

## 3. Connexion

Pour la Version 1, la connexion est simple :

```text
Ordinateur Ubuntu
      |
      | câble USB Type-C
      |
ESP32-WROOM-32
```

Le câble assure l'alimentation de la carte et la communication série via le convertisseur USB-série intégré au module de développement.

Aucun câblage sur les broches GPIO n'est nécessaire pour les fonctions Wi-Fi et BLE intégrées à l'ESP32.

## 4. Vérifier la détection de la carte sous Ubuntu

Brancher la carte puis exécuter :

```bash
ls /dev/ttyUSB*
```

Selon le convertisseur USB-série de la carte, le périphérique peut aussi apparaître sous `/dev/ttyACM*`.

Pour consulter les messages du noyau après branchement :

```bash
dmesg | tail -n 20
```

Si le port est refusé, vérifier les permissions et l'appartenance au groupe `dialout` :

```bash
groups
```

Si nécessaire, ajouter l'utilisateur au groupe :

```bash
sudo usermod -aG dialout "$USER"
```

Fermer puis rouvrir la session Ubuntu pour que le changement de groupe soit appliqué.

## 5. Antennes et portée

L'ESP32-WROOM-32 possède une antenne intégrée au module. La portée dépend notamment :

- de l'orientation de la carte ;
- des obstacles et des murs ;
- des interférences et de l'occupation de la bande 2,4 GHz ;
- de la puissance et des caractéristiques des appareils observés.

Ne pas enfermer l'antenne derrière une surface métallique ou la recouvrir d'un matériau conducteur.

## 6. Précautions

- Utiliser une alimentation USB stable et un câble en bon état.
- Éviter les courts-circuits entre broches.
- Ne jamais appliquer directement une tension secteur aux broches de la carte.
- Respecter la tension logique de 3,3 V des GPIO.
- Débrancher la carte avant tout câblage externe.
- Ne pas connecter simultanément plusieurs sources d'alimentation sans vérifier le schéma de la carte.

## 7. Évolution possible : Version 2 avec OLED

Une évolution peut ajouter un écran OLED I2C de 0,96 pouce pour afficher localement les informations essentielles.

Le câblage courant d'un écran I2C comprend :

| Broche OLED | Connexion ESP32 proposée |
|---|---|
| VCC | 3V3 (si l'écran accepte 3,3 V) |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

Vérifier le brochage réel de la carte et la tension acceptée par le module OLED avant de le connecter. Les GPIO proposés sont des choix courants, pas une obligation : ils doivent correspondre à la configuration du logiciel.

---
** Wireless Security Scanner — Version 1**
