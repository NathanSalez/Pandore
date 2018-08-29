/**
 *  @class : pnd::String (composée d'un std::string)
 *  @brief : Chaine de caractère, auquel on ajoute les méthodes de cryptage.
 *  @note : pour ces méthodes, on utilise une clé, membre de la classe.
 *  @author : Naysson
 */

#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <cstdlib>      //POur la gestion du random
#include "Exception.h"

//#define __MODE_DEBUG__STRING__

namespace pnd {

    namespace cst {     // cst pour constantes
        //Les clés pour chiffrer/déchiffrer les chaines sont dans l'intervalle [10,20]
        const unsigned short maxKey = 20;
        const unsigned short minKey = 10;

        // Tous les caractères pouvant être écrits facilement au clavier sont dans l'intervalle [32,126]
        // sauf le caractère de tabulation ( = 9)
        const unsigned char maxIntervalLetter = 126;
        const unsigned char minIntervalLetter = 32;
    }

    class String {
    public:
        String();       //Lorsqu'on doit faire le mot de passe
        String(const std::string& ); //Lorsque l'utilisateur a choisi le mot de passe,
                                    // ou dans le cas où on veut pas faire le constructeur par défaut (en mettant "" en argument)
        String(std::string, unsigned short ); // LOrsqu'on charge les données du fichier
        ~String()=default;

        unsigned short getKey() const;
        void setKey(unsigned short );
        std::string getData() const;
        void setData(std::string &);
        bool isCiphered() const;

        String &operator=(const std::string& copy);     //cf ligne 318 du main

        //Les méthodes de cryptage et décryptage sont identiques (chiffrement symétrique)
        void change();


        friend std::ostream &operator<<(std::ostream &stream, String const &thisOne);

    protected:
        std::string data;
        unsigned short key;
        bool ciphered; // to cipher (eng) = chiffrer (fr)
    };

    std::string stringBuild(unsigned short); // pour avoir une chaine écrite de façon aléatoire
}

#endif