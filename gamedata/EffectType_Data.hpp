/*!
  \file     EffectType_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the EffectType_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_EFFECT_TYPE_DATA_HPP
#define LEGACYMUD_EFFECT_TYPE_DATA_HPP

#include <string>
#include <EffectType.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief EffectType Data class for legacyMUD.  
    */
    class EffectType_Data {
    public:
        
        /*!
          \brief EffectType_Data class default constructor. 
        */     
        EffectType_Data() { }
        
        /*!
          \brief EffectType_Data class destructor. 
        */   
        ~EffectType_Data(){ }

        /*!
          \brief Converts an EffectType enum to a string for saving data
          
          \param[in]  effectEnum        enum value from EffectType

          \post Returns a string of the effect type.
        */        
        static std::string enumToString(engine::EffectType effectEnum);  
        
        /*!
          \brief Converts an effect type string to an enum for loading data
          
          \param[in]  effectStr        effect string
          
          \post Returns an EffectType enum
        */        
        static engine::EffectType stringToEnum(std::string effectStr);               

};

}}

#endif