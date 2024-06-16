/**
 * \page Sudoku
 * \author Maël_Gogdet
 * \file sudoku.c
 * \date 25/11/2023
 * \brief Programme du jeu de Sudoku à la demande d'un client.
 * 
 * Ce programme est un jeu de sudoku simple commandé par un client. Il charge une grille, l'affiche et demande à l'utilisateur de la remplir jusqu'à ce qu'elle soit pleine.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h> //utf8

/**
 * \brief Définit la taille en n*n de la grille qui sera utilisée pour jouer.
 */
#define TAILLE 9

/**
 * \brief Crée un tableau en 2 dimensions de taille TAILLE qui servira de réceptacle à la grille.
 */
typedef int tGrille[TAILLE][TAILLE];

/**
 * \fn void chargerGrille(tGrille g)
 * 
 * \brief Procédure qui charge la grille de sudoku à aprtir d'un fichier.
 *
 * Cette procédure charge une grille depuis un fichier .sud. Le fichier doit exister et être valide.
 *
 * \param g Tableau à charger.
 */
void chargerGrille(tGrille g){
    char nomFichier[30];
    FILE * f;
    
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        
        fread(g, sizeof(int), TAILLE*TAILLE, f);
        
        fclose(f);
    }
    
}

/**
 * \fn void afficherGrille(tGrille grille)
 * 
 * \brief Procédure qui affiche la grille dans le terminal.
 *
 * Cette fonction affiche la grille dans le terminal.
 *
 * \param grille Le tableau (grille) à afficher.
 */
void afficherGrille(tGrille grille) {
    
    printf("    1  2  3    4  5  6    7  8  9\n");
    printf("  +----------+----------+----------+\n");

    
    for (int ligne = 0; ligne < 9; ligne++) {
        if (ligne % 3 == 0 && ligne != 0) {
            printf("  +----------+----------+----------+\n");
        }
        printf("%d | ", ligne + 1);
        
        for (int colonne = 0; colonne < 9; colonne++) {
            if (colonne % 3 == 0 && colonne != 0) {
                printf("| ");
            }
            
            if (grille[ligne][colonne] == 0) {
                printf(".  ");
            
            } else {
                printf("%d  ", grille[ligne][colonne]);
            }
        }
        printf("|\n");
    }
    
    printf("  +----------+----------+----------+\n");
}

/**
 * \fn void saisir(int *valeurFinale, int n)
 * 
 * \brief Procédure qui permet de saisir une valeur.
 *
 * Cette procédure permet de saisir une valeur et de vérifier qu'elle est bien entre 1 et n².
 *
 * \param n Valeur saisie par l'utilisateur.
 * \param valeurFinale Valeur correcte qui sera utilisée.
 */
void saisir(int *valeurFinale, int n){
    
    int valeur = 0;
    char temporaire[100];

    do {
        
        fgets(temporaire, sizeof(temporaire), stdin);
        
        if (sscanf(temporaire, "%d", &valeur) != 0){
            if (valeur >= 1 && valeur <= n) {
                
                *valeurFinale = valeur;
                break;
            } else {
                
                printf("La valeur doit être comprise entre 1 et %d.\n", TAILLE);
            }
        } else {
            
            printf("Veuillez entrer un entier valide.\n");
        }
    } while (1);
}

/**
 * \fn bool verifierValeur(tGrille grille, int ligne, int colonne, int valeur)
 * 
 * \brief Fonction qui vérifie si une valeur est déjà sur une même ligne, colonne, ou bloc.
 *
 * Cette fonction vérifie si une valeur est placable sur les coordonnées données en paramètres dans la grille.
 *
 * \param ligne Ligne de la valeur
 * \param colonne Colonne de la valeur
 * \param valeur Valeur à vérifier
 * \param grille Grille dans laquelle vérifier la valeur.
 *
 * \return Vrai si la valeur peut être placée à l'endroit désiré par l'utilisateur, Faux sinon.
 */
bool verifierValeur(tGrille grille, int ligne, int colonne, int valeur) {
    
    for (int i = 0; i < TAILLE; i++) {
        if (grille[ligne-1][i] == valeur) {
            printf("La valeur est déjà sur la même ligne.\n");
            return false;
        }
    }

    
    for (int j = 0; j < TAILLE; j++) {
        if (grille[j][colonne-1] == valeur) {
            printf("La valeur est déjà sur la même colonne.\n");
            return false;
        }
    }
    
    
   
    return true;
}

/**
 * \fn int verifierPleine(tGrille grille)
 * 
 * \brief Fonction qui vérifie si la grille est pleine.
 *
 * Cette fonction vérifie si une grille est remplie.
 *
 * \param grille Grille à vérifier
 *
 * \return Vrai si la grille est pleine, Faux sinon.
 */
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

/**
 * \fn int main()
 * 
 * \brief Programme principal
 * 
 * Cette fonction est le programme principal du sudoku, elle appelle toutes les autres procédures et fonctions de manière ordonnée, et tourne tant que la grille n'est pas pleine.
 * 
*/
int main() {
    
    tGrille grilleSudoku;
    int valeurSaisie;
    int numLigne = 0;
    int numColonne = 0;
    
    chargerGrille(grilleSudoku);
    
    while (verifierPleine(grilleSudoku) == 0){
        
        afficherGrille(grilleSudoku);
        
        printf("Indices de la case ?\n");
        printf("Entrez une valeur de ligne entre 1 et %d : ", TAILLE);
        saisir(&numLigne, TAILLE);
        printf("Entrez une valeur de colonne entre 1 et %d : ", TAILLE);
        saisir(&numColonne, TAILLE);
        
        if (grilleSudoku[numLigne-1][numColonne-1] != 0) {
            
            printf("IMPOSSIBLE, la case n'est pas libre.\n");
        }else {
            
            printf("Valeur à insérer ?\n");
            
            saisir(&valeurSaisie, TAILLE);
            
            if (verifierValeur(grilleSudoku, numLigne, numColonne, valeurSaisie)) {
                grilleSudoku[numLigne-1][numColonne-1] = valeurSaisie;
            }
        }
    }
    
    printf("Grille pleine, fin de partie");
}