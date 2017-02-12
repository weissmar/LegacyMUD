/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/03/2017
 * \modified    02/12/2017
 * \course      CS467, Winter 2017
 * \file        AreaSize.hpp
 *
 * \details     Header file for AreaSize enum. 
 ************************************************************************/

#ifndef AREA_SIZE_HPP
#define AREA_SIZE_HPP

namespace legacymud { namespace engine {

/*!
 * \enum    legacymud::engine::AreaSize
 * \brief   Enumerates the possible sizes of an area.
 *
 * This enum is used to specify the size of an Area as well as the range of
 * weapons.
 */
enum class AreaSize {
    NONE,       //!< Does not have an area size.
    SMALL,      //!< Area is small. Only small range weapons can be used.
    MEDIUM,     //!< Area is medium. Only small and medium range weapons can be used.
    LARGE       //!< Area is large. All weapons can be used.
};

}}

#endif