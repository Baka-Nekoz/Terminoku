#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define TAILLE 9
typedef int tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g){
    char nomFichier[30];
    FILE * f;
    //Demande du nom de fichier à l'utilisateur
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    //Tentative d'ouverture du fichier dont le nom a été donné par l'utilisateur
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        //Le fichier donné n'existe pas :(
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        //Le fichier existe, mettre ses valeurs dans un type tGrille.
        fread(g, sizeof(int), TAILLE*TAILLE, f);
        //Fermeture du fichier.
        fclose(f);
    }
    
}

// Procédure pour afficher la grille dans le terminal
void afficherGrille(tGrille grille) {
    //affichage des deux premières lignes de construction de la grille
    printf("    1  2  3    4  5  6    7  8  9\n");
    printf("  +----------+----------+----------+\n");

    //Ajouter une ligne intermédiaire pour séparer toutes les 3 lignes
    for (int ligne = 0; ligne < 9; ligne++) {
        if (ligne % 3 == 0 && ligne != 0) {
            printf("  +----------+----------+----------+\n");
        }
        printf("%d | ", ligne + 1);
        //Ajout d'une barre séparatrice toutes les 3 valeurs
        for (int colonne = 0; colonne < 9; colonne++) {
            if (colonne % 3 == 0 && colonne != 0) {
                printf("| ");
            }
            //Print un . suivi de deux espaces si le caractères est un 0 (la case est vide)
            if (grille[ligne][colonne] == 0) {
                printf(".  ");
            //Print le caractère de la grille correspondant à la ligne et la colonne en cours dans la boucle si ce n'est pas un 0.
            } else {
                printf("%d  ", grille[ligne][colonne]);
            }
        }
        printf("|\n");
    }
    //Affiche la ligne de fin de grille
    printf("  +----------+----------+----------+\n");
}

void saisir(int *valeurFinale, int n){
    //Initialisation de valeur qui sera la valeur finale et de temporaire, qui servira de variable temporaire pour stocker l'entrée de l'utilisateur.
    int valeur = 0;
    char temporaire[100];

    do {
        //Mettre l'entére dans l'utilisateur dans la variable temporaire sans avoir beosin de la formater avec un scanf
        fgets(temporaire, sizeof(temporaire), stdin);
        //Vérification de la faisabilité de la conversion de temporaire en un entier.
        if (sscanf(temporaire, "%d", &valeur) != 0){
            if (valeur >= 1 && valeur <= n) {
                //La valeur est valide, et comprise entre 1 et n², on assigne donc cette valeur à valeurFinale
                *valeurFinale = valeur;
                break;
            } else {
                //La valeur est bien un entier mais n'est pas comprise entre 1 et n²
                printf("La valeur doit être comprise entre 1 et %d.\n", TAILLE);
            }
        } else {
            //La valeur n'est pas un entier.
            printf("Veuillez entrer un entier valide.\n");
        }
    } while (1);
}

//Fonction booléenne qui vérifie si une valeur est déjà dans une ligne, colonne ou bloc.
bool verifierValeur(tGrille grille, int ligne, int colonne, int valeur) {
    //vérification de la ligne en parcourant chaque élément de celle ci à la recherche de la valeur souhaitée par l'utilisateur.
    for (int i = 0; i < TAILLE; i++) {
        if (grille[ligne-1][i] == valeur) {
            printf("La valeur est déjà sur la même ligne.\n");
            return false;
        }
    }

    //vérification de la colonne en parcourant chaque élément de celle ci à la recherche de la valeur souhaitée par l'utilisateur.
    for (int j = 0; j < TAILLE; j++) {
        if (grille[j][colonne-1] == valeur) {
            printf("La valeur est déjà sur la même colonne.\n");
            return false;
        }
    }
    
    //vérification du bloc en parcourant chaque élément de celui ci à la recherche de la valeur souhaitée par l'utilisateur.
    /*
    int debutBlocLigne = (ligne / 3) * 3;
    int debutBlocColonne = (colonne / 3) * 3;

    for (int k = debutBlocLigne; k < debutBlocLigne + 3; k++) {
        for (int l = debutBlocColonne; l < debutBlocColonne + 3; l++) {
            if (grille[k][l] == valeur) {
                printf(" valeur est déjà dans le même bloc.\n");
                return false;
            }
        }
    }
    */
    return true;
}

//Fonction qui vérifie si la grille est remplie (retourne 0 si elle trouve ne serait-ce qu'un seul 0 et retourne 1 si elle n'en trouve aucun après avoir tout parcouru)
int verifierPleine(tGrille grille) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    // Déclaration des variables et types
    tGrille grilleSudoku;
    int valeurSaisie;
    int numLigne = 0;
    int numColonne = 0;
    // Initialisation de la grille
    chargerGrille(grilleSudoku);
    //Boucle tourne tant que la grille n'est pas remplie.
    while (verifierPleine(grilleSudoku) == 0){
        //Affichage de la grille
        afficherGrille(grilleSudoku);
        //Entrée et vérification de la validité des valeurs que l'utilisateur souhaite entrer
        printf("Indices de la case ?\n");
        printf("Entrez une valeur de ligne entre 1 et %d : ", TAILLE);
        saisir(&numLigne, TAILLE);
        printf("Entrez une valeur de colonne entre 1 et %d : ", TAILLE);
        saisir(&numColonne, TAILLE);
        //Vérification de la plaçabilité (ça se dit ?) des valeurs entrées par l'utilisateur.
        if (grilleSudoku[numLigne-1][numColonne-1] != 0) {
            //La case n'est pas libre.
            printf("IMPOSSIBLE, la case n'est pas libre.\n");
        }else {
            //La case est libre, demande de la valeur à insérer.
            printf("Valeur à insérer ?\n");
            //Vérification de la validité de cette valeur.
            saisir(&valeurSaisie, TAILLE);
            //Vérification de la possibilité du placement de cette valeur.
            if (verifierValeur(grilleSudoku, numLigne, numColonne, valeurSaisie)) {
                grilleSudoku[numLigne-1][numColonne-1] = valeurSaisie;
            }
        }
    }
    //Si on arrive ici, c'est donc que la boucle a fini de tourner et que la grille est pleine.
    printf("Grille pleine, fin de partie");
}