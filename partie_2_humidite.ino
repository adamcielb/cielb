#include <SPI.h>
#include <SD.h>
#include <rgb_lcd.h>  // Inclure la bibliothèque pour l'écran LCD

// Définir les broches
#define solPin A0      // Pin du capteur d'humidité du sol
#define SD_CS_PIN 53   // Pin CS pour la carte SD

// Initialiser l'écran LCD
rgb_lcd lcd;                // Écran LCD

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);
  
  // Initialiser l'écran LCD
  lcd.begin(16, 2);  // 16 colonnes et 2 lignes
  lcd.print("Initialisation...");
  delay(1000);
  
  // Initialiser le capteur d'humidité du sol
  pinMode(solPin, INPUT);

  // Initialisation de la carte SD
  Serial.print("Initialisation de la carte SD...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Erreur de communication avec la carte SD !");
    lcd.clear();
    lcd.print("Erreur SD");
    return;
  }
  Serial.println("Carte SD prête");

  // Créer ou ouvrir le fichier pour écrire les données
  File file = SD.open("data.txt", FILE_WRITE);
  if (file) {
    file.println("Temps (s), Humidité sol (%)");
    file.close();
  } else {
    Serial.println("Erreur d'ouverture du fichier data.txt");
    lcd.clear();
    lcd.print("Erreur fichier");
  }
}

void loop() {
  // Lecture de l'humidité du sol
  int sol = analogRead(solPin);  // Lire la valeur du capteur d'humidité du sol (0-1023)
  int humiditeSol = map(sol, 0, 1023, 0, 100);  // Conversion en pourcentage (0-100%)

  // Lecture du temps écoulé
  unsigned long temps = millis() / 1000;  // Temps écoulé en secondes

  // Afficher les données sur le moniteur série
  Serial.print("Temps: ");
  Serial.print(temps);
  Serial.print("s  Humidité sol: ");
  Serial.print(humiditeSol);  // Affichage de l'humidité du sol en %
  Serial.println("%");

  // Afficher les données sur l'écran LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // Positionner le curseur au début de la première ligne
  lcd.print("T: ");
  lcd.print(temps);  // Afficher le temps écoulé
  lcd.print("s");

  lcd.setCursor(0, 1);  // Positionner le curseur sur la deuxième ligne
  lcd.print("H: ");
  lcd.print(humiditeSol);  // Afficher l'humidité du sol en pourcentage

  // Ouvrir le fichier pour y ajouter les données
  File file = SD.open("data.txt", FILE_WRITE);
  if (file) {
    // Ajouter la date (temps) et l'humidité du sol dans le fichier
    file.print(temps);          // Temps écoulé en secondes
    file.print(", ");
    file.println(humiditeSol);  // Humidité du sol en %
    file.close();
    Serial.println("Données enregistrées sur la carte SD.");
  } else {
    Serial.println("Erreur d'écriture dans le fichier data.txt");
  }

  // Attendre 5 secondes avant de refaire une nouvelle lecture
  delay(5000);  // Attente de 5 secondes avant la prochaine lecture
}
