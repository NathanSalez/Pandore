#include "../include/File.h"

namespace pnd {
    void saveStandardString(std::string const &test, std::ofstream &stream) {
        if (stream.good()) {
            stream << test << std::endl;
        }
        else {
            throw Exception(21, "saveStandardString(std::string const&, std::ofstream &)",
                            "Problème de manipulation du fichier.", file);
        }
    }

    std::string loadStandardString(std::ifstream &stream)
    {
        std::string test;
        if( stream.good() )
        {
            getline(stream,test);
            if( !test.empty() )
                return test;
            else
                throw Exception(25,"loadStandardString(std::ifstream &)","Chaine modifiée dans le fichier.", file);
        }
        else {
            throw Exception(21, "loadStandardString(std::ifstream &)",
                            "Problème de manipulation du fichier.", file);
        }
    }

    void saveString(String & test, std::ofstream &stream)  //Il ne faut pas oublier le cryptage
    {
        if (stream.good()) {
            stream << test.getKey() << " ";     // Attention au cas du 1er caractère étant un chiffre.
            test.change();
            stream << test << std::endl;
        }
        else {
            throw Exception(21, "saveString(pnd::String const&, std::ofstream &)",
                            "Problème de manipulation du fichier.", file);
        }
    }

    String loadString(std::ifstream &stream)        //Idem pour le décryptage
    {
        if( stream.good() )
        {
            unsigned short key =0;
            std::string data;

            stream >> key;
            getline(stream,data);

            if( data.empty() )
                throw Exception(25,"loadString(std::ifstream &)","Chaine modifiée dans le fichier.", file);

            data.erase( data.begin() );     // ON enlève le 1er caractère qui est l'espace
            String test(data,key);
            test.change();
#ifdef __MODE_DEBUG__FILE__
            std::clog << "loadString(std::ifstream &) - test = " << test << std::endl;
#endif

            return test;
        }
        else {
            throw Exception(21, "loadString(std::ifstream &)",
                            "Problème de manipulation du fichier.", file);
        }

    }

    void saveCombination( Combination & test, std::ofstream &stream)
    {
        if (stream.good()) {
            std::map<std::string, String, cmpLength> data = test.getData();

            stream << data.size() << " " << test.getCombinationType() << std::endl;

            for( auto it = data.begin(); it != data.end(); it++)
            {
                saveStandardString(it->first,stream);
                saveString(it->second,stream);
            }
        }
        else {
            throw Exception(21, "saveCombination( Combination const&, std::ofstream &)",
                            "Problème de manipulation du fichier.", file);
        }
    }

    Combination loadCombination(std::ifstream &stream)
    {

        if( stream.good() )
        {
            unsigned long size=0;
            unsigned short annexe = 10; combinationType type;
            std::string saut;
            stream >> size >> annexe; //On doit passer à la ligne suivante après ça :
            getline(stream,saut);
            switch(annexe)
            {
                case 0 : type = normal;break;
                case 1 : type = email; break;
                case 2 : type = password; break;
                case 3 : type = mixed; break;

                default: throw Exception(22,"loadCombination(std::ifstream &)","Valeur de type erronée dans le fichier.",file);
            }
#ifdef __MODE_DEBUG__FILE__
            std::clog << "loadCombination(std::ifstream &) - annexe = type = " << annexe << std::endl;
#endif
            if(size != 0)
            {
                Combination test(type);
                for(unsigned long i=0; i< size; i++)
                {
                    std::string cle = loadStandardString(stream);
#ifdef __MODE_DEBUG__FILE__
                    std::clog << "loadCombination(std::ifstream &) - cle = " << cle << std::endl;
#endif
                    String value = loadString(stream);

                    std::pair<std::string,String> x(cle,value);

                    if( Combination::validKeyType(type,cle) )
                    {
                        test.addField(x);
                    }
                    else
                    {
                        throw Exception(23,"loadCombination(std::ifstream &)","Valeur de clé erronée dans le fichier.",file);
                    }
                }
                return test;
            }
            else
            {
                throw Exception(24,"loadCombination(std::ifstream &)","Valeur de size erronée dans le fichier.",file);
            }
        }
        else {
            throw Exception(21, "loadCombination(std::ifstream &)",
                            "Problème de manipulation du fichier.", file);
        }

    }

    bool is_readable(const char *filePath)  // cf developpez.com
    {
        std::ifstream fichier( filePath);
        return !fichier.fail();
    }

}