/*!
  \file     GlobalVerbs.cpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details This file contains the actual grammar definitions and aliases for all global verbs.
*/
#include "GlobalVerbs.hpp"
#include <WordManager.hpp>

namespace legacymud {

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
    vi.description = "look";
    parser::WordManager::addGlobalVerb("look", vi);
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
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
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "warp";
    parser::WordManager::addBuilderVerb("warp", vi);
    vi.description = "warp to";
    parser::WordManager::addBuilderVerb("warp to", vi);

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
    vi.description = "edit (object) (attrib)";
    parser::WordManager::addBuilderVerb("edit", vi);

    //vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    //vi.description = "edit (attrib)";
    //parser::WordManager::addBuilderVerb("edit", vi);
    
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


}