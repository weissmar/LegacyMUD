/*!
  \file     CharacterSize_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the CharacterSize_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_CHARACTER_SIZE_DATA_HPP
#define LEGACYMUD_CHARACTER_SIZE_DATA_HPP

#include <string>
#include <CharacterSize.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Character Size Data class for legacyMUD.  
    */
    class CharacterSize_Data {
    public:
        
        /*!
          \brief CharacterSize_Data class default constructor. 
        */     
        CharacterSize_Data() { }
        
        /*!
          \brief CharacterSize_Data class destructor. 
        */   
        ~CharacterSize_Data(){ }

        /*!
          \brief Converts a CharacterSize enum to a string for saving data
          
          \param[in]  sizeEnum        enum value of character size

          \post Returns a string of character size.
        */        
        static std::string enumToString(engine::CharacterSize sizeEnum);  
        
        /*!
          \brief Converts an character size string to an enum for loading data
          
          \param[in]  sizeStr        character size string
          
          \post Returns an CharacterSize enum
        */        
        static engine::CharacterSize stringToEnum(std::string sizeStr);               

};

}}

#endif