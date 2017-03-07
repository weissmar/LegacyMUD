/*!
  \file     EquipmentSlot_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the EquipmentSlot_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_EQUIPMENT_SLOT_DATA_HPP
#define LEGACYMUD_EQUIPMENT_SLOT_DATA_HPP

#include <string>
#include <EquipmentSlot.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief EquipmentSlot Data class for legacyMUD.  
    */
    class EquipmentSlot_Data {
    public:
        
        /*!
          \brief EquipmentSlot_Data class default constructor. 
        */     
        EquipmentSlot_Data() { }
        
        /*!
          \brief EquipmentSlot_Data class destructor. 
        */   
        ~EquipmentSlot_Data(){ }

        /*!
          \brief Converts an EquipmentSlot enum to a string for saving data
          
          \param[in]  slotEnum        enum value from EquipmentSlot

          \post Returns a string of the equipment slot.
        */        
        static std::string enumToString(engine::EquipmentSlot slotEnum);  
        
        /*!
          \brief Converts an equipment slot string to an enum for loading data
          
          \param[in]  slotStr        equipment slot string
          
          \post Returns an EquipmentSlot enum
        */        
        static engine::EquipmentSlot stringToEnum(std::string slotStr);               

};

}}

#endif