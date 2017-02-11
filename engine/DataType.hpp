/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/03/2017
 * \course      CS467, Winter 2017
 * \file        DataType.hpp
 *
 * \details     Header file for DataType enum. 
 ************************************************************************/

#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::DataType
 * \brief   Enumerates the possible data types.
 *
 * This enum is used to specify the data types of class attributes so the
 * game logic can know what types of data to collect from the user.
 */
enum class DataType {
    INT_TYPE,               //!< The attribute is an int
    BOOL_TYPE,              //!< The attribute is a bool
    FLOAT_TYPE,             //!< The attribute is a float
    STRING_TYPE,            //!< The attribute is a std::string
    COMMAND_ENUM,           //!< The attribute is a CommandEnum
    EFFECT_TYPE,            //!< The attribute is an EffectType
    AREA_SIZE,              //!< The attribute is a AreaSize
    DAMAGE_TYPE,            //!< The attribute is a DamageType
    ITEM_RARITY,            //!< The attribute is an ItemRarity
    EQUIPMENT_SLOT,         //!< The attribute is an EquipmentSlot
    ITEM_POSITION,          //!< The attribute is an ItemPosition
    CHARACTER_SIZE,         //!< The attribute is a CharacterSize
    EXIT_DIRECTION,         //!< The attribute is an ExitDirection
    AREA_PTR,               //!< The attribute is an Area*
    SPECIAL_SKILL_PTR,      //!< The attribute is a SpecialSkill*
    ITEM_TYPE_PTR,          //!< The attribute is an ItemType*
    INTERACTIVE_NOUN_PTR,   //!< The attribute is an InteractiveNoun*
    CREATURE_TYPE_PTR,      //!< The attribute is a CreatureType*
    QUEST_PTR,              //!< The attribute is a Quest*
    PLAYER_CLASS_PTR,       //!< The attribute is a PlayerClass*
    ITEM_PTR,               //!< The attribute is an Item*
    NON_COMBATANT_PTR       //!< The attribute is a NonCombatant*
};

}}

#endif