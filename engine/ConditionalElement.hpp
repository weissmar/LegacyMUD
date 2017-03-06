/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/01/2017
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
        ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID);
        /*ConditionalElement(const ConditionalElement &otherElement);
        ConditionalElement & operator=(const ConditionalElement &otherElement);
        virtual ~ConditionalElement();*/

        /*!
         * \brief   Gets whether or not this condtitional element is conditional.
         *
         * \return  Returns a bool indicating whether or not this element is conditional.
         */
        bool isConditional() const;

        /*!
         * \brief   Gets the item type that fulfills the condition of this element.
         *
         * \return  Returns an ItemType* with the condition item type.
         */
        ItemType* getConditionItem() const;

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
        std::string getAltDescription() const;

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
         * \brief   Gets the response of this object to the command move.
         *
         * \param[in] aPlayer   Specifies the player that is moving the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          move.
         */
        virtual std::string move(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command read.
         *
         * \param[in] aPlayer   Specifies the player that is reading the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          read.
         */
        virtual std::string read(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command break.
         *
         * \param[in] aPlayer   Specifies the player that is breaking the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          break.
         */
        virtual std::string breakIt(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command climb.
         *
         * \param[in] aPlayer   Specifies the player that is climbing the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          climb.
         */
        virtual std::string climb(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command turn.
         *
         * \param[in] aPlayer   Specifies the player that is turning the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          turn.
         */
        virtual std::string turn(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command push.
         *
         * \param[in] aPlayer   Specifies the player that is pushing the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          push.
         */
        virtual std::string push(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command pull.
         *
         * \param[in] aPlayer   Specifies the player that is pulling the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          pull.
         */
        virtual std::string pull(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command eat.
         *
         * \param[in] aPlayer   Specifies the player that is eating the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          eat.
         */
        virtual std::string eat(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command drink.
         *
         * \param[in] aPlayer   Specifies the player that is drinking the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          drink.
         */
        virtual std::string drink(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();
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