/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     03/07/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        EnumToString.cpp
 *
 * \details     Implementation file for Engine enums' ToString functions. 
 ************************************************************************/

#include "EnumToString.hpp"

namespace legacymud { namespace engine {

std::string areaSizeToString(AreaSize aSize){
    switch(aSize){
        case AreaSize::NONE:
            return "none";
            break;
        case AreaSize::SMALL:
            return "small";
            break;
        case AreaSize::MEDIUM:
            return "medium";
            break;
        case AreaSize::LARGE:
            return "large";
            break;
        default:
            return "none";
    }
}


std::string characterSizeToString(CharacterSize aCharacterSize){
    switch (aCharacterSize){
        case CharacterSize::TINY:
            return "tiny";
            break;
        case CharacterSize::SMALL:
            return "small";
            break;
        case CharacterSize::MEDIUM:
            return "medium";
            break;
        case CharacterSize::LARGE:
            return "large";
            break;
        case CharacterSize::HUGE:
            return "huge";
            break;
        default:
            return "none";
            break;
    }
}


std::string commandEnumToString(CommandEnum aCommandEnum){
    switch(aCommandEnum){
        case CommandEnum::INVALID:
            return "invalid";
            break;
        case CommandEnum::HELP:
            return "help";
            break;
        case CommandEnum::LOOK:
            return "look";
            break;
        case CommandEnum::LISTEN:
            return "listen";
            break;
        case CommandEnum::TAKE:
            return "take";
            break;
        case CommandEnum::PUT:
            return "put";
            break;
        case CommandEnum::DROP:
            return "drop";
            break;
        case CommandEnum::INVENTORY:
            return "inventory";
            break;
        case CommandEnum::MORE:
            return "more";
            break;
        case CommandEnum::EQUIPMENT:
            return "equipment";
            break;
        case CommandEnum::EQUIP:
            return "equip";
            break;
        case CommandEnum::UNEQUIP:
            return "unequip";
            break;
        case CommandEnum::TRANSFER:
            return "transfer";
            break;
        case CommandEnum::SPEAK:
            return "speak";
            break;
        case CommandEnum::SHOUT:
            return "shout";
            break;
        case CommandEnum::WHISPER:
            return "whisper";
            break;
        case CommandEnum::QUIT:
            return "quit";
            break;
        case CommandEnum::GO:
            return "go";
            break;
        case CommandEnum::MOVE:
            return "move";
            break;
        case CommandEnum::STATS:
            return "stats";
            break;
        case CommandEnum::QUESTS:
            return "quests";
            break;
        case CommandEnum::SKILLS:
            return "skills";
            break;
        case CommandEnum::ATTACK:
            return "attack";
            break;
        case CommandEnum::TALK:
            return "talk";
            break;
        case CommandEnum::SHOP:
            return "shop";
            break;
        case CommandEnum::BUY:
            return "buy";
            break;
        case CommandEnum::SELL:
            return "sell";
            break;
        case CommandEnum::SEARCH:
            return "search";
            break;
        case CommandEnum::USE_SKILL:
            return "use skill";
            break;
        case CommandEnum::READ:
            return "read";
            break;
        case CommandEnum::BREAK:
            return "break";
            break;
        case CommandEnum::CLIMB:
            return "climb";
            break;
        case CommandEnum::TURN:
            return "turn";
            break;
        case CommandEnum::PUSH:
            return "push";
            break;
        case CommandEnum::PULL:
            return "pull";
            break;
        case CommandEnum::EAT:
            return "eat";
            break;
        case CommandEnum::DRINK:
            return "drink";
            break;
        case CommandEnum::EDIT_MODE:
            return "edit mode";
            break;
        case CommandEnum::WARP:
            return "warp";
            break;
        case CommandEnum::COPY:
            return "copy";
            break;
        case CommandEnum::CREATE:
            return "create";
            break;
        case CommandEnum::EDIT_ATTRIBUTE:
            return "edit attribute";
            break;
        case CommandEnum::EDIT_WIZARD:
            return "edit wizard";
            break;
        case CommandEnum::SAVE:
            return "save";
            break;
        case CommandEnum::LOAD:
            return "load";
            break;
        case CommandEnum::DELETE:
            return "delete";
            break;
        default:
            return "none";
            break;
    }
}


std::string damageTypeToString(DamageType aDamageType){
    switch (aDamageType){
        case DamageType::NONE:
            return "none";
            break;
        case DamageType::CRUSHING:
            return "crushing";
            break;
        case DamageType::PIERCING:
            return "piercing";
            break;
        case DamageType::ELECTRIC:
            return "electric";
            break;
        case DamageType::FIRE:
            return "fire";
            break;
        case DamageType::WATER:
            return "water";
            break;
        case DamageType::WIND:
            return "wind";
            break;
        case DamageType::EARTH:
            return "earth";
            break;
        case DamageType::HEAL:
            return "heal";
            break;
        default:
            return "none";
            break;
    }
}


std::string dataTypeToString(DataType aDataType){
    switch (aDataType){
        case DataType::INT_TYPE:
            return "int";
            break;
        case DataType::BOOL_TYPE:
            return "bool";
            break;
        case DataType::FLOAT_TYPE:
            return "float";
            break;
        case DataType::STRING_TYPE:
            return "string";
            break;
        case DataType::COMMAND_ENUM:
            return "command enum";
            break;
        case DataType::EFFECT_TYPE:
            return "effect type";
            break;
        case DataType::AREA_SIZE:
            return "area size";
            break;
        case DataType::DAMAGE_TYPE:
            return "damage type";
            break;
        case DataType::ITEM_RARITY:
            return "item rarity";
            break;
        case DataType::EQUIPMENT_SLOT:
            return "equipement slot";
            break;
        case DataType::ITEM_POSITION:
            return "item position";
            break;
        case DataType::CHARACTER_SIZE:
            return "character size";
            break;
        case DataType::EXIT_DIRECTION:
            return "exit direction";
            break;
        case DataType::X_P_TIER:
            return "xp tier";
            break;
        case DataType::AREA_PTR:
            return "area ptr";
            break;
        case DataType::SPECIAL_SKILL_PTR:
            return "special skill ptr";
            break;
        case DataType::ITEM_TYPE_PTR:
            return "item type ptr";
            break;
        case DataType::INTERACTIVE_NOUN_PTR:
            return "interactive noun ptr";
            break;
        case DataType::CREATURE_TYPE_PTR:
            return "creature type ptr";
            break;
        case DataType::QUEST_PTR:
            return "quest ptr";
            break;
        case DataType::PLAYER_CLASS_PTR:
            return "player class ptr";
            break;
        case DataType::ITEM_PTR:
            return "item ptr";
            break;
        case DataType::NON_COMBATANT_PTR:
            return "non combatant ptr";
            break;
        default:
            return "none";
            break;
    }
}


std::string effectTypeToString(EffectType anEffectType){
    switch (anEffectType){
        case EffectType::NONE:
            return "none";
            break;
        case EffectType::DAMAGE:
            return "damage";
            break;
        case EffectType::FALL:
            return "fall";
            break;
        case EffectType::LONG_FALL:
            return "long fall";
            break;
        case EffectType::LOST_ITEM:
            return "lost item";
            break;
        case EffectType::DROP_ALL_ITEMS:
            return "drop all items";
            break;
        case EffectType::GAIN_MONEY:
            return "gain money";
            break;
        case EffectType::LOSE_MONEY:
            return "lose money";
            break;
        case EffectType::HEAL:
            return "heal";
            break;
        case EffectType::GET_SPECIAL_POINTS:
            return "get special points";
            break;
        default:
            return "none";
            break;
    }
}


std::string equipmentSlotToString(EquipmentSlot anEquipmentSlot){
    switch (anEquipmentSlot){
        case EquipmentSlot::NONE:
            return "none";
            break;
        case EquipmentSlot::HEAD:
            return "head";
            break;
        case EquipmentSlot::SHOULDERS:
            return "shoulders";
            break;
        case EquipmentSlot::NECK:
            return "neck";
            break;
        case EquipmentSlot::TORSO:
            return "torso";
            break;
        case EquipmentSlot::BELT:
            return "belt";
            break;
        case EquipmentSlot::LEGS:
            return "legs";
            break;
        case EquipmentSlot::ARMS:
            return "arms";
            break;
        case EquipmentSlot::HANDS:
            return "hands";
            break;
        case EquipmentSlot::RIGHT_HAND:
            return "right hand";
            break;
        case EquipmentSlot::LEFT_HAND:
            return "left hand";
            break;
        case EquipmentSlot::FEET:
            return "feet";
            break;
        case EquipmentSlot::RIGHT_RING:
            return "right ring";
            break;
        case EquipmentSlot::LEFT_RING:
            return "left ring";
            break;
        default:
            return "none";
            break;
    }
}


std::string exitDirectionToString(ExitDirection anExitDirection){
    switch (anExitDirection){
        case ExitDirection::NORTH:
            return "north";
            break;
        case ExitDirection::SOUTH:
            return "south";
            break;
        case ExitDirection::EAST:
            return "east";
            break;
        case ExitDirection::WEST:
            return "west";
            break;
        case ExitDirection::NORTHEAST:
            return "northeast";
            break;
        case ExitDirection::NORTHWEST:
            return "northwest";
            break;
        case ExitDirection::SOUTHEAST:
            return "southeast";
            break;
        case ExitDirection::SOUTHWEST:
            return "southwest";
            break;
        case ExitDirection::UP:
            return "up";
            break;
        case ExitDirection::DOWN:
            return "down";
            break;
        default:
            return "none";
            break;
    }
}


std::string itemPositionToString(ItemPosition anItemPosition){
    switch (anItemPosition){
        case ItemPosition::NONE:
            return "none";
            break;
        case ItemPosition::GROUND:
            return "ground";
            break;
        case ItemPosition::INVENTORY:
            return "inventory";
            break;
        case ItemPosition::EQUIPPED:
            return "equipped";
            break;
        case ItemPosition::IN:
            return "in";
            break;
        case ItemPosition::ON:
            return "on";
            break;
        case ItemPosition::UNDER:
            return "under";
            break;
        default:
            return "none";
            break;
    }
}


std::string itemRarityToString(ItemRarity anItemRarity){
    switch (anItemRarity){
        case ItemRarity::COMMON:
            return "common";
            break;
        case ItemRarity::UNCOMMON:
            return "uncommon";
            break;
        case ItemRarity::RARE:
            return "rare";
            break;
        case ItemRarity::LEGENDARY:
            return "legendary";
            break;
        case ItemRarity::QUEST:
            return "quest";
            break;
        default:
            return "none";
            break;
    }
}


std::string objectTypeToString(ObjectType anObjectType){
    switch (anObjectType){
        case ObjectType::NONE:
            return "none";
            break;
        case ObjectType::AREA:
            return "area";
            break;
        case ObjectType::ARMOR_TYPE:
            return "armor type";
            break;
        case ObjectType::CONTAINER:
            return "container";
            break;
        case ObjectType::CREATURE:
            return "creature";
            break;
        case ObjectType::CREATURE_TYPE:
            return "creature type";
            break;
        case ObjectType::EXIT:
            return "exit";
            break;
        case ObjectType::FEATURE:
            return "feature";
            break;
        case ObjectType::ITEM:
            return "item";
            break;
        case ObjectType::ITEM_TYPE:
            return "item type";
            break;
        case ObjectType::NON_COMBATANT:
            return "non-combatant";
            break;
        case ObjectType::PLAYER:
            return "player";
            break;
        case ObjectType::PLAYER_CLASS:
            return "player class";
            break;
        case ObjectType::QUEST:
            return "quest";
            break;
        case ObjectType::QUEST_STEP:
            return "quest step";
            break;
        case ObjectType::SPECIAL_SKILL:
            return "special skill";
            break;
        case ObjectType::WEAPON_TYPE:
            return "weapon type";
            break;
        default:
            return "none";
            break;
    }
}


std::string xpTierToString(XPTier aXPTier){
    switch (aXPTier){
        case XPTier::TRIVIAL:
            return "trivial";
            break;
        case XPTier::EASY:
            return "easy";
            break;
        case XPTier::NORMAL:
            return "normal";
            break;
        case XPTier::HARD:
            return "hard";
            break;
        case XPTier::LEGENDARY:
            return "legendary";
            break;
        default:
            return "none";
            break;
    }
}

}}