/*!
  \file     AreaSize_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the AreaSize_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_AREA_SIZE_DATA_HPP
#define LEGACYMUD_AREA_SIZE_DATA_HPP

#include <string>
#include <AreaSize.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Area Size Data class for legacyMUD.  
    */
    class AreaSize_Data {
    public:
        
        /*!
          \brief AreaSize_Data class default constructor. 
        */     
        AreaSize_Data() { }
        
        /*!
          \brief AreaSize_Data class destructor. 
        */   
        ~AreaSize_Data(){ }

        /*!
          \brief Converts an AreaSize enum to a string for saving data
          
          \param[in]  sizeEnum        enum value of area size

          \post Returns a string of area size.
        */        
        static std::string enumToString(engine::AreaSize sizeEnum);  
        
        /*!
          \brief Converts an area size string to an enum for loading data
          
          \param[in]  sizeStr        area size string
          
          \post Returns an AreaSize enum
        */        
        static engine::AreaSize stringToEnum(std::string sizeStr);               

};

}}

#endif