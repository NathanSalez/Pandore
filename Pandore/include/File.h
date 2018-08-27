/**
 *  @brief : Ensemble de fonctions servant à manipuler les données avec un fichier binaire.
 *  @note : - Les emplacements des fichiers binaire et texte sont données par les variables dataFile***Path.
 *  @author : Naysson
 */

#ifndef __FILE_INCLUDED__

#define __FILE_INCLUDED__


#include <fstream>
#include "Combination.h"


namespace cst
{
    const std::string dataFileTextPath("./files/data.pass");
    const std::string dataFileTextPathSecret("./files/.help/data.pass");

    const unsigned short minLength = 10;    //Longueur minimum du mot de passe.
}

//#define __MODE_DEBUG__FILE__

namespace pnd
{
    void saveStandardString(std::string const&, std::ofstream &);
    void saveString(String &, std::ofstream &);     //On chiffre
    void saveCombination( Combination &, std::ofstream &);

    std::string loadStandardString(std::ifstream &);
    String loadString(std::ifstream &);             //On déchiffre
    Combination loadCombination(std::ifstream &);

    bool is_readable(const char *);
}

#endif