#include "../include/CombinationList.h"

namespace pnd {

    CombinationList::CombinationList() : passwordApp(""), pathDataFile("/dev/null") {}       //Constructeur par défaut, on ne fait rien.

    CombinationList::CombinationList(pnd::String password) : passwordApp(password) {
#ifdef __MODE_DEBUG__COMBINATIONLIST__
        std::clog << "CombinationList::CombinationList(pnd::String &) - Entrée dans la fonction." << std::endl;
#endif
        this->pathDataFile = "/dev/null";       //On évite de faire des sauvegardes malencontreuses.
    }

    CombinationList::CombinationList(const std::string& fileNameData) : passwordApp(""), pathDataFile(fileNameData) {}


/// GETTER/SETTER
    void CombinationList::setPassword(String &password) {
        if( !password.getData().empty() )
        {
            this->passwordApp = password;
        }
        else
        {
            throw Exception(22,"void setPassword(String&)","Mot de passe vide.",level::combinationList);
        }
    }


    String CombinationList::getPassword() const
    {
        return this->passwordApp;
    }

    std::vector<Combination> CombinationList::getDataList() const
    {
        return this->dataList;
    }

    void CombinationList::setPathDataFile(std::string const& filePath)
    {
        this->pathDataFile = filePath;
    }

    std::string CombinationList::getPathDataFile() const
    {
        return this->pathDataFile;
    }


    void CombinationList::addCombination(Combination const &newOne) {
        this->dataList.push_back(newOne);
    }


    void CombinationList::removeCombination(unsigned long place) {
        //place varie de 0 à length-1
        if( place >= 0 && place <= this->dataList.size() - 1 )
        {
            this->dataList.erase( this->dataList.begin() + place);
        }
        else
        {
            throw Exception(23,"void removeCombination(unsigned long)","Index situé en dehors de l'intervalle prévue.",level::combinationList);
        }
    }


    void CombinationList::modifyCombination(unsigned long place, std::pair<std::string, String> const& paire ) {
        //place varie de 0 à length-1
        if( place >= 0 && place <= this->dataList.size() - 1 )
        {
            // On vérifie la compatibilité entre la combinaison à modifier et la clé donnée
            if( Combination::validKeyType( this->dataList[place].getCombinationType(), paire.first ) )
            {
                // A ce moment-là, on peut effectuer la modification
                this->dataList[place].modifyField(paire);
            }
            else
            {
                throw Exception(11,"void modifyCombination(unsigned long, std::pair<std::string, String> const&)","La clé en argument est incompatible avec le type de la combinaison.",level::combinationList);
            }
        }
        else
        {
            throw Exception(23,"void modifyCombination(unsigned long, std::pair<std::string, String> const&)","Index situé en dehors de l'intervalle prévue.",level::combinationList);
        }
    }

    std::ostream &operator<<(std::ostream &stream, CombinationList const &thisOne) {
        stream << "\n--------------LISTE DES COMBINAISONS--------------" << std::endl;
        for (unsigned long compteur = 0; compteur < thisOne.dataList.size() ; compteur++)
        {
            stream << compteur << " : " << thisOne.dataList[compteur] << std::endl;
        }
        stream << "------------------------FIN------------------------" << std::endl;
        return stream;
    }

}
