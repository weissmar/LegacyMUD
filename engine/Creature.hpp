/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Creature.hpp
 *
 * Overview:
 *     Header file for Creature class. Defines the members and 
 * functions that apply to all creatures.
 ************************************************************************/

#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include "Combatant.hpp"
#include "Area.hpp"
#include "CreatureType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Creature: public Combatant {
    public:
        Creature();
        Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Creature(const Creature &otherCreature);
        Creature & operator=(const Creature &otherCreature);
        virtual ~Creature();
        CreatureType* getType();
        bool getAmbulatory();
        bool setType(CreatureType *aType);
        bool setAmbulatory(bool ambulatory);
        Area* move();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual bool attack(Player*, Item*, InteractiveNoun*);
        virtual std::string useSkill(SpecialSkill*, InteractiveNoun*); 
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        CreatureType *type;
        bool ambulatory;
};

}}

#endif