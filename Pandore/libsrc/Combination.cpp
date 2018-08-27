#include "../include/Combination.h"



namespace pnd {

    //Construction du masque des combinationType :
    std::vector<std::string> Combination::mask(combinationType typeC)
    {
        std::vector<std::string> maskReturn;
        switch(typeC)
        {
            case normal :
                maskReturn.emplace_back("site");        // On utilise emplace_back car on crée l'objet directement dans le vector
                maskReturn.emplace_back("pseudo");
                break;

            case email :
                maskReturn.emplace_back("mail");
                maskReturn.emplace_back("adresse");
                break;

            case password :
                maskReturn.emplace_back("site");
                break;

            case mixed :
                maskReturn.emplace_back("site");
                maskReturn.emplace_back("adresse");
                break;

            default : throw Exception(13,"std::vector<std::string> Combination::mask(combinationType)","Le type en argument n'existe pas.",combination);


        }
        maskReturn.emplace_back("mot de passe");
        return maskReturn;
    }

    bool Combination::validKeyType(combinationType type, std::string const& cle) {
        std::vector<std::string> mask = Combination::mask(type);

        auto finder = find(mask.begin(),mask.end(),cle);
        return !(finder == mask.end());     // si finder == mask.end(), alors la clé n'a pas été trouvée parmi les clés associés au type.
    }


    Combination::Combination(): type(normal) {}

    Combination::Combination(combinationType type) : type(type) {}

    Combination::Combination(combinationType type, std::map<std::string, String, cmpLength> const& data) : type(type)
    {
        this->setData(data);
    }

    combinationType Combination::getCombinationType() const
    {
        return this->type;
    }

    void Combination::addField( std::pair<std::string, pnd::String> const& information)
    {
        std::string cle ( information.first );

        if( validKeyType(this->type,information.first) ) {
            data[cle] = information.second;
        }
        else
        {
            throw Exception(11, "void addField(std::pair<pnd::String, pnd::String>&)",
                                  "La clé en argument est incompatible avec le type de la combinaison.", combination);
        }
    }

    void Combination::setData( std::map<std::string, String, cmpLength> const& data)
    {
        for(auto it = data.begin(); it != data.end(); it++)
        {
            if( validKeyType(type,it->first) )
            {
                this->data[it->first] = it->second;
            }
            else
            {
                throw Exception(11,"setData( const std::map<std::string, String, cmpLength> )","La clé en argument est incompatible avec le type de la combinaison.",combination);
            }
        }
    }

    std::map<std::string, String, cmpLength> Combination::getData() const
    {
        return this->data;
    };

    void Combination::removeField( std::string const& cle)
    {
        // On trouve d'abord l'élément correspondant à la clé.
        auto iterator = this->data.find(cle);
        if( iterator != this->data.end() )
        {
            this->data.erase(iterator);
        }
        else
        {
            throw Exception(12,"void removeField(std::string const&)","La clé n'a pas été trouvée dans la combinaison.",combination);
        }

    }

    void Combination::modifyField(std::pair<std::string, String> const& paire)
    {
        auto iterator = this->data.find( paire.first );
        if (iterator != this->data.end() )
        {
            this->data[paire.first] = paire.second;
        }
        else
        {
            throw Exception(12,"void modifyField(std::string const&, pnd::String const&)"," La clé n'a pas été trouvée dans la combinaison.",combination);
        }
    }

    std::ostream &operator<<(std::ostream &stream, Combination const &thisOne) {
        for(auto it=thisOne.data.begin(); it != thisOne.data.end(); it++)
        {
            stream << it->first << " : \"" << it->second << "\"\t";
        }
        return stream;
    }
}