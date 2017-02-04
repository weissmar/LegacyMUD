/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ObjectType.hpp
 *
 * Overview:
 *     Header file for ObjectType enum. 
 ************************************************************************/

#ifndef OBJECT_TYPE_HPP
#define OBJECT_TYPE_HPP

namespace legacymud { namespace engine {

enum class ObjectType {
    ACTION,
    AREA,
    ARMOR_TYPE,
    CONTAINER,
    CREATURE,
    CREATURE_TYPE,
    EXIT,
    FEATURE,
    ITEM,
    ITEM_TYPE,
    NON_COMBATANT,
    PLAYER,
    PLAYER_CLASS,
    QUEST,
    QUEST_STEP,
    SPECIAL_SKILL,
    WEAPON_TYPE
};

}}

#endif