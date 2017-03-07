/*!
  \file     DataManager.hpp
  \author   Keith Adkins
  \created  2/20/2017
  \modified 3/7/2017
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
          
          The function retrieves game data from the game engine and saves it to disk.
          
          \param[in]  fileDescriptor        fileDescriptor to the game data file
          \param[in]  gameObjectManagerPtr  pointer to the game object manager       
          \pre fileDescriptor               should point to a validated file
          \pre gameObjectManagerPtr         should not be null
          \post Returns true if all game data is saved.  Otherwise, it returns false.
        */        
        bool saveGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr);  
               
        /*!
          \brief Load the game data
          
          The function loads the game data from disk and populates all the game data needed by the engine.
          
          \param[in]  fileDescriptor        fileDescriptor to the game data file
          \param[in]  gameObjectManagerPtr  pointer to the game object manager       
          \pre fileDescriptor               should point to a validated file
          \pre gameObjectManagerPtr         should not be null
          \post Returns true if all game data is loaded.  Otherwise, it returns false.
        */        
        bool loadGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr);                 

};

}}

#endif