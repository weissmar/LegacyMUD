/*!
  \file     DamageType_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the DamageType_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_DAMAGE_TYPE_DATA_HPP
#define LEGACYMUD_DAMAGE_TYPE_DATA_HPP

#include <string>
#include <DamageType.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Damage Type Data class for legacyMUD.  
    */
    class DamageType_Data {
    public:
        
        /*!
          \brief DamageType_Data class default constructor. 
        */     
        DamageType_Data() { }
        
        /*!
          \brief DamageType_Data class destructor. 
        */   
        ~DamageType_Data(){ }

        /*!
          \brief Converts a DamageType enum to a string for saving data
          
          \param[in]  typeEnum        enum value of damage type

          \post Returns a string of damage type.
        */        
        static std::string enumToString(engine::DamageType sizeEnum);  
        
        /*!
          \brief Converts a damage type string to an enum for loading data
          
          \param[in]  typeStr        character size string
          
          \post Returns a DamageType enum
        */        
        static engine::DamageType stringToEnum(std::string typeStr);               

};

}}

#endif