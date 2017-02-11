/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        EquipmentSlot.hpp
 *
 * \details     Header file for EquipmentSlot enum. 
 ************************************************************************/

#ifndef EQUIPMENT_SLOT_HPP
#define EQUIPMENT_SLOT_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::EquipmentSlot
 * \brief   Enumerates the possible equipment slots on a combatant.
 *
 * This enum is used to specify the possible equipment slots on a combatant,
 * as well as the slots that an item can be equipped to.
 */
enum class EquipmentSlot {
    NONE,           //!< None - this item cannot be equipped
    HEAD,           //!< Worn on the head
    SHOULDERS,      //!< Worn on the shoulders
    NECK,           //!< Worn around the neck
    TORSO,          //!< Worn on the torso
    BELT,           //!< Worn around the waist 
    LEGS,           //!< Worn on the legs
    ARMS,           //!< Worn on the arms
    HANDS,          //!< Worn on the hands
    RIGHT_HAND,     //!< Carried in the right hand
    LEFT_HAND,      //!< Carried in the left hand
    FEET,           //!< Worn on the feet
    RIGHT_RING,     //!< Worn on the right ring finger
    LEFT_RING       //!< Worn on the left ring finger
};

}}

#endif