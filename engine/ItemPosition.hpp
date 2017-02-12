/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        ItemPosition.hpp
 *
 * \details     Header file for ItemPosition enum. 
 ************************************************************************/

#ifndef ITEM_POSITION_HPP
#define ITEM_POSITION_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::ItemPosition
 * \brief   Enumerates the possible item positions.
 *
 * This enum is used to specify the possible item positions.
 */
enum class ItemPosition {
    NONE,       //!< No position specified
    GROUND,     //!< Item is on the ground (in an area)
    INVENTORY,  //!< Item is in a character's inventory
    EQUIPPED,   //!< Item is equipped by a character
    IN,         //!< Item is in a container
    ON,         //!< Item is on a container
    UNDER       //!< Item is under a container
};

}}

#endif