/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        CommandEnum.hpp
 *
 * \details     Header file for CommandEnum enum. 
 ************************************************************************/

#ifndef COMMAND_ENUM_HPP
#define COMMAND_ENUM_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::CommandEnum
 * \brief   Enumerates the possible commands.
 *
 * This enum is used to specify the command entered by the user, as well
 * as to define responses to those commands.
 */
enum class CommandEnum {
    INVALID,        //!< This is used to indicate no command or an invalid command.
    HELP,           //!< Displays a list of some available commands.
    LOOK,           //!< Displays the full description of the current area or specified direct object.
    LISTEN,         //!< Displays an optional description of any sounds in the current area.
    TAKE,           //!< Puts the specified item into inventory.
    PUT,            //!< Puts the specified item in, on, or under the specified container.
    DROP,           //!< Drops the specified item onto the ground.
    INVENTORY,      //!< Displays the player's inventory and equipped items.
    MORE,           //!< Displays details of the specified item or skill.
    EQUIPMENT,      //!< Display the player's equipped items.
    EQUIP,          //!< Equips the specified item.
    UNEQUIP,        //!< Unequips the specified item.
    TRANSFER,       //!< Gives the specified item to the specified character.
    SPEAK,          //!< Says the specified text to all players in the current area.
    SHOUT,          //!< Shouts the specified text to all players within n links of the current area.
    WHISPER,        //!< Whispers the specified text to the specified player.
    QUIT,           //!< Logs the player out of the game.
    GO,             //!< Moves the player to the specified area.
    MOVE,           //!< Moves the specified item.
    STATS,          //!< Displays the player's stats.
    QUESTS,         //!< Displays the player's quests.
    SKILLS,         //!< Displays the player's skills.
    ATTACK,         //!< Initiates or continues combat with the specified combatant, using either the default attack or the specified attack skill.
    TALK,           //!< Initiates a conversation with the specified non-combatant.
    SHOP,           //!< Lists any items the non-combatant the player is talking to has for sale.
    BUY,            //!< Purchases the specified item from the non-combatant the player is talking to.
    SELL,           //!< Sells the specified item to the non-combatant the player is speaking to.
    SEARCH,         //!< Lists any items in the specified container.
    USE_SKILL,      //!< Activates the specified skill.
    READ,           //!< Reads the specified item.
    BREAK,          //!< Breaks the specified item.
    CLIMB,          //!< Climbs the specified item.
    TURN,           //!< Turns the specified item.
    PUSH,           //!< Pushes the specified item.
    PULL,           //!< Pulls the specified item.
    EAT,            //!< Eats the specified item.
    DRINK,          //!< Drinks the specified item.
    EDIT_MODE,      //!< Toggles between edit mode and normal mode.
    WARP,           //!< World builder command. Moves the player to the specified area.
    COPY,           //!< World builder command. Creates a copy of the specified object and places it in the current area.
    CREATE,         //!< World builder command. Starts the creation wizard for the specified object type.
    EDIT_ATTRIBUTE, //!< World builder command. Edits the specified attribute of the specified object (or the current area if not specified).
    EDIT_WIZARD,    //!< World builder command. Starts the edit wizard for the specified object.
    SAVE,           //!< World builder command. Saves the game to the specified file (or the default file if not specified).
    LOAD,           //!< World builder command. Loads the game from the specified file (or the default file if not specified).
    DELETE          //!< World builder command. Deletes the specified object from the game.
};

}}

#endif