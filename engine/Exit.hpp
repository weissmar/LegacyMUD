/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/15/2017
 * \course      CS467, Winter 2017
 * \file        Exit.hpp
 *
 * \details     Header file for Exit class. Defines the members and 
 *              functions that apply to all exits. Exits define in-game
 *              exits in areas.
 ************************************************************************/

#ifndef EXIT_HPP
#define EXIT_HPP

#include <string>
#include <mutex>
#include <atomic>
#include <vector>
#include "parser.hpp"
#include "ConditionalElement.hpp"
#include "EffectType.hpp"
#include "ExitDirection.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "GameObjectManager.hpp"

namespace legacymud { namespace engine {

class Area;

/*!
 * \details     Exits define in-game exits in areas.
 */
class Exit: public ConditionalElement {
    public:
        Exit();
        Exit(ExitDirection direction, Area *location, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        Exit(ExitDirection direction, Area *location, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID);
        /*Exit(const Exit &otherExit);
        Exit & operator=(const Exit &otherExit);
        virtual ~Exit();*/

        /*!
         * \brief   Gets the direction of this exit.
         *
         * \return  Returns an ExitDirection with the direction of this exit.
         */
        ExitDirection getDirection() const;

        /*!
         * \brief   Gets the direction of this exit as a string.
         *
         * \return  Returns a std::string with the direction of this exit.
         */
        std::string getDirectionString() const;

        /*!
         * \brief   Gets the area that this exit connects to.
         *
         * \return  Returns an Area* with the area that this exit connects to.
         */
        Area* getConnectArea() const;

        /*!
         * \brief   Gets the location of this exit.
         *
         * \return  Returns an Area* with the location of this exit.
         */
        Area* getLocation() const;

        virtual InteractiveNoun* getObjectLocation() const;

        /*!
         * \brief   Sets the direction of this exit.
         * 
         * \param[in] aDirection    Specifies the direction of this exit
         *
         * \return  Returns a bool indicating whether or not setting the direction
         *          was successful.
         */
        bool setDirection(ExitDirection aDirection);

        /*!
         * \brief   Sets the connecting area of this exit.
         * 
         * \param[in] anArea    Specifies the connecting area of this exit
         *
         * \return  Returns a bool indicating whether or not setting the connecting 
         *          area was successful.
         */
        bool setConnectArea(Area *anArea);

        /*!
         * \brief   Sets the location of this exit.
         * 
         * \param[in] anArea    Specifies the location of this exit
         *
         * \return  Returns a bool indicating whether or not setting the location 
         *          was successful.
         */
        bool setLocation(Area *anArea);

        /*!
         * \brief   Sets the description of this conditional element.
         *
         * \param[in] description   Specifies the description.
         *
         * \return  Returns a bool indicating whether or not the description
         *          was set successfully.
         */
        virtual bool setDescription(std::string description);

        /*!
         * \brief   Sets the alternate description of this conditional element.
         *
         * \param[in] altDescription    Specifies the alternate description.
         *
         * \return  Returns a bool indicating whether or not the alternate 
         *          description was set successfully.
         */
        virtual bool setAltDescription(std::string altDescription);

        /*!
         * \brief   Gets the description.
         *
         * \return  Returns a std::string with the exit description
         */
        virtual std::string getName() const;

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
         * \param[in] string               Holds the data to be deserialized.
         * \param[in] GameObjectManager    Provides access to other game objects in memory.
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static Exit* deserialize(std::string, GameObjectManager*);

        /*!
         * \brief   Gets the response of this object to the command look.
         * 
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(Player *aPlayer, std::vector<EffectType> *effects);  

        /*!
         * \brief   Gets the response of this object to the command listen.
         * 
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          listen.
         */
        virtual std::string listen(std::vector<EffectType> *effects); 

        /*!
         * \brief   Moves the specified player or character through this exit 
         *          to the connectArea.
         *
         * This function moves the specified player or character through this 
         * exit. After adding the player/character to the area, it calls go() 
         * on the player/character, passing a pointer of the area in the anArea 
         * parameter, so the player/character can respond to the go command.
         *
         * \param[in] aPlayer   Specifes the player that entered the command and,
         *                      if character == nullptr, the player to be added 
         *                      to this area.
         * \param[out] anArea   Specifies the area to add the player/character to.  
         * \param[in] character Optionally specifies the character to move to this
         *                      area, or nullptr if the player is the one moving.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          go.
         */
        virtual std::string go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects);

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
        //virtual bool editAttribute(Player *aPlayer, std::string attribute);

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
        void addInitialDirectionalAliases(ExitDirection direction);
        void addDirectionalAliases(ExitDirection direction);
        void removeDirectionalAliases(ExitDirection direction);
        
        std::atomic<ExitDirection> direction;
        Area *location;
        mutable std::mutex locationMutex;
        Area *connectArea;
        mutable std::mutex connectAreaMutex;
};

}}

#endif