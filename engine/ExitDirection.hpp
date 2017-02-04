/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ExitDirection.hpp
 *
 * Overview:
 *     Header file for ExitDirection enum. 
 ************************************************************************/

#ifndef EXIT_DIRECTION_HPP
#define EXIT_DIRECTION_HPP

namespace legacymud { namespace engine {

enum class ExitDirection {
    NORTH, 
    SOUTH, 
    EAST, 
    WEST, 
    NORTHEAST, 
    NORTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    UP, 
    DOWN
};

}}

#endif