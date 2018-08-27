/**
 * @brief : Programme principal de l'application Pandore version console.
 * @author : Naysson
 * @todo : - Ajouter le type mixed pour les combinaisons.       FAIT
 *         - Proposer une recherche de combinaisons par champ.
 *         - Utiliser cette recherche pour la suppressions et modification.
 *         - Proposer une question personnelle au permier lancement de l'application.
 */

#include <limits>
#include <ctime>

#include "../include/CombinationList.h"

namespace cst
{
    const unsigned short maxAttempts = 5;
    const unsigned short minNumbers = 2;
    const unsigned short minSpecials = 2;
}

//#define __MODE_DEBUG__MAIN__

void helpPrint();
void passwordMenuPrint(int&);
void mainMenuPrint(int&);
void typeCombinationMenuPrint(pnd::combinationType& );      // Affiche chaque combinaisonType et ses champs/clés
void keyCombinationTypePrint(pnd::combinationType const&, int& ); // Affiche chaque champs/clés de la combinaisonType en paramètre.
template <typename T> void readChoice(T& choix,T min, T max);  // pour lire un entier,flottant,booléen dans l'intervalle [min,max]

pnd::Combination makeCombination();
void fillFields(pnd::Combination&);     // pour remplir un pnd::COmbination en fonction de son type
std::string readString(const std::string& description);

pnd::String buildNewPassword(int& choix);
bool verifyPassword(const pnd::String &);     // utilisé lorsque l'utilisateur crée un mot de passe.

pnd::CombinationList loadCombinationList();

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
            if (pnd::is_readable(cst::dataFileTextPath.c_str()) || pnd::is_readable(cst::dataFileTextPathSecret.c_str())  ) {

                //On appelle le constructeur suivant (le fichier data.pass existe)
                pnd::CombinationList list = loadCombinationList();

                //On demande le mot de passe à l'utilisateur.
                std::string sentPassword;
                unsigned short attempts = 0;
                bool valid = false;
                do {
                    attempts++;
                    sentPassword = readString("Mot de passe de l'application : ");

                    if (sentPassword == list.getPassword().getData()) {
                        valid = true;
                    }
                } while (attempts < cst::maxAttempts && !valid);

                if (valid)
                {
                    std::cout << "Mot de passe correct, chargement en cours..." << std::endl;
                    dataList = list;

                }
                else
                {
                    throw pnd::Exception(41, "main(int, char **)",
                                         "Nombre maximum de tentatives atteint.",
                                         pnd::main);
                }
            }
            else
            {
                //On doit construire le mot de passe de l'application.
                int choix;
                passwordMenuPrint(choix);
                pnd::String newPassword( buildNewPassword(choix) );
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
                        pnd::Combination newCombination = makeCombination();
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

                                pnd::String newContent(readString("Nouveau contenu du champ " + field + " : "));


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

                    case 5 : // changer le mot de passe de l'appli.
                    {
                        int choice = 0;
                        std::string sentPassword = readString("Mot de passe de l'application : ");

                        if (sentPassword == dataList.getPassword().getData()) {
                            passwordMenuPrint(choice);
                            pnd::String newPassword = buildNewPassword(choice);
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
            pnd::CombinationList helpList(dataList);
            dataList.setPathDataFile(cst::dataFileTextPath);
            helpList.setPathDataFile(cst::dataFileTextPathSecret);

        }
        else {
            if (argc == 2 && argv[1] == std::string("-h") ) {
                helpPrint();
            }
            else {
                throw pnd::Exception(42, "main(int,char**)", "Arguments de l'application incorrects.", pnd::main);
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
                pnd::CombinationList helpList(cst::dataFileTextPathSecret);
                helpList.setPathDataFile(cst::dataFileTextPath);
                std::cerr << "Modification du fichier de données." << std::endl;
            }

            default:break;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/***************************************************************************************************************************/
/***************************************************************************************************************************/
/***************************************************************************************************************************/

void mainMenuPrint(int& choix)
{
    std::cout << "\nFonctionnalités disponibles :\n" << std::endl;
    std::cout << " 1 - Lire l'ensemble des combinaisons." << std::endl;
    std::cout << " 2 - Ajouter une combinaison." << std::endl;
    std::cout << " 3 - Modifier une combinaison." << std::endl;
    std::cout << " 4 - Supprimer une combinaison." << std::endl;
    std::cout << " 5 - Changer le mot de passe de l'application." << std::endl;
    std::cout << " 0 - Quitter l'application.\n" << std::endl;
    readChoice(choix,0,5);
}

void passwordMenuPrint(int& choix)
{
    std::cout << "\nChoisissez la construction du mot de passe de l'application parmi les propositions suivantes :\n" << std::endl;
    std::cout << " 1 - Génération aléatoire." << std::endl;
    std::cout << " 2 - Génération manuelle.\n" << std::endl;
    readChoice(choix,1,2);
}

void typeCombinationMenuPrint(pnd::combinationType& choixType)
{
    int choix;
    std::cout <<"\nChoisissez le type de la combinaison à ajouter parmi les suivants :\n" << std::endl;
    std::cout << " 1 - site, pseudo, mot de passe." << std::endl;
    std::cout << " 2 - mail, adresse, mot de passe." << std::endl;
    std::cout << " 3 - site, mot de passe." << std::endl;
    std::cout << " 4 - site, adresse, mot de passe." << std::endl;
    std::cout << " 0 - Revenir au menu précédent." << std::endl;
    readChoice(choix,0, static_cast<int>(cst::maxCombinationTypes));
    switch(choix)
    {
        case 0 : break;
        case 1 : choixType = pnd::normal; break;
        case 2 : choixType = pnd::email;break;
        case 3 : choixType = pnd::password; break;
        case 4 : choixType = pnd::mixed; break;

        default: throw pnd::Exception(43,"void typeCombinationMenuPrint(pnd::combinationType&)","Type non connu.",pnd::main);
    }
}

void keyCombinationTypePrint(pnd::combinationType const& type, int& choix )
{
    std::cout << "\nChoisissez le champ à modifier parmi les suivants :\n" << std::endl;
    std::vector<std::string> mask = pnd::Combination::mask(type);
    for( unsigned long i = 0; i<mask.size();i++)
    {
        std::cout << i+1 << " - " << mask[i] << std::endl;
    }
    std::cout << "0 - Revenir au menu précédent." << std::endl;
    readChoice(choix,0, static_cast<int>(mask.size()));
    --choix;        //Seul le choix "menu précédent" sera négatif.
}

template <typename T> void readChoice(T& choix,T min, T max)       //cf developpez.com
{
    if (max < min)
    {
        throw pnd::Exception(44,"template <typename T> void readChoice(T& choix,T min, T max)","min est supérieure à max",pnd::main);
    }
    std::cout << "Votre choix : " ;
    while ( ! ( std::cin >> choix ) || choix < min || choix > max )
    {
        if ( std::cin.eof() )
        {
            // ^D  (^Z sous windows); Fin du flux d'entree !
            throw pnd::Exception(45,"template <typename T> void readChoice(T& ,T , T )","Fin du flux d'entrée.",pnd::main);
        }
        else if ( std::cin.fail() )
        {
            std::cout << "Saisie incorrecte, veuillez recommencer : ";
            std::cin.clear();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        }
        else
        {
            std::cout << "Le chiffre n'est pas entre " << min << " et " << max <<", veuillez recommencer : ";
        }
    }
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );   // on enlève le '\n' à la fin de la saisie.
}

pnd::String buildNewPassword(int& choix)
{
    pnd::String returnedPassword("");
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
                    returnedPassword = newPassword;
                }
            }while( !valid);
            break;

        case 2 :
            do {
                pnd::String newPassword( readString("Veuillez saisir un nouveau mot de passe : (Minimum 10 caractères, plus de lettres que de chiffres, 2 chiffres, 2 caractères spéciaux)"));
                if( verifyPassword(newPassword) )
                {
                    valid = true;
                    returnedPassword = newPassword;
                }
            }while( !valid);
            break;

        default: throw pnd::Exception(46,"pnd::String& buildNewPassword(int& )","Argument 1 invalide.",pnd::main);

    }
    return returnedPassword;
}

bool verifyPassword(const pnd::String &test)
{
    // Le mot de passe doit avoir plus de 10 caractères (+ de lettres que de chiffres, au moins 2 chiffres, au moins 2 caractères spéciaux.
    std::string chaine = test.getData();
    if( chaine.size() >= cst::minLength)
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
        return nbNumbers >= cst::minNumbers && nbSpecials >= cst::minSpecials && nbLetters >= nbNumbers;
    }
    else
    {
        return false;
    }
}

void fillFields(pnd::Combination& thisOne)
{
#ifdef __MODE_DEBUG__MAIN__
    std::clog << "void fillFields - type = " << thisOne.getCombinationType() << std::endl;
#endif
    if( thisOne.getCombinationType() >= cst::maxCombinationTypes )
    {
        std::cout << "Retour au menu précédent." << std::endl;
    }
    else {

        std::vector <std::string> mask = pnd::Combination::mask( thisOne.getCombinationType() );    //ON récupère le masque
        for( std::string& key : mask)
        {
            pnd::String value( readString("Veuillez saisir le champ " + key + " : ") );
            std::pair<std::string, pnd::String> pair(key,value);
            thisOne.addField(pair);
        }
    }
}

std::string readString(const std::string& description) // cf http://sdz.tdct.org/sdz/la-saisie-securisee-en-c.html
{
    std::string variable;

    std::cout << description;
    getline(std::cin, variable); // getline() ici est une fonction et non une méthode de la classe istream
    if (std::cin.bad() || std::cin.eof() || std::cin.fail()) {
        throw pnd::Exception(47, "std::string readString(const std::string& )", "Fin du flux d'entrée.", pnd::main);
    }
    // L'utilisation de vider_buffer() n'est plus nécessaire car getline() supprime le caractère '\n'
    return variable;
}


pnd::Combination makeCombination()
{
    pnd::combinationType newType;
    typeCombinationMenuPrint(newType); // On demande le type de la combinaison.

    if( newType < cst::maxCombinationTypes ) {
        pnd::Combination newCombination(newType);   // ON construit la nouvelle combinaison.
        fillFields(newCombination);
        return newCombination;
    }
    else {
        pnd::Combination x;
        return x;
    }
}


void helpPrint()
{
    std::cout << "Bienvenue sur l'application Pandore - version 1.3\n" << std::endl;
    std::cout << "A l'aide de cette application, vous pouvez sauvegarder vos mots de passe en sécurité." << std::endl;
    std::cout << "Lors du premier lancement de l'application, vous devez choisir un mot de passe maître qui vous permet d'accéder aux données sensibles." << std::endl;
    std::cout << "Lors des autres lancements, ce mot de passe est demandé automatiquement.\n\n" << std::endl;

    std::cout << "Les mots de passes sont enregistrés à l'aide d'une combinaison. Une combinaison est un ensemble de champ/valeur traduisant les données liées au mot de passe." << std::endl;
    std::cout << "Il existe différents types de combinaison :" << std::endl;
    std::cout << "- normal = ( site - pseudo - mot de passe )" << std::endl;
    std::cout << "- email = ( mail - adresse - mot de passe )" << std::endl;
    std::cout << "- password = (site - mot de passe)" << std::endl;
    std::cout << "[V1.3] - mixed = ( site - adresse - mot de passe )\n\n" << std::endl;

    std::cout << "Actions possibles :" << std::endl;
    std::cout << "- Ajouter une combinaison." << std::endl;
    std::cout << "- Supprimer une combinaison." << std::endl;
    std::cout << "- Modifier la valeur d'un champ d'une combinaison" << std::endl;
    std::cout << "- Afficher l'ensemble des combinaisons." << std::endl;
    std::cout << "[V1.3] - Rechercher une combinaison à l'aide d'un champ (site,mail etc..)" << std::endl;
    std::cout << "- Changer le mot de passe de l'application. (le mot de passe actuel sera demandé)\n\n" << std::endl;

    std::cout << "Lors de la saisie au clavier, vous pouvez arrêter à tout moment l'application à l'aide de CTRL+D sur Linux, CTRL+Z sur Windows." << std::endl;
    std::cout << "Cela entrainera la fermeture de l'application. Cette fermeture peut entraîner la perte des données les plus récentes.\n\n" << std::endl;

    std::cout << "Attention, lors de la suppression et de la modification d'une combinaison, vous devez donner un numéro caractérisant la combinaison concernée." << std::endl;
    std::cout << "Ce numéro est affiché lorsque l'on demande l'affichage de toutes les combinaisons." << std::endl;
    std::cout << "[V1.X]Une recherche par champ sera proposée dans une future version pour ces actions.\n\n" << std::endl;

    std::cout << "En cas de perte du mot de passe maître de l'application, veuillez faire une demande de mot de passe au créateur de l'application." << std::endl;
    std::cout << "Vous devrez joindre le répertoire files à votre demande." << std::endl;
    std::cout << "Ce dernier vous donnera votre mot de passe dans un fichier à télécharger."<< std::endl;
    std::cout << "[V1.X] La réponse à votre question personnelle, choisie au 1er lancement de Pandore, sera demandée pour doubler la sécurité.\n\n\n" << std::endl;

    std::cout << "Naysson, joignable à l'adresse retiti@hotmail.fr" << std::endl;
}

pnd::CombinationList loadCombinationList()
{
    if ( pnd::is_readable(cst::dataFileTextPath.c_str()) )
    {
        pnd::CombinationList normalOne(cst::dataFileTextPath);
        return normalOne;
    }
    else
    {
        pnd::CombinationList anormalOne(cst::dataFileTextPathSecret);
        std::clog << "Attention, le fichier data.pass original n'est pas acessible. Utilisation du fichier de secours." << std::endl;
        return anormalOne;
    }
}
