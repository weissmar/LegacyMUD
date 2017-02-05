/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      EffectType.hpp
 *
 * Overview:
 *     Header file for EffectType enum. 
 ************************************************************************/

#ifndef EFFECT_TYPE_HPP
#define EFFECT_TYPE_HPP

namespace legacymud { namespace engine {

enum class EffectType {
    NONE,
    DAMAGE,
    FALL,
    LONG_FALL,
    LOST_ITEM,
    DROP_ALL_ITEMS,
    GAIN_MONEY,
    LOSE_MONEY,
    HEAL,
    GET_SPECIAL_POINTS
};

}}

#endif