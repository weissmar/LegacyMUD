/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        XPTier.hpp
 *
 * \details     Header file for XPTier enum. 
 ************************************************************************/

#ifndef X_P_TIER_HPP
#define X_P_TIER_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::XPTier
 * \brief   Enumerates the possible experience point tiers.
 *
 * This enum is used to specify the possible experience point tiers for 
 * defeating creatures.
 */
enum class XPTier {
    TRIVIAL,    //!< The creature is trivial to defeat, a tiny amount of XP is gained
    EASY,       //!< The creature is easy to defeat, a small amount of XP is gained
    NORMAL,     //!< The creature is normal to defeat, a moderate amount of XP is gained
    HARD,       //!< The creature is hard to defeat, a substantial amount of XP is gained
    LEGENDARY   //!< The creature is really hard to defeat, a huge amount of XP is gained
};

}}

#endif