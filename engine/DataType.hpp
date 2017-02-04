/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      DataType.hpp
 *
 * Overview:
 *     Header file for DataType enum. 
 ************************************************************************/

#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

namespace legacymud { namespace engine {

enum class DataType {
    INT_TYPE,
    BOOL_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    COMMAND_ENUM,
    EFFECT_TYPE,
    AREA_SIZE,
    DAMAGE_TYPE,
    ITEM_RARITY,
    EQUIPMENT_SLOT,
    ITEM_POSITION,
    CHARACTER_SIZE,
    EXIT_DIRECTION,
    AREA_PTR,
    SPECIAL_SKILL_PTR,
    ITEM_TYPE_PTR,
    INTERACTIVE_NOUN_PTR,
    CREATURE_TYPE_PTR,
    QUEST_PTR,
    PLAYER_CLASS_PTR,
    ITEM_PTR,
    NON_COMBATANT_PTR
};

}}

#endif