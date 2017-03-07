/*!
  \file     CommandEnum_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the CommandEnum_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_COMMAND_ENUM_DATA_HPP
#define LEGACYMUD_COMMAND_ENUM_DATA_HPP

#include <string>
#include <CommandEnum.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief Command Enum Data class for legacyMUD.  
    */
    class CommandEnum_Data {
    public:
        
        /*!
          \brief Class default constructor. 
        */     
        CommandEnum_Data() { }
        
        /*!
          \brief Class destructor. 
        */   
        ~CommandEnum_Data(){ }

        /*!
          \brief Converts an CommandEnum enum to a string for saving data
          
          \param[in]  commandEnum        enum value from CommandEnum

          \post Returns a string of the command.
        */        
        static std::string enumToString(engine::CommandEnum commandEnum);  
        
        /*!
          \brief Converts command string to an enum for loading data
          
          \param[in]  commandStr        command string
          
          \post Returns a CommandEnum 
        */        
        static engine::CommandEnum stringToEnum(std::string commandStr);               

};

}}

#endif