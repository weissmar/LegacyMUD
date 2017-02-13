/*!
  \file     Sentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the Sentence class.
*/

#include "Sentence.hpp"
#include "Grammar.hpp"
#include "VDISentence.hpp"
#include "VDPISentence.hpp"
#include "VDPTSentence.hpp"
#include "VDSentence.hpp"
#include "VDTSentence.hpp"
#include "VPISentence.hpp"
#include "VPTSentence.hpp"
#include "VSentence.hpp"
#include "VTISentence.hpp"
#include "VTPISentence.hpp"
#include "VTPTSentence.hpp"
#include "VTSentence.hpp"

namespace legacymud { namespace parser {

// Creates an instance of a Sentence subclass based on Grammar rules.
Sentence *Sentence::makeSentence(Grammar rules, VerbType type, const LexicalData &playerLex, const LexicalData &areaLex) {
    switch (rules.takesDirectObject()) {
    case Grammar::NO:
        if (rules.takesPreposition()) {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                // Verb-Preposition Unsupported
                return nullptr;
                break;
            case Grammar::YES:
                return new VPISentence(type, playerLex, areaLex);
                break;
            case Grammar::TEXT:
                return new VPTSentence(type, playerLex, areaLex);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                return new VSentence(type, playerLex, areaLex);
                break;
            case Grammar::YES:
                // Verb-Indirect Unsupported
                return nullptr;
                break;
            case Grammar::TEXT:
                // Verb-Indirect Text Unsupported
                return nullptr;
                break;
            }
        }
        break;
    case Grammar::YES:
        if (rules.takesPreposition()) {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                // Verb-Direct-Preposition Unsupported
                return nullptr;
                break;
            case Grammar::YES:
                return new VDPISentence(type, playerLex, areaLex);
                break;
            case Grammar::TEXT:
                return new VDPTSentence(type, playerLex, areaLex);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                return new VDSentence(type, playerLex, areaLex);
                break;
            case Grammar::YES:
                return new VDISentence(type, playerLex, areaLex);
                break;
            case Grammar::TEXT:
                return new VDTSentence(type, playerLex, areaLex);
                break;
            }
        }
        break;
    case Grammar::TEXT:
        if (rules.takesPreposition()) {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                // Verb-Direct Text-Preposition Unsupported
                return nullptr;
                break;
            case Grammar::YES:
                return new VTPISentence(type, playerLex, areaLex);
                break;
            case Grammar::TEXT:
                return new VTPTSentence(type, playerLex, areaLex);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                return new VTSentence(type, playerLex, areaLex);
                break;
            case Grammar::YES:
                return new VTISentence(type, playerLex, areaLex);
                break;
            case Grammar::TEXT:
                // Verb-Direct Text-Indirect Text Unsupported
                return nullptr;
                break;
            }
        }
        break;
    }

    return nullptr;
}


} }
