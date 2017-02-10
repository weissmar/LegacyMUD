/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/08/2017
 * Last Modified: 02/08/2017
 * Course:        CS467, Winter 2017
 * Filename:      XPTier.hpp
 *
 * Overview:
 *     Header file for XPTier enum. 
 ************************************************************************/

#ifndef X_P_TIER_HPP
#define X_P_TIER_HPP

namespace legacymud { namespace engine {

enum class XPTier {
    TRIVIAL,
    EASY,
    NORMAL,
    HARD,
    LEGENDARY
};

}}

#endif