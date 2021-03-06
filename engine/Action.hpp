/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/15/2017
 * \course      CS467, Winter 2017
 * \file        Action.hpp
 *
 * \details     Header file for Action class. Defines the members and 
 *              functions that apply to all actions. Actions are stored
 *              in InteractiveNouns and hold a command that can be applied
 *              to that object, along with information about verb aliases
 *              (and grammar) for the command, and the effects of the
 *              command when used with the corresponding noun.
 ************************************************************************/

#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <map>
#include <vector>
#include <atomic>
#include <mutex>
#include "CommandEnum.hpp"
#include "EffectType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace parser {
    class Grammar;
}}


namespace legacymud { namespace engine {

/*!
 * \details     This class defines actions that can be taken by/on each 
 *              InteractiveNoun object along with the verb aliases and 
 *              grammar for each action. It also records the results of
 *              the action by way of text and an EffectType.
 */
class Action {
    public:
        Action();
        Action(CommandEnum command);
        Action(CommandEnum command, bool valid, std::string flavorText, EffectType effect);
        Action(const Action &otherAction);
        Action & operator=(const Action &otherAction);
        bool operator==(const Action &otherAction) const;
        virtual ~Action();

        /*!
         * \brief   Gets the command that is associated with this action.
         *
         * \return  Returns a CommandEnum indicating the command that is
         *          associated with this action.
         */
        CommandEnum getCommand() const;

        /*!
         * \brief   Gets whether this action is currently valid.
         *
         * \return  Returns a bool indicating whether this action is 
         *          currently valid.
         */
        bool getValid() const;

        /*!
         * \brief   Gets the flavor text associated with this action.
         *          
         * \return  Returns a std::string with the flavor text associated
         *          with this action.
         */
        std::string getFlavorText() const;

        /*!
         * \brief   Gets the effect of this action.
         *
         * \return  Returns an EffectType indicating the effect of this
         *          action.
         */
        EffectType getEffect() const;

        /*!
         * \brief   Gets the grammar associated with the specified alias.
         *
         * \param[in] alias     Specifies the alias for which to get the 
         *                      grammar.
         *                  
         * \return  Returns a parser::Grammar* with the grammar associated
         *          with the indicated alias.
         */
        parser::Grammar* getGrammar(std::string alias) const;

        /*!
         * \brief   Gets the aliases and associated grammar for this action.
         *
         * \return  Returns a map of std::string aliases to parser::Grammar*
         *          for this action.
         */        
        std::map<std::string, parser::Grammar*> getAliasesAndGrammar() const;

        /*!
         * \brief   Gets the aliases for this action.
         *
         * \return  Returns a vector of std::string aliases for this action.
         */        
        std::vector<std::string> getAliases() const;

        /*!
         * \brief   Gets the name (string version of command) for this action
         *
         * \return  Returns a std::string with the name for this action.
         */      
        std::string getName() const;

        /*!
         * \brief   Gets a string representation of this action
         *
         * \return  Returns a std::string with the string version of this action.
         */  
        std::string getToString() const;

        /*!
         * \brief   Gets whether or not the specfied alias is an alias for this
         *          action.
         * 
         * \param[in] anAlias   Specifies the alias to look up.
         *
         * \return  Returns true if the specified alias is an alias of this 
         *          action, false otherwise.
         */  
        bool isAlias(std::string anAlias) const;

        /*!
         * \brief   Sets whether or not this action is currently valid.
         * 
         * \param[in] validCommand  Specifies whether or not the action
         *                          is valid.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          validity of the action succeeded.
         */
        bool setValid(bool validCommand);

        /*!
         * \brief   Sets the flavor text associated with this action.
         * 
         * \param[in] flavorText    Specifies the text to be used when
         *                          this action is taken.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          flavor text succeeded.
         */
        bool setFlavorText(std::string flavorText);

        /*!
         * \brief   Sets the effect of this action.
         * 
         * \param[in] type  Specifies the effect of this action.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          effect succeeded.
         */
        bool setEffect(EffectType type);

        /*!
         * \brief   Adds an alias of the command for this action.
         *
         * This function adds an alias-grammar pair to the map of aliases
         * for this action.
         * 
         * \param[in] alias     Specifies the verb alias to be added.
         * \param[in] grammar   Specifies the grammar to be associated with
         *                      the alias.
         *
         * \return  Returns a bool indicating whether or not adding the
         *          alias to the action succeeded.
         */
        bool addAlias(std::string alias, parser::Grammar *grammar);

        /*!
         * \brief   Removes the alias from this action.
         *
         * This function removes the alias-grammar pair indicated by the
         * specified alias string from this aciton.
         * 
         * \param[in] alias     Specifies the verb alias to remove.
         *
         * \return  Returns a bool indicating whether or not removing the
         *          specified alias succeeded.
         */
        bool removeAlias(std::string alias);

        /*!
         * \brief   Gets the attribute signature of the class.
         *
         * This function returns a map of string to DataType with the 
         * attributes required by the Action class to instantiate a new
         * action.
         * 
         * \return  Returns a map of std::string to DataType indicating 
         *          the required attributes.
         */
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::atomic<CommandEnum> command;
        std::atomic<bool> valid;
        std::string flavorText;
        mutable std::mutex flavorTextMutex;
        std::atomic<EffectType> effect;
        std::map<std::string, parser::Grammar*> aliases;
        mutable std::mutex aliasesMutex;
};

}}

#endif