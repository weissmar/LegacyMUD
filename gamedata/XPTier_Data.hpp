/*!
  \file     XPTier_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the XPTier_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_XPTIER_DATA_HPP
#define LEGACYMUD_XPTIER_DATA_HPP

#include <string>
#include <XPTier.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief XPTier Data class for legacyMUD.  
    */
    class XPTier_Data {
    public:
        
        /*!
          \brief XPTier_Data class default constructor. 
        */     
        XPTier_Data() { }
        
        /*!
          \brief XPTier_Data class destructor. 
        */   
        ~XPTier_Data(){ }

        /*!
          \brief Converts a XPTier enum to a string for saving data
          
          \param[in]  tierEnum        enum of XPTier

          \post Returns a string of xp tier.
        */        
        static std::string enumToString(engine::XPTier tierEnum);  
        
        /*!
          \brief Converts an xp tier string to an enum for loading data
          
          \param[in]  tierStr         xp tier string
          
          \post Returns an XPTier enum
        */        
        static engine::XPTier stringToEnum(std::string tierStr);               

};

}}

#endif