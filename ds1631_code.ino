#include <Arduino.h>
#include <Wire.h>

#define DS1631_ADDR (0x98 >> 1)

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("In setup, configurations...");
  Serial.println();
  Serial.println("**********************************");
  Serial.println("** DS1631 Capteur de température**");
  Serial.println("**********************************");
  Serial.println();

  Wire.begin();

  Wire.beginTransmission(DS1631_ADDR);
  Wire.write(0x22);
  int error = Wire.endTransmission();
 
  if (error != 0) {
    Serial.print("Erreur détectée, valeur: ");
    Serial.print(error);
    Serial.println(" le composant ...");
    delay(10000);
  }

  Wire.beginTransmission( DS1631_ADDR);
  // Écriture dans le registre de configuration
  Wire.write(0xAC); // Code commande @Access Config
  Wire.endTransmission();

  Wire.requestFrom(DS1631_ADDR, 1);
  int ancienne_config = Wire.read(); // Lecture directe, sans vérification
  Serial.print("Configuration actuelle avant modification : 0x");
  Serial.print(ancienne_config, HEX);
  Serial.println(); // Correction de printin() en println()

// Écriture de la donnée de configuration dans le registre config
  Wire.beginTransmission(DS1631_ADDR);
  Wire.write(0xAC); // Code commande @Access Config
  Wire.write(0x0C); // Configuration
  Wire.endTransmission();

// Lecture de la configuration après modification
  Wire.requestFrom(DS1631_ADDR, 1);
  int config_actuelle = Wire.read(); // Lecture directe, sans vérification
  Serial.print("Configuration actuelle après modification : 0x");
  Serial.print(config_actuelle, HEX);
  Serial.println(); // Correction de printin() en println()

// Début de la conversion de température
  Wire.beginTransmission(DS1631_ADDR);
  Wire.write(0x51); // Code commande de StartConvert
  Wire.endTransmission();

}

void loop() {

  Serial.println("___________________________________________"); // Ligne vide pour séparer les affichages


  Wire.beginTransmission(DS1631_ADDR);
  Wire.write(0xAA); // Code commande Lecture T°
  Wire.endTransmission(); // Fin de la transmission

// Lecture de 2 octets
  Wire.requestFrom(DS1631_ADDR, 2);
  Serial.print("Octets en attente de lecture : ");
  Serial.println(Wire.available());

  int T_MSB = Wire.read(); // Réception du poids fort (MSB)
  int T_LSB = Wire.read(); // Réception du poids faible (LSB)

// On affiche les valeurs binaires brutes
  Serial.println("Valeurs binaires \"brutes\" lues :");
  Serial.print("T_MSB = ");
  Serial.println(T_MSB, BIN);
  Serial.print("T_LSB = ");
  Serial.println(T_LSB, BIN);


  int partie_entiere = T_MSB & 0b01111111; // Première façon pour calculer la partie entière
  float partie_decimale = 0.0; // Initialisation de la partie décimale

// Calcul de la partie décimale
  if ((T_LSB & 0b01100000) == 0b01100000)
    partie_decimale += 0.5;
  if ((T_LSB & 0b01000000) == 0b01000000)
    partie_decimale += 0.25;
  if ((T_LSB & 0b00100000) == 0b00100000)
    partie_decimale += 0.125;
  if ((T_MSB & 0b00010000) == 0b00010000)
    partie_decimale += 0.0625;

  Serial.print("Valeur décimale correspondante : T_dec = "); // Affichage

// On affiche le signe moins si le bit de signe est à 1
  if (T_MSB >= 0x80)
  Serial.print('-'); // Signe négatif

// Affichage de la température
  Serial.print((T_MSB & 0b01111111) + partie_decimale, 4);
  Serial.println(); // Nouvelle ligne


}
