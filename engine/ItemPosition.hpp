/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/03/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ItemPosition.hpp
 *
 * Overview:
 *     Header file for ItemPosition enum. 
 ************************************************************************/

#ifndef ITEM_POSITION_HPP
#define ITEM_POSITION_HPP

namespace legacymud { namespace engine {

enum class ItemPosition {
    NONE,
    GROUND,
    INVENTORY,
    EQUIPPED,
    IN,
    ON,
    UNDER
};

}}

#endif