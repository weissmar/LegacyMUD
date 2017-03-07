/*!
  \file     ItemRarity_Data.hpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the ItemRarity_Data class. Functions are provided to
            convert between string and enum values. This class should not be instantiated.
*/

 
#ifndef LEGACYMUD_ITEM_RARITY_DATA_HPP
#define LEGACYMUD_ITEM_RARITY_DATA_HPP

#include <string>
#include <ItemRarity.hpp>


namespace legacymud { namespace gamedata {

    /*!
      \brief ITem Rarity Data class for legacyMUD.  
    */
    class ItemRarity_Data {
    public:
        
        /*!
          \brief ItemRarityData class default constructor. 
        */     
        ItemRarity_Data() { }
        
        /*!
          \brief ItemRarityData class destructor. 
        */   
        ~ItemRarity_Data(){ }

        /*!
          \brief Converts an item rarity enum to a string for saving data
          
          \param[in]  rarityEnum        enum value from ItemRarity

          \post Returns a string of the item rarity.
        */        
        static std::string enumToString(engine::ItemRarity rarityEnum);  
        
        /*!
          \brief Converts an item rarity string to an enum for loading data
          
          \param[in]  rarityStr        rarity string
          
          \post Returns an ItemRarity enum
        */        
        static engine::ItemRarity stringToEnum(std::string rarityStr);               

};

}}

#endif