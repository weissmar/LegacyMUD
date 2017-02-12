/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        CharacterSize.hpp
 *
 * \details     Header file for CharacterSize enum. 
 ************************************************************************/

#ifndef CHARACTER_SIZE_HPP
#define CHARACTER_SIZE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::CharacterSize
 * \brief   Enumerates the possible sizes of a character.
 *
 * This enum is used to specify the size of a creature type as well as the
 * size of a player. This value is used in combat to calculate size advantages
 * and disadvantages.
 */
enum class CharacterSize {
    TINY,   //!< Character is tiny.
    SMALL,  //!< Character is small.
    MEDIUM, //!< Character is medium.
    LARGE,  //!< Character is large.
    HUGE    //!< Character is huge.
};

}}

#endif