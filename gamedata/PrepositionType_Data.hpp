/*!
  \file     PrepositionType_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the PrepositionType_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_PREPOSITION_TYPE_DATA_HPP
#define LEGACYMUD_PREPOSITION_TYPE_DATA_HPP

#include <string>
#include <PrepositionType.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief PrepositionType Data class for legacyMUD.  
    */
    class PrepositionType_Data {
    public:
        
        /*!
          \brief PrepositionType_Data class default constructor. 
        */     
        PrepositionType_Data() { }
        
        /*!
          \brief PrepositionType_Data class destructor. 
        */   
        ~PrepositionType_Data(){ }

        /*!
          \brief Converts a PrepositionType enum to a string for saving data
          
          \param[in]  prepositionEnum        enum value 

          \post Returns a string of PrepositionType enum.
        */        
        static std::string enumToString(parser::PrepositionType prepositionEnum);  
        
        /*!
          \brief Converts a PrepositionType string to an enum for loading data
          
          \param[in]  prepositionStr        PrepositionType string
          
          \post Returns a PrepositionType enum
        */        
        static parser::PrepositionType stringToEnum(std::string prepositionStr);               

};

}}

#endif