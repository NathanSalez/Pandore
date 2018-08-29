/**
 *  @class : pnd::CombinationList (composée d'un std::vector)
 *  @brief : Classe constituée d'un ensemble de combinaisons et d'un mot de passe pour l'application.
 *  @note : Le constructeur (const char *) et le destructeur ont respectivement pour but de charger et sauvegarder les données.
 *  @author : Naysson
 */

#ifndef __COMBINATIONLIST_INCLUDED__

#define __COMBINATIONLIST_INCLUDED__

#include "Combination.h"

//#define __MODE_DEBUG__COMBINATIONLIST__

namespace pnd
{
    class CombinationList {

    public:
        CombinationList();
        explicit CombinationList(String password);      // A utiliser au 1er lancement de l'application
        explicit CombinationList(const std::string& fileNameData);    //A utiliser la plupart du temps
        ~CombinationList()=default;     // dans ce destructeur, on sauvegarde tous les données


        void addCombination(Combination const &);
        void removeCombination(unsigned long place);    // CONVENTION : 1er élément <=> place = 0
        void modifyCombination(unsigned long place, std::pair<std::string, String> const& );
        //modifier la valeur du champ (caractérisé par le std::string) de la combinaison (position caractérisé par place) par la valeur String.




        void setPassword(String &password);
        pnd::String getPassword() const;

        std::vector<Combination> getDataList() const;

        void setPathDataFile(std::string const&);
        std::string getPathDataFile() const;

        friend std::ostream &operator<<(std::ostream &stream, CombinationList const &thisOne);

    protected:
        std::vector<Combination> dataList;      //Ce tableau de combinaison doit être trié en fonction de leur type.
        pnd::String passwordApp;
        std::string pathDataFile;

    };
}

#endif
