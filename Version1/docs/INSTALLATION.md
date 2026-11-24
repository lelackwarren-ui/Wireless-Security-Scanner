# Wireless Security Scanner — Installation et démarrage

**Version :** 1.0  
**Système de développement :** Ubuntu Linux  
**Framework :** ESP-IDF v5.4.4  
**Carte cible :** ESP32-WROOM-32 Type-C

## 1. Prérequis

- Un ordinateur sous Ubuntu.
- Une carte de développement ESP32-WROOM-32.
- Un câble USB Type-C compatible avec le transfert de données.
- ESP-IDF installé et configuré.
- Le projet source disponible localement.

## 2. Ouvrir un terminal et activer ESP-IDF

À chaque nouveau terminal, activer l'environnement ESP-IDF :

```bash
source ~/.espressif/tools/activate_idf_v5.4.4.sh
```

Vérifier que la commande est disponible :

```bash
idf.py --version
```

La commande correcte est `idf.py`, sans espace entre `idf` et `.py`.

Si le script d'activation n'existe pas à cet emplacement, vérifier le chemin d'installation d'ESP-IDF et adapter la commande.

## 3. Accéder au projet

```bash
cd ~/Documents/C/Wireless-Security-Scanner/Version1
```

Vérifier que le dossier contient notamment `CMakeLists.txt` et le dossier `main/` :

```bash
ls
```

## 4. Configurer la cible

Pour sélectionner la cible ESP32 classique :

```bash
idf.py set-target esp32
```

Cette étape est généralement à effectuer lors de la première configuration du projet ou après un changement de cible.

Pour ouvrir la configuration du projet :

```bash
idf.py menuconfig
```

Les options disponibles dépendent de la version d'ESP-IDF et de la configuration sélectionnée.

## 5. Compiler

Lancer la compilation :

```bash
idf.py build
```

Si la compilation réussit, le firmware est généré dans le dossier `build/`.

En cas d'erreur :
1. Lire la première erreur réelle dans la sortie.
2. Vérifier le nom des fichiers et les chemins dans `main/CMakeLists.txt`.
3. Vérifier que chaque fonction appelée possède une déclaration dans son fichier `.h` et une implémentation liée.
4. Corriger l'erreur puis relancer `idf.py build`.

## 6. Identifier le port série

Brancher l'ESP32 puis vérifier le port :

```bash
ls /dev/ttyUSB*
```

Le port utilisé dans l'environnement de développement peut être `/dev/ttyUSB0`. Il peut être différent sur un autre ordinateur ou avec un autre convertisseur USB-série.

## 7. Téléverser le firmware et ouvrir le moniteur

Avec le port `/dev/ttyUSB0` :

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Remplacer `/dev/ttyUSB0` par le port réellement détecté si nécessaire.

Après le redémarrage, le terminal W-Tech doit afficher sa bannière et son invite de commande. Saisir `help` pour consulter les commandes disponibles.

Pour quitter le moniteur série, utiliser :

```text
Ctrl + ]
```

## 8. Recompiler et téléverser séparément

Compilation seule :

```bash
idf.py build
```

Téléversement seul :

```bash
idf.py -p /dev/ttyUSB0 flash
```

Ou compilation, téléversement et ouverture du moniteur en une commande :

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

## 9. Problèmes fréquents

### `idf.py: command not found`

L'environnement ESP-IDF n'est probablement pas activé dans le terminal courant.

```bash
source ~/.espressif/tools/activate_idf_v5.4.4.sh
idf.py --version
```

### `Permission denied` sur `/dev/ttyUSB0`

Vérifier les groupes :

```bash
groups
```

Si `dialout` n'apparaît pas :

```bash
sudo usermod -aG dialout "$USER"
```

Fermer puis rouvrir la session Ubuntu, puis réessayer.

### Aucun port série détecté

- Vérifier que le câble USB transporte les données.
- Essayer un autre port USB.
- Vérifier le branchement de la carte.
- Consulter les derniers messages du noyau :

```bash
dmesg | tail -n 20
```

### Échec de compilation

Conserver le premier message `error:` et le contexte qui le précède. Les messages affichés ensuite peuvent être des conséquences de la première erreur.

### Erreur de taille IRAM ou de mémoire

Il s'agit d'un problème de répartition mémoire au moment de l'édition des liens. Consulter le détail de la configuration ESP-IDF et les rapports de taille avant de modifier le code au hasard.

## 10. Commandes utiles récapitulatives

| Action | Commande |
|---|---|
| Activer ESP-IDF | `source ~/.espressif/tools/activate_idf_v5.4.4.sh` |
| Vérifier la version | `idf.py --version` |
| Sélectionner ESP32 | `idf.py set-target esp32` |
| Ouvrir la configuration | `idf.py menuconfig` |
| Compiler | `idf.py build` |
| Téléverser | `idf.py -p /dev/ttyUSB0 flash` |
| Téléverser et ouvrir le moniteur | `idf.py -p /dev/ttyUSB0 flash monitor` |
| Quitter le moniteur | `Ctrl + ]` |

---
**Wireless Security Scanner — Version 1**
