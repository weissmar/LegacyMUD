/*!
  \file     CommandEnum_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the CommandEnum_Data class.
*/


#include "CommandEnum_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string CommandEnum_Data::enumToString(engine::CommandEnum command) {

    std::string commandStr;

    switch (command) {
        case engine::CommandEnum::INVALID : commandStr = "INVALID";
            break;
        case engine::CommandEnum::HELP : commandStr = "HELP";
            break;  
        case engine::CommandEnum::LOOK : commandStr = "LOOK";
            break;             
        case engine::CommandEnum::LISTEN : commandStr = "LISTEN";
            break; 
        case engine::CommandEnum::TAKE : commandStr = "TAKE";
            break;
        case engine::CommandEnum::PUT : commandStr = "PUT";
            break;    
        case engine::CommandEnum::DROP : commandStr = "DROP";
            break; 
        case engine::CommandEnum::INVENTORY : commandStr = "INVENTORY";
            break;
        case engine::CommandEnum::MORE : commandStr = "MORE";
            break;    
        case engine::CommandEnum::EQUIPMENT : commandStr = "EQUIPMENT";
            break; 
        case engine::CommandEnum::EQUIP : commandStr = "EQUIP";
            break;
        case engine::CommandEnum::UNEQUIP : commandStr = "UNEQUIP";
            break;    
        case engine::CommandEnum::TRANSFER : commandStr = "TRANSFER";
            break; 
        case engine::CommandEnum::SPEAK : commandStr = "SPEAK";
            break; 
        case engine::CommandEnum::SHOUT : commandStr = "SHOUT";
            break;            
        case engine::CommandEnum::WHISPER : commandStr = "WHISPER";
            break;
        case engine::CommandEnum::QUIT : commandStr = "QUIT";
            break;    
        case engine::CommandEnum::GO : commandStr = "GO";
            break; 
        case engine::CommandEnum::MOVE : commandStr = "MOVE";
            break;
        case engine::CommandEnum::STATS : commandStr = "STATS";
            break;    
        case engine::CommandEnum::QUESTS : commandStr = "QUESTS";
            break; 
        case engine::CommandEnum::SKILLS : commandStr = "SKILLS";
            break;
        case engine::CommandEnum::ATTACK : commandStr = "ATTACK";
            break;    
        case engine::CommandEnum::TALK : commandStr = "TALK";
            break; 
        case engine::CommandEnum::SHOP : commandStr = "SHOP";
            break;
        case engine::CommandEnum::BUY : commandStr = "BUY";
            break;    
        case engine::CommandEnum::SELL : commandStr = "SELL";
            break; 
        case engine::CommandEnum::SEARCH : commandStr = "SEARCH";
            break; 
        case engine::CommandEnum::USE_SKILL : commandStr = "USE_SKILL";
            break;  
        case engine::CommandEnum::READ : commandStr = "READ";
            break;
        case engine::CommandEnum::BREAK : commandStr = "BREAK";
            break;    
        case engine::CommandEnum::CLIMB : commandStr = "CLIMB";
            break; 
        case engine::CommandEnum::TURN : commandStr = "TURN";
            break;
        case engine::CommandEnum::PUSH : commandStr = "PUSH";
            break;    
        case engine::CommandEnum::PULL : commandStr = "PULL";
            break; 
        case engine::CommandEnum::EAT : commandStr = "EAT";
            break;
        case engine::CommandEnum::DRINK : commandStr = "DRINK";
            break;    
        case engine::CommandEnum::EDIT_MODE : commandStr = "EDIT_MODE";
            break; 
        case engine::CommandEnum::WARP : commandStr = "WARP";
            break;
        case engine::CommandEnum::COPY : commandStr = "COPY";
            break;    
        case engine::CommandEnum::CREATE : commandStr = "CREATE";
            break; 
        case engine::CommandEnum::EDIT_ATTRIBUTE : commandStr = "EDIT_ATTRIBUTE";
            break; 
        case engine::CommandEnum::EDIT_WIZARD : commandStr = "EDIT_WIZARD";
            break;             
        case engine::CommandEnum::SAVE : commandStr = "SAVE";
            break; 
        case engine::CommandEnum::LOAD : commandStr = "LOAD";
            break; 
        case engine::CommandEnum::DELETE : commandStr = "DELETE";
            break;             
            
    }  

    return commandStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::CommandEnum CommandEnum_Data::stringToEnum(std::string commandStr) {

    legacymud::engine::CommandEnum command;
    
    if (commandStr == "INVALID") 
        command = engine::CommandEnum::INVALID;    
    else if (commandStr == "HELP") 
        command = engine::CommandEnum::HELP;     
    else if (commandStr == "LOOK") 
        command = engine::CommandEnum::LOOK; 
    else if (commandStr == "LISTEN") 
        command = engine::CommandEnum::LISTEN; 
    else if (commandStr == "TAKE") 
        command = engine::CommandEnum::TAKE; 
    else if (commandStr == "PUT") 
        command = engine::CommandEnum::PUT; 
    else if (commandStr == "DROP") 
        command = engine::CommandEnum::DROP; 
    else if (commandStr == "INVENTORY") 
        command = engine::CommandEnum::INVENTORY; 
    else if (commandStr == "MORE") 
        command = engine::CommandEnum::MORE; 
    else if (commandStr == "EQUIPMENT") 
        command = engine::CommandEnum::EQUIPMENT; 
    else if (commandStr == "EQUIP") 
        command = engine::CommandEnum::EQUIP; 
    else if (commandStr == "UNEQUIP") 
        command = engine::CommandEnum::UNEQUIP; 
    else if (commandStr == "TRANSFER") 
        command = engine::CommandEnum::TRANSFER; 
    else if (commandStr == "SPEAK") 
        command = engine::CommandEnum::SPEAK;   
    else if (commandStr == "SHOUT") 
        command = engine::CommandEnum::SHOUT;      
    else if (commandStr == "WHISPER") 
        command = engine::CommandEnum::WHISPER; 
    else if (commandStr == "QUIT") 
        command = engine::CommandEnum::QUIT; 
    else if (commandStr == "GO") 
        command = engine::CommandEnum::GO; 
    else if (commandStr == "MOVE") 
        command = engine::CommandEnum::MOVE; 
    else if (commandStr == "STATS") 
        command = engine::CommandEnum::STATS; 
    else if (commandStr == "QUESTS") 
        command = engine::CommandEnum::QUESTS; 
    else if (commandStr == "SKILLS") 
        command = engine::CommandEnum::SKILLS; 
    else if (commandStr == "ATTACK") 
        command = engine::CommandEnum::ATTACK; 
    else if (commandStr == "TALK") 
        command = engine::CommandEnum::TALK; 
    else if (commandStr == "SHOP") 
        command = engine::CommandEnum::SHOP; 
    else if (commandStr == "BUY") 
        command = engine::CommandEnum::BUY;    
    else if (commandStr == "SELL") 
        command = engine::CommandEnum::SELL;   
    else if (commandStr == "SEARCH") 
        command = engine::CommandEnum::SEARCH;     
    else if (commandStr == "USE_SKILL") 
        command = engine::CommandEnum::USE_SKILL; 
    else if (commandStr == "READ") 
        command = engine::CommandEnum::READ; 
    else if (commandStr == "BREAK") 
        command = engine::CommandEnum::BREAK; 
    else if (commandStr == "CLIMB") 
        command = engine::CommandEnum::CLIMB; 
    else if (commandStr == "TURN") 
        command = engine::CommandEnum::TURN; 
    else if (commandStr == "PUSH") 
        command = engine::CommandEnum::PUSH; 
    else if (commandStr == "PULL") 
        command = engine::CommandEnum::PULL; 
    else if (commandStr == "EAT") 
        command = engine::CommandEnum::EAT; 
    else if (commandStr == "DRINK") 
        command = engine::CommandEnum::DRINK; 
    else if (commandStr == "EDIT_MODE") 
        command = engine::CommandEnum::EDIT_MODE; 
    else if (commandStr == "WARP") 
        command = engine::CommandEnum::WARP; 
    else if (commandStr == "COPY") 
        command = engine::CommandEnum::COPY;    
    else if (commandStr == "CREATE") 
        command = engine::CommandEnum::CREATE;     
    else if (commandStr == "EDIT_ATTRIBUTE") 
        command = engine::CommandEnum::EDIT_ATTRIBUTE; 
    else if (commandStr == "EDIT_WIZARD") 
        command = engine::CommandEnum::EDIT_WIZARD; 
    else if (commandStr == "SAVE") 
        command = engine::CommandEnum::SAVE; 
    else if (commandStr == "LOAD") 
        command = engine::CommandEnum::LOAD; 
    else if (commandStr == "DELETE") 
        command = engine::CommandEnum::DELETE; 
     
    return command;
}

    
}}  

    