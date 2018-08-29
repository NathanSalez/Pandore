/**
 *  @brief : Ensemble de fonctions utiles pour la saisie et l'affichage de données.
 *  @note : - Les emplacements des fichiers binaire et texte sont données par les variables dataFile***Path.
 *  @author : Naysson
 */

#ifndef __UTILS_INCLUDED__

#define __UTILS_INCLUDED__

#include <limits>
#include "libFile.h"


//define __MODE_DEBUG__UTILS__

template <typename T> void readChoice(T& choix,T min, T max);  // pour lire un entier,flottant,booléen dans l'intervalle [min,max]
template<typename T> void readChoice(T &choix, T min, T max)       //cf developpez.com
{
    if (max < min) {
        throw pnd::Exception(44, "template <typename T> void readChoice(T& choix,T min, T max)",
                             "min est supérieure à max", pnd::level::main);
    }
    std::cout << "Votre choix : ";
    while (!(std::cin >> choix) || choix < min || choix > max) {
        if (std::cin.eof()) {
            // ^D  (^Z sous windows); Fin du flux d'entree !
            throw pnd::Exception(45, "template <typename T> void readChoice(T& ,T , T )", "Fin du flux d'entrée.",
                                 pnd::level::main);
        } else if (std::cin.fail()) {
            std::cout << "Saisie incorrecte, veuillez recommencer : ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "Le chiffre n'est pas entre " << min << " et " << max << ", veuillez recommencer : ";
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n');   // on enlève le '\n' à la fin de la saisie.
}
void readString(const std::string& description, std::string& answer);

void helpPrint();
void passwordMenuPrint(int&);
void mainMenuPrint(int&);
void SearchMenuPrint(int&);
void typeCombinationMenuPrint(pnd::combinationType& );      // Affiche chaque combinaisonType et ses champs/clés
void keyCombinationTypePrint(pnd::combinationType const&, int& ); // Affiche chaque champs/clés de la combinaisonType en paramètre.



#endif