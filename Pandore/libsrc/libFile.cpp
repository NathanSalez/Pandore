#include "../include/libFile.h"

namespace pnd {
    void saveStandardString(const std::string& test, std::ofstream &stream) {
        if (stream.good()) {
            stream << test << std::endl;
        }
        else {
            throw Exception(31, "void saveStandardString(const std::string &, std::ofstream &)",
                            "Problème de manipulation du fichier.", level::file);
        }
    }

    std::string& loadStandardString(std::ifstream &stream, std::string& test)
    {
        if( stream.good() )
        {
            getline(stream,test);
            if( !test.empty() )
                return test;
            else
                throw Exception(35,"std::string& loadStandardString(std::ifstream &, std::string&)","Chaine modifiée dans le fichier.", level::file);
        }
        else {
            throw Exception(31, "std::string& loadStandardString(std::ifstream &, std::string& )",
                            "Problème de manipulation du fichier.", level::file);
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
            throw Exception(31, "void saveString(pnd::String const&, std::ofstream &)",
                            "Problème de manipulation du fichier.", level::file);
        }
    }


    String& loadString(std::ifstream &stream, String& test)        //Idem pour le décryptage
    {
        if( stream.good() )
        {
            unsigned short key =0;
            std::string data;

            stream >> key;
            getline(stream,data);

            if( data.empty() )
                throw Exception(35,"String& loadString(std::ifstream &, String&)","Chaine modifiée dans le fichier.", level::file);

            data.erase( data.begin() );     // ON enlève le 1er caractère qui est l'espace

            test.setData(data);
            test.setKey(key);

            test.change();
#ifdef __MODE_DEBUG__FILE__
            std::clog << "String& loadString(std::ifstream &, String&) - test = " << test << std::endl;
#endif

            return test;
        }
        else {
            throw Exception(31, "String& loadString(std::ifstream &, String&)",
                            "Problème de manipulation du fichier.", level::file);
        }

    }

    void saveCombination(Combination & test, std::ofstream &stream)
    {
        if (stream.good()) {
            std::map<std::string, String, cmpLength> data = test.getData();

            stream << data.size() << " " << test.getCombinationTypeValue() << std::endl;
            for( auto& pair : data)
            {
                saveStandardString(pair.first,stream);
                saveString(pair.second,stream);
            }
        }
        else {
            throw Exception(31, "void saveCombination( Combination const&, std::ofstream &)",
                            "Problème de manipulation du fichier.",level::file);
        }
    }

    Combination& loadCombination(std::ifstream &stream, Combination& test)
    {

        if( stream.good() )
        {
            unsigned long size=0;
            unsigned short annexeType = 10; combinationType type;
            std::string saut;
            stream >> size >> annexeType; //On doit passer à la ligne suivante après ça :
            getline(stream,saut);

            if( 0 < annexeType && annexeType <= cst::maxCombinationTypes )
            {
                type = static_cast<pnd::combinationType>(annexeType);
            }
            else
            {
                throw Exception(32,"Combination& loadCombination(std::ifstream &, Combination&)","Valeur de type erronée dans le fichier.",level::file);
            }
#ifdef __MODE_DEBUG__FILE__
            std::clog << "Combination& loadCombination(std::ifstream &, Combination&) - annexeType = " << annexeType << std::endl;
#endif
            if(size != 0)
            {
                test.setCombinationType(type);
                for(unsigned long i=0; i< size; i++)
                {
                    std::string key;
                    loadStandardString(stream,key);
#ifdef __MODE_DEBUG__FILE__
                    std::clog << "Combination& loadCombination(std::ifstream &, Combination&) - key = " << key << std::endl;
#endif
                    String value;
                    loadString(stream,value);

                    std::pair<std::string,String> x(key,value);

                    if( Combination::validKeyType(type,key) )
                    {
                        test.addField(x);
                    }
                    else
                    {
                        throw Exception(33,"Combination& loadCombination(std::ifstream &, Combination&)","Valeur de clé erronée dans le fichier.",level::file);
                    }
                }
                return test;
            }
            else
            {
                throw Exception(34,"Combination& loadCombination(std::ifstream &, Combination&)","Valeur de size erronée dans le fichier.",level::file);
            }
        }
        else {
            throw Exception(31, "Combination& loadCombination(std::ifstream &, Combination&)",
                            "Problème de manipulation du fichier.", level::file);
        }

    }


    void saveCombinationList(CombinationList & thisOne, std::ofstream & fileStream)
    {

        //std::ofstream fileStream(thisOne.getPathDataFile(), std::ios::out);
        if ( fileStream.good() )
        {
            //On enregistre d'abord le mot de passe de l'application.
            pnd::String passwordApp ( thisOne.getPassword());
            saveString(passwordApp,fileStream);

            //Puis la liste des combinaisons
            fileStream << thisOne.getDataList().size() << std::endl;
            for(auto& combination : thisOne.getDataList() )
            {
                saveCombination(combination,fileStream);
            }

            fileStream.close();
        }
        else
        {
            throw Exception(31,"void saveCombinationList(CombinationList &, std::ofstream &)","Problème de manipulation du fichier.",level::combinationList);
        }

    }


    CombinationList& loadCombinationList(std::ifstream& fileStream, CombinationList& thisOne)
    {

        if ( fileStream.good() )
        {
            //On charge d'abord le mot de passe de l'application.
#ifdef __MODE_DEBUG__FILE__
            std::clog << "CombinationList& loadCombinationList(std::ifstream& , CombinationList& ) - L'ouverture du fichier a réussi." << std::endl;
#endif
            String passwordApp("");
            thisOne.setPassword( loadString(fileStream,passwordApp) );
#ifdef __MODE_DEBUG__FILE__
            std::clog << "CombinationList& loadCombinationList(std::ifstream& , CombinationList& ) - passwordApp = " << passwordApp << std::endl;
#endif
            if( passwordApp.getData().length() < cst::minLength)      // Attention au changement du mot de passe.
                throw Exception(35,"CombinationList& loadCombinationList(std::ifstream& , CombinationList& )",
                                "Chaine modifiée dans le fichier.", level::combinationList);

            //Puis la liste des combinaisons
            unsigned long size = 0;
            fileStream >> size;
            if( size != 0 )
            {
                for(unsigned long i=0; i <size;i++)
                {
                    if ( fileStream.good() ) {
                        Combination newOne;
                        thisOne.addCombination( loadCombination(fileStream,newOne) );
                    }
                    else { // L'utilisateur a modifié size : ce dernier est devenu plus petit.
                        throw Exception(34, "CombinationList& loadCombinationList(std::ifstream& , CombinationList& )",
                                        "Valeur de size erronée dans le fichier.", level::combinationList);
                    }
                }
            }
            fileStream.close();
        }
        else {
            throw Exception(31, "CombinationList& loadCombinationList(std::ifstream& , CombinationList& )",
                            "Problème de manipulation du fichier.", level::file);
        }

    }


    bool is_readable(const char *filePath)  // cf developpez.com
    {
        std::ifstream fichier( filePath);
        return !fichier.fail();
    }

}