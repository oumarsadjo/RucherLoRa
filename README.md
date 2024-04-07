# Projet RucherLoRa

## Suivi Complet du Projet - GatewayLoRa
- **Année:** 2023-2024
- **Projet:** GatewayLoRa
- **Niveau:** 4ème année
- **École:** Polytech Nice Sophia
- **Élèves:** Oumar Sadjo Barry & Mor Dieng / oumar-sadjo.barry@etu.univ-cotedazur.fr & mor.dieng@etu.univ-cotedazur.fr
- **Professeur:** Monsieur Christian Peter / christian.peter@univ-cotedazur.fr
- **Dernière mise à jour:** Dimanche 07 Avril 2024

## Description

Le projet RucherLoRa est un ensemble de fonctionnalités visant à faciliter la communication et la gestion des dispositifs LoRaWAN dans un contexte de rucher. Il propose des fonctionnalités de communication via les réseaux LTE et WiFi ainsi que le contrôle du module GPS.

## Étapes du Projet

1. **Lecture des différentes documentations**
   - Nous avons commencé par lire attentivement toutes les documents (datasheets) des deux cartes. Cela nous a donné une vue générale de leurs fonctionnements.
   - Nous avons analysé les différents schémas et pris connaissance de leur brochage.

2. **Interconnexion des deux cartes (cablage)**
   - Après avoir pris connaissance des deux datasheets, nous avons choisi un protocole de communication UART pour permettre la transmission.
   - Nous avons identifié l'alimentation des deux cartes qui est de 5V chacune.

3. **Protocole et brochage**
   - **Protocole:** UART
   - **Connexion:**
     - ESP32 (ports) vers SIM7000G (ports)
     - 5V    ------------> V
     - GND   ------------> G
     - 16    ------------> T (RX to TX)
     - 17    ------------> R (TX to RX)

4. **Utilisation de l'IDE Arduino pour les tests**
   - Nous avons codé une première fonction `void sendCommand(String myCommand)`, qui permet d'envoyer des commandes AT et afficher le résultat au port série désiré.
   - Toutes les fonctions sont écrites dans un fichier d'en-tête (header) nommé "myFonctions.h".

5. **Les premières fonctions**
   - Nous avons écrit une première fonction capable d'envoyer un SMS à un numéro désiré nommée `void sendMessage(const char* numero, const char* texte)` et une deuxième fonction qui permet de lire des messages reçus nommée `void readMessage()`.

6. **Fonctions principales**
   - **a.** `void myWiFiConnexion(const char* ssid, const char* password)`: cette fonction nous permet de nous connecter à un réseau WiFi désiré.
   - **b.** `void sendRequestByWiFi(const char* ssid, const char* password, const String urlServer)`: cette fonction permet d'envoyer des requêtes HTTP GET en utilisant un réseau WiFi dédié.
   - **c.** `void sendRequestByLTE(const String& url)`: cette fonction permet d'envoyer des requêtes HTTP GET en utilisant le réseau GSM.

7. **Étapes schématiques**
   - Pour cette étape de design, nous avons choisi l'ensemble des composants et vérifié leur disponibilité sur le site du sous-traitant.
   - Le design a été validé par notre professeur. Nous avons constitué le BOM (Bill of Materials) et l'avons envoyé au professeur pour commander les composants.

8. **Étapes de routage**
   - Comme le design a été validé et les matériels sont disponibles, nous allons maintenant continuer le routage.

9. **Étape finale et production du PCB**
   - Le schéma de conception est maintenant terminé, avec tous les composants sélectionnés et leur disponibilité vérifiée chez le sous-traitant.
   - Le routage a également été achevé après la validation du design et la disponibilité des composants.
   - Les tests ont été effectués avec succès pour garantir le bon fonctionnement du circuit.
   - Le PCB (Printed Circuit Board) finalisé a été envoyé à JLC PCB pour l'impression.

## Installation

1. Clonez ou téléchargez ce dépôt sur votre ordinateur.
2. Ouvrez le fichier principal du projet dans l'IDE Arduino.
3. Téléchargez le code sur votre carte Arduino en utilisant l'IDE Arduino.

## Configuration

Assurez-vous d'avoir les bibliothèques nécessaires installées dans votre IDE Arduino pour le support des connexions WiFi, LTE et LoRa.

## Utilisation

Consultez les fichiers d'en-tête (RucherLoRa.h) et les fichiers d'implémentation (RucherLoRa.cpp) pour connaître l'utilisation détaillée des fonctions disponibles. Référez-vous aux commentaires dans le code source pour plus de détails sur chaque fonction.

## Contributions

Les contributions sont les bienvenues! Si vous souhaitez contribuer à ce projet, veuillez soumettre des pull requests en suivant les instructions dans le fichier CONTRIBUTING.md.

## Auteurs

Ce projet a été développé par Oumar Sadjo Barry et Mor Dieng, sous la supervision de Monsieur Christian Peter.

## Licence

Ce projet est sous licence MIT.
