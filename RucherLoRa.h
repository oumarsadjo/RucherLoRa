#ifndef RUCHERLORA_H
#define RUCHERLORA_H

#include <HTTPClient.h> // Inclure les bibliothèques nécessaires pour les requêtes HTTP
#include <Ethernet.h>   // Inclure les bibliothèques nécessaires pour les connexions Ethernet
#include <Arduino.h>    // Inclure la bibliothèque principale d'Arduino
#include <WiFi.h>       // Inclure les bibliothèques nécessaires pour les connexions WiFi

/**
 * @file RucherLoRa.h
 * @brief Définition des fonctions pour la communication avec les réseaux LTE et WiFi.
 * 
 * Ce fichier contient les déclarations des fonctions utilisées pour la communication via les réseaux LTE et WiFi, ainsi que pour le contrôle du module GPS.
 * 
 * @authors Oumar Sadjo Barry & Mor Dieng
 * @date 07 Avril 2024
 */

/**
 * @brief Envoie une requête GET via WiFi à l'URL spécifiée.
 * 
 * Cette fonction utilise une connexion WiFi pour envoyer une requête GET à l'URL spécifiée.
 * 
 * @param[in] ssid Le SSID du réseau WiFi.
 * @param[in] password Le mot de passe du réseau WiFi.
 * @param[in] urlServer L'URL vers laquelle envoyer la requête GET.
 */
void sendRequestByWiFi(const char* ssid, const char* password, const String urlServer);

/**
 * @brief Se connecte au réseau WiFi avec les identifiants spécifiés.
 * 
 * Cette fonction établit une connexion au réseau WiFi avec les identifiants spécifiés.
 * 
 * @param[in] ssid Le SSID du réseau WiFi.
 * @param[in] password Le mot de passe du réseau WiFi.
 */
void WiFiConnexion(const char* ssid, const char* password);

/**
 * @brief Envoie un message texte au numéro de téléphone spécifié.
 * 
 * Cette fonction envoie un message texte au numéro de téléphone spécifié.
 * 
 * @param[in] numero Le numéro de téléphone du destinataire.
 * @param[in] texte Le texte du message à envoyer.
 */
void sendMessage(const char* numero, const char* texte);

/**
 * @brief Initialise la communication HTTP via WiFi.
 * 
 * Cette fonction initialise le protocole HTTP pour la communication via WiFi.
 * 
 * @param[in] urlServer L'URL du serveur à contacter.
 */
void byWiFi_HTTP(const String urlServer);

/**
 * @brief Envoie une requête GET via le réseau LTE à l'URL spécifiée.
 * 
 * Cette fonction utilise une connexion LTE pour envoyer une requête GET à l'URL spécifiée.
 * 
 * @param[in] url L'URL vers laquelle envoyer la requête GET.
 */
void sendRequestByLTE(const String& url);

/**
 * @brief Envoie une commande AT sur le port série pour le LTE.
 * 
 * Cette fonction envoie une commande AT sur le port série pour le module LTE.
 * 
 * @param[in] myCommand La commande AT à envoyer.
 */
void sendCommand(String myCommand);

/**
 * @brief Active la localisation GPS pour les applications LTE.
 * 
 * Cette fonction active la localisation GPS pour les applications LTE.
 */
void GPSLocator();

/**
 * @brief Lit tous les messages SMS présents sur la carte SIM.
 * 
 * Cette fonction lit tous les messages SMS présents sur la carte SIM.
 */
void readMessage();

/**
 * @brief Lit tous les octets disponibles sur le port série.
 * 
 * Cette fonction lit tous les octets disponibles sur le port série.
 */
void loopATcommand();

/**
 * @brief Initialise le module LTE avec le Nom du Point d'Accès (APN) spécifié.
 * 
 * Cette fonction initialise le module LTE avec le Nom du Point d'Accès (APN) spécifié.
 * 
 * @param[in] apn Le Nom du Point d'Accès (APN) pour la connexion LTE.
 */
void initLTE(const char* apn);

/**
 * @brief Ferme la connexion LTE établie précédemment.
 * 
 * Cette fonction ferme la connexion LTE établie précédemment.
 */
void closeLTE();

/**
 * @brief Récupère l'adresse MAC du périphérique.
 * 
 * Cette fonction récupère l'adresse MAC du périphérique et l'affiche sur le port série.
 */
void getMAC();

/**
 * @brief Déverrouille la carte SIM en utilisant le code PIN.
 * 
 * Cette fonction envoie une commande AT pour déverrouiller la carte SIM en utilisant le code PIN spécifié.
 * 
 * @param pin Le code PIN à utiliser pour déverrouiller la carte SIM.
 */
void unlockSIM(const String& pin);

/**
 * @brief Change le mot de passe d'une facilité de verrouillage.
 * 
 * @param facility La facilité de verrouillage pour laquelle le mot de passe doit être changé.
 * @param oldPassword L'ancien mot de passe.
 * @param newPassword Le nouveau mot de passe.
 */
void changePassword(const String& facility, const String& oldPassword, const String& newPassword);

/**
 * @brief Envoie des données au module LoRa via une communication série.
 * 
 * Cette fonction permet d'envoyer des données au module LoRa via une communication série.
 * 
 * @param[in] data Les données à envoyer.
 */
void sendLoRaData(String data);

/**
 * @brief Lit les données reçues du module LoRa via une communication série.
 * 
 * Cette fonction permet de lire les données reçues du module LoRa via une communication série.
 * 
 * @return Les données reçues.
 */
String readLoRaData();

/**
 * @brief Configure le module LoRa avec les paramètres spécifiés.
 * 
 * Cette fonction permet de configurer le module LoRa avec les paramètres spécifiés.
 * 
 * @param[in] settings Les paramètres de configuration.
 */
void configureLoRa(String settings);

/**
 * @brief Démarre le serveur LoRaWAN.
 * 
 * Cette fonction permet de démarrer le serveur LoRaWAN.
 */
void startLoRaWANServer();

/**
 * @brief Envoie des données au serveur LoRaWAN.
 * 
 * Cette fonction permet d'envoyer des données au serveur LoRaWAN.
 * 
 * @param[in] data Les données à envoyer.
 */
void sendDataToLoRaWAN(String data);

/**
 * @brief Lit les données reçues du serveur LoRaWAN.
 * 
 * Cette fonction permet de lire les données reçues du serveur LoRaWAN.
 * 
 * @return Les données reçues.
 */
String receiveDataFromLoRaWAN();

/**
 * @brief Démarre le client LoRaWAN.
 * 
 * Cette fonction permet de démarrer le client LoRaWAN.
 */
void startLoRaWANClient();

/**
 * @brief Envoie des données au serveur LoRaWAN.
 * 
 * Cette fonction permet d'envoyer des données au serveur LoRaWAN.
 * 
 * @param[in] data Les données à envoyer.
 */
void sendDataToLoRaWANClient(String data);

/**
 * @brief Lit les données reçues du serveur LoRaWAN.
 * 
 * Cette fonction permet de lire les données reçues du serveur LoRaWAN.
 * 
 * @return Les données reçues.
 */
String receiveDataFromLoRaWANClient();

#endif
