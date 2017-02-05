/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      CommandEnum.hpp
 *
 * Overview:
 *     Header file for CommandEnum enum. 
 ************************************************************************/

#ifndef COMMAND_ENUM_HPP
#define COMMAND_ENUM_HPP

namespace legacymud { namespace engine {

enum class CommandEnum {
    INVALID,
    HELP,
    LOOK,
    LISTEN,
    TAKE,
    PUT,
    DROP,
    INVENTORY,
    MORE,
    EQUIPMENT,
    EQUIP,
    UNEQUIP,
    TRANSFER,
    SPEAK,
    SHOUT,
    WHISPER,
    QUIT,
    GO,
    MOVE,
    STATS,
    QUESTS,
    SKILLS,
    ATTACK,
    TALK,
    SHOP,
    BUY,
    SELL,
    SEARCH,
    USE_SKILL,
    READ,
    BREAK,
    CLIMB,
    TURN,
    PUSH,
    PULL,
    EAT,
    DRINK,
    EDIT_MODE,
    WARP,
    COPY,
    CREATE,
    ADD,
    EDIT_ATTRIBUTE,
    EDIT_WIZARD,
    SAVE,
    LOAD,
    DELETE
};

}}

#endif