/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        ConditionalElement.hpp
 *
 * \details     Header file for ConditionalElement base class. Defines the
 *              members and functions that apply to all conditional elements.
 *              This class should not be instantiated.
 ************************************************************************/

#ifndef CONDITIONAL_ELEMENT_HPP
#define CONDITIONAL_ELEMENT_HPP

#include <string>
#include <mutex>
#include <atomic>
#include "InteractiveNoun.hpp"

namespace legacymud { namespace engine {

class ItemType;

/*!
 * \details     This class defines the inheritable characteristics that define 
 *              all in-game conditional elements. This class should not be instantiated.
 */
class ConditionalElement: public InteractiveNoun {
    public:
        ConditionalElement();
        ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        /*ConditionalElement(const ConditionalElement &otherElement);
        ConditionalElement & operator=(const ConditionalElement &otherElement);
        virtual ~ConditionalElement();*/

        /*!
         * \brief   Gets whether or not this condtitional element is conditional.
         *
         * \return  Returns a bool indicating whether or not this element is conditional.
         */
        bool isConditional();

        /*!
         * \brief   Gets the item type that fulfills the condition of this element.
         *
         * \return  Returns an ItemType* with the condition item type.
         */
        ItemType* getConditionItem();

        /*!
         * \brief   Gets the description of this conditional element.
         *
         * \return  Returns a std::string with the description of this conditional
         *          element.
         */
        std::string getDescription() const;

        /*!
         * \brief   Gets the alternate description of this conditional element.
         *
         * \return  Returns a std::string with the alternate description of this 
         *          conditional element.
         */
        std::string getAltDescription();

        /*!
         * \brief   Sets whether or not this element is conditional.
         *
         * \param[in] isConditional     Specifies the conditional status of this
         *                              element.
         *
         * \return  Returns a bool indicating whether or not the conditional status 
         *          was set successfully.
         */
        bool setConditional(bool isConditional);

        /*!
         * \brief   Sets the condition item type for this conditional element.
         *
         * \param[in] anItemType    Specifies the condition item type.
         *
         * \return  Returns a bool indicating whether or not the condition item type 
         *          was set successfully.
         */
        bool setConditionItem(ItemType *anItemType);

        /*!
         * \brief   Sets the description of this conditional element.
         *
         * \param[in] description   Specifies the description.
         *
         * \return  Returns a bool indicating whether or not the description
         *          was set successfully.
         */
        bool setDescription(std::string description);

        /*!
         * \brief   Sets the alternate description of this conditional element.
         *
         * \param[in] altDescription    Specifies the alternate description.
         *
         * \return  Returns a bool indicating whether or not the alternate 
         *          description was set successfully.
         */
        bool setAltDescription(std::string altDescription);

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
    private:
        std::atomic<bool> conditionSet;
        ItemType *conditionItem;
        mutable std::mutex conditionItemMutex;
        std::string description;
        mutable std::mutex descriptionMutex;
        std::string altDescription;
        mutable std::mutex altDescriptionMutex;
};

}}

#endif