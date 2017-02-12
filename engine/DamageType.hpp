/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        DamageType.hpp
 *
 * \details     Header file for DamageType enum. 
 ************************************************************************/

#ifndef DAMAGE_TYPE_HPP
#define DAMAGE_TYPE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::DamageType
 * \brief   Enumerates the possible types of damage.
 *
 * This enum is used to specify types of damage that may be inflicted by 
 * different skills, weapons, and effects. Additionally, this enum is used
 * to specify the types of damage that armor or characters or weak or 
 * resistant to.
 */
enum class DamageType {
    NONE,       //!< No damage type
    CRUSHING,   //!< Crushing damage
    PIERCING,   //!< Piercing damage
    ELECTRIC,   //!< Electric damage
    FIRE,       //!< Fire elemental damage
    WATER,      //!< Water elemental damage
    WIND,       //!< Wind elemental damage
    EARTH,      //!< Earth elemental damage
    HEAL        //!< Instead of damage, a healing effect
};

}}

#endif