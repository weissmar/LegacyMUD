/*!
  \file     ItemPosition_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the ItemPosition_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_ITEM_POSITION_DATA_HPP
#define LEGACYMUD_ITEM_POSITION_DATA_HPP

#include <string>
#include <ItemPosition.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Item Position Data class for legacyMUD.  
    */
    class ItemPosition_Data {
    public:
        
        /*!
          \brief ItemPosition_Data class default constructor. 
        */     
        ItemPosition_Data() { }
        
        /*!
          \brief ItemPosition_Data class destructor. 
        */   
        ~ItemPosition_Data(){ }

        /*!
          \brief Converts a ItemPosition enum to a string for saving data
          
          \param[in]  positionEnum        enum value of item position

          \post Returns a string of item position.
        */        
        static std::string enumToString(engine::ItemPosition positionEnum);  
        
        /*!
          \brief Converts an item position string to an enum for loading data
          
          \param[in]  positionStr         item position string
          
          \post Returns an ItemPosition enum
        */        
        static engine::ItemPosition stringToEnum(std::string positionStr);               

};

}}

#endif