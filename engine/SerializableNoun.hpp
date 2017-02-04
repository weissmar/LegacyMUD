/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      SerializableNoun.hpp
 *
 * Overview:
 *     Header file for SerializableNoun abstract base class. Requires all
 * descendant classes to implement functions for serializing and 
 * deserializing.
 ************************************************************************/

#ifndef SERIALIZABLE_NOUN_HPP
#define SERIALIZABLE_NOUN_HPP

#include <string>

namespace legacymud { namespace engine {

class SerializableNoun {
    public:
        virtual ~SerializableNoun() {};
        virtual std::string serialize() = 0;
        virtual bool deserialize(std::string) = 0;
};

}}

#endif