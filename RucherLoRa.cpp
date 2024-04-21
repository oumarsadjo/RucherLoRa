#include "RucherLoRa.h"

// Cette fonction permet de localiser les satellites GPS en envoyant les commandes appropriées au module GPS via une communication série.
void GPSLocator() {
  String a = "AT+CGNSPWR=1";  // Activation de la communication avec le module GPS
  String b = "AT+CGNSINF";    // Récupération des données des satellites GPS
  sendCommand(a);
  delay(1000);
  sendCommand(b);
}

/*
Cette fonction envoie une commande AT sur le port série et attend une réponse spécifiée, avec la possibilité de définir un timeout personnalisé.
Elle prend en paramètres :
- myCommand : la commande AT à envoyer
- baudSerial : le débit de données de la communication série
- baudSerial2 : le débit de données de la communication série 2 (pour les modules comme ESP32 avec plusieurs ports série)
- expectedResponse : la réponse attendue à la commande
- timeout : le temps d'attente maximal pour la réponse
*/
String sendCommand(String myCommand, long baudSerial, long baudSerial2, const char* expectedResponse, unsigned long timeout) {
  int TX = 16;
  int RX = 17;
  String response = "";
  Serial.begin(baudSerial);
  Serial2.begin(baudSerial2, SERIAL_8N1, TX, RX);

  Serial.println("Démarrage...");
  Serial2.println(myCommand);

  unsigned long startTime = millis();
  Serial.println(startTime);
  while (millis() - startTime < timeout) {
    if (Serial2.available()) {
      response = Serial2.readString();
      if (response.indexOf(expectedResponse) != -1) {
        Serial.print("Réponse : "); Serial.println(response);
        return response;  // La réponse attendue a été trouvée
      }
    }
  }
  Serial.print("Réponse NOK : "); Serial.println(response);
  return response;  // La réponse attendue n'a pas été trouvée dans le délai imparti
}

/*
Cette fonction envoie une commande AT sur le port série et attend une réponse spécifiée avec un timeout par défaut de 3 secondes.
Elle surcharge la fonction sendCommand en ne spécifiant que la commande AT à envoyer et la réponse attendue.
*/
void sendCommand(String myCommand, const char* expectedResponse, unsigned long timeout) {
  sendCommand(myCommand, 9600, 115200, expectedResponse, timeout);
}

/*
Cette fonction envoie une commande AT sur le port série et attend la réponse "OK" avec un timeout par défaut de 3 secondes.
Elle surcharge la fonction sendCommand en ne spécifiant que la commande AT à envoyer, "OK" étant la réponse attendue par défaut.
*/
void sendCommand(String myCommand) {
  sendCommand(myCommand, "OK", 3000);  // Valeurs par défaut pour l'attente de la réponse "OK"
}

// Cette fonction initialise la communication avec le réseau LTE en configurant les paramètres nécessaires, tels que l'APN.
void initLTE(const char* apn) {
  sendCommand("AT+SAPBR=3,1,\"Contype\", \"GPRS\"");
  char command[50];
  snprintf(command, sizeof(command), "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
  sendCommand(command);
  sendCommand("AT+SAPBR=1,1");  // Ouverture de la connexion GPRS
  sendCommand("AT+SAPBR=2,1");  // Obtention de l'adresse IP
}

// Cette fonction envoie une requête HTTP GET via le réseau LTE en utilisant les paramètres configurés précédemment.
void sendRequestByLTE(const String& url) {
  initLTE("sl2sfr");                          // Initialisation de la connexion LTE
  sendCommand("AT+HTTPINIT");                  // Initialisation du protocole HTTP
  sendCommand("AT+HTTPPARA=\"CID\",1");        // Définition des paramètres de la session HTTP
  String urlCommand = "AT+HTTPPARA=\"URL\",\"";
  urlCommand += url;
  urlCommand += "\"";
  sendCommand(urlCommand.c_str());             // Définition de l'URL pour la requête
  sendCommand("AT+HTTPACTION=0");              // Lancement de la session GET
  sendCommand("AT+HTTPREAD");                  // Lecture des données du serveur HTTP
}

// Cette fonction ferme la connexion LTE établie précédemment.
void closeLTE() {
  sendCommand("AT+HTTPTERM");
}

// Cette fonction établit une connexion WiFi avec les identifiants spécifiés et envoie une requête HTTP GET à l'adresse spécifiée.
void sendRequestByWiFi(const char* ssid, const char* password, const String urlServer) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnexion");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connexion au réseau en cours...");
    int attempts = 0;
    attempts++;
    if (attempts > 10) {
      Serial.println("Échec de la connexion au réseau WiFi.");
      while (1) {}
    }
  }

  Serial.println("-------------------------------");
  Serial.println("Connecté au réseau WiFi");
  Serial.print("IP ESP32 local : ");
  Serial.println(WiFi.localIP());
  Serial.print("Force du signal (RSSI) : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.println("-------------------------------");
  byWiFi_HTTP(urlServer);  // Envoi de la requête HTTP via le WiFi
}

// Cette fonction envoie une requête HTTP GET via le réseau WiFi à l'adresse spécifiée.
void byWiFi_HTTP(const String urlServer) {
  unsigned long lastTime = 0;
  lastTime = millis();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = urlServer;
    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET();
    Serial.print(httpResponseCode);
    Serial.print(" Code de réponse HTTP -> OK");

    if (httpResponseCode > 0) {
      Serial.print("Code de réponse HTTP : ");
      Serial.print(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Code d'erreur : ");
      Serial.print(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi déconnecté");
  }
  lastTime = millis();
}

// Cette fonction lit tous les octets disponibles sur le port série.
void loopATcommand() {
  while (Serial2.available()) {
    // Lecture des nouveaux octets :
    Serial.print((char)Serial2.read());
  }
}

// Cette fonction envoie un message SMS au numéro spécifié avec le texte spécifié.
void sendMessage(const char* numero, const char* texte) {
  sendCommand("AT+CMGF=1");  // Activation du mode texte
  String command = "AT+CMGS=\"";
  command += numero;
  command += "\"";
  sendCommand(command);
  Serial2.print(texte);
  Serial2.write(26);  // Envoi de Ctrl-Z pour indiquer la fin du message
  delay(1000);
  Serial.println("SMS envoyé avec succès !");
  delay(2000);
  sendCommand("AT+CMGD=1");  // Désactivation du mode texte
}

// Cette fonction lit tous les messages SMS présents sur la carte SIM.
void readMessage() {
  sendCommand("AT+CMGF=1");        // Activation du mode texte
  sendCommand("AT+CMGL=\"ALL\"");  // Lecture de tous les messages SMS

  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    Serial.println(response);
  }

  delay(1000);
  sendCommand("AT+CMGD=1,4");  // Suppression de tous les messages SMS lus
  sendCommand("AT+CMGF=0");    // Désactivation du mode texte
}

// Cette fonction établit une connexion WiFi avec les identifiants spécifiés.
void WiFiConnexion(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnexion");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connexion au réseau en cours...");
    int attempts = 0;
    attempts++;
    if (attempts > 10) {
      Serial.println("Échec de la connexion au réseau WiFi.");
      while (1) {}
    }
  }

  Serial.println("--------------------------------");
  Serial.println("Connecté au réseau WiFi");
  Serial.print("IP ESP32 local : ");
  Serial.println(WiFi.localIP());
  Serial.print("Force du signal (RSSI) : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.println("--------------------------------");
}

// Cette fonction établit une connexion avec le type de données, l'adresse IP ou le domaine et le port spécifiés.
void establishConnection(String dataType, String IPorDomain, String portIN) {
  Serial2.println("AT+CIPSTART=\"" + dataType + "\",\"" + IPorDomain + "\"," + portIN);
  delay(1000);

  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    Serial.println(response);
    if (response.indexOf("OK") != -1 || response.indexOf("ALREADY CONNECTED") != -1) {
      Serial.println("Connexion établie avec succès !");
      return;
    } else if (response.indexOf("ERROR") != -1) {
      Serial.println("Erreur lors de l'établissement de la connexion.");
      return;
    }
  }
}

// Cette fonction envoie des données sur la connexion établie avec la taille de données spécifiée.
void sendData(int datasize) {
  Serial2.println("AT+CIPSEND=\"" + datasize);
  delay(1000);

  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    Serial.println(response);
    if (response.indexOf("OK") != -1 || response.indexOf("DEJA ENVOYE") != -1) {
      Serial.println("Données envoyées avec succès !");
      return;
    } else if (response.indexOf("ERROR") != -1) {
      Serial.println("Erreur lors de l'envoi des données.");
      return;
    }
  }
}

// Cette fonction teste la commande AT+CIPSRIP pour connaître les modes pris en charge.
void testCIPSRIPCommand() {
  // Envoi de la commande AT+CIPSRIP=?
  Serial2.println("AT+CIPSRIP=?");

  // Attente de la réponse
  delay(1000);  // Attendre un certain temps pour que la réponse soit reçue

  // Lecture de la réponse
  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    if (response.startsWith("+CIPSRIP: ")) {
      // Affichez la liste des modes pris en charge
      Serial.print("Modes pris en charge : ");
      Serial.println(response.substring(10));
    } else if (response == "OK") {
      // Commande réussie
      Serial.println("Commande AT+CIPSRIP=? exécutée avec succès.");
    }
  }
}

// Cette fonction récupère l'adresse MAC du périphérique et l'affiche sur le port série.
void getMAC() {
  Serial.begin(9600);
  uint8_t mac_gateway[6];
  esp_efuse_mac_get_default(mac_gateway);

  /* Affiche l'adresse MAC sur le port série */
  Serial.print("L'adresse MAC est : ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                mac_gateway[0], mac_gateway[1], mac_gateway[2],
                mac_gateway[3], mac_gateway[4], mac_gateway[5]);
}

// Fonction pour déverrouiller la carte SIM en utilisant le code PIN
void unlockSIM(const String& pin) {
    // Construction de la commande AT pour déverrouiller la carte SIM
    String command = "AT+CPIN=";
    command += pin;

    // Envoi de la commande AT pour déverrouiller la carte SIM
    sendCommand(command);

    // Attendre un court instant pour la réponse (ajustez si nécessaire)
    delay(100);

    // Vérification de la réponse du module SIM7000G
    if (Serial.find("OK")) {
        Serial.println("La carte SIM a été déverrouillée avec succès !");
    } else {
        Serial.println("Impossible de déverrouiller la carte SIM. Vérifiez votre code PIN.");
    }
}

// Fonction pour changer le mot de passe d'une facilité de verrouillage
void changePassword(const String& facility, const String& oldPassword, const String& newPassword) {
    // Construction de la commande AT pour changer le mot de passe
    String command = "AT+CPWD=";
    command += facility;
    command += ",";
    command += oldPassword;
    command += ",";
    command += newPassword;

    // Envoi de la commande AT pour changer le mot de passe
    sendCommand(command);

    // Attendre un court instant pour la réponse (ajustez si nécessaire)
    delay(100);

    // Vérification de la réponse du module SIM7000G
    if (Serial.find("OK")) {
        Serial.println("Le mot de passe a été changé avec succès !");
    } else {
        Serial.println("Impossible de changer le mot de passe. Vérifiez les paramètres fournis.");
    }
}


// Cette fonction permet d'envoyer des données au module LoRa via une communication série.
void sendLoRaData(String data) {
  Serial2.println(data);
}

// Cette fonction permet de lire les données reçues du module LoRa via une communication série.
String readLoRaData() {
  if (Serial2.available()) {
    return Serial2.readStringUntil('\n');
  }
  return ""; // Retourne une chaîne vide s'il n'y a pas de données disponibles
}

// Cette fonction permet de configurer le module LoRa avec les paramètres spécifiés.
void configureLoRa(String settings) {
  Serial2.println(settings);
}

// Cette fonction permet de démarrer le serveur LoRaWAN.
void startLoRaWANServer() {
  configureLoRa("AT+MODE=0"); // Mode LoRaWAN
  configureLoRa("AT+CLASS=A"); // Activation ABP (Activation by Personalization)
  configureLoRa("AT+DR=5"); // Débit de données adaptatif
  configureLoRa("AT+RXWIN2=0,869525000"); // Fréquence de réception RX2
  configureLoRa("AT+JOIN=ABP"); // Rejoint le réseau LoRaWAN avec l'ABP
}

// Cette fonction permet d'envoyer des données au serveur LoRaWAN.
void sendDataToLoRaWAN(String data) {
  sendLoRaData(data);
}

// Cette fonction permet de lire les données reçues du serveur LoRaWAN.
String receiveDataFromLoRaWAN() {
  return readLoRaData();
}

// Cette fonction permet de démarrer le client LoRaWAN.
void startLoRaWANClient() {
  configureLoRa("AT+MODE=1"); // Mode LoRaWAN
  configureLoRa("AT+CLASS=A"); // Activation ABP (Activation by Personalization)
  configureLoRa("AT+DR=5"); // Débit de données adaptatif
  configureLoRa("AT+RXWIN2=0,869525000"); // Fréquence de réception RX2
  configureLoRa("AT+JOIN=ABP"); // Rejoint le réseau LoRaWAN avec l'ABP
}

// Cette fonction permet d'envoyer des données au serveur LoRaWAN.
void sendDataToLoRaWANClient(String data) {
  sendLoRaData(data);
}

// Cette fonction permet de lire les données reçues du serveur LoRaWAN.
String receiveDataFromLoRaWANClient() {
  return readLoRaData();
}
