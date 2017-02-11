/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        ExitDirection.hpp
 *
 * \details     Header file for ExitDirection enum. 
 ************************************************************************/

#ifndef EXIT_DIRECTION_HPP
#define EXIT_DIRECTION_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::ExitDirection
 * \brief   Enumerates the possible exit directions.
 *
 * This enum is used to specify the possible directions for an area's
 * exit.
 */
enum class ExitDirection {
    NORTH,      //!< The exit is to the north
    SOUTH,      //!< The exit is to the south
    EAST,       //!< The exit is to the east
    WEST,       //!< The exit is to the west
    NORTHEAST,  //!< The exit is to the northeast
    NORTHWEST,  //!< The exit is to the northwest
    SOUTHEAST,  //!< The exit is to the southeast
    SOUTHWEST,  //!< The exit is to the southwest 
    UP,         //!< The exit is above 
    DOWN        //!< The exit is below
};

}}

#endif