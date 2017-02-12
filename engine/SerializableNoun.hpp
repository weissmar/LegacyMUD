/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        SerializableNoun.hpp
 *
 * \details     Header file for SerializableNoun abstract base class. Requires
 *              all descendant classes to implement functions for serializing 
 *              and deserializing.
 ************************************************************************/

#ifndef SERIALIZABLE_NOUN_HPP
#define SERIALIZABLE_NOUN_HPP

#include <string>

namespace legacymud { namespace engine {

/*!
 * \details     Requires all descendant classes to implement functions for 
 *              serializing and deserializing.
 */
class SerializableNoun {
    public:
        virtual ~SerializableNoun() {};

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize() = 0;

        /*!
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string) = 0;
};

}}

#endif