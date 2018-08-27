/**
 *  @class : pnd::Exception (dérivée de std::exception
 *  @brief : Gestion des erreurs de l'application Pandore
 *  @note : Création d'un enum pour gérer le niveau de l'exception.
 *  @author : Naysson
 */

 /*
  * Liste des numéros d'exceptions :
  * String.cpp
  *     1 - "La clé entrée est trop grande."
  *     2 - "La clé entrée est trop petite."
  *
  * Combination.cpp
  *     11 - "La clé en argument est incompatible avec le type de la combinaison."
  *     12 - "La clé n'a pas été trouvée dans la combinaison."
  *     13 - "Le type en argument n'existe pas."
  *
  * File.cpp
  *     21 - "Problème de manipulation du fichier."
  *     22 - "Valeur de type erronée dans le fichier."
  *     23 - "Valeur de clé erronée dans le fichier."
  *     24 - "Valeur de size erronée dans le fichier."
  *     25 - "Chaine modifiée dans le fichier."
  *
  * CombinationList.cpp
  *     31 - "Le fichier de données n'a pas pu être ouvert."
  *     32 - "Mot de passe vide."
  *     33 - "Index situé en dehors de l'intervalle prévue."
  *
  * main.cpp
  *     41 - "Nombre maximum de tentatives atteint."
  *     42 - "Arguments de l'application incorrects."
  *     43 - "Type non connu." - NON UTILISE
  *     44 - "min est supérieure à max"
  *     45 - "Fin du flux d'entrée."
  *     46 - "Argument 1 invalide."
  *     47 - "Flux d'entrée endommagée."
  *
  */
#ifndef __EXCEPTION_INCLUDED__

#define __EXCEPTION_INCLUDED__

#include <iostream>
#include <sstream>
#include <exception>

namespace pnd {

    enum level {string, combination, file , combinationList, main };

    class Exception : public std::exception
            {
    public:
        Exception(int numero , std::string const& nomFonction, std::string const &phrase, level niveau);

        virtual const char *what() const noexcept;

        level getNiveau() const noexcept;
        int getNumero() const noexcept;

         ~Exception() = default;

    protected:
        int number;               //Numéro de l'erreur
        std::string description;            //Description de l'erreur
        level niveau;               //Niveau de l'erreur
    };

}

#endif