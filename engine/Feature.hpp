/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Feature.hpp
 *
 * Overview:
 *     Header file for Feature class. Defines the members and 
 * functions that apply to all features.
 ************************************************************************/

#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <string>
#include "ConditionalElement.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Feature: public ConditionalElement {
    public:
        Feature();
        Feature(std::string name, std::string placement, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        Feature(const Feature &otherFeature);
        Feature & operator=(const Feature &otherFeature);
        virtual ~Feature();
        std::string getName();
        std::string getPlacement();
        bool setName(std::string);
        bool setPlacement(std::string);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        std::string placement;
};

}}

#endif