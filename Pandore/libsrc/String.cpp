#include "../include/String.h"

namespace pnd {

     std::string stringBuild(unsigned short key) {
        // La taille de la chaine correspond à la clé.
         std::string dataReturned;
        for (unsigned short i = 0; i < key; i++) {
            char car = static_cast<char>(rand() % (cst::maxIntervalLetter - cst::minIntervalLetter) + cst::minIntervalLetter);
            dataReturned.push_back(car);
        }
#ifdef __MODE_DEBUG__STRING__
        std::clog << "pnd::stringBuild - Chaine obtenue (" << key << ") = " << dataReturned << std::endl;
#endif
        return dataReturned;
    }


    String::String() : ciphered(false)
    {
#ifdef __MODE_DEBUG__STRING__
        std::clog << "String::String() - Entrée dans la fonction." << std::endl;
#endif
        // cf openclassroom pour la formule
        this->key = static_cast<unsigned short>(rand() % (cst::maxKey - cst::minKey) + cst::minKey);


        // Traitement pour obtenir une chaine aléaoire
        // dont la longueur est identique à la clé.
        this->data = stringBuild(this->key);

    }

    String::String(std::string chaine, unsigned short key) : ciphered(true)
    {
        // Par définition, on utilise ce constructeur pour le chargement des données d'un fichier, qui sont chiffrés.
#ifdef __MODE_DEBUG__STRING__
        std::clog << "String::String(std::string, unsigned short) - Entrée dans la fonction." << std::endl;
        std::clog << "String::String(std::string, unsigned short) - key = " << key << std::endl;
#endif
        if( key <= cst::maxKey && cst::minKey <= key)
        {
            this->key = key;
            this->data = chaine;
        }
        else
        {
            if( key > cst::maxKey)
            {
                throw Exception(1,"String(std::string, unsigned short)","La clé entrée est trop grande.",level::string);
            }
            else
            {
                throw Exception(2,"String(std::string, unsigned short)","La clé entrée est trop petite.",level::string);
            }
        }
    }

    String::String(const std::string& chaine) : data(chaine), ciphered(false) {
        // Pour la clé, on utilise la bibliothèque cstdlib
        // L'initialisation se fait dans le programme principal

        this->key = static_cast<unsigned short>(rand() % (cst::maxKey - cst::minKey) + cst::minKey);

#ifdef __MODE_DEBUG__STRING__
        std::clog << "pnd::String::String(const std::string&) - clé utilisée pour la chaine " << chaine << " : " << this->key
                  << std::endl;
#endif
    }

    String& String::operator=(const std::string& copy)
    {
        this->data = copy;
        this->key = static_cast<unsigned short>(rand() % (cst::maxKey - cst::minKey) + cst::minKey);
        return *this;
    }

/// GETTER/SETTER
    unsigned short String::getKey() const {
        return this->key;
    }

    std::string String::getData() const {
         return this->data;
     }

    void String::setKey(unsigned short key) {
        if (key >= cst::minKey && key <= cst::maxKey) {
            this->key = key;
        }
        else {
            if( key > cst::maxKey)
            {
                throw Exception(1,"setKey(unsigned short)","La clé entrée est trop grande.",level::string);
            }
            else
            {
                throw Exception(2,"setKey(unsigned short)","La clé entrée est trop petite.",level::string);
            }
        }
    }

    void String::setData(std::string &data) {
         this->data = data;
     }

    bool String::isCiphered() const {
        return this->ciphered;
    }


    void String::change() {

        for (auto it = this->data.begin(); it != this->data.end(); it++) {
#ifdef __MODE_DEBUG__STRING__
            std::clog << "pnd::String::change() - Caractère étudiée : " << *it;
#endif
            if (*it != '\t') {
                *it = static_cast<char>( cst::maxIntervalLetter + cst::minIntervalLetter - *it - this->key);
            }
#ifdef __MODE_DEBUG__STRING__
            std::clog << " => " << *it << std::endl;
#endif

        }

        // On indique que la chaine est chiffrée/non chiffrée
        this->ciphered = true - this->ciphered;
#ifdef __MODE_DEBUG__STRING__
        std::clog << std::boolalpha;
        std::clog << "pnd::String::change() - Chaine chiffrée : " << this->ciphered << std::endl;
        std::clog << std::noboolalpha;
#endif

    }

    std::ostream &operator<<(std::ostream &stream, String const &thisOne) {
         stream << thisOne.data;
         return stream;
     }

}
