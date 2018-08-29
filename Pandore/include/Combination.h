/**
 *  @class : pnd::Combination (composée d'un std::map)
 *  @brief : Ensemble de pairs(clé,valeur) dont les types sont données par le type combinationType.
 *  @note :
 *  * Pour l'affichage des combinaisons, on veut que ça se fasse de la manière suivante :
    * site - pseudo - mot de passe
    * mail - adresse - mot de passe
    * site - mot de passe
 *  @author : Naysson
 */

#ifndef __COMBINATION_INCLUDED__

#define __COMBINATION_INCLUDED__

#include <map>
#include <algorithm>
#include <vector>
#include "String.h"

//#define __MODE_DEBUG__COMBINATION__

namespace pnd
{
    namespace cst
    {
        const unsigned short maxCombinationTypes = 4;
    }

    enum class combinationType {quit,normal,email,password,mixed,end};

    class cmpLength
    {
    public:
        bool operator()(const std::string& a, const std::string& b)
        {
            return a.length() < b.length();
        }
    };


class Combination
    {
    public:
        Combination();
        explicit Combination(combinationType type);
        Combination(combinationType type, std::map<std::string, String, cmpLength> const&);
        ~Combination() = default;

        combinationType getCombinationType() const;
        unsigned short getCombinationTypeValue() const;
        void setCombinationType(const combinationType& newType);

        void setData(std::map<std::string, String, cmpLength> const&);
        std::map<std::string, String, cmpLength> getData() const;

        void addField( std::pair<std::string, String> const& );
        void removeField( std::string const& );    // supprime la combinaison dont la clé correspond à l'argument.
        void modifyField( std::pair<std::string, String> const& );     // modifie la valeur de la combinaison dont la clé correponds au 1er argument, par le deuxième argument.

        static std::vector<std::string> mask(combinationType);     //Donne un tableau de clés (std::string) en fonction du type donnée.
        static bool validKeyType(combinationType type, std::string const& key);     //Verifie si la clé donnée est compatible avec le type.

    friend std::ostream &operator<<(std::ostream &stream, Combination const &thisOne);

    protected:
        std::map<std::string, String, cmpLength> data;
        combinationType type;

    };

}
#endif