/**
 *  @brief : Ensemble de fonctions servant à manipuler les données avec un texte.
 *  @note : - Les emplacements des fichiers binaire et texte sont données par les variables dataFile***Path.
 *  @author : Naysson
 */

#ifndef __FILE_INCLUDED__

#define __FILE_INCLUDED__


#include <fstream>
#include "CombinationList.h"



//#define __MODE_DEBUG__FILE__

namespace pnd
{
    namespace cst
    {
        const std::string dataFileTextPath("./files/data.pass");
        const std::string dataFileTextPathSecret("./files/.help/data.pass");

        const unsigned short minLength = 10;    //Longueur minimum du mot de passe.
    }

    void saveStandardString(const std::string &, std::ofstream &);
    void saveString(String &, std::ofstream &);     //On chiffre
    void saveCombination(Combination &, std::ofstream &);
    void saveCombinationList(CombinationList &, std::ofstream &);


    std::string& loadStandardString(std::ifstream &, std::string&);
    String& loadString(std::ifstream &, String&);             //On déchiffre
    Combination& loadCombination(std::ifstream &, Combination&);
    CombinationList& loadCombinationList(std::ifstream &, CombinationList& );

    bool is_readable(const char *);
}

#endif