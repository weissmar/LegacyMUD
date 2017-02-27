/*!
  \file     InteractiveNounStub.hpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details This file contains a stub class for testing InteractiveNoun functions.
*/

#ifndef INTERACTIVENOUNSTUB_HPP
#define INTERACTIVENOUNSTUB_HPP

#include <InteractiveNoun.hpp>

namespace legacymud { namespace test {

/*!
  \brief This class contains the minimum implementation required for testing InteractiveNoun.
*/
class InteractiveNounStub : public engine::InteractiveNoun {
    public:
        /*!
         * \brief   Gets the name.
         */
        virtual std::string getName() const;
        
        /*!
         * \brief   Gets the object type.
         */
        virtual engine::ObjectType getObjectType() const;

        /*!
         * \brief   Makes a copy of the object.
         */
        virtual engine::InteractiveNoun* copy();

        /*!
         * \brief   Edits an attribute of the object. 
         */
        virtual bool editAttribute(engine::Player*, std::string);

        /*!
         * \brief   Starts the edit wizard for the object.
         */
        virtual bool editWizard(engine::Player*);

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();

        /*!
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string);
};

}}

#endif