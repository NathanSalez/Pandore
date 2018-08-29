/**
 * @brief : Programme principal de l'application Pandore version console.
 * @author : Naysson
 * @todo : - Proposer une recherche de combinaisons par champ.     DONE
 *         - Optimiser l'utilisation de la mémoire.         DON
 *         - Utiliser cette recherche pour la suppression et modification.
 *         - Proposer une question personnelle au permier lancement de l'application.
 *
 *         \}]/Vs3}6gfwD7>#
 */

#include <ctime>
#include "../include/libUtils.h"

namespace pnd::cst
{
    const unsigned short maxAttempts = 5;
    const unsigned short minNumbersInPassword = 2;
    const unsigned short minSpecialsInPassword = 2;
}

//#define __MODE_DEBUG__MAIN__

void makeCombination(pnd::Combination&);
void fillFields(pnd::Combination&);     // pour remplir un pnd::COmbination en fonction de son type

void buildNewPassword(int& choix, pnd::String& newOne);
bool verifyPassword(const pnd::String &);     // utilisé lorsque l'utilisateur crée un mot de passe.

pnd::CombinationList& loadDataApp(pnd::CombinationList& dataApp);

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/

int main(int argc, char **argv) {
    srand((unsigned int) time(nullptr));       //Pour l'obtention des clés dans les constructeurs de String.

    try {
        std::cout << std::endl << std::endl;
        pnd::CombinationList dataList;          // INSTANCE UTILISEE AU COURS DE TOUTE L'APPLICATION
        if (argc == 1) {

            std::cout << "Bienvenue sur l'application Pandore, créé par Naysson." << std::endl << std::endl;
            // L'application a-t-elle déjà été lancée ? On vérifie cela avec la présence du fichier data.pass et du fichier de secours.
            if (pnd::is_readable(pnd::cst::dataFileTextPath.c_str()) || pnd::is_readable(pnd::cst::dataFileTextPathSecret.c_str())  ) {

                //On appelle le constructeur suivant (le fichier data.pass existe)
                pnd::CombinationList list;
                loadDataApp(list);

                //On demande le mot de passe à l'utilisateur.
                std::string sentPassword;
                unsigned short attempts = 0;
                bool valid = false;
                do {
                    attempts++;
                    readString("Mot de passe de l'application : ",sentPassword);

                    if (sentPassword == list.getPassword().getData()) {
                        valid = true;
                    }
                } while (attempts < pnd::cst::maxAttempts && !valid);

                if (valid)
                {
                    std::cout << "Mot de passe correct, chargement en cours..." << std::endl;
                    dataList = list;

                }
                else
                {
                    throw pnd::Exception(41, "main(int, char **)",
                                         "Nombre maximum de tentatives atteint.",
                                         pnd::level::main);
                }
            }
            else
            {
                //On doit construire le mot de passe de l'application.
                int choix;
                passwordMenuPrint(choix);
                pnd::String newPassword;
                buildNewPassword(choix,newPassword);

                pnd::CombinationList list( newPassword );
                std::cout << "Mot de passe correct, chargement en cours..." << std::endl;
                dataList = list;

            }

            //A présent, on commence l'utilisation de l'application.
            int choix = 0;
            do {
                mainMenuPrint(choix);
                switch(choix)
                {
                    case 1 : // lire l'ensemble des combinaisons.
                    {
                        if (!dataList.getDataList().empty()) {
                            std::cout << dataList << std::endl;
                        } else {
                            std::cerr << "Aucune combinaison disponible." << std::endl;
                        }
                        break;
                    }       // On met des accolades pour expliciter la portée des variables

                    case 2 : // ajouter une combinaison.
                    {
                        pnd::Combination newCombination;
                        makeCombination(newCombination);
                        if( ! newCombination.getData().empty()) {
                            dataList.addCombination(newCombination);
                            std::cout << "Ajout de la combinaison réussie !" << std::endl;
                        }
                        else
                        {
                            std::cout << "Retour au menu précédent." << std::endl;
                        }
                        break;
                    }

                    case 3 : // modifier une combinaison.
                    {
                        if( !dataList.getDataList().empty() ) {
                            int combinationChoice;
                            int fieldChoice;

                            std::cout
                                    << "Quelle combinaison voulez-vous modifier ? (Les numéros sont données par l'affichage)"
                                    << std::endl;
                            readChoice(combinationChoice, 0, static_cast<int>(dataList.getDataList().size() - 1));

                            pnd::combinationType type = dataList.getDataList()[combinationChoice].getCombinationType();       // On trouve le type associé à la combination choisie.

                            // On demande le champ à modifier.
                            keyCombinationTypePrint(type, fieldChoice);
                            if (fieldChoice <
                                0)       // Cf fonction en bas (si on retourne au menu précédent, alors fieldChoice = -1)
                            {
                                std::cout << "Retour au menu précédent." << std::endl;
                            } else {
                                std::string field = pnd::Combination::mask(
                                        type)[fieldChoice];      // On trouve la clé du champ à modifier.

                                std::string dataContent;
                                readString("Nouveau contenu du champ " + field + " : ",dataContent);
                                pnd::String newContent(dataContent);


                                std::pair<std::string, pnd::String> newPair(field, newContent);
                                dataList.modifyCombination(static_cast<unsigned long>(combinationChoice), newPair);

                                std::cout << "Modification du champ " << field << " de la combinaison "
                                          << combinationChoice
                                          << " réussie !" << std::endl;
                            }
                        }
                        else
                        {
                            std::cerr << "Aucune combinaison a été créée. Modification impossible." << std::endl;
                        }
                        break;
                    }

                    case 4 : // supprimer une combinaison.
                    {
                        if ( !dataList.getDataList().empty() )
                        {
                            std::cout
                                    << " Quel combinaison voulez-vous supprimer ? (Les numéros sont données par l'affichage)"
                                    << std::endl;
                            int choice = 0;
                            readChoice(choice, 0, static_cast<int>(dataList.getDataList().size() - 1));
                            dataList.removeCombination(static_cast<unsigned long>(choice));
                            std::cout << "Suppression de la combinaison " << choice << " réussie !" << std::endl;
                        }
                        else
                        {
                            std::cerr << "Aucune combinaison a été créée. Suppression impossible." << std::endl;
                        }
                        break;
                    }

                    case 5 : // rechercher une combinaison dans la liste.
                    {
                        if ( !dataList.getDataList().empty() ) {
                            int choixRecherche = 0;

                            SearchMenuPrint(choixRecherche);
                            std::string key, description;
                            switch (choixRecherche) {
                                case 0 :
                                    std::cout << "Retour au menu précédent." << std::endl;
                                    break;

                                case 1 : // recherche par identifiant
                                {
                                    int id = 0;
                                    std::cout << "Donnez l'identifiant de la combinaison." << std::endl;
                                    readChoice(id, 0, static_cast<int>(dataList.getDataList().size() - 1));

                                    std::cout << "Combinaison recherchée :" << std::endl;
                                    std::cout << id << " : " << dataList.getDataList()[id] << std::endl;
                                    break;
                                }

                                case 2 : // recherche par site
                                    key = "site";
                                    description = "Ecrivez le nom du " + key + " : ";
                                    break;

                                case 3 : // par mot de passe
                                    key = "mot de passe";
                                    description = "Ecrivez le " + key + " : ";
                                    break;

                                case 4 : // par pseudo
                                    key = "pseudo";
                                    description = "Ecrivez le " + key + " : ";
                                    break;

                                case 5 : // par adresse mail
                                    key = "adresse";
                                    description = "Ecrivez l'adresse mail : ";
                                    break;

                                default:
                                    throw pnd::Exception(43,"main(int,char**)","Type non connu.",pnd::level::main);
                            }
                            if (2 <= choixRecherche && choixRecherche <= 5)
                            {   // On regroupe toutes les clés,
                                std::string searched;
                                readString(description,searched);
                                std::vector<pnd::Combination> annexeDataList = dataList.getDataList();
                                bool notFirstFound = true;

                                for (unsigned long identifiant = 0; identifiant < annexeDataList.size(); identifiant++)
                                {
                                    // On vérifie que la clé existe dans la combinaison
                                    auto iterator = annexeDataList[identifiant].getData().find(key);
                                    if( iterator != annexeDataList[identifiant].getData().end() )
                                    {
                                        // On utilise la méthode find() pour trouver une occurence de <value> dans dataList.getDataList[identifiant].getData()[<key>]
                                        // En effet, on cherche une occurence de searched dans actualValue
                                        std::string actualValue(annexeDataList[identifiant].getData()[key].getData());

                                        if (actualValue.find(searched) !=
                                            std::string::npos)   // npos est la valeur retournée dans le cas d'échec.
                                        {
                                            if (notFirstFound) {
                                                std::cout << "Combinaisons correspondants à la recherche (" << key
                                                          << " => '" << searched << "') :" << std::endl;
                                                notFirstFound = false;
                                            }
                                            std::cout << identifiant << " : " << annexeDataList[identifiant]
                                                      << std::endl;
                                        }
                                    }
                                }

                                if (notFirstFound)
                                {
                                    std::cout << "\nAucune combinaison a été trouvée." << std::endl;
                                }
                            }
                        }
                        else
                        {
                            std::cerr << "Aucune combinaison a été créée, recherche impossible." << std::endl;
                        }

                        break;


                    }

                    case 6 : // changer le mot de passe de l'appli.
                    {
                        int choice = 0;
                        std::string sentPassword;
                        readString("Mot de passe de l'application : ",sentPassword);

                        if (sentPassword == dataList.getPassword().getData()) {
                            passwordMenuPrint(choice);
                            pnd::String newPassword;
                            buildNewPassword(choice,newPassword);
                            dataList.setPassword( newPassword );
                            std::cout << "Modification du mot de passe réussie !" << std::endl;
                        }
                        else
                        {
                            std::cerr << "Mauvais mot de passe. Retour au menu." << std::endl;
                        }
                        break;
                    }

                    default: std::cout << "A bientôt sur Pandore !" << std::endl;
                }

            }while(choix!= 0);
            // Fin de l'application.
            // Tout s'est bien passé donc on sauvegarde le contenu dans le fichier de secours et le fichier initial.
            pnd::CombinationList helpDataList(dataList);

            dataList.setPathDataFile(pnd::cst::dataFileTextPath);
            helpDataList.setPathDataFile(pnd::cst::dataFileTextPathSecret);

            std::ofstream saver(pnd::cst::dataFileTextPath);
            std::ofstream saverTwo(pnd::cst::dataFileTextPathSecret);

            saveCombinationList(dataList,saver);
            saveCombinationList(helpDataList,saverTwo);

        }
        else {
            if (argc == 2 && argv[1] == std::string("-h") ) {
                helpPrint();
            }
            else {
                throw pnd::Exception(42, "main(int,char**)", "Arguments de l'application incorrects.", pnd::level::main);
            }

        }


    }

    catch (pnd::Exception const &e) {
        std::cerr << e.what() << std::endl;
        switch (e.getNumero() ) {

            case 45 :   //L'utilisateur a voulu fermer l'application.
                std::cerr << "Fermeture prématurée de l'application." << std::endl;
                break;

            case 22 : // Le fichier de données a été modifié à la main. La sauvegarde est compromise.
            case 23 :
            case 24 :
            case 25 :
                std::cerr << "Veuillez éviter de modifier le fichier data.pass." << std::endl;
            case 47 :  //La sauvegarde des données est compromise, on transfère le contenu du fichier de secours vers le fichier initial.
            {
                pnd::CombinationList helpList(pnd::cst::dataFileTextPathSecret);
                helpList.setPathDataFile(pnd::cst::dataFileTextPath);
                std::cerr << "Modification du fichier de données." << std::endl;
            }

            default:break;
        }
        return EXIT_FAILURE;
    }

    catch(std::exception const& error)
    {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/


void buildNewPassword(int& choix, pnd::String& newOne)
{
    bool valid = false;
    switch(choix)
    {
        // Génération aléatoire => on construit un pnd::String avec le constructeur par défaut String()
        case 1 :
            do {
                pnd::String newPassword;
                std::cout << "Le mot de passe généré est le suivant : '" << newPassword.getData() << "'" << std::endl;
                std::cout << "Voulez-vous le garder ? 1:oui 0:non (si oui, n'oubliez pas de le noter.)" << std::endl;
                readChoice(valid,false,true);
                if( valid )
                {
                    newOne = newPassword;
                }
            }while( !valid);
            break;

        case 2 :
            do {
                std::string dataPassword;
                readString("Veuillez saisir un nouveau mot de passe : (Minimum 10 caractères, plus de lettres que de chiffres, 2 chiffres, 2 caractères spéciaux)",dataPassword);
                pnd::String newPassword( dataPassword );
                if( verifyPassword(newPassword) )
                {
                    valid = true;
                    newOne = newPassword;
                }
            }while( !valid);
            break;

        default: throw pnd::Exception(46,"pnd::String& buildNewPassword(int& )","Argument 1 invalide.",pnd::level::main);

    }
}

bool verifyPassword(const pnd::String &test)
{
    // Le mot de passe doit avoir plus de 10 caractères (+ de lettres que de chiffres, au moins 2 chiffres, au moins 2 caractères spéciaux.
    std::string chaine = test.getData();
    if( chaine.size() >= pnd::cst::minLength)
    {
        unsigned short nbNumbers = 0, nbSpecials = 0, nbLetters = 0;   // compteur de chiffres, caractères spéciaux et de lettres.
        for(char character : chaine)
        {
            if( '0' <= character && character <= '9')
            {
                nbNumbers++;
            }
            else if( ('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z') )
            {
                nbLetters++;
            }
            else
            {
                nbSpecials++;
            }
        }
#ifdef __MODE_DEBUG__MAIN__
        std::clog << "bool verifyPassword(const pnd::String &) - nbLetters = " << nbLetters << " & nbNumbers = " << nbNumbers << " & nbSpecials = " << nbSpecials << std::endl;
#endif
        return nbNumbers >= pnd::cst::minNumbersInPassword && nbSpecials >= pnd::cst::minSpecialsInPassword && nbLetters >= nbNumbers;
    }
    else
    {
        return false;
    }
}

void fillFields(pnd::Combination& thisOne)
{
#ifdef __MODE_DEBUG__MAIN__
    std::clog << "void fillFields - type = " << static_cast<unsigned short>(thisOne.getCombinationType()) << std::endl;
#endif
    if( thisOne.getCombinationType() >= pnd::combinationType::end )
    {
        std::cout << "Retour au menu précédent." << std::endl;
    }
    else {

        std::vector <std::string> mask = pnd::Combination::mask( thisOne.getCombinationType() );    //ON récupère le masque
        for( std::string& key : mask)
        {
            std::string dataValue;
            readString("Veuillez saisir le champ " + key + " : ",dataValue);
            pnd::String value( dataValue );
            std::pair<std::string, pnd::String> pair(key,value);
            thisOne.addField(pair);
        }
    }
}


void makeCombination(pnd::Combination& newOne)
{
    pnd::combinationType newType;
    typeCombinationMenuPrint(newType); // On demande le type de la combinaison.

    if( pnd::combinationType::quit < newType && newType < pnd::combinationType::end) {
        newOne.setCombinationType(newType);
        fillFields(newOne);
    }
}

pnd::CombinationList& loadDataApp(pnd::CombinationList& dataApp)
{
    std::string dataFilePath;
    if ( pnd::is_readable(pnd::cst::dataFileTextPath.c_str()) )
    {
        dataFilePath = pnd::cst::dataFileTextPath;
    }
    else
    {
        dataFilePath = pnd::cst::dataFileTextPathSecret;
        std::clog << "Attention, le fichier data.pass original n'est pas accessible. Utilisation du fichier de secours." << std::endl;
    }

    std::ifstream fileStream(dataFilePath);
    dataApp.setPathDataFile(dataFilePath);

    return loadCombinationList(fileStream,dataApp);
}
