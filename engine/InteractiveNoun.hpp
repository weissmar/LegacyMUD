/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
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
#include <Grammar.h>
#include "CommandEnum.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Action;
class SpecialSkill;

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
         * \brief   Removess the specified noun alias from this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to remove
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was found and removed successfully.
         */
        bool removeAlias(std::string);
        
        virtual ObjectType getObjectType();
        virtual std::string look();  
        virtual std::string listen(); 
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool put(Player*, Item*, InteractiveNoun*, ItemPosition);
        virtual bool drop(Player*);
        virtual std::string more(); 
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*, InteractiveNoun*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*);
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);
        virtual std::string move(Player*); 
        virtual bool attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool);
        virtual std::string talk(Player*, NonCombatant*); 
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string search(Player*); 
        virtual std::string useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill); 
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual std::string warp(Player*, Area*); 
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
    private:
        std::vector<Action*> actions;
        std::vector<std::string> aliases;
};

}}

#endif