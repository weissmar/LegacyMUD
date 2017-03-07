/*!
  \file     GrammarSupport_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the GrammarSupport_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_GRAMMAR_SUPPORT_DATA_HPP
#define LEGACYMUD_GRAMMAR_SUPPORT_DATA_HPP

#include <string>
#include <Grammar.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Support Enum Data for legacyMUD.  
    */
    class GrammarSupport_Data {
    public:
        
        /*!
          \brief GrammarSupport_Data class default constructor. 
        */     
        GrammarSupport_Data() { }
        
        /*!
          \brief GrammarSupport_Data class destructor. 
        */   
        ~GrammarSupport_Data(){ }

        /*!
          \brief Converts a Support enum to a string for saving data
          
          \param[in]  supportEnum        enum value for Support in the Grammar class

          \post Returns a string of support.
        */        
        static std::string enumToString(parser::Grammar::Support supportEnum);  
        
        /*!
          \brief Converts a support string to an enum for loading data
          
          \param[in]  supportStr        support string
          
          \post Returns an EffectType enum
        */        
        static parser::Grammar::Support stringToEnum(std::string supportStr);               

};

}}

#endif