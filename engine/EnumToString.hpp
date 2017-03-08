/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     03/07/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        EnumToString.hpp
 *
 * \details     Header file for Engine enums' ToString functions. These
 *              functions convert from a enum class type to a lowercase
 *              std::string.
 ************************************************************************/

#ifndef ENUM_TO_STRING_HPP
#define ENUM_TO_STRING_HPP

#include <string>
#include "AreaSize.hpp"
#include "CharacterSize.hpp"
#include "CommandEnum.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"
#include "EffectType.hpp"
#include "EquipmentSlot.hpp"
#include "ExitDirection.hpp"
#include "ItemPosition.hpp"
#include "ItemRarity.hpp"
#include "ObjectType.hpp"
#include "XPTier.hpp"

namespace legacymud { namespace engine {

std::string areaSizeToString(AreaSize aSize);
std::string characterSizeToString(CharacterSize aCharacterSize);
std::string commandEnumToString(CommandEnum aCommandEnum);
std::string damageTypeToString(DamageType aDamageType);
std::string dataTypeToString(DataType aDataType);
std::string effectTypeToString(EffectType anEffectType);
std::string equipmentSlotToString(EquipmentSlot anEquipmentSlot);
std::string exitDirectionToString(ExitDirection anExitDirection);
std::string itemPositionToString(ItemPosition anItemPosition);
std::string itemRarityToString(ItemRarity anItemRarity);
std::string objectTypeToString(ObjectType anObjectType);
std::string xpTierToString(XPTier aXPTier);

}}

#endif