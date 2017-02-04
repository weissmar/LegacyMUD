/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Exit.hpp
 *
 * Overview:
 *     Header file for Exit class. Defines the members and 
 * functions that apply to all exits.
 ************************************************************************/

#ifndef EXIT_HPP
#define EXIT_HPP

#include <string>
#include "ConditionalElement.hpp"
#include "Area.hpp"
#include "EffectType.hpp"
#include "ExitDirection.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Exit: public ConditionalElement {
    public:
        Exit();
        Exit(ExitDirection direction, EffectType effect, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        Exit(const Exit &otherExit);
        Exit & operator=(const Exit &otherExit);
        virtual ~Exit();
        ExitDirection getDirection();
        EffectType getEffect();
        Area* getConnectArea();
        bool setDirection(ExitDirection aDirection);
        bool setEffect(EffectType anEffect);
        bool setConnectArea(Area *anArea);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        ExitDirection direction;
        EffectType effect;
        Area *connectArea;
};

}}

#endif