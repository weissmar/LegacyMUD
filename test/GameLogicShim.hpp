/*!
  \file     GameLogicShim.hpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the GameLogicShim class for accessing
            private members of the GameLogic class.
*/
#ifndef LEGACYMUD_TEST_GAMELOGICSHIM_HPP
#define LEGACYMUD_TEST_GAMELOGICSHIM_HPP

#include <GameLogic.hpp>

namespace legacymud { namespace test {

class GameLogicShim {
    public:
        /*!
         * \brief   Creates the shim layer for the specified GameLogic instance.
         */
        GameLogicShim(engine::GameLogic *logic);

        /*!
         * \brief   Loads the specified player into the game.
         * 
         * This function loads the specified player into the game and stores the
         * fileDescriptor in the player object.
         * 
         * \param[in] aPlayer           Specifies the player to load.
         * \param[in] fileDescriptor    Specifies the player's server identifier.
         *
         * \return  Returns a bool indicating whether or not loading the player
         *          was successful.
         */
        bool loadPlayer(engine::Player *aPlayer, int fileDescriptor);

        /*!
         * \brief   Hibernates the specified player.
         * 
         * This function hibernates the specified player by removing them from the 
         * active game (other players will no longer receive notices about this
         * player). The player must be loaded before they can play more.
         * 
         * \param[in] aPlayer           Specifies the player to hibernate.
         *
         * \return  Returns a bool indicating whether or not hibernating the player
         *          was successful.
         */
        bool hibernatePlayer(engine::Player *aPlayer);

        /*!
         * \brief   Rolls the specifed number of the specified sided dice.
         * 
         * This function "rolls" dice using a random number generator.
         * 
         * \param[in] numSides      Specifies the number of sides each die has.
         * \param[in] numDice       Specifies the number of dice to roll.
         *
         * \return  Returns an int with the results of the roll (the total value of
         *          all the dice rolled).
         */
        int rollDice(int numSides, int numDice);

        /*!
         * \brief   Creates a new instance of the specified object type.
         * 
         * This function asks the specified player for the required fields and creates
         * the specfied new object.
         * 
         * \param[in] aPlayer   Specifies the player who entered the command.
         * \param[in] type      Specifies the type of object to create.
         *
         * \return  Returns a bool indicating whether or not creating the object
         *          was successful.
         */
        bool createObject(engine::Player *aPlayer, engine::ObjectType type);

        /*!
         * \brief   Gets the object type based on the input string.
         * 
         * \param[in] input     Specifies the object type in string format.
         *
         * \return  Returns an ObjectType with the object type.
         */
        engine::ObjectType getObjectType(const std::string &input);

        /*!
         * \brief   Sends the specified message to the specified player.
         * 
         * \param[in] aPlayer   Specifies the player to message.
         * \param[in] message   Specifies the message to send.
         */
        void messagePlayer(engine::Player *aPlayer, std::string message);

        /*!
         * \brief   Sends the specified message to all active players,
         *          except the specified player.
         * 
         * \param[in] aPlayer   Optionally specifies the player to exclude 
         *                      from this message.
         * \param[in] message   Specifies the message to send.
         */
        void messageAllPlayers(engine::Player *aPlayer, std::string message);

        /*!
         * \brief   Sends the specified message to all active players
         *          in the specifed area, except the specified player.
         * 
         * \param[in] aPlayer   Optionally specifies the player to exclude 
         *                      from this message.
         * \param[in] message   Specifies the message to send.
         * \param[in] anArea    Specifies the area.
         */
        void messageAreaPlayers(engine::Player *aPlayer, std::string message, engine::Area *anArea);

        /*!
         * \brief   Sends message prompt to user and gets response back.
         * 
         * \param[in] FD            Specifies the user's file descriptor.
         * \param[in] outMessage    Specifies the prompt message to send.
         * \param[out] response     Specifies the variable to return the user's
         *                          response in.
         * 
         * \note    This function disconnects the user if it cannot receive a
         *          message from them.
         *
         * \return  Returns a bool indicating whether or not getting the value
         *          was successful.
         */
        bool getValueFromUser(int FD, std::string outMessage, std::string &response);

        /*!
         * \brief   Starts combat between the specfied player and the specified
         *          creature.
         * 
         * \param[in] aPlayer       Specifies the player entering combat.
         * \param[in] aCreature     Specifies the creature entering combat.
         *
         * \return  Returns a bool indicating whether or not entering combat
         *          was successful.
         */
        bool startCombat(engine::Player* aPlayer, engine::Creature *aCreature);

        /*!
         * \brief   Ends combat between the specfied player and the specified
         *          creature.
         * 
         * \param[in] aPlayer       Specifies the player leaving combat.
         * \param[in] aCreature     Specifies the creature leaving combat.
         *
         * \return  Returns a bool indicating whether or not ending combat
         *          was successful.
         */
        bool endCombat(engine::Player *aPlayer, engine::Creature *aCreature);

        /*!
         * \brief   Starts a conversation between the specfied player and the 
         *          specified non-combatant.
         * 
         * \param[in] aPlayer   Specifies the player entering a conversation.
         * \param[in] aNPC      Specifies the non-combatant entering a conversation.
         *
         * \return  Returns a bool indicating whether or not starting a conversation
         *          was successful.
         */
        bool startConversation(engine::Player *aPlayer, engine::NonCombatant *aNPC);

        /*!
         * \brief   Ends the conversation the specfied player is currently engaged in.
         * 
         * \param[in] aPlayer   Specifies the player leaving the conversation.
         *
         * \return  Returns a bool indicating whether or not ending the conversation
         *          was successful.
         */
        bool endConversation(engine::Player *aPlayer);

        /*!
         * \brief   Handles a parse error that has one result.
         * 
         * \param[in] result   Specifies the result received from the parser.
         * \param[in] aPlayer   Specifies the player that sent the messsage.
         */
        void handleParseError(engine::Player *aPlayer, parser::ParseResult result);

        /*!
         * \brief   Prints the type, status, command, and unparsed fields of the
         *          specified ParseResult to stdout.
         * 
         * \param[in] result   Specifies the result received from the parser.
         */
        void printParseResult(parser::ParseResult result);

        /*!
         * \brief   Handles a parse error that has multiple results.
         * 
         * \param[in] results   Specifies the results received from the parser.
         * \param[in] aPlayer   Specifies the player that sent the messsage.
         */
        void handleParseErrorMult(engine::Player *aPlayer, std::vector<parser::ParseResult> results);

        /*!
         * \brief   Sends a clarifying question with the options to the player.
         * 
         * \param[in] aPlayer           Specifies the player that sent the messsage.
         * \param[in] optionsVector     Specifies the options to choose from.
         */
        void sendClarifyingQuery(engine::Player *aPlayer, std::vector<engine::InteractiveNoun*> optionsVector);

        /*!
         * \brief   Clarifies a choice with the user.
         * 
         * \param[in] aPlayer           Specifies the player that sent the messsage.
         * \param[in] optionsVector     Specifies the options to choose from.
         * 
         * \return  Returns a pointer to the chosen object.
         */
        engine::InteractiveNoun* clarifyChoice(engine::Player *aPlayer, std::vector<engine::InteractiveNoun*> optionsVector);

        /*!
         * \brief   Blocks until gets a message from the dedicated message  
         *          queue for the specified player.
         * 
         * \param[in] aPlayer   Specifies the player.
         * 
         * \return  Returns the message.
         */
        std::string blockingGetMsg(engine::Player *aPlayer);

        /*!
         * \brief   Adds a dedicated message queue for the specified player.
         * 
         * \param[in] aPlayer   Specifies the player.
         */
        void addPlayerMessageQueue(engine::Player *aPlayer);

        /*!
         * \brief   Removes the dedicated message queue for the specified player.
         * 
         * \param[in] aPlayer   Specifies the player.
         */
        void removePlayerMessageQueue(engine::Player *aPlayer);

        /*!
         * \brief   Gets a message from the dedicated message queue for the 
         *          specified player.
         * 
         * \param[in] aPlayer   Specifies the player.
         * 
         * \return  Returns the message or empty string if queue is empty.
         */
        std::string getMsgFromPlayerQ(engine::Player *aPlayer);

        /*!
         * \brief   Executes the specified command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] result    Specifies the results from the parser.
         *
         * \return  Returns a bool indicating whether or not executing the command
         *          was successful.
         */
        bool executeCommand(engine::Player *aPlayer, parser::ParseResult result);

        /*!
         * \brief   Clarifies the direct object to use.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] result    Specifies the results from the parser.
         *
         * \return  Returns a pointer to the chosen interactive noun.
         */
        engine::InteractiveNoun* clarifyDirect(engine::Player *aPlayer, parser::ParseResult result);

        /*!
         * \brief   Clarifies the indirect object to use.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] result    Specifies the results from the parser.
         *
         * \return  Returns a pointer to the chosen interactive noun.
         */
        engine::InteractiveNoun* clarifyIndirect(engine::Player *aPlayer, parser::ParseResult result);

        /*!
         * \brief   Executes the help command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the help command
         *          was successful.
         */
        bool helpCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the effects of an action.
         * 
         * \param[in] aPlayer   Specifies the player experiencing the effects.
         * \param[in] effects   Specifies the effects to execute.
         *
         * \return  Returns a string with a description of the effects.
         */
        std::string handleEffects(engine::Player *aPlayer, std::vector<engine::EffectType> effects);

        /*!
         * \brief   Executes the look command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] param     Optionally specifies the object being looked at.
         *
         * \return  Returns a bool indicating whether or not executing the look command
         *          was successful.
         */
        bool lookCommand(engine::Player *aPlayer, engine::InteractiveNoun *param);

        /*!
         * \brief   Executes the listen command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the listen command
         *          was successful.
         */
        bool listenCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the take command.
         * 
         * \param[in] aPlayer       Specifies the player taking the directObj.
         * \param[in] directObj     Specifies the direct object being taken.
         * \param[in] indirectObj   Optionally specifies the indirect object the directObj is 
         *                          being taken from.
         *                          
         * \return  Returns a bool indicating whether or not executing the take command
         *          was successful.
         */
        bool takeCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj);

        /*!
         * \brief   Executes the put command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object being placed.
         * \param[in] indirectObj   Specifies the indirect object the directObj is being placed
         *                          on/in/under.
         * \param[in] aPosition     Specifies the position the directObj is being placed in.
         *
         * \return  Returns a bool indicating whether or not executing the put command
         *          was successful.
         */
        bool putCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj, engine::ItemPosition aPosition);

        /*!
         * \brief   Executes the drop command.
         * 
         * \param[in] aPlayer       Specifies the player dropping the item.
         * \param[in] directObj     Specifies the item being dropped.
         *
         * \return  Returns a bool indicating whether or not executing the drop command
         *          was successful.
         */
        bool dropCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the inventory command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the inventory command
         *          was successful.
         */
        bool inventoryCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the more command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *                          
         * \return  Returns a bool indicating whether or not executing the more command
         *          was successful.
         */
        bool moreCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the equipment command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the equipment command
         *          was successful.
         */
        bool equipmentCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the equip command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the equip command
         *          was successful.
         */
        bool equipCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the unequip command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the unequip command
         *          was successful.
         */
        bool unequipCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the transfer command.
         * 
         * \param[in] aPlayer       Specifies the player transferring the item.
         * \param[in] directObj     Specifies the direct object being transferred.
         * \param[in] indirectObj   Specifies the indirect object receiving the item.
         *
         * \return  Returns a bool indicating whether or not executing the transfer command
         *          was successful.
         */
        bool transferCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj);

        /*!
         * \brief   Executes the speak command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the speak command
         *          was successful.
         */
        bool speakCommand(engine::Player *aPlayer, const std::string &stringParam);

        /*!
         * \brief   Executes the shout command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the shout command
         *          was successful.
         */
        bool shoutCommand(engine::Player *aPlayer, const std::string &stringParam);

        /*!
         * \brief   Executes the whisper command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] indirectObj   Specifies the indirect object recevied from the 
         *                          parser.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the whisper command
         *          was successful.
         */
        bool whisperCommand(engine::Player *aPlayer, engine::InteractiveNoun *indirectObj, const std::string &stringParam);

        /*!
         * \brief   Executes the quit command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the quit command
         *          was successful.
         */
        bool quitCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the go command.
         * 
         * \param[in] aPlayer   Specifies the player that is moving.
         * \param[in] param     Specifies where to go.
         * 
         * \return  Returns a bool indicating whether or not executing the go command
         *          was successful.
         */
        bool goCommand(engine::Player *aPlayer, engine::InteractiveNoun *param);

        /*!
         * \brief   Executes the move command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *                          
         * \return  Returns a bool indicating whether or not executing the move command
         *          was successful.
         */
        bool moveCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the stats command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the stats command
         *          was successful.
         */
        bool statsCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the quests command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the quests command
         *          was successful.
         */
        bool questsCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the skills command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the skills command
         *          was successful.
         */
        bool skillsCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the attack command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         * \param[in] indirectObj   Specifies the indirect object recevied from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the attack command
         *          was successful.
         */
        bool attackCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj);

        /*!
         * \brief   Executes the talk command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] param     Specifies the character being talked to.
         *
         * \return  Returns a bool indicating whether or not executing the talk command
         *          was successful.
         */
        bool talkCommand(engine::Player *aPlayer, engine::InteractiveNoun *param);

        /*!
         * \brief   Executes the shop command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the shop command
         *          was successful.
         */
        bool shopCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the buy command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the buy command
         *          was successful.
         */
        bool buyCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the sell command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the sell command
         *          was successful.
         */
        bool sellCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the search command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the search command
         *          was successful.
         */
        bool searchCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the useSkill command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         * \param[in] indirectObj   Specifies the indirect object recevied from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the useSkillcommand
         *          was successful.
         */
        bool useSkillCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj);

        /*!
         * \brief   Executes the read command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the read command
         *          was successful.
         */
        bool readCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the break command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the break command
         *          was successful.
         */
        bool breakCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the climb command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the climb command
         *          was successful.
         */
        bool climbCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the turn command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the turn command
         *          was successful.
         */
        bool turnCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the push command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the push command
         *          was successful.
         */
        bool pushCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the pull command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the pull command
         *          was successful.
         */
        bool pullCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the eat command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the eat command
         *          was successful.
         */
        bool eatCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the drink command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the drink command
         *          was successful.
         */
        bool drinkCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the edit mode command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         *
         * \return  Returns a bool indicating whether or not executing the edit mode command
         *          was successful.
         */
        bool editModeCommand(engine::Player *aPlayer);

        /*!
         * \brief   Executes the warp command.
         * 
         * \param[in] aPlayer   Specifies the player entering the command.
         * \param[in] param     Specifies where to warp to.
         *
         * \return  Returns a bool indicating whether or not executing the warp command
         *          was successful.
         */ 
        bool warpCommand(engine::Player *aPlayer, engine::InteractiveNoun *param);

        /*!
         * \brief   Executes the copy command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the copy command
         *          was successful.
         */
        bool copyCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the create command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] stringParam   Specifies the indirect object recevied from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the create command
         *          was successful.
         */
        bool createCommand(engine::Player *aPlayer, const std::string &stringParam);

        /*!
         * \brief   Executes the edit attribute command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] indirectObj   Specifies the direct object received from the 
         *                          parser.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the edit attribute command
         *          was successful.
         */
        bool editAttributeCommand(engine::Player *aPlayer, engine::InteractiveNoun *indirectObj, const std::string &stringParam);

        /*!
         * \brief   Executes the edit wizard command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the edit wizard command
         *          was successful.
         */
        bool editWizardCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Executes the save command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the save command
         *          was successful.
         */
        bool saveCommand(engine::Player *aPlayer, const std::string &stringParam);

        /*!
         * \brief   Executes the load command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] stringParam   Specifies the string parameter received from the
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the load command
         *          was successful.
         */
        bool loadCommand(engine::Player *aPlayer, const std::string &stringParam);

        /*!
         * \brief   Executes the delete command.
         * 
         * \param[in] aPlayer       Specifies the player entering the command.
         * \param[in] directObj     Specifies the direct object received from the 
         *                          parser.
         *
         * \return  Returns a bool indicating whether or not executing the delete command
         *          was successful.
         */
        bool deleteCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj);

        /*!
         * \brief   Converts string to int and validates.
         * 
         * \param[in] number    String to attempt to convert.
         * \param[in] min       Minimum acceptable value.
         * \param[in] max       Maximum acceptable value.
         *
         * \return  Returns an int with the converted number if it passed validation,
         *          -1 otherwise.
         */
        int validateStringNumber(std::string number, int min, int max);

        account::Account *getAccountSystem();

        telnet::Server *getTelnetServer();

        engine::GameObjectManager *getGameObjectManager();

        std::queue<std::pair<std::string, int>> *getMessageQueue();

    private:
        engine::GameLogic *_logic;
};

}}

#endif
