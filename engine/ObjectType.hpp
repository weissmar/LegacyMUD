/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/10/2017
 * \course      CS467,          //!< Winter 2017
 * \file        ObjectType.hpp
 *
 * \details     Header file for ObjectType enum. 
 ************************************************************************/

#ifndef OBJECT_TYPE_HPP
#define OBJECT_TYPE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::ObjectType
 * \brief   Enumerates the possible object types.
 *
 * This enum is used to specify the possible types of concrete classes
 * that inherit from InteractiveNoun.
 */
enum class ObjectType {
    AREA,           //!< Area class
    ARMOR_TYPE,     //!< ArmorType class
    CONTAINER,      //!< Container class
    CREATURE,       //!< Creature class
    CREATURE_TYPE,  //!< CreatureType class
    EXIT,           //!< Exit class
    FEATURE,        //!< Feature class
    ITEM,           //!< Item class
    ITEM_TYPE,      //!< ItemType class
    NON_COMBATANT,  //!< NonCombatant class
    PLAYER,         //!< Player class
    PLAYER_CLASS,   //!< PlayerClass class
    QUEST,          //!< Quest class
    QUEST_STEP,     //!< QuestStep class
    SPECIAL_SKILL,  //!< SpecialSkill class
    WEAPON_TYPE     //!< WeaponType class
};

}}

#endif