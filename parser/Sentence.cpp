/*!
  \file     Sentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
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

#include <iostream>

namespace legacymud { namespace parser {

// Finds a matching verb of VerbType and creates an instance of a Sentence subclass based on Grammar rules.
Sentence *Sentence::makeSentence(const Grammar &rules, PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    std::cout << "Entered makeSentence with verb of: " << verb.getAlias() << std::endl;
    std::cout << "direct: " << (rules.takesDirectObject() == Grammar::NO ? "NO" : (rules.takesDirectObject() == Grammar::YES ? "YES" : (rules.takesDirectObject() == Grammar::TEXT ? "TEXT" : "UNKNOWN"))) << " (" << static_cast<int>(rules.takesDirectObject()) << ")" << std::endl;
    std::cout << "prep: " << (rules.takesPreposition() ? "true" : "false") << std::endl;
    std::cout << "indirect: " << (rules.takesIndirectObject() == Grammar::NO ? "NO" : (rules.takesIndirectObject() == Grammar::YES ? "YES" : "TEXT")) << std::endl;
    switch (rules.takesDirectObject()) {
    case Grammar::NO:
        if (rules.takesPreposition()) {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                // Verb-Preposition Unsupported
                std::cout << "direct: NO, prep: true, indirect: NO" << std::endl;
                return nullptr;
                break;
            case Grammar::YES:
                std::cout << "direct: NO, prep: true, indirect: YES" << std::endl;
                return new VPISentence(verb, type, command);
                break;
            case Grammar::TEXT:
                std::cout << "direct: NO, prep: true, indirect: TEXT" << std::endl;
                return new VPTSentence(verb, type, command);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                std::cout << "direct: NO, prep: false, indirect: NO" << std::endl;
                return new VSentence(verb, type, command);
                break;
            case Grammar::YES:
                // Verb-Indirect Unsupported
                std::cout << "direct: NO, prep: false, indirect: YES" << std::endl;
                return nullptr;
                break;
            case Grammar::TEXT:
                // Verb-Indirect Text Unsupported
                std::cout << "direct: NO, prep: false, indirect: TEXT" << std::endl;
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
                std::cout << "direct: YES, prep: true, indirect: NO" << std::endl;
                return nullptr;
                break;
            case Grammar::YES:
                std::cout << "direct: YES, prep: true, indirect: YES" << std::endl;
                return new VDPISentence(verb, type, command);
                break;
            case Grammar::TEXT:
                std::cout << "direct: YES, prep: true, indirect: TEXT" << std::endl;
                return new VDPTSentence(verb, type, command);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                std::cout << "direct: YES, prep: false, indirect: NO" << std::endl;
                return new VDSentence(verb, type, command);
                break;
            case Grammar::YES:
                std::cout << "direct: YES, prep: false, indirect: YES" << std::endl;
                return new VDISentence(verb, type, command);
                break;
            case Grammar::TEXT:
                std::cout << "direct: YES, prep: false, indirect: TEXT" << std::endl;
                return new VDTSentence(verb, type, command);
                break;
            }
        }
        break;
    case Grammar::TEXT:
        if (rules.takesPreposition()) {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                // Verb-Direct Text-Preposition Unsupported
                std::cout << "direct: TEXT, prep: true, indirect: NO" << std::endl;
                return nullptr;
                break;
            case Grammar::YES:
                std::cout << "direct: TEXT, prep: true, indirect: YES" << std::endl;
                return new VTPISentence(verb, type, command);
                break;
            case Grammar::TEXT:
                std::cout << "direct: TEXT, prep: true, indirect: TEXT" << std::endl;
                return new VTPTSentence(verb, type, command);
                break;
            }
        }
        else {
            switch (rules.takesIndirectObject()) {
            case Grammar::NO:
                std::cout << "direct: TEXT, prep: false, indirect: NO" << std::endl;
                return new VTSentence(verb, type, command);
                break;
            case Grammar::YES:
                std::cout << "direct: TEXT, prep: false, indirect: YES" << std::endl;
                return new VTISentence(verb, type, command);
                break;
            case Grammar::TEXT:
                // Verb-Direct Text-Indirect Text Unsupported
                std::cout << "direct: TEXT, prep: false, indirect: TEXT" << std::endl;
                return nullptr;
                break;
            }
        }
        break;
    }

    return nullptr;
}


} }
