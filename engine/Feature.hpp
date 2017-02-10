/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/08/2017
 * Course:        CS467, Winter 2017
 * Filename:      Feature.hpp
 *
 * Overview:
 *     Header file for Feature class. Defines the members and 
 * functions that apply to all features.
 ************************************************************************/

#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <string>
#include "ConditionalElement.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Feature: public ConditionalElement {
    public:
        Feature();
        Feature(std::string name, std::string placement, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        Feature(const Feature &otherFeature);
        Feature & operator=(const Feature &otherFeature);
        virtual ~Feature();
        std::string getName();
        std::string getPlacement();
        bool setName(std::string);
        bool setPlacement(std::string);
        virtual ObjectType getObjectType();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual std::string listen(); 
        virtual std::string move(Player*); 
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        std::string placement;
};

}}

#endif