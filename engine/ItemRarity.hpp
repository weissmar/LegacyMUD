/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ItemRarity.hpp
 *
 * Overview:
 *     Header file for ItemRarity enum. 
 ************************************************************************/

#ifndef ITEM_RARITY_HPP
#define ITEM_RARITY_HPP

namespace legacymud { namespace engine {

enum class ItemRarity {
    COMMON,
    UNCOMMON,
    RARE,
    LEGENDARY,
    QUEST
};

}}

#endif