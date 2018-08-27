#include "../include/Exception.h"

namespace pnd {
    Exception::Exception(int numero, std::string const& nomFonction, std::string const &phrase, level niveau = main)
            : number(numero), niveau(niveau)
    {
        std::ostringstream flux;   //Un flux permettant d'écrire dans une chaîne
        flux << "Erreur " << numero << " - " << nomFonction << " : " << phrase;
        this->description = flux.str();
    }


    const char *Exception::what() const noexcept {
        return this->description.c_str();
    }

    level Exception::getNiveau() const noexcept {
        return this->niveau;
    }

    int Exception::getNumero() const noexcept {
        return this->number;
    }
}