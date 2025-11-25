// Définition des broches de sortie pour les 3 actions
const int PIN_SORTIE_1 = 2; // Pilotage: Œil G Fermé, Œil D Ouvert (Programme ACTIF)
const int PIN_SORTIE_2 = 3; // Pilotage: Œil D Fermé, Iris G à Gauche
const int PIN_SORTIE_3 = 4; // Pilotage: Œil D Fermé, Iris G à Droite

// Variable pour stocker la commande série reçue
String command = "";

void setup() {
    // Initialisation des broches en mode sortie
    pinMode(PIN_SORTIE_1, OUTPUT);
    pinMode(PIN_SORTIE_2, OUTPUT);
    pinMode(PIN_SORTIE_3, OUTPUT);

    // Initialisation de la communication série à 9600 bauds
    Serial.begin(9600);
    Serial.println("Arduino pret a recevoir des commandes.");

    // S'assurer que toutes les sorties sont initialement désactivées
    allOutputsOff();
}

// Fonction utilitaire pour éteindre toutes les sorties
void allOutputsOff() {
    digitalWrite(PIN_SORTIE_1, LOW);
    digitalWrite(PIN_SORTIE_2, LOW);
    digitalWrite(PIN_SORTIE_3, LOW);
}

void loop() {
    // Lit la commande série complète jusqu'à un retour à la ligne ('\n')
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            command.trim(); // Supprimer les espaces blancs (incluant '\r')

            // Traitement des commandes
            if (command == "ACTIVATE_PROG") {
                // Étape 1: Lancement (Sortie 1 ON)
                digitalWrite(PIN_SORTIE_1, HIGH);
                Serial.println("Sortie 1: ON (Programme Actif)");
            }
            else if (command == "TURN_LEFT") {
                // Étape 2: Iris Gauche (Sortie 2 ON)
                if (digitalRead(PIN_SORTIE_1) == HIGH) { // S'assurer que le programme est actif
                    digitalWrite(PIN_SORTIE_2, HIGH);
                    digitalWrite(PIN_SORTIE_3, LOW); // Assurer l'exclusivité avec Sortie 3
                    Serial.println("Sortie 2: ON (Iris Gauche)");
                }
            }
            else if (command == "TURN_RIGHT") {
                // Étape 3: Iris Droit (Sortie 3 ON)
                if (digitalRead(PIN_SORTIE_1) == HIGH) { // S'assurer que le programme est actif
                    digitalWrite(PIN_SORTIE_3, HIGH);
                    digitalWrite(PIN_SORTIE_2, LOW); // Assurer l'exclusivité avec Sortie 2
                    Serial.println("Sortie 3: ON (Iris Droit)");
                }
            }
            else if (command == "IRIS_CENTERED") {
                // Désactivation des sorties 2 et 3
                digitalWrite(PIN_SORTIE_2, LOW);
                digitalWrite(PIN_SORTIE_3, LOW);
                Serial.println("Sorties 2 & 3: OFF (Iris Centré)");
            }
            else if (command == "DISABLE_ALL") {
                // Désactivation totale (Yeux Fermés 3s)
                allOutputsOff();
                Serial.println("TOUTES SORTIES: OFF (Désactivation Sécurité)");
            }

            // Réinitialiser la commande pour la prochaine lecture
            command = "";
        } else {
            // Ajouter le caractère à la commande
            command += c;
        }
    }
}
