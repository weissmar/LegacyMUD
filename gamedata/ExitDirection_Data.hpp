/*!
  \file     ExitDirection_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the ExitDirection_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_EXIT_DIRECTION_DATA_HPP
#define LEGACYMUD_EXIT_DIRECTION_DATA_HPP

#include <string>
#include <ExitDirection.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Exit Direction Data class for legacyMUD.  
    */
    class ExitDirection_Data {
    public:
        
        /*!
          \brief ExitDirection_Data class default constructor. 
        */     
        ExitDirection_Data() { }
        
        /*!
          \brief ExitDirection_Data class destructor. 
        */   
        ~ExitDirection_Data(){ }

        /*!
          \brief Converts a ExitDirection enum to a string for saving data
          
          \param[in]  directionEnum        enum value of exit direction

          \post Returns a string of exit direction.
        */        
        static std::string enumToString(engine::ExitDirection directionEnum);  
        
        /*!
          \brief Converts a exit direction string to an enum for loading data
          
          \param[in]  directionStr        exit direction string
          
          \post Returns a ExitDirection enum
        */        
        static engine::ExitDirection stringToEnum(std::string directionStr);               

};

}}

#endif