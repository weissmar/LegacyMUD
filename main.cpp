#include <iostream>
#include <vector>
#include <cstring>
#include <TextParser.hpp>
#include <VerbInfo.hpp>
#include <WordManager.hpp>
#include <LexicalData.hpp>

/******************************************
 * Helper Functions
 *****************************************/

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Set up the edit mode verbs
void setEditModeVerbs() {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);
}

void setGlobalVerbs() {
    parser::VerbInfo vi;

    // HELP command
    vi.command = engine::CommandEnum::HELP;
    vi.description = "help";
    parser::WordManager::addGlobalVerb("help", vi);

    // LOOK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::LOOK;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "look";
    parser::WordManager::addGlobalVerb("look", vi);
    vi.description = "look at";
    parser::WordManager::addGlobalVerb("look at", vi);

    // LISTEN command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::LISTEN;
    vi.description = "listen";
    parser::WordManager::addGlobalVerb("listen", vi);

    // TAKE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TAKE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "pick up";
    parser::WordManager::addGlobalVerb("pick up", vi);
    vi.description = "take";
    parser::WordManager::addGlobalVerb("take", vi);
    
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    // "pick up" alias
    vi.description = "pick up";
    parser::WordManager::addGlobalVerb("pick up", vi);
    // "take" additionally supports "from"
    vi.grammar.addPreposition("from", parser::PrepositionType::FROM);
    vi.description = "take";
    parser::WordManager::addGlobalVerb("take", vi);

    // PUT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::PUT;
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    vi.description = "put";
    parser::WordManager::addGlobalVerb("put", vi);

    // DROP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::DROP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "drop";
    parser::WordManager::addGlobalVerb("drop", vi);

    // INVENTORY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::INVENTORY;
    vi.description = "inventory";
    parser::WordManager::addGlobalVerb("inventory", vi);
    vi.description = "inv";
    parser::WordManager::addGlobalVerb("inv", vi);

    // MORE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::MORE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "more";
    parser::WordManager::addGlobalVerb("more", vi);

    // EQUIPMENT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EQUIPMENT;
    vi.description = "eq";
    parser::WordManager::addGlobalVerb("eq", vi);
    vi.description = "equipment";
    parser::WordManager::addGlobalVerb("equipment", vi);

    // EQUIP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EQUIP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "wear";
    parser::WordManager::addGlobalVerb("wear", vi);
    vi.description = "put on";
    parser::WordManager::addGlobalVerb("put on", vi);
    vi.description = "equip";
    parser::WordManager::addGlobalVerb("equip", vi);

    // UNEQUIP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::UNEQUIP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "remove";
    parser::WordManager::addGlobalVerb("remove", vi);
    vi.description = "take off";
    parser::WordManager::addGlobalVerb("take off", vi);
    vi.description = "unequip";
    parser::WordManager::addGlobalVerb("unequip", vi);

    // TRANSFER command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TRANSFER;
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "give";
    parser::WordManager::addGlobalVerb("give", vi);

    // SPEAK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SPEAK;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "say";
    parser::WordManager::addGlobalVerb("say", vi);

    // SHOUT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SHOUT;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "shout";
    parser::WordManager::addGlobalVerb("shout", vi);
    vi.description = "yell";
    parser::WordManager::addGlobalVerb("yell", vi);

    // WHISPER command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::WHISPER;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "whisper";
    parser::WordManager::addGlobalVerb("whisper", vi);

    // QUIT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUIT;
    vi.description = "quit";
    parser::WordManager::addGlobalVerb("quit", vi);
    vi.description = "exit";
    parser::WordManager::addGlobalVerb("exit", vi);
    vi.description = "logout";
    parser::WordManager::addGlobalVerb("logout", vi);

    // GO command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::GO;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "go";
    parser::WordManager::addGlobalVerb("go", vi);
    vi.description = "go to";
    parser::WordManager::addGlobalVerb("go to", vi);

    // STATS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::STATS;
    vi.description = "stats";
    parser::WordManager::addGlobalVerb("stats", vi);

    // QUESTS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUESTS;
    vi.description = "quests";
    parser::WordManager::addGlobalVerb("quests", vi);

    // SKILLS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SKILLS;
    vi.description = "skills";
    parser::WordManager::addGlobalVerb("skills", vi);

    // ATTACK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::ATTACK;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "attack";
    parser::WordManager::addGlobalVerb("attack", vi);

    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("with", parser::PrepositionType::WITH);
    vi.grammar.addPreposition("using", parser::PrepositionType::WITH);
    vi.description = "attack with";
    parser::WordManager::addGlobalVerb("attack", vi);

    // TALK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TALK;
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "talk";
    parser::WordManager::addGlobalVerb("talk", vi);

    // SHOP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SHOP;
    vi.description = "shop";
    parser::WordManager::addGlobalVerb("shop", vi);

    // BUY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::BUY;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "buy";
    parser::WordManager::addGlobalVerb("buy", vi);

    // SELL command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SELL;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "sell";
    parser::WordManager::addGlobalVerb("sell", vi);

    // SEARCH command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SEARCH;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "search";
    parser::WordManager::addGlobalVerb("search", vi);
    vi.description = "open";
    parser::WordManager::addGlobalVerb("open", vi);

    // USE_SKILL command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::USE_SKILL;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "use";
    parser::WordManager::addGlobalVerb("use", vi);
    
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::TO);
    vi.description = "use on";
    parser::WordManager::addGlobalVerb("use", vi);

}

void setBuilderVerbs() {
    parser::VerbInfo vi;

    // WARP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::WARP;
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "warp";
    parser::WordManager::addBuilderVerb("warp", vi);

    // COPY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::COPY;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "copy";
    parser::WordManager::addBuilderVerb("copy", vi);

    // CREATE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::CREATE;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "create";
    parser::WordManager::addBuilderVerb("create", vi);
    vi.description = "new";
    parser::WordManager::addBuilderVerb("new", vi);
    vi.description = "add";
    parser::WordManager::addBuilderVerb("add", vi);

    // EDIT_ATTRIBUTE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EDIT_ATTRIBUTE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::TEXT);
    vi.description = "edit (attrib)";
    parser::WordManager::addBuilderVerb("edit", vi);

    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.description = "edit (attrib) of (object)";
    parser::WordManager::addBuilderVerb("edit", vi);

    // EDIT_WIZARD command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EDIT_WIZARD;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "edit (object)";
    parser::WordManager::addBuilderVerb("edit", vi);

    // SAVE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SAVE;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.description = "save";
    parser::WordManager::addBuilderVerb("save", vi);

    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "save filename";
    parser::WordManager::addBuilderVerb("save", vi);

    // LOAD command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::LOAD;
    vi.description = "load";
    parser::WordManager::addBuilderVerb("load", vi);

    // DELETE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::DELETE;
    vi.description = "delete";
    parser::WordManager::addBuilderVerb("delete", vi);
    
}

parser::LexicalData areaLex;
parser::LexicalData playerLex;
std::vector<parser::ParseResult> results;
parser::TextParser tp;
std::string input;

/******************************************
 * Test Cases
 *****************************************/
// Test the happy path of the HELP command
void TextParserTest_HelpHappyPath() {
    input = "help";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::HELP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::HELP) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the HELP command with invalid input after
void TextParserTest_HelpInvalidSuffix() {
    input = "help me";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_DIRECT != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_DIRECT) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::HELP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::HELP) << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    // Unparsed text should contain "me"
    if (::strcmp("me", results.begin()->unparsed.c_str()) != 0) std::cout << "Expected " << "me" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
}

// Test the happy path of the standalone LOOK command
void TextParserTest_LookStandaloneHappyPath() {
    std::string input = "look";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::LOOK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::LOOK) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the LOOK command with object
void TextParserTest_LookHappyPath() {
    // std::string input = "look candle";
    std::vector<std::string> inputs = {
        "look candle",
        "look at candle",
        "look at the candle"
    }; 
    
    // TODO: Add candle to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::LOOK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::LOOK) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the LISTEN command
void TextParserTest_ListenHappyPath() {
    std::string input = "listen";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::LISTEN) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::LISTEN) << std::endl;
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the EDIT_MODE command
void TextParserTest_EditModeValidityCheck() {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);

    input = "editmode";
    auto it = parser::WordManager::getEditModeVerbs(input).begin();
    if (it == parser::WordManager::getEditModeVerbs(input).end()) return;
    if (engine::CommandEnum::EDIT_MODE != it->command) std::cout << "Expected " << static_cast<int>(engine::CommandEnum::EDIT_MODE) << ", Got " << static_cast<int>(it->command) << std::endl;
    if (::strcmp("editmode", it->description.c_str()) != 0) std::cout << "Expected " << "editmode" << ", Got " << it->description.c_str() << std::endl;
    if (parser::Grammar::NO != it->grammar.takesDirectObject()) std::cout << "Expected " << parser::Grammar::NO << ", Got " << it->grammar.takesDirectObject() << std::endl;
    if (parser::Grammar::NO != it->grammar.takesIndirectObject()) std::cout << "Expected " << parser::Grammar::NO << ", Got " << it->grammar.takesIndirectObject() << std::endl;
    if (it->grammar.takesPreposition()) std::cout << "Expected false, got true\n";
}

// Test the happy path of the TAKE command
void TextParserTest_TakeHappyPath() {
    std::vector<std::string> inputs = {
        "take candle",
        "pick up candle",
        "take the candle",
        "pick up the candle"
    };
    
    // TODO: Add candle to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::TAKE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::TAKE) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the PUT command
void TextParserTest_PutHappyPath() {
    std::vector<std::string> inputs = {
        "put torch on table",
        "put torch on the table",
        "put the torch on table",
        "put the torch on the table"
    };
    
    // TODO: Add torch to areaVM and table to areaNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::PUT) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::PUT) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        if (engine::ItemPosition::ON != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::ON) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (1 != results.begin()->indirect.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (*results.begin()->indirect.begin() == nullptr) std::cout << "Expected " << "nullptr" << ", Got " << results.begin()->unparsed.empty() << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the DROP command
void TextParserTest_DropHappyPath() {
    std::vector<std::string> inputs = {
        "drop candle",
        "drop the candle"
    };

    // TODO: Add candle to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::DROP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::DROP) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the INVENTORY command
void TextParserTest_InventoryHappyPath() {
    std::vector<std::string> inputs = {
        "inventory",
        "inv"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::INVENTORY) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVENTORY) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the MORE command
void TextParserTest_MoreItemHappyPath() {
    std::vector<std::string> inputs = {
        "more torch",
        "more heal"
    };

    // TODO: Add torch and heal to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::MORE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::MORE) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the EQUIPMENT command
void TextParserTest_EquipmentHappyPath() {
    std::vector<std::string> inputs = {
        "equipment",
        "eq"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::EQUIPMENT) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EQUIPMENT) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the EQUIP command
void TextParserTest_EquipHappyPath() {
    std::vector<std::string> inputs = {
        "wear helmet",
        "put on helmet",
        "equip helmet",
        "wear the helmet",
        "put on the helmet",
        "equip the helmet"
    };

    // TODO: Add helmet to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::EQUIP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EQUIP) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the UNEQUIP command
void TextParserTest_UnequipHappyPath() {
    std::vector<std::string> inputs = {
        "remove helmet",
        "take off helmet",
        "unequip helmet",
        "remove the helmet",
        "take off the helmet",
        "unequip the helmet"
    };

    // TODO: Add helmet to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::UNEQUIP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::UNEQUIP) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the TRANSFER command
void TextParserTest_TransferHappyPath() {
    std::vector<std::string> inputs = {
        "give torch to Joe",
        "give the torch to Joe"
    };

    // TODO: Add torch to playerVM and Joe to areaNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::UNEQUIP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::UNEQUIP) << std::endl;
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the SPEAK command
void TextParserTest_SpeakHappyPath() {
    input = "say Hi everyone!";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SPEAK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SPEAK) << std::endl;
    if (::strcmp("Hi everyone!", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "Hi everyone!" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}


// Test the happy path of the SHOUT command
void TextParserTest_ShoutHappyPath() {
    std::vector<std::string> inputs = {
        "shout HELLO THERE",
        "yell HELLO THERE"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::SHOUT) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SHOUT) << std::endl;
        if (::strcmp("HELLO THERE", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "HELLO THERE" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
        // Should not be any objects or position
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        results.clear();
    }
}

// Test the happy path of the WHISPER command
void TextParserTest_WhisperHappyPath() {
    input = "whisper Don't tell anyone to Joe";
    
    // TODO: Add Joe to areaNM
    
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::WHISPER) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::WHISPER) << std::endl;
    if (1 != results.begin()->indirect.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
    if (*results.begin()->indirect.begin() == nullptr) std::cout << "Expected " << "nullptr" << ", Got " << results.begin()->unparsed.empty() << std::endl;
    if (::strcmp("Don't tell anyone", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "Don't tell anyone" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any direct object or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test the happy path of the QUIT command
void TextParserTest_QuitHappyPath() {
    std::vector<std::string> inputs = {
        "quit",
        "exit",
        "logout"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::QUIT) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::QUIT) << std::endl;
        // Should not be any objects, position or unparsed text
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the GO command
void TextParserTest_GoHappyPath() {
    std::vector<std::string> inputs = {
        "go north",
        "go to north",
        "go to the north"
    };

    // TODO: Add exit to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::GO) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::GO) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object, position, or unparsed text
        if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the implied GO command with exit alias only
void TextParserTest_GoImpliedHappyPath() {
    std::vector<std::string> inputs = {
        "north",
        "n"
    };

    // TODO: Add exit to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::GO) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::GO) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object, position, or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the STATS command
void TextParserTest_StatsHappyPath() {
    input = "stats";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::STATS) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::STATS) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the QUESTS command
void TextParserTest_QuestsHappyPath() {
    input = "quests";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::QUESTS) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::QUESTS) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SKILLS command
void TextParserTest_SkillsHappyPath() {
    input = "skills";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SKILLS) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SKILLS) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the ATTACK command
void TextParserTest_AttackHappyPath() {
    std::vector<std::string> inputs = {
        "attack troll",
        "attack the troll"
    };

    // TODO: Add troll to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::ATTACK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::ATTACK) << std::endl;
        if (1 != results.begin()->direct.size()) std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl;
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the ATTACK command with a skill
void TextParserTest_AttackWithSkillHappyPath() {
    std::vector<std::string> inputs = {
        "attack troll with fireball",
        "attack the troll with fireball"
    };

    // TODO: Add troll to areaVM and fireball to playerNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::ATTACK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::ATTACK) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        if (1 != results.begin()->indirect.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        // Should not be any unparsed text
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the TALK command
void TextParserTest_TalkHappyPath() {
    input = "talk to Sarah";

    // TODO: Add Sarah to areaVM

    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::TALK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::TALK) << std::endl;
    if (1 != results.begin()->indirect.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
    // Should not be any direct objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SHOP command
void TextParserTest_ShopHappyPath() {
    input = "shop";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SHOP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SHOP) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the BUY command
void TextParserTest_BuyHappyPath() {
    input = "buy torch";

    // TODO: Add torch to areaVM

    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::BUY) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::BUY) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SELL command
void TextParserTest_SellHappyPath() {
    input = "sell torch";

    // TODO: Add torch to playerVM

    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SELL) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SELL) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SEARCH command
void TextParserTest_SearchHappyPath() {
    std::vector<std::string> inputs = {
        "search chest",
        "open chest",
        "search the chest",
        "open the chest"
    };

    // TODO: Add chest to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::SEARCH) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SEARCH) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the USE_SKILL command
void TextParserTest_UseSkillHappyPath() {
    input = "use heal";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::USE_SKILL) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::USE_SKILL) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the USE_SKILL command on a target
void TextParserTest_UseSkillOnTargetHappyPath() {
    input = "use heal on Joe";

    // TODO: Add heal to playerVM and Joe to areaNM

    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::USE_SKILL) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::USE_SKILL) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    if (1 != results.begin()->indirect.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
    if (*results.begin()->indirect.begin() == nullptr) std::cout << "Expected " << "nullptr" << ", Got " << results.begin()->unparsed.empty() << std::endl;
    // Should not be any position or unparsed text
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the MOVE command
void TextParserTest_MoveHappyPath() {
    std::vector<std::string> inputs = {
        "move chair",
        "move the chair"
    };

    // TODO: Add chair to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::MOVE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::MOVE) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the READ command
void TextParserTest_ReadHappyPath() {
    std::vector<std::string> inputs = {
        "read book",
        "read the book"
    };

    // TODO: Add book to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::READ) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::READ) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the BREAK command
void TextParserTest_BreakHappyPath() {
    std::vector<std::string> inputs = {
        "break book",
        "break the book"
    };

    // TODO: Add book to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::BREAK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::BREAK) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the CLIMB command
void TextParserTest_ClimbHappyPath() {
    std::vector<std::string> inputs = {
        "climb ladder",
        "climb the ladder"
    };

    // TODO: Add ladder to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::CLIMB) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::CLIMB) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the TURN command
void TextParserTest_TurnHappyPath() {
    std::vector<std::string> inputs = {
        "turn knob",
        "turn the knob"
    };

    // TODO: Add knob to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::TURN) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::TURN) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the PUSH command
void TextParserTest_PushHappyPath() {
    std::vector<std::string> inputs = {
        "push table",
        "push the table"
    };

    // TODO: Add table to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::PUSH) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::PUSH) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the PULL command
void TextParserTest_PullHappyPath() {
    std::vector<std::string> inputs = {
        "pull table",
        "pull the table"
    };

    // TODO: Add table to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::PULL) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::PULL) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the EAT command
void TextParserTest_EatHappyPath() {
    std::vector<std::string> inputs = {
        "eat bread",
        "eat the bread"
    };

    // TODO: Add bread to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::EAT) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EAT) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the DRINK command
void TextParserTest_DrinkHappyPath() {
    std::vector<std::string> inputs = {
        "drink potion",
        "drink the potion"
    };

    // TODO: Add potion to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::DRINK) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::DRINK) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test the happy path of the EDIT_MODE command
void TextParserTest_EditModeHappyPath() {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);

    input = "editmode";
    results = tp.parse(input, playerLex, areaLex, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::EDIT_MODE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EDIT_MODE) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (!results.begin()->unparsed.empty()) std::cout << "Expected " << "" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
}

// Test the EDIT_MODE command without permissions
void TextParserTest_EditModeNotAdmin() {
    input = "editmode";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_VERB != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_VERB) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::INVALID) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVALID) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (::strcmp("editmode", results.begin()->unparsed.c_str()) != 0) std::cout << "Expected " << "editmode" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
}

// Test the happy path of the WARP command
void TextParserTest_WarpHappyPath() {
    input = "warp 1";

    // TODO: Add area with ID of 1 to WordManager::addNoun

    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::WARP) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::WARP) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the COPY command
void TextParserTest_CopyHappyPath() {
    input = "copy 1";

    // TODO: Add item with ID of 1 to WordManager::addNoun

    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::COPY) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::COPY) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the CREATE command
void TextParserTest_CreateHappyPath() {
    std::vector<std::string> inputs = {
        "create item",
        "new item"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex, true, true);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::CREATE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::CREATE) << std::endl;
        if (::strcmp("item", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "item" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
        // Should not be any objects or position
        if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        results.clear();
    }
}

// Test the happy path of the ADD command
void TextParserTest_AddHappyPath() {
    input = "add exit";

    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::CREATE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::CREATE) << std::endl;
    if (::strcmp("exit", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "exit" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test the happy path of the EDIT_ATTRIBUTE command
void TextParserTest_EditAttributeHappyPath() {
    input = "edit long description";
    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_DIRECT != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_DIRECT) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::EDIT_ATTRIBUTE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EDIT_ATTRIBUTE) << std::endl;
    if (::strcmp("long description", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "long description" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test the happy path of the EDIT_WIZARD command
void TextParserTest_EditWizardHappyPath() {
    input = "edit torch";

    // TODO: Add torch to areaVM

    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::EDIT_WIZARD) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::EDIT_WIZARD) << std::endl;
    if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
    // Should not be any indirect objects, position or unparsed text
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SAVE command
void TextParserTest_SaveHappyPath() {
    std::string input = "save";
    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SAVE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SAVE) << std::endl;
    // Should not be any objects, position or unparsed text
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
    if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
}

// Test the happy path of the SAVE command with filename
void TextParserTest_SaveFilenameHappyPath() {
    std::string input = "save filename.dat";
    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::SAVE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::SAVE) << std::endl;
    if (::strcmp("filename.dat", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "filename.dat" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test the happy path of the LOAD command
void TextParserTest_LoadHappyPath() {
    std::string input = "load filename.dat";
    results = tp.parse(input, playerLex, areaLex, true, true);
    if (1 != results.size()) return;
    if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::INVALID) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVALID) << std::endl;
    if (::strcmp("filename.dat", results.begin()->directAlias.c_str()) != 0) std::cout << "Expected " << "filename.dat" << ", Got " << results.begin()->directAlias.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test the happy path of the DELETE command
void TextParserTest_DeleteHappyPath() {
    std::vector<std::string> inputs = {
        "delete potion",
        "delete the potion",
        "delete 1"
    };

    // TODO: Add potion with ID of 1 to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex, true, true);
        if (1 != results.size()) return;
        if (parser::ParseStatus::VALID != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::VALID) << ", Got " << static_cast<int>(results[0].status) << std::endl;
        if (results.begin()->command != engine::CommandEnum::DELETE) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::DELETE) << std::endl;
        if (1 != results.begin()->direct.size()) { std::cout << "Expected " << 1 << ", Got " << results.begin()->direct.size() << std::endl; return; }
        // Should not be any indirect object or unparsed text
        if (0 != results.begin()->indirect.size()) { std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl; return; }
        if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
        if (results.begin()->unparsed.empty()) std::cout << "Expected " << !results.begin()->unparsed.empty() << ", Got " << results.begin()->unparsed.empty() << std::endl;
        results.clear();
    }
}

// Test an unknown verb only
void TextParserTest_InvalidVerbOnly() {
    std::string input = "foo";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_VERB != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_VERB) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::INVALID) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVALID) << std::endl;
    if (::strcmp("foo", results.begin()->unparsed.c_str()) != 0) std::cout << "Expected " << "foo" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test an unknown verb with unknown direct object
void TextParserTest_InvalidVerbInvalidDirectObject() {
    std::string input = "foo bar";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_VERB != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_VERB) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::INVALID) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVALID) << std::endl;
    if (::strcmp("foo bar", results.begin()->unparsed.c_str()) != 0) std::cout << "Expected " << "foo bar" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

// Test an unknown verb with known direct object
void TextParserTest_InvalidVerbValidDirectObject() {
    std::string input = "foo torch";
    results = tp.parse(input, playerLex, areaLex);
    if (1 != results.size()) return;
    if (parser::ParseStatus::INVALID_VERB != results[0].status) std::cout << "Expected " << static_cast<int>(parser::ParseStatus::INVALID_VERB) << ", Got " << static_cast<int>(results[0].status) << std::endl;
    if (results.begin()->command != engine::CommandEnum::INVALID) std::cout << "Expected " << static_cast<int>(results.begin()->command) << ", Got " << static_cast<int>(engine::CommandEnum::INVALID) << std::endl;
    if (::strcmp("foo torch", results.begin()->unparsed.c_str()) != 0) std::cout << "Expected " << "foo torch" << ", Got " << results.begin()->unparsed.c_str() << std::endl;
    // Should not be any objects or position
    if (0 != results.begin()->direct.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->direct.size() << std::endl;
    if (0 != results.begin()->indirect.size()) std::cout << "Expected " << 0 << ", Got " << results.begin()->indirect.size() << std::endl;
    if (engine::ItemPosition::NONE != results.begin()->position) std::cout << "Expected " << static_cast<int>(engine::ItemPosition::NONE) << ", Got " << static_cast<int>(results.begin()->position) << std::endl;
}

int main(int argc, char *argv[]) {
    setEditModeVerbs();
    setGlobalVerbs();
    setBuilderVerbs();
for (int i = 0; i < 1; ++i) {
    TextParserTest_HelpHappyPath();
    TextParserTest_HelpInvalidSuffix();
    TextParserTest_LookStandaloneHappyPath();
    TextParserTest_LookHappyPath();
    TextParserTest_ListenHappyPath();
    TextParserTest_EditModeValidityCheck();
    TextParserTest_TakeHappyPath();
    TextParserTest_PutHappyPath();
    TextParserTest_DropHappyPath();
    TextParserTest_InventoryHappyPath();
    TextParserTest_MoreItemHappyPath();
    TextParserTest_EquipmentHappyPath();
    TextParserTest_EquipHappyPath();
    TextParserTest_UnequipHappyPath();
    TextParserTest_TransferHappyPath();
    TextParserTest_SpeakHappyPath();
    TextParserTest_ShoutHappyPath();
    TextParserTest_WhisperHappyPath();
    TextParserTest_QuitHappyPath();
    TextParserTest_GoHappyPath();
    TextParserTest_GoImpliedHappyPath();
    TextParserTest_StatsHappyPath();
    TextParserTest_QuestsHappyPath();
    TextParserTest_SkillsHappyPath();
    TextParserTest_AttackHappyPath();
    TextParserTest_AttackWithSkillHappyPath();
    TextParserTest_TalkHappyPath();
    TextParserTest_ShopHappyPath();
    TextParserTest_BuyHappyPath();
    TextParserTest_SellHappyPath();
    TextParserTest_SearchHappyPath();
    TextParserTest_UseSkillHappyPath();
    TextParserTest_UseSkillOnTargetHappyPath();
    TextParserTest_MoveHappyPath();
    TextParserTest_ReadHappyPath();
    TextParserTest_BreakHappyPath();
    TextParserTest_ClimbHappyPath();
    TextParserTest_TurnHappyPath();
    TextParserTest_PushHappyPath();
    TextParserTest_PullHappyPath();
    TextParserTest_EatHappyPath();
    TextParserTest_DrinkHappyPath();
    TextParserTest_EditModeHappyPath();
    TextParserTest_EditModeNotAdmin();
    TextParserTest_WarpHappyPath();
    TextParserTest_CopyHappyPath();
    TextParserTest_CreateHappyPath();
    TextParserTest_AddHappyPath();
    TextParserTest_EditAttributeHappyPath();
    TextParserTest_EditWizardHappyPath();
    TextParserTest_SaveHappyPath();
    TextParserTest_SaveFilenameHappyPath();
    TextParserTest_LoadHappyPath();
    TextParserTest_DeleteHappyPath();
    TextParserTest_InvalidVerbOnly();
    TextParserTest_InvalidVerbInvalidDirectObject();
    TextParserTest_InvalidVerbValidDirectObject();
}

    std::cout << "Hello World!" << std::endl;
    return 0;
}

