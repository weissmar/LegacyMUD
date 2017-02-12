/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        EffectType.hpp
 *
 * \details     Header file for EffectType enum. 
 ************************************************************************/

#ifndef EFFECT_TYPE_HPP
#define EFFECT_TYPE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::EffectType
 * \brief   Enumerates the possible types of effect.
 *
 * This enum is used to specify the possible effect types of various
 * actions, items, etc.
 */
enum class EffectType {
    NONE,               //!< No effect
    DAMAGE,             //!< Causes generic damage
    FALL,               //!< Causes the character to fall, inflicts crushing damage
    LONG_FALL,          //!< Causes the character to fall a long way, inflicts severe crushing damage
    LOST_ITEM,          //!< Causes the character to lose a random item from inventory
    DROP_ALL_ITEMS,     //!< Causes the character to drop all items in inventory
    GAIN_MONEY,         //!< Causes the character to gain money
    LOSE_MONEY,         //!< Causes the character to lose money
    HEAL,               //!< Causes the character to be healed
    GET_SPECIAL_POINTS  //!< Causes the character to regain special points
};

}}

#endif