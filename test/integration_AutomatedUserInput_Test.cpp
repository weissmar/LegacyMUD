/*!
  \file     integration_AutomatedUserInput_Test.cpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details This file contains a framework for automating the connection
           of a user and simulating their input.
*/

#include "GlobalVerbs.hpp"
#include "TestServer.hpp"

#include <parser.hpp>
#include <GameObjectManager.hpp>
#include <Area.hpp>
#include <Player.hpp>
#include <PlayerClass.hpp>
#include <SpecialSkill.hpp>
#include <Container.hpp>
#include <Exit.hpp>
#include <GameLogic.hpp>
#include <Account.hpp>

#include <gtest/gtest.h>

#include <unistd.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;
namespace test = legacymud::test;
namespace account = legacymud::account;

engine::GameLogic *logic;
test::TestServer *server;
account::Account *accountSystem;
engine::GameObjectManager *gom;
engine::Area *currentArea;
engine::Area *destinationArea;
engine::PlayerClass *mageClass;
engine::PlayerClass *fighterClass;

std::thread serverThread;

// Helper functions
void signInAsRegularUser();
void signInAsAdminUser();
std::string getInputResponse();

// Test fixture to set up and tear down each test
class Integration_Engine_ParserTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
        // Set up global verbs
        legacymud::setBuilderVerbs();
        legacymud::setEditModeVerbs();
        legacymud::setGlobalVerbs();
    }

    static void TearDownTestCase() {
        // Remove all global verbs
        parser::WordManager::resetAll();
    }

    virtual void SetUp() {
        // Set up a game instance with 2 connected areas, a mage class, and a fighter class
        logic = new engine::GameLogic();
        server = new test::TestServer();
        accountSystem = new account::Account("");
        server->initServer(0, 0, 0, logic);
        serverThread = std::thread(&legacymud::test::TestServer::startListening, server);
        logic->startGame(true, "", server, accountSystem);
        /*
        gom = new engine::GameObjectManager();
        currentArea = new engine::Area("A small area", "A simple small area", "A simple small area with nothing in it", engine::AreaSize::SMALL);
        destinationArea = new engine::Area("A large area", "A simple large area", "A simple large area with nothing in it", engine::AreaSize::LARGE);
        engine::Exit *northExit = new engine::Exit(engine::ExitDirection::NORTH, engine::EffectType::NONE, currentArea, destinationArea, false, nullptr, "An exit to the north", "");
        northExit->addNounAlias("north");
        northExit->addNounAlias("n");
        northExit->addVerbAlias(engine::CommandEnum::GO, "north", parser::Grammar::YES, parser::Grammar::NO, std::map<std::string, parser::PrepositionType>());
        northExit->addVerbAlias(engine::CommandEnum::GO, "n", parser::Grammar::YES, parser::Grammar::NO, std::map<std::string, parser::PrepositionType>());
        engine::SpecialSkill *fireball = new engine::SpecialSkill("fireball", 10, engine::DamageType::FIRE, 5, 15);
        engine::SpecialSkill *fury = new engine::SpecialSkill("fury", 8, engine::DamageType::CRUSHING, 3, 10);
        mageClass = new engine::PlayerClass(1, "mage", fireball, 1, -1, engine::DamageType::FIRE, engine::DamageType::CRUSHING, 0.1f);
        fighterClass = new engine::PlayerClass(1, "fighter", fury, 1, -1, engine::DamageType::CRUSHING, engine::DamageType::FIRE, 0.1f);
        gom->addObject(currentArea, -1);
        gom->addObject(destinationArea, -1);
        gom->addObject(northExit, -1);
        gom->addObject(fireball, -1);
        gom->addObject(fury, -1);
        gom->addObject(mageClass, -1);
        gom->addObject(fighterClass, -1);
        */
    }

    virtual void TearDown() {
        server->shutDownServer();
        serverThread.join();
    }

};

TEST_F(Integration_Engine_ParserTest, TakeAppleTest) {
    server->simulatePlayerConnection();
    ::usleep(100);
    signInAsAdminUser();
    ::usleep(100);
    server->simulatePlayerInput("take apple");
    std::cout << getInputResponse() << std::endl;
    /*engine::Player *player = new engine::Player(engine::CharacterSize::TINY, mageClass, "username", 0, "Eleminster", "A scraggly old mage", currentArea);
    gom->addObject(player, 0);

    std::string input = "go north";
    std::cout << "Getting parse results" << std::endl;
    std::vector<parser::ParseResult> results = parser::TextParser::parse(input, player->getLexicalData(), currentArea->getLexicalData());
    std::cout << "Checking results" << std::endl;
    ASSERT_FALSE(results.empty());
    ASSERT_TRUE(results[0].status == parser::ParseStatus::VALID);
    ASSERT_TRUE(logic->executeCommand(player, results[0])); */
}

std::string getInputResponse() {
    std::string output;
    while (output.empty()) {
        logic->processInput(1);
        output = server->tryGetToPlayerMsg();
    }
    return output;
}

void signInAsRegularUser() {
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("new");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("foo@bar.com");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("asdf");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("asdf");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("Eleminster");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("1");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("2");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("A scraggly old mage");
    std::cout << server->getToPlayerMsg() << std::endl;
}

void signInAsAdminUser() {
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("new");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("foo@bar.com");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("default");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("asdf");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("asdf");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("Eleminster");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("1");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("2");
    std::cout << server->getToPlayerMsg() << std::endl;
    server->simulatePlayerInput("A scraggly old mage");
    std::cout << server->getToPlayerMsg() << std::endl;
    
}
}