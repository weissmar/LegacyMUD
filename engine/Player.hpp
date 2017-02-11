/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        Player.hpp
 *
 * \details     Header file for Player class and Command struct. Defines 
 *              the members and functions that apply to all players. 
 *              Player defines the in-game players that are controlled
 *              by users of the game. Command defines a command from a
 *              user.
 ************************************************************************/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <queue>
#include <vector>
#include <tuple>
#include "Combatant.hpp"
#include "CharacterSize.hpp"
#include "CommandEnum.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Area;
class Quest;
class NonCombatant;
class PlayerClass;
class InteractiveNoun;
class SpecialSkill;

/*!
 * \details     Command defines a command from a user.
 */
struct Command {
    CommandEnum commandE;
    InteractiveNoun *firstParam;
    InteractiveNoun *secondParam;
};

/*!
 * \details     Player defines the in-game players that are controlled
 *              by users of the game.
 */
class Player: public Combatant {
    public:
        Player();
        Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Player(const Player &otherPlayer);
        Player & operator=(const Player &otherPlayer);
        virtual ~Player();

        /*!
         * \brief   Gets the experience points of this player.
         *
         * \return  Returns an int with the experience points.
         */
        int getExperiencePoints();

        /*!
         * \brief   Gets the level of this player.
         *
         * \return  Returns an int with the level.
         */
        int getLevel();

        /*!
         * \brief   Gets the size of this player.
         *
         * \return  Returns a CharacterSize with the size.
         */
        CharacterSize getSize();

        /*!
         * \brief   Gets the user name of this player.
         *
         * \return  Returns a std::string with the user name.
         */
        std::string getUser();

        /*!
         * \brief   Gets the player class of this player.
         *
         * \return  Returns a PlayerClass* with the player class.
         */
        PlayerClass* getPlayerClass();

        /*!
         * \brief   Gets the non-combatant this player is in conversation with.
         *
         * \return  Returns a NonCombatant* with the non-combatant this player
         *          is in conversation with.
         */
        NonCombatant* getInConversation();

        /*!
         * \brief   Gets whether or not this player is active.
         *
         * \return  Returns a bool indicating whether or not this player is 
         *          active.
         */
        bool isActive();

        /*!
         * \brief   Gets the file descriptor of this player.
         *
         * \return  Returns an int with the file descriptor.
         */
        int getFileDescriptor();

        /*!
         * \brief   Gets whether or not this player's combat queue is empty.
         *
         * \return  Returns a bool whether or not this player's combat queue 
         *          is empty.
         */
        bool queueIsEmpty();

        /*!
         * \brief   Gets whether or not this player is in edit mode.
         *
         * \return  Returns a bool whether or not this player is in edit mode.
         */
        bool isEditMode();

        /*!
         * \brief   Gets the quest list of this player.
         *
         * \return  Returns a std::vector<std::tuple<Quest*, int>> with the 
         *          quest list.
         */
        std::vector<std::tuple<Quest*, int>> getQuestList();

        /*!
         * \brief   Gets the verb lookup for the items in this player's 
         *          inventory and equipment.
         *
         * \return  Returns a std::multimap<std::string, InteractiveNoun*> with  
         *          the verb lookup
         */
        std::multimap<std::string, InteractiveNoun*> getVerbLookup();

        /*!
         * \brief   Gets the noun lookup for the items in this player's 
         *          inventory and equipment.
         *
         * \return  Returns a std::multimap<std::string, InteractiveNoun*> with  
         *          the noun lookup
         */
        std::multimap<std::string, InteractiveNoun*> getNounLookup();

        /*!
         * \brief   Adds the specified points to the experience points of 
         *          this player.
         *          
         * \param[in] gainedXP  Specifies the points to add.
         *
         * \return  Returns an int with the new total of experience points.
         */
        int addToExperiencePts(int gainedXP);

        /*!
         * \brief   Levels up this player.
         *
         * \return  Returns a bool indicating whether or not the player was
         *          successfully leveled up.
         */
        bool levelUp();

        /*!
         * \brief   Sets the size of this player.
         *          
         * \param[in] size  Specifies the size of the player.
         *
         * \return  Returns a bool indicating whether or not the size was
         *          successfully set.
         */
        bool setSize(CharacterSize size);

        /*!
         * \brief   Sets the player class of this player.
         *          
         * \param[in] aClass  Specifies the player class of the player.
         *
         * \return  Returns a bool indicating whether or not the player class was
         *          successfully set.
         */
        bool setPlayerClass(PlayerClass *aClass);

        /*!
         * \brief   Sets the active status of this player.
         *          
         * \param[in] active  Specifies the active status of the player.
         *
         * \return  Returns a bool indicating whether or not the active status was
         *          successfully set.
         */
        bool setActive(bool active); 

        /*!
         * \brief   Sets the file descriptor of this player.
         *          
         * \param[in] FD    Specifies the file descriptor of the player.
         *
         * \return  Returns a bool indicating whether or not the file descriptor was
         *          successfully set.
         */
        bool setFileDescriptor(int FD);

        /*!
         * \brief   Sets the non-combatant that the player is talking to.
         *          
         * \param[in] anNPC     Specifies the non-combatant that the player is talking to.
         *
         * \return  Returns a bool indicating whether or not the non-combatant that the 
         *          player is talking to was successfully set.
         */
        bool setInConversation(NonCombatant *anNPC);

        /*!
         * \brief   Gets the next command in the player's combat queue.
         *
         * \return  Returns a Command* with the command or nullptr if the queue is empty.
         */
        Command* getNextCommand();

        /*!
         * \brief   Adds a command to the player's combat queue.
         * 
         * \param[in] aCommand  Specifies the command to add.
         *
         * \return  Returns a bool indicating whether or not the command was successfully 
         *          added.
         */
        bool addCommand(Command *aCommand);

        /*!
         * \brief   Sets whether or not the player is in edit mode.
         * 
         * \param[in] editing  Specifies whether or not the player is in edit mode.
         *
         * \return  Returns a bool indicating whether or not the edit mode status was 
         *          successfully set.
         */
        bool setEditMode(bool editing);

        /*!
         * \brief   Adds a quest to the player's quest list.
         * 
         * \param[in] aQuest    Specifies the quest to add.
         * \param[in] step      Specifies the step of the quest the player is currently on.
         *
         * \return  Returns a bool indicating whether or not the quest was successfully 
         *          added.
         */
        bool addQuest(Quest *aQuest, int step);

        /*!
         * \brief   Updates a quest in the player's quest list.
         * 
         * \param[in] aQuest    Specifies the quest to update.
         * \param[in] step      Specifies the step of the quest the player is currently on.
         *
         * \return  Returns a bool indicating whether or not the quest was successfully 
         *          updated.
         */
        bool updateQuest(Quest *aQuest, int step); 

        /*!
         * \brief   Adds specified verbs/object pair to verb lookup.
         * 
         * \param[in] verbs     Specifies verbs to add.
         * \param[in] anObject  Specifies object associated with the verbs to add.
         *
         * \return  Returns a bool indicating whether or not the verbs were successfully 
         *          added.
         */
        bool addVerbs(std::vector<std::string> verbs, InteractiveNoun *anObject);

        /*!
         * \brief   Adds specified nouns/object pair to noun lookup.
         * 
         * \param[in] nouns     Specifies nouns to add.
         * \param[in] anObject  Specifies object associated with the nouns to add.
         *
         * \return  Returns a bool indicating whether or not the nouns were successfully 
         *          added.
         */
        bool addNouns(std::vector<std::string> nouns, InteractiveNoun *anObject);

        /*!
         * \brief   Removes verbs associated with the specified object from the verb
         *          lookup.
         * 
         * \param[in] anObject  Specifies object associated with the verbs to remove.
         *
         * \return  Returns a bool indicating whether or not the verbs were successfully 
         *          removed.
         */
        bool removeVerbs(InteractiveNoun *anObject);

        /*!
         * \brief   Removes nouns associated with the specified object from the noun
         *          lookup.
         * 
         * \param[in] anObject  Specifies object associated with the nouns to remove.
         *
         * \return  Returns a bool indicating whether or not the nouns were successfully 
         *          removed.
         */
        bool removeNouns(InteractiveNoun *anObject);

        /*!
         * \brief   Gets the object type.
         *
         * \return  Returns an ObjectType indicating the actual class the object
         *          belongs to.
         */
        virtual ObjectType getObjectType();

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();

        /*!
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string);

        /*!
         * \brief   Gets the response of this object to the command look.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look();  

        /*!
         * \brief   Executes the take command on this player.
         * 
         * This function takes the specified item and places it in the inventory of 
         * this player.
         *
         * \param[in] aPlayer   Specifies the player that is taking the item.
         * \param[in] anItem    Specifies the item that is being taken.
         * \param[in] character This parameter is ignored in this case
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool indicating whether or not the item was taken
         *          successfully.
         */
        virtual bool take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter);

        /*!
         * \brief   Executes the equip command on this player.
         * 
         * This function equips the specified item on this player.
         *
         * \param[in] aPlayer   Specifies the player that is equipping the 
         *                      item.
         * \param[in] anItem    Specifies the item being equipped.
         * \param[in] character This parameter is ignored in this case.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool indicating whether or not the item was equipped
         *          successfully.
         */
        virtual bool equip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Executes the unequip command on this player.
         * 
         * This function unequips the specified item from this player.
         *
         * \param[in] aPlayer   Specifies the player that is unequipping the item.
         * \param[in] anItem    Specifies the item being unequipped.
         * \param[in] character This parameter is ignored in this case.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be equipped on this player.
         *
         * \return  Returns a bool indicating whether or not the item was unequipped
         *          successfully.
         */
        virtual bool unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Executes the transfer command on this player.
         * 
         * This function transfers the specified container to or from this player. If 
         * the character parameter == nullptr, then this is the giver. If the
         * destination parameter matches this player, then this is the receiver.
         *
         * \param[in] aPlayer       Specifies the player that entered the command.
         * \param[in] anItem        Specifies the item being transferred.
         * \param[in] character     Specifies the character that is transferring the item,
         *                          or nullptr if the player is the one transferring.
         * \param[in] destination   Specifies the character the item is being transferred to.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be in the inventory of the specified transferring character.
         *
         * \return  Returns a bool indicating whether or not the item was transferred
         *          successfully.
         */
        virtual bool transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination);

        /*!
         * \brief   Moves this player to the specified area.
         *
         * This function moves this player to the specified area. 
         *
         * \param[in] aPlayer   Specifes the player to move to the specified
         *                      area.
         * \param[in] anArea    Specifies the area to add the character to.  
         * \param[in] character This parameter is ignored in this case.
         *
         * \return  Returns a bool indicating whether or not moving the player
         *          to this area was successful.
         */
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);

        /*!
         * \brief   Executes the attack command from this player.
         * 
         * This function executes an attack using the specified skill or item from this player
         * against the specified character.
         *
         * \param[in] aPlayer           Specifies the player that is attacking.
         * \param[in] anItem            Optionally specifies the item being used in the attack.
         * \param[in] aSkill            Optionally specifies the skill being used in the attack.
         * \param[in] character         Specifies the character that is being attacked.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker. This
         *                              parameter should be false when this function is called on
         *                              a player.
         * 
         * \pre The item, if specified, must be in the inventory of this player.
         * \pre The skill, if specified, must be the skill of this player's player class.
         * \pre The playerAttacker parameter should be false when this function is called on a player.
         *
         * \return  Returns a bool indicating whether or not the attack was executed 
         *          successfully.
         */
        virtual bool attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker);

        /*!
         * \brief   Gets the response of this object to the command talk. 
         * 
         * This function gets the response of this player to the command talk. 
         * 
         * \param[in] aPlayer   Specifies the player that is talking with the non-combatant.
         * \param[in] aNPC      Specifies the non-combatant the player is talking to.
         *
         * \return  Returns a std::string with the response to the command
         *          talk.
         */
        virtual std::string talk(Player *aPlayer, NonCombatant *aNPC); 

        /*!
         * \brief   Executes the buy command on this player.
         *
         * \param[in] aPlayer   Specifies the player that is buying the object.
         * \param[in] anItem    Specifies the item being purchased.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not buying the item
         *          was successful.
         */
        virtual bool buy(Player *aPlayer, Item *anItem);

        /*!
         * \brief   Executes the sell command on this player.
         *
         * \param[in] aPlayer   Specifies the player that is selling the object.
         * \param[in] anItem    Specifies the item being sold.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not selling the item
         *          was successful.
         */
        virtual bool sell(Player *aPlayer, Item *anItem);
        
        /*!
         * \brief   Gets the response of this object to the command search.
         *
         * \param[in] aPlayer   Specifies the player that is searching the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          search.
         */
        virtual std::string search(Player *aPlayer); 

        /*!
         * \brief   Executes the specified skill on the specified recipient from the player.
         *
         * This function executes the specified skill from this player on the specified 
         * recipient.  
         *
         * \param[in] aPlayer       Specifies the player that is using the skill.
         * \param[in] aSkill        Specifies the skill to use.
         * \param[in] character     This parameter is ignored in this case.
         * \param[in] aRecipient    Specifies the recipient of the skill.
         * \param[in] playerSkill   Specifies whether or not the player is using the skill.
         *                          This should be true when this function is called on a
         *                          player.
         *
         * \return  Returns a std::string with the results of the skill.
         */
        virtual std::string useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill); 

        /*!
         * \brief   Moves the specified player to this area.
         *
         * This function moves the specified player to this area. After adding
         * the player to this area, it calls go() on the player, passing a 
         * pointer of this area in the anArea parameter, so the player can 
         * respond to the go command.
         *
         * \param[in] aPlayer   Specifes the player to be added to this area.
         * \param[out] anArea   Specifies the area to add the player to.    
         *
         * \return  Returns a bool indicating whether or not moving the player
         *          to this area was successful.
         */
        virtual std::string warp(Player* aPlayer, Area* anArea); 

        /*!
         * \brief   Creates a copy of this object.
         *
         * This function creates a new object with the same attributes as this
         * object and returns a pointer to the new object.
         *
         * \return  Returns a pointer to the newly created object or nullptr if 
         *          the copy was unsuccessful.
         */
        virtual InteractiveNoun* copy();

        /*!
         * \brief   Edits an attribute of this object.
         *
         * This function edits the specified attribute of this object. It asks 
         * the user for the new value and then sets it to that.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         * \param[in] attribute Specifies the attribute to edit.
         *
         * \return  Returns bool indicating whether or not editing the attribute
         *          was successful.
         */
        virtual bool editAttribute(Player *aPlayer, std::string attribute);

        /*!
         * \brief   Edits  this object.
         *
         * This function edits this object. It interacts with the user to determine
         * which attributes to update and what the new values should be and then
         * makes those changes.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         *
         * \return  Returns bool indicating whether or not editing this object
         *          was successful.
         */
        virtual bool editWizard(Player *aPlayer);

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
        int experiencePoints;
        int level;
        CharacterSize size;
        PlayerClass *playerClass;
        NonCombatant *inConversation;
        std::string username;
        bool active;
        int fileDescriptor;
        std::queue<Command*> combatQueue;
        bool editMode;
        std::vector<std::tuple<Quest*, int>> questList;
        std::multimap<std::string, InteractiiveNoun*> verbLookup;
        std::multimap<std::string, InteractiiveNoun*> nounLookup;
};

}}

#endif