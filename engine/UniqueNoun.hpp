/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      UniqueNoun.hpp
 *
 * Overview:
 *     Header file for UniqueNoun base class. Allows for all descendant
 * class instances to be uniquely identified by an int.
 ************************************************************************/

#ifndef UNIQUE_NOUN_HPP
#define UNIQUE_NOUN_HPP

namespace legacymud { namespace engine {

class UniqueNoun {
    public:
        UniqueNoun();
        ~UniqueNoun();
        int getID();
    private:
        int ID;
        static int nextID;
};

}}

#endif