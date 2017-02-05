/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      EquipmentSlot.hpp
 *
 * Overview:
 *     Header file for EquipmentSlot enum. 
 ************************************************************************/

#ifndef EQUIPMENT_SLOT_HPP
#define EQUIPMENT_SLOT_HPP

namespace legacymud { namespace engine {

enum class EquipmentSlot {
    NONE,
    HEAD,
    SHOULDERS,
    NECK,
    TORSO,
    BELT,
    LEGS,
    ARMS,
    HANDS,
    RIGHT_HAND,
    LEFT_HAND,
    FEET,
    RIGHT_RING,
    LEFT_RING
};

}}

#endif