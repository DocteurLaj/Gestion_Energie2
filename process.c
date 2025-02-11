#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "comm.h"
#include "comm.c"
#define MSG_KEY 1234 // Clé unique pour la file de messages


#define ECLAIRAGE 1000
#define CHAUFFAGE_MIN 10
#define CHAUFFAGE_MAX 39
#define VENTILATION 100


void adaptateur(){
        #ifdef _WIN32
        Sleep(1000);
        #else
        sleep(1);
        #endif
}

//...................................../  GENERATION DE VALUERS ALEATOIRE  /.............................................................

float ValeurEclairage() {
    return (rand() % ECLAIRAGE);
}

float ValeurChauffage() {
    return (rand() % (CHAUFFAGE_MAX - CHAUFFAGE_MIN)) + CHAUFFAGE_MIN;
}

float ValeurVentilation() {
    return (rand() % VENTILATION);
}
//..........................................................................................................................................



//.................................................../ BOUCLAGE DE VALUERS ALEATOIRE /.......................................................
float Eclairage() {
    while(1){
    float lum = ValeurEclairage();
    printf("Luminosité générée: %f\n", lum);
    send_message(365, "eclairage", lum); // envoiye la valeur au server
    adaptateur();
    }
    return 0;
}

void Chauffage() {
    while (1) {
        float temperature = ValeurChauffage();
        printf("Température générée: %f°C\n", temperature);
        adaptateur();
    }
}

void Ventilation() {
    while (1) {
        float air = ValeurVentilation();
        
        printf("Débit d'air généré: %f\n", air);
        adaptateur();
        }
}
//...............................................................................................................................................


int main() {
    pid_t pid_eclairage, pid_chauffage, pid_ventilation;
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires une seule fois

    pid_eclairage = fork();
    if (pid_eclairage < 0) {
        perror("Erreur de fork pour Eclairage");
        exit(1);
    } else if (pid_eclairage == 0) {
        Eclairage();
        exit(0);
    }

    pid_chauffage = fork();
    if (pid_chauffage < 0) {
        perror("Erreur de fork pour Chauffage");
        exit(1);
    } else if (pid_chauffage == 0) {
        Chauffage();
        exit(0);
    }

    pid_ventilation = fork();
    if (pid_ventilation < 0) {
        perror("Erreur de fork pour Ventilation");
        exit(1);
    } else if (pid_ventilation == 0) {
        Ventilation();
        exit(0);
    }

    // Dans le processus parent, attendre que les enfants se terminent
    for (int i = 0; i < 3; i++) {
        wait(NULL); // Attendre tous les processus enfants
    }

    printf("Tous les processus enfants ont terminé.\n");
    return 0;
}
