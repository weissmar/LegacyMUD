/*!
  \file     DataManager.hpp
  \author   Keith Adkins
  \created  2/20/2017
  \modified 3/8/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the DataManager class.
*/

 
#ifndef LEGACYMUD_DATA_MANAGER_HPP
#define LEGACYMUD_DATA_MANAGER_HPP

#include <string>


namespace legacymud {
    namespace engine{
        class GameObjectManager;    // forward declaration
    }
    namespace gamedata {

    /*!
      \brief Data management class for legacyMUD.  
    */
    class DataManager {
    public:
        
        /*!
          \brief DataManager class default constructor. 
        */     
        DataManager() { }
        
        /*!
          \brief DataManager class destructor. 
        */   
        ~DataManager(){ }

        /*!
          \brief Saves the game data at server launch.
          
          This function retrieves game data from the game engine and saves it to disk.
          
          \param[in]  filename              file where data is to be saved
          \param[in]  gameObjectManagerPtr  pointer to the game object manager       
          \param[in]  startAreaId           ID of the starting area
          \pre gameObjectManagerPtr         Should not be null.
          \pre The following pointers in objects held by the GameObjectManager should not be null:
               all location pointers, CreatureType of a Creature, ItemType of an Item, PlayerClass of
               a Player, and SpecialSkill of a PlayerClass.
               
          \post Returns true if all game data is saved.  Otherwise, it returns false.
        */        
        bool saveGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr, int startAreaId);  
               
        /*!
          \brief Load the game data
          
          The function loads the game data from disk and populates all the game data needed by the engine.
          It is meant to be executed only during game launch and expects to be passed an instantiated GameObjectManager
          that does not contain any objects.
          
          \param[in]  filename              file containing data to be loaded
          \param[in]  gameObjectManagerPtr  pointer to the game object manager
          \param[out] startAreaId           ID of the starting area
          \pre gameObjectManagerPtr         Should not be null and should not contain any game objects.
          
          \post Returns true if all game data is loaded.  Otherwise, it returns false.
        */        
        bool loadGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr, int &startAreaId);                 

};

}}

#endif