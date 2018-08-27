#include "../include/CombinationList.h"

namespace pnd {

    CombinationList::CombinationList() : passwordApp(""), pathDataFile("/dev/null") {}       //Constructeur par défaut, on ne fait rien.

    CombinationList::CombinationList(pnd::String password) : passwordApp(password) {
#ifdef __MODE_DEBUG__COMBINATIONLIST__
        std::clog << "CombinationList::CombinationList(pnd::String &) - Entrée dans la fonction." << std::endl;
#endif
        this->pathDataFile = "/dev/null";       //On évite de faire des sauvegardes malencontreuses.
    }


    CombinationList::CombinationList(const std::string& fileNameData) : passwordApp(""), pathDataFile(fileNameData) {
#ifdef __MODE_DEBUG__COMBINATIONLIST__
        std::clog << "CombinationList::CombinationList(const char*) - Entrée dans la fonction." << std::endl;
#endif
        this->pathDataFile = fileNameData;
        // 1ere étape, on initialise un flux de lecture
        std::ifstream fileStream(fileNameData, std::ios::in);
        if ( fileStream.is_open() )
        {
            //On charge d'abord le mot de passe de l'application.
#ifdef __MODE_DEBUG__COMBINATIONLIST__
            std::clog << "CombinationList::CombinationList(const char*) - L'ouverture du fichier a réussi." << std::endl;
#endif
            this->passwordApp = loadString(fileStream);
#ifdef __MODE_DEBUG__COMBINATIONLIST__
            std::clog << "CombinationList::CombinationList(const char*) - passwordApp = " << this->passwordApp << std::endl;
#endif
            if( this->passwordApp.getData().length() < cst::minLength)      // Attention au changement du mot de passe.
                throw Exception(25,"CombinationList(const std::string&)",
                                "Chaine modifiée dans le fichier.", combinationList);

            //Puis la liste des combinaisons
            unsigned long size = 0;
            fileStream >> size;
            if( size !=0 )
            {
                for(unsigned long i=0; i <size;i++)
                {
                    if ( !fileStream.eof() ) {
                        this->dataList.push_back(loadCombination(fileStream));
                    }
                    else { // L'utilisateur a modifié size : ce dernier est devenu plus petit.
                        throw Exception(24, "CombinationList(const std::string&)",
                                        "Valeur de size erronée dans le fichier.", combinationList);
                    }
                }
            }
            fileStream.close();
        }
        else
        {
            throw Exception(31,"CombinationList(const char*)","Le fichier de données n'a pas pu être ouvert.",combinationList);
        }

    }

    CombinationList::~CombinationList() {
#ifdef __MODE_DEBUG__COMBINATIONLIST__
        std::clog << "CombinationList::~CombinationList() - Entrée dans la fonction." << std::endl;
        std::clog << "Fichier affecté : " << this->pathDataFile << std::endl;
#endif
        std::ofstream fileStream(this->pathDataFile, std::ios::out);
        if ( fileStream.is_open() )
        {
            //On enregistre d'abord le mot de passe de l'application.
            saveString(this->passwordApp,fileStream);

            //Puis la liste des combinaisons
            fileStream << this->dataList.size() << std::endl;
            for(auto it = this->dataList.begin(); it != this->dataList.end(); it++)
            {
                saveCombination(*it,fileStream);
            }

            fileStream.close();
        }
        else
        {
            throw Exception(31,"~CombinationList()","Le fichier de données n'a pas pu être ouvert.",combinationList);
        }

    }


/// GETTER/SETTER
    void CombinationList::setPassword(String &password) {
        if( !password.getData().empty() )
        {
            this->passwordApp = password;
        }
        else
        {
            throw Exception(32,"void setPassword(String&)","Mot de passe vide.",combinationList);
        }
    }


    String CombinationList::getPassword() {
        return this->passwordApp;
    }

    std::vector<Combination> CombinationList::getDataList()
    {
        return this->dataList;
    }

    void CombinationList::setPathDataFile(std::string const& filePath)
    {
        this->pathDataFile = filePath;
    }

    std::string CombinationList::getPathDataFile() {
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
            throw Exception(33,"void removeCombination(unsigned long)","Index situé en dehors de l'intervalle prévue.",combinationList);
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
                throw Exception(11,"void modifyCombination(unsigned long, std::pair<std::string, String> const&)","La clé en argument est incompatible avec le type de la combinaison.",combinationList);
            }
        }
        else
        {
            throw Exception(33,"void modifyCombination(unsigned long, std::pair<std::string, String> const&)","Index situé en dehors de l'intervalle prévue.",combinationList);
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
