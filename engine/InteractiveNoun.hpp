/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/26/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.hpp
 *
 * \details     Header file for InteractiveNoun base class. Defines necessary
 *              members for aliasing nouns and commands and interface for 
 *              defining command actions. This class also allows for all 
 *              descendant class instances to be uniquely identified by an 
 *              int. Finally, this class requires all descendant classes 
 *              to implement functions for serializing and deserializing. 
 *              This class should not be instantiated.
 ************************************************************************/

#ifndef INTERACTIVE_NOUN_HPP
#define INTERACTIVE_NOUN_HPP

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <mutex>
#include <atomic>
#include "parser.hpp"
#include "CommandEnum.hpp"
#include "ObjectType.hpp"
#include "ItemPosition.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

class Action;
class SpecialSkill;
class Player;
class Item;
class Area;
class NonCombatant;
class Combatant;

/*!
 * \details     Defines necessary members for aliasing nouns and commands 
 *              and interface for defining command actions for inherited classes. 
 *              This class also allows for all descendant class instances to be  
 *              uniquely identified by an int. Finally, this class requires all 
 *              descendant classes to implement functions for serializing and 
 *              deserializing. This class should not be instantiated.
 */
class InteractiveNoun {
    public:
        InteractiveNoun();
        InteractiveNoun(const InteractiveNoun &otherNoun);
        InteractiveNoun & operator=(const InteractiveNoun &otherNoun);
        virtual ~InteractiveNoun();

        /*!
         * \brief   Gets the ID of this noun.
         * 
         * \return  Returns an int with the ID.
         */
        int getID() const;

        /*!
         * \brief   Gets the action that is associated with the specified 
         *          CommandEnum.
         *          
         * \param[in] command   Specifies the command of the action to return.
         *
         * \return  Returns an Action* with the action that is associated with
         *          the specified command.
         */
        Action* getAction(CommandEnum command) const;

        /*!
         * \brief   Gets any actions that are associated with the specified 
         *          alias
         *          
         * \param[in] alias     Specifies verb alias to look up.
         *
         * \return  Returns a std::vector<Action*> with the actions that include
         *          the specified alias.
         */
        std::vector<Action*> getActions(std::string alias) const;

        /*!
         * \brief   Gets all of the noun aliases for this interactive noun.
         *
         * \return  Returns a std::vector<std::string> with the noun aliases.
         */
        std::vector<std::string> getNounAliases() const;

        /*!
         * \brief   Gets all of the verb aliases for the actions associated with
         *          this interactive noun.
         *
         * \return  Returns a std::vector<std::string> with the verb aliases.
         */
        std::vector<std::string> getVerbAliases() const;

        /*!
         * \brief   Gets whether or not this interactive noun supports the 
         *          specified command.
         *
         * \param[in] command   Specifies the command to check.
         *
         * \return  Returns a bool indicating whether or not this interactive 
         *          noun supports the specified command.
         */
        bool checkAction(CommandEnum command) const;

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
        virtual bool addNounAlias(std::string);

        /*!
         * \brief   Removes the specified noun alias from this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to remove
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was found and removed successfully.
         */
        virtual bool removeNounAlias(std::string);

        /*!
         * \brief   Adds an alias of the specified command for this interactive noun.
         *
         * This function adds an alias-grammar pair to the map of aliases
         * for the Action associated with the specified command for this interactive 
         * noun.
         * 
         * \param[in] aCommand      Specifies the command the alias is aliasing.
         * \param[in] alias         Specifies the verb alias to be added.
         * \param[in] direct        Specifies support for direct objects.
         * \param[in] indirect      Specifies support for indirect objects.
         * \param[in] prepositions  Specifies supported prepositions.
         *
         * \return  Returns a bool indicating whether or not adding the
         *          alias to the interactive noun succeeded.
         */
        virtual bool addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions);

        /*!
         * \brief   Removes the verb alias for the specified command from this 
         * interactive noun.
         *
         * This function removes the alias-grammar pair indicated by the
         * specified alias string from the Action associated with the specified 
         * command for this interactive noun.
         * 
         * \param[in] aCommand  Specifies the command the alias is aliasing.
         * \param[in] alias     Specifies the verb alias to remove.
         *
         * \return  Returns a bool indicating whether or not removing the
         *          specified alias succeeded.
         */
        virtual bool removeVerbAlias(CommandEnum aCommand, std::string alias);

        /*!
         * \brief   Registers the specified alias - object pair in this object's 
         *          lexical data.
         * 
         * \param[in] isVerb    Specifies whether or not the alias is a verb.
         * \param[in] alias     Specifies the alias to register.
         * \param[in] anObject  Specifies the object being aliased.
         * 
         * \note    This function will be overridden in child classes that 
         *          support registering aliases.
         *
         * \return  Returns a bool indicating whether or not registering the
         *          specified alias succeeded.
         */
        virtual bool registerAlias(bool isVerb, std::string alias, InteractiveNoun *anObject) { return false; }

        /*!
         * \brief   Unregisters the specified alias - object pair in this object's 
         *          lexical data.
         * 
         * \param[in] isVerb    Specifies whether or not the alias is a verb.
         * \param[in] alias     Specifies the alias to unregister.
         * \param[in] anObject  Specifies the object being aliased.
         * 
         * \note    This function will be overridden in child classes that 
         *          support registering aliases.
         *
         * \return  Returns a bool indicating whether or not unregistering the
         *          specified alias succeeded.
         */
        virtual bool unregisterAlias(bool isVerb, std::string alias, InteractiveNoun *anObject) { return false; }

        /*!
         * \brief   Gets the text and effect for the specified command.
         *
         * \param[in] aCommand  Specifies the command to look up.
         * \param[out] anEffect Specifies the effect of the command.
         *
         * \return  Returns a std::string with the flavor text for the specified
         *          command.
         */
        std::string getTextAndEffect(CommandEnum aCommand, EffectType &anEffect) const;

        /*!
         * \brief   Gets the name. This is a pure virtual function for
         *          interactive noun.
         */
        virtual std::string getName() const = 0;
        
        /*!
         * \brief   Gets the object type. This is a pure virtual function for
         *          interactive noun.
         */
        virtual ObjectType getObjectType() const = 0;

        /*!
         * \brief   Gets the object's response to the look command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the look command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string look(std::vector<EffectType>*) { return ""; }  

        /*!
         * \brief   Gets the object's response to the listen command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the listen command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string listen(std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the take command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the take command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string take(Player*, Item*, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the put command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the put command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string put(Player*, Item*, InteractiveNoun*, ItemPosition, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the drop command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the drop command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string drop(Player*, std::vector<EffectType>*) { return ""; }

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
         * \return  Returns an empty string.
         */
        virtual std::string equip(Player*, Item*, InteractiveNoun*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the unequip command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the unequip command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string unequip(Player*, Item*, InteractiveNoun*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the transfer command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the transfer command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the go command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the go command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the move command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the move command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string move(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the attack command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the attack command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the talk command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the talk command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string talk(Player*, NonCombatant*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the buy command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the buy command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string buy(Player*, Item*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the sell command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the sell command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string sell(Player*, Item*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the search command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the search command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string search(Player*, std::vector<EffectType>*) { return ""; }

        /*!
         * \brief   Gets the object's response to the useSkill command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the useSkill command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string useSkill(Player*, SpecialSkill*, InteractiveNoun*, Combatant*, bool, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the read command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the read command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string read(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the break command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the break command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string breakIt(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the climb command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the climb command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string climb(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the turn command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the turn command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string turn(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the push command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the push command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string push(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the pull command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the pull command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string pull(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the eat command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the eat command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string eat(Player*, std::vector<EffectType>*) { return ""; } 

        /*!
         * \brief   Gets the object's response to the drink command.
         * 
         * \note    This function will be overridden in child classes that 
         *          support the drink command.
         *
         * \return  Returns an empty string.
         */
        virtual std::string drink(Player*, std::vector<EffectType>*) { return ""; } 

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

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize() = 0;

        /*!
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string) = 0;
    private:
        std::vector<Action*> actions;
        mutable std::mutex actionsMutex;
        std::vector<std::string> aliases;
        mutable std::mutex aliasesMutex;
        const int ID;
        static std::atomic<int> nextID;
};

}}

#endif