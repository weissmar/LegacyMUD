/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      DamageType.hpp
 *
 * Overview:
 *     Header file for DamageType enum. 
 ************************************************************************/

#ifndef DAMAGE_TYPE_HPP
#define DAMAGE_TYPE_HPP

namespace legacymud { namespace engine {

enum class DamageType {
    CRUSHING,
    PIERCING,
    ELECTRIC,
    FIRE,
    WATER,
    WIND,
    EARTH,
    HEAL
};

}}

#endif