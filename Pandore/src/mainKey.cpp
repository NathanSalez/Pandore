#include "../include/libFile.h"

int main(void)
{

    try {
        if (pnd::is_readable(pnd::cst::dataFileTextPath.c_str())) {
            //On appelle le constructeur suivant (le fichier data.pass existe)
            pnd::CombinationList list(pnd::cst::dataFileTextPath);
            std::cout << "Mot de passe de l'application : '" << list.getPassword() << "'" << std::endl;
        }
        else
        {
            throw pnd::Exception(17,"int main(void)","Le fichier de données n'a pas pu être ouvert.",pnd::level::main);
        }
    }

    catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
