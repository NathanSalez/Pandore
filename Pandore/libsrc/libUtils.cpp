#include "../include/libUtils.h"


    void readString(const std::string& description, std::string& answer) // cf http://sdz.tdct.org/sdz/la-saisie-securisee-en-c.html
    {
        std::cout << description;
        getline(std::cin, answer); // getline() ici est une fonction et non une méthode de la classe istream
        if (std::cin.bad() || std::cin.eof() || std::cin.fail()) {
            throw pnd::Exception(47, "void readString(const std::string& )", "Fin du flux d'entrée.", pnd::level::main);
        }
        // L'utilisation de vider_buffer() n'est plus nécessaire car getline() supprime le caractère '\n'
    }


void mainMenuPrint(int& choix)
{
    std::cout << "\nFonctionnalités disponibles :\n" << std::endl;
    std::cout << " 1 - Lire l'ensemble des combinaisons." << std::endl;
    std::cout << " 2 - Ajouter une combinaison." << std::endl;
    std::cout << " 3 - Modifier une combinaison." << std::endl;
    std::cout << " 4 - Supprimer une combinaison." << std::endl;
    std::cout << " 5 - Rechercher une combinaison." << std::endl;
    std::cout << " 6 - Changer le mot de passe de l'application." << std::endl;
    std::cout << " 0 - Quitter l'application.\n" << std::endl;
    readChoice(choix,0,6);
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

    readChoice(choix,0, static_cast<int>(pnd::cst::maxCombinationTypes));

    if ( choix == 0 )
        return; // L'utilisateur retourne au menu précédent.
    if( 0 < choix && choix <= pnd::cst::maxCombinationTypes )
    {
        choixType = static_cast<pnd::combinationType>(choix);
    }
    else
    {
        throw pnd::Exception(43,"void typeCombinationMenuPrint(pnd::combinationType&)","Type non connu.",pnd::level::main);
    }
}


void SearchMenuPrint(int& choix)
{
    std::cout << "\nChoisissez le mode de recherche parmi les suivants :\n" << std::endl;
    std::cout << " 1 - Par identifiant numérique." << std::endl;
    std::cout << " 2 - Par nom de site." << std::endl;
    std::cout << " 3 - Par mot de passe." << std::endl;
    std::cout << " 4 - Par pseudo." << std::endl;
    std::cout << " 5 - Par adresse mail." << std::endl;
    std::cout << " 0 - Revenir au menu précédent." << std::endl;

    readChoice(choix,0,5);
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



