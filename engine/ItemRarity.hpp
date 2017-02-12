/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        ItemRarity.hpp
 *
 * \details     Header file for ItemRarity enum. 
 ************************************************************************/

#ifndef ITEM_RARITY_HPP
#define ITEM_RARITY_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::ItemRarity
 * \brief   Enumerates the possible levels of item rarity.
 *
 * This enum is used to specify the rarity of an item.
 */
enum class ItemRarity {
    COMMON,     //!< Item is common
    UNCOMMON,   //!< Item is uncommon
    RARE,       //!< Item is rare
    LEGENDARY,  //!< Item is legendary
    QUEST       //!< Item is a quest item
};

}}

#endif