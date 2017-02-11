/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/11/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.hpp
 *
 * \details     Header file for InteractiveNoun base class. Defines necessary
 *              members for aliasing nouns and commands and interface for defining
 *              command actions. This class should not be instantiated.
 ************************************************************************/

#ifndef INTERACTIVE_NOUN_HPP
#define INTERACTIVE_NOUN_HPP

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "CommandEnum.hpp"
#include "ObjectType.hpp"
#include "ItemPosition.hpp"

namespace legacymud { namespace parser {
    class Grammar;
}}

namespace legacymud { namespace engine {
class Player;
class Item;
class Area;
class Action;
class SpecialSkill;
class NonCombatant;
class Combatant;

/*!
 * \details     Defines necessary members for aliasing nouns and commands 
 *              and interface for defining command actions for inherited classes. 
 *              This class should not be instantiated.
 */
class InteractiveNoun {
    public:
        InteractiveNoun();
        InteractiveNoun(const InteractiveNoun &otherNoun);
        InteractiveNoun & operator=(const InteractiveNoun &otherNoun);
        virtual ~InteractiveNoun();

        /*!
         * \brief   Gets the action that is associated with the specified 
         *          CommandEnum.
         *          
         * \param[in] command   Specifies the command of the action to return.
         *
         * \return  Returns an Action* with the action that is associated with
         *          the specified command.
         */
        Action* getAction(CommandEnum command);

        /*!
         * \brief   Gets any actions that are associated with the specified 
         *          alias
         *          
         * \param[in] alias     Specifies verb alias to look up.
         *
         * \return  Returns a std::vector<Action*> with the actions that include
         *          the specified alias.
         */
        std::vector<Action*> getActions(std::string alias);

        /*!
         * \brief   Gets all of the noun aliases for this interactive noun.
         *
         * \return  Returns a std::vector<std::string> with the noun aliases.
         */
        std::vector<std::string> getNounAliases();

        /*!
         * \brief   Gets all of the verb aliases for the actions associated with
         *          this interactive noun.
         *
         * \return  Returns a std::vector<std::string> with the verb aliases.
         */
        std::vector<std::string> getVerbAliases();

        /*!
         * \brief   Gets whether or not this interactive noun supports the 
         *          specified command.
         *
         * \param[in] command   Specifies the command to check.
         *
         * \return  Returns a bool indicating whether or not this interactive 
         *          noun supports the specified command.
         */
        bool checkAction(CommandEnum command);

        /*!
         * \brief   Adds an action containing the specified command, if one 
         *          isn't already associated with this noun.
         *
         * \param[in] command   Specifies the command to add.
         *
         * \return  Returns a Action* with either the newly created action,
         *          or, if the command alread had an associated action for
         *          this noun, the preexisting action.
         */
        Action* addAction(CommandEnum command);

        /*!
         * \brief   Removes the action associated with the specified command,
         *          if it exists, from this noun.
         *
         * \param[in] command   Specifies the command to remove.
         *
         * \return  Returns a bool indicating whether or not the command was
         *          found and removed successfully.
         */
        bool removeAction(CommandEnum command);

        /*!
         * \brief   Adds the specified noun alias to this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to add.
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was added successfully.
         */
        bool addAlias(std::string);

        /*!
         * \brief   Removes the specified noun alias from this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to remove
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was found and removed successfully.
         */
        bool removeAlias(std::string);
        
        /*!
         * \brief   Gets the object type. This is a pure virtual function for
         *          interactive noun.
         */
        virtual ObjectType getObjectType() = 0;

        /*!
         * \brief   Gets the object's response to the look command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the look command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string look() { return ""; }  

        /*!
         * \brief   Gets the object's response to the listen command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the listen command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string listen() { return ""; } 

        /*!
         * \brief   Gets the object's response to the take command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the take command.
         *
         * \return  Returns false.
         */
        virtual bool take(Player*, Item*, InteractiveNoun*) { return false; }

        /*!
         * \brief   Gets the object's response to the put command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the put command.
         *
         * \return  Returns false.
         */
        virtual bool put(Player*, Item*, InteractiveNoun*, ItemPosition) { return false; }

        /*!
         * \brief   Gets the object's response to the drop command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the drop command.
         *
         * \return  Returns false.
         */
        virtual bool drop(Player*) { return false; }

        /*!
         * \brief   Gets the object's response to the more command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the more command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string more() { return ""; } 

        /*!
         * \brief   Gets the object's response to the equip command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the equip command.
         *
         * \return  Returns false.
         */
        virtual bool equip(Player*, Item*, InteractiveNoun*) { return false; }

        /*!
         * \brief   Gets the object's response to the unequip command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the unequip command.
         *
         * \return  Returns false.
         */
        virtual bool unequip(Player*, Item*, InteractiveNoun*) { return false; }

        /*!
         * \brief   Gets the object's response to the transfer command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the transfer command.
         *
         * \return  Returns false.
         */
        virtual bool transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*) { return false; }

        /*!
         * \brief   Gets the object's response to the go command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the go command.
         *
         * \return  Returns false.
         */
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character) { return false; }

        /*!
         * \brief   Gets the object's response to the move command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the move command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string move(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the attack command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the attack command.
         *
         * \return  Returns false.
         */
        virtual bool attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool) { return false; }

        /*!
         * \brief   Gets the object's response to the talk command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the talk command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string talk(Player*, NonCombatant*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the buy command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the buy command.
         *
         * \return  Returns false.
         */
        virtual bool buy(Player*, Item*) { return false; }

        /*!
         * \brief   Gets the object's response to the sell command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the sell command.
         *
         * \return  Returns false.
         */
        virtual bool sell(Player*, Item*) { return false; }

        /*!
         * \brief   Gets the object's response to the search command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the search command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string search(Player*) { return ""; }

        /*!
         * \brief   Gets the object's response to the useSkill command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the useSkill command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string useSkill(Player*, SpecialSkill*, InteractiveNoun*, Combatant*, bool) { return ""; } 

        /*!
         * \brief   Gets the object's response to the read command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the read command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string read(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the break command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the break command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string breakIt(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the climb command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the climb command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string climb(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the turn command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the turn command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string turn(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the push command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the push command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string push(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the pull command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the pull command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string pull(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the eat command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the eat command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string eat(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the drink command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the drink command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string drink(Player*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the warp command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the warp command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string warp(Player*, Area*) { return ""; } 

        /*!
         * \brief   Makes a copy of the object. This is a pure virtual function
         *          for interactive noun.
         */
        virtual InteractiveNoun* copy() = 0;

        /*!
         * \brief   Edits an attribute of the object. This is a pure virtual 
         *          function for interactive noun.
         */
        virtual bool editAttribute(Player*, std::string) = 0;

        /*!
         * \brief   Starts the edit wizard for the object. This is a pure virtual
         *          function for interactive noun.
         */
        virtual bool editWizard(Player*) = 0;
    private:
        std::vector<Action*> actions;
        std::vector<std::string> aliases;
};

}}

#endif