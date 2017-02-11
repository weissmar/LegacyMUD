/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        UniqueNoun.hpp
 *
 * \details     Header file for UniqueNoun base class. Allows for all 
 *              descendant class instances to be uniquely identified by 
 *              an int. This class should not be instantiated.
 ************************************************************************/

#ifndef UNIQUE_NOUN_HPP
#define UNIQUE_NOUN_HPP

namespace legacymud { namespace engine {

/*!
 * \details     This class allows for all descendant class instances to be  
 *              uniquely identified by an int. This class should not be 
 *              instantiated.
 */
class UniqueNoun {
    public:
        UniqueNoun();

        /*!
         * \brief   Gets the ID of the unique noun.
         *
         * \return  Returns an int with the ID.
         */
        int getID();
    protected:
        ~UniqueNoun() {}
    private:
        const int ID;
        static int nextID;
};

}}

#endif