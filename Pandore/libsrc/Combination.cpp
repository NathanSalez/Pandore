#include "../include/Combination.h"



namespace pnd {

    //Construction du masque des combinationType :
    std::vector<std::string> Combination::mask(combinationType typeC)
    {
        std::vector<std::string> maskReturn;
        switch(typeC)
        {
            case combinationType::normal :
                maskReturn.emplace_back("site");        // On utilise emplace_back car on crée l'objet directement dans le vector
                maskReturn.emplace_back("pseudo");
                break;

            case combinationType::email :
                maskReturn.emplace_back("mail");
                maskReturn.emplace_back("adresse");
                break;

            case combinationType::password :
                maskReturn.emplace_back("site");
                break;

            case combinationType::mixed :
                maskReturn.emplace_back("site");
                maskReturn.emplace_back("adresse");
                break;

            default : throw Exception(13,"std::vector<std::string> Combination::mask(combinationType)","Le type en argument n'existe pas ou n'est pas configuré.",level::combination);


        }
        maskReturn.emplace_back("mot de passe");
        return maskReturn;
    }

    bool Combination::validKeyType(combinationType type, std::string const& cle) {
#ifdef __MODE_DEBUG__COMBINATION__
        std::clog << "bool validKeyType(combinationType type, std::string const&) - cle = " << cle << " && type = " << static_cast<int>(type) << std::endl;
#endif
        std::vector<std::string> mask = Combination::mask(type);

        auto finder = find(mask.begin(),mask.end(),cle);
        return !(finder == mask.end());     // si finder == mask.end(), alors la clé n'a pas été trouvée parmi les clés associés au type.
    }


    Combination::Combination(): type(combinationType::quit) {}

    Combination::Combination(combinationType type) : type(type) {}

    Combination::Combination(combinationType type, std::map<std::string, String, cmpLength> const& data) : type(type)
    {
        this->setData(data);
    }

    combinationType Combination::getCombinationType() const
    {
        return this->type;
    }

    unsigned short Combination::getCombinationTypeValue() const
    {
        return static_cast<unsigned short>(this->type);
    }

    void Combination::setCombinationType(const combinationType& newType)
    {
#ifdef __MODE_DEBUG__COMBINATION__
        std::clog << "void Combination::setCombinationType(const combinationType&) - newType = " << static_cast<int>(newType) << std::endl;
#endif
        if( newType < combinationType::end )
        {
            this->type = newType;
        }
        else
        {
            throw Exception(13,"void Combination::setCombinationType(const combinationType&)","Le type en argument n'existe pas.",level::combination);
        }
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
                                  "La clé en argument est incompatible avec le type de la combinaison.", level::combination);
        }
    }

    void Combination::setData( std::map<std::string, String, cmpLength> const& data)
    {
        for(auto& pair : data)
        {
            if( validKeyType(type,pair.first) )
            {
                this->data[pair.first] = pair.second;
            }
            else
            {
                throw Exception(11,"void setData( const std::map<std::string, String, cmpLength> )","La clé en argument est incompatible avec le type de la combinaison.",level::combination);
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
            throw Exception(12,"void removeField(std::string const&)","La clé n'a pas été trouvée dans la combinaison.",level::combination);
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
            throw Exception(12,"void modifyField(std::string const&, pnd::String const&)"," La clé n'a pas été trouvée dans la combinaison.",level::combination);
        }
    }

    std::ostream &operator<<(std::ostream &stream, Combination const &thisOne) {
        for(auto& pair : thisOne.data )
        {
            stream << pair.first << " : \"" << pair.second << "\"\t";
        }
        return stream;
    }
}