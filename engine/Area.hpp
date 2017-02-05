/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Area.hpp
 *
 * Overview:
 *     Header file for Area class. Defines the members and 
 * functions that apply to all areas.
 ************************************************************************/

#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "AreaSize.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Character;
class Exit;
class Item;
class Feature;

class Area: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        Area();
        Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size);
        Area(const Area &otherArea);
        Area & operator=(const Area &otherArea);
        virtual ~Area();
        std::string getName(); 
        std::string getShortDesc();
        std::string getLongDesc();
        AreaSize getSize();
        std::vector<Item*> getItems();
        std::vector<Character*> getCharacters();
        std::vector<Feature*> getFeatures();
        std::vector<Exit*> getExits();
        std::multimap<std::string, InteractiveNoun*> getVerbLookup();
        bool setName(std::string name);
        bool setShortDesc(std::string shortDescription);
        bool setLongDesc(std::string longDescription);
        bool setSize(AreaSize size);
        bool addItem(Item *anItem);
        bool addCharacter(Character *aCharacter);
        bool addFeature(Feature *aFeature);
        bool addExit(Exit *anExit);
        bool removeItem(Item *anItem);
        bool removeCharacter(Character *aCharacter);
        bool removeFeature(Feature *aFeature);
        bool removeExit(Exit *anExit);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual std::string listen(); 
        virtual bool go(Player*, Area*);
        virtual std::string search(); 
        virtual std::string warp(Player*, Area*); 
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        std::string shortDescription;
        std::string longDescription;
        AreaSize size;
        std::vector<Item*> itemContents;
        std::vector<Character*> characterContents;
        std::vector<Feature*> featureContents;
        std::vector<Exit*> exitContents;
        std::multimap<std::string, InteractiiveNoun*> verbLookup;
};

}}

#endif