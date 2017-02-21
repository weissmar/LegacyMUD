/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Area.hpp
 *
 * \details     Header file for Area class. Defines the members and 
 *              functions that apply to all areas. Areas define in-game
 *              areas (rooms, fields, etc) and keep track of the items,
 *              characters, features, and exits in them, as well as the 
 *              nouns and verbs currently usable in the area.
 ************************************************************************/

#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <map>
#include <atomic>
#include <mutex>
#include <LexicalData.hpp>
#include "InteractiveNoun.hpp"
#include "AreaSize.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Character;
class Exit;
class Item;
class Feature;

/*!
 * \details     This class defines in-game areas (rooms, fields, etc) and keeps
 *              track of the items, characters, features, and exits in them, as 
 *              well as the nouns and verbs currently usable in the area.
 */
class Area: public InteractiveNoun {
    public:
        Area();
        Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size);
        /*Area(const Area &otherArea);
        Area & operator=(const Area &otherArea);
        virtual ~Area();*/

        /*!
         * \brief   Gets the name of this area.
         *
         * \return  Returns a std::string with the name of this area.
         */
        std::string getName() const; 

        /*!
         * \brief   Gets the short description of this area.
         *
         * \return  Returns a std::string with the short description
         *          of this area.
         */
        std::string getShortDesc() const;

        /*!
         * \brief   Gets the long description of this area.
         *
         * \return  Returns a std::string with the long description of 
         *          this area.
         */
        std::string getLongDesc() const;

        /*!
         * \brief   Gets the size of this area.
         *
         * \return  Returns an AreaSize indicating the size of this
         *          area.
         */
        AreaSize getSize() const;

        /*!
         * \brief   Gets a list of the items in this area.
         *
         * \return  Returns a std::vector<Item*> with the list of items
         *          in this area.
         */
        std::vector<Item*> getItems() const;

        /*!
         * \brief   Gets a list of the characters in this area.
         *
         * \return  Returns a std::vector<Item*> with the list of characters
         *          in this area.
         */
        std::vector<Character*> getCharacters() const;

        /*!
         * \brief   Gets a list of the features in this area.
         *
         * \return  Returns a std::vector<Item*> with the list of features
         *          in this area.
         */
        std::vector<Feature*> getFeatures() const;

        /*!
         * \brief   Gets a list of the exits in this area.
         *
         * \return  Returns a std::vector<Item*> with the list of exits
         *          in this area.
         */
        std::vector<Exit*> getExits() const;

        /*!
         * \brief   Gets the lexical data for this Area's contents.
         * 
         * \return  Returns a parser::LexicalData by reference with the 
         *          contents' lexical data.
         */
        parser::LexicalData getLexicalData() const;

        /*!
         * \brief   Gets the full description of an area for first time
         *          entry.
         *          
         * \param[in] excludeID     Optionally specifies the ID of a
         *                          player to exclude from the description.
         * 
         * \return  Returns a std::string with the full description.
         */
        std::string getFullDescription(int excludeID) const;

        /*!
         * \brief   Sets the name of this area.
         *
         * \param[in] name  Specifies the name of this area.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          name of this area succeeded.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the short description of this area.
         *
         * \param[in] shortDescription  Specifies the short description of 
         *                              this area.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          short description of this area succeeded.
         */
        bool setShortDesc(std::string shortDescription);

        /*!
         * \brief   Sets the long description of this area.
         *
         * \param[in] longDescription   Specifies the long description of 
         *                              this area.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          long description of this area succeeded.
         */
        bool setLongDesc(std::string longDescription);

        /*!
         * \brief   Sets the size of this area.
         *
         * \param[in] size  Specifies the size of this area.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          size of this area succeeded.
         */
        bool setSize(AreaSize size);

        /*!
         * \brief   Adds an item to this area.
         *
         * \param[in] anItem    Specifies the item to add to this area.
         *
         * \return  Returns a bool indicating whether or not adding the 
         *          item to this area succeeded.
         */
        bool addItem(Item *anItem);

        /*!
         * \brief   Adds a character to this area.
         *
         * \param[in] aCharacter    Specifies the character to add to this area.
         *
         * \return  Returns a bool indicating whether or not adding the 
         *          character to this area succeeded.
         */
        bool addCharacter(Character *aCharacter);

        /*!
         * \brief   Adds a feature to this area.
         *
         * \param[in] aFeature    Specifies the feature to add to this area.
         *
         * \return  Returns a bool indicating whether or not adding the 
         *          feature to this area succeeded.
         */
        bool addFeature(Feature *aFeature);

        /*!
         * \brief   Adds an exit to this area.
         *
         * \param[in] anExit    Specifies the exit to add to this area.
         *
         * \return  Returns a bool indicating whether or not adding the 
         *          exit to this area succeeded.
         */
        bool addExit(Exit *anExit);

        /*!
         * \brief   Removes an item from this area.
         *
         * \param[in] anItem    Specifies the item to remove from this area.
         *
         * \return  Returns a bool indicating whether or not removing the 
         *          item from this area succeeded.
         */
        bool removeItem(Item *anItem);

        /*!
         * \brief   Removes a character from this area.
         *
         * \param[in] aCharacter    Specifies the character to remove from 
         *                          this area.
         *
         * \return  Returns a bool indicating whether or not removing the 
         *          character from this area succeeded.
         */
        bool removeCharacter(Character *aCharacter);

        /*!
         * \brief   Removes a feature from this area.
         *
         * \param[in] aFeature  Specifies the feature to remove from 
         *                      this area.
         *
         * \return  Returns a bool indicating whether or not removing the 
         *          feature from this area succeeded.
         */
        bool removeFeature(Feature *aFeature);

        /*!
         * \brief   Removes an exit from this area.
         *
         * \param[in] anExit    Specifies the exit to remove from this area.
         *
         * \return  Returns a bool indicating whether or not removing the 
         *          exit from this area succeeded.
         */
        bool removeExit(Exit *anExit);

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
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string);

        /*!
         * \brief   Gets the response of this object to the command look.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look();  

        /*!
         * \brief   Gets the response of this object to the command listen.
         *
         * \return  Returns a std::string with the response to the command
         *          listen.
         */
        virtual std::string listen(); 

        /*!
         * \brief   Moves the specified player or character to this area.
         *
         * This function moves the specified player or character to this area. 
         * After adding the player/character to this area, it calls go() on  
         * the player/character, passing a pointer of this area in the anArea 
         * parameter, so the player/character can respond to the go command.
         *
         * \param[in] aPlayer   Specifes the player that entered the command and,
         *                      if character == nullptr, the player to be added 
         *                      to this area.
         * \param[out] anArea   Specifies the area to add the player/character to.  
         * \param[in] character Optionally specifies the character to move to this
         *                      area, or nullptr if the player is the one moving.
         *
         * \return  Returns a bool indicating whether or not moving the player
         *          or character to this area was successful.
         */
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);

        /*!
         * \brief   Gets the response of this object to the command search.
         *
         * \param[in] aPlayer   Specifies the player that is searching the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          search.
         */
        virtual std::string search(Player *aPlayer); 

        /*!
         * \brief   Moves the specified player to this area.
         *
         * This function moves the specified player to this area. After adding
         * the player to this area, it calls go() on the player, passing a 
         * pointer of this area in the anArea parameter, so the player can 
         * respond to the go command.
         *
         * \param[in] aPlayer   Specifes the player to be added to this area.
         * \param[out] anArea   Specifies the area to add the player to.    
         *
         * \return  Returns a bool indicating whether or not moving the player
         *          to this area was successful.
         */
        virtual std::string warp(Player* aPlayer, Area* anArea); 

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
        virtual bool editAttribute(Player*, std::string attribute);

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
        virtual bool editWizard(Player*);

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
        std::string shortDescription;
        mutable std::mutex shortDescMutex;
        std::string longDescription;
        mutable std::mutex longDescMutex;
        std::atomic<AreaSize> size;
        std::vector<Item*> itemContents;
        mutable std::mutex itemContentMutex;
        std::vector<Character*> characterContents;
        mutable std::mutex charContentMutex;
        std::vector<Feature*> featureContents;
        mutable std::mutex featContentMutex;
        std::vector<Exit*> exitContents;
        mutable std::mutex exitContentMutex;
        parser::LexicalData contentsLexicalData;
        mutable std::mutex lexicalMutex;
};

}}

#endif