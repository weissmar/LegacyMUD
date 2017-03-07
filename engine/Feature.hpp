/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        Feature.hpp
 *
 * \details     Header file for Feature class. Defines the members and 
 *              functions that apply to all features. Features define 
 *              in-game features of areas.
 ************************************************************************/

#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <string>
#include <mutex>
#include <vector>
#include "parser.hpp"
#include "ConditionalElement.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"
#include "GameObjectManager.hpp"

namespace legacymud { namespace engine {

class Area;

/*!
 * \details     Features define in-game features of areas.
 */
class Feature: public ConditionalElement {
    public:
        Feature();
        Feature(std::string name, std::string placement, Area *location, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        Feature(std::string name, std::string placement, Area *location, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID);

        /*!
         * \brief   Gets the name of this feature.
         *
         * \return  Returns a std::string with the name of this feature.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the placement of this feature.
         *
         * \return  Returns a std::string with the placement of this feature.
         */
        std::string getPlacement() const;

        /*!
         * \brief   Gets the location of this feature.
         *
         * \return  Returns an Area* with the location of this feature.
         */
        Area* getLocation() const;

        /*!
         * \brief   Sets the name of this feature.
         * 
         * \param[in] name  Specifies the name of this feature.
         *
         * \return  Returns a bool indicating whether or not setting the name
         *          was successful.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the placement of this feature.
         * 
         * \param[in] placement Specifies the placement of this feature.
         *
         * \return  Returns a bool indicating whether or not setting the placement
         *          was successful.
         */
        bool setPlacement(std::string placement);

        /*!
         * \brief   Sets the location of this feature.
         * 
         * \param[in] anArea    Specifies the location of this feature.
         *
         * \return  Returns a bool indicating whether or not setting the location 
         *          was successful.
         */
        bool setLocation(Area *anArea);

        /*!
         * \brief   Adds the specified noun alias to this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to add.
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was added successfully.
         */
        virtual bool addNounAlias(std::string);

        /*!
         * \brief   Removes the specified noun alias from this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to remove
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was found and removed successfully.
         */
        virtual bool removeNounAlias(std::string);

        /*!
         * \brief   Adds an alias of the specified command for this interactive noun.
         *
         * This function adds an alias-grammar pair to the map of aliases
         * for the Action associated with the specified command for this interactive 
         * noun.
         * 
         * \param[in] aCommand      Specifies the command the alias is aliasing.
         * \param[in] alias         Specifies the verb alias to be added.
         * \param[in] direct        Specifies support for direct objects.
         * \param[in] indirect      Specifies support for indirect objects.
         * \param[in] prepositions  Specifies supported prepositions.
         *
         * \return  Returns a bool indicating whether or not adding the
         *          alias to the interactive noun succeeded.
         */
        virtual bool addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions);

        /*!
         * \brief   Removes the verb alias for the specified command from this 
         * interactive noun.
         *
         * This function removes the alias-grammar pair indicated by the
         * specified alias string from the Action associated with the specified 
         * command for this interactive noun.
         * 
         * \param[in] aCommand  Specifies the command the alias is aliasing.
         * \param[in] alias     Specifies the verb alias to remove.
         *
         * \return  Returns a bool indicating whether or not removing the
         *          specified alias succeeded.
         */
        virtual bool removeVerbAlias(CommandEnum aCommand, std::string alias);

        /*!
         * \brief   Gets the object type.
         *
         * \return  Returns an ObjectType indicating the actual class the object
         *          belongs to.
         */
        virtual ObjectType getObjectType() const;

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();

        /*!
         * \brief   Deserializes and creates an object of this type from the
         *          specified string of serialized data.
         * 
         * \param[in] string    Holds the data to be deserialized.
         * \param[in] GameObjectManager    Provides access to other game objects in memory.
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static Feature* deserialize(std::string, GameObjectManager* gom);

        /*!
         * \brief   Gets the response of this object to the command look.
         *
         * \param[in] aPlayer   Specifies the player that entered the command
         * 
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(Player *aPlayer, std::vector<EffectType> *effects);  

        /*!
         * \brief   Gets the response of this object to the command listen.
         *
         * \return  Returns a std::string with the response to the command
         *          listen.
         */
        virtual std::string listen(std::vector<EffectType> *effects); 

        /*!
         * \brief   Creates a copy of this object.
         *
         * This function creates a new object with the same attributes as this
         * object and returns a pointer to the new object.
         *
         * \return  Returns a pointer to the newly created object or nullptr if 
         *          the copy was unsuccessful.
         */
        virtual InteractiveNoun* copy();

        /*!
         * \brief   Edits an attribute of this object.
         *
         * This function edits the specified attribute of this object. It asks 
         * the user for the new value and then sets it to that.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         * \param[in] attribute Specifies the attribute to edit.
         *
         * \return  Returns bool indicating whether or not editing the attribute
         *          was successful.
         */
        virtual bool editAttribute(Player *aPlayer, std::string attribute);

        /*!
         * \brief   Edits  this object.
         *
         * This function edits this object. It interacts with the user to determine
         * which attributes to update and what the new values should be and then
         * makes those changes.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         *
         * \return  Returns bool indicating whether or not editing this object
         *          was successful.
         */
        virtual bool editWizard(Player *aPlayer);

        /*!
         * \brief   Gets the attribute signature of the class.
         *
         * This function returns a map of string to DataType with the 
         * attributes required by the Action class to instantiate a new
         * action.
         * 
         * \return  Returns a map of std::string to DataType indicating 
         *          the required attributes.
         */
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        mutable std::mutex nameMutex;
        std::string placement;
        mutable std::mutex placementMutex;
        Area *location;
        mutable std::mutex locationMutex;
};

}}

#endif