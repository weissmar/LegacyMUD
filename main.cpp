/*!
  \file     main.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 03/09/2017
  \course   CS467, Winter 2017
 
  \details This file contains the main function for LegacyMUD.
*/

#include <Server.hpp>
#include <GameLogic.hpp>
#include <parser.hpp>
#include <Account.hpp>
#include <GlobalVerbs.hpp>

#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Maximum number of players that can connect to the server
const int MAX_PLAYERS = 10;
// Server timeout period in seconds
const int SERVER_TIMEOUT = 300;

int main(int argc, char *argv[]) {
    legacymud::telnet::Server ts;
    legacymud::engine::GameLogic logic;
    int serverPort;
    std::string file = "";

    // seed random number genrator
    std::srand(std::time(0));

    // Validate command line entry. 
    if (argc != 3) {
        std::cout << "Error: Usage is " << argv[0] << " [port number] [game data filename]" << std::endl;
        return 1;
    }
    
    // Convert from c-string to int.
    serverPort = ::atoi(argv[1]);
    // Get game data filename
    file = std::string(argv[2]);

    legacymud::account::Account accountM(file + ".accounts");
    
    // initialize parser vocab
    legacymud::setGlobalVerbs();
    legacymud::setBuilderVerbs();
    legacymud::setEditModeVerbs();
    
    // Add ignore words
    parser::WordManager::addIgnoreWord("the");
    parser::WordManager::addIgnoreWord("a");
    parser::WordManager::addIgnoreWord("an");


    // initialize server
    if (!ts.initServer(serverPort, MAX_PLAYERS, SERVER_TIMEOUT, &logic)) {
        return 1;
    }
    std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);
    serverThread.detach();

    // initialize account system
    accountM.initialize();
    
    // start logic
    logic.startGame(true, file, &ts, &accountM);
    
    while (true) {
        logic.processInput(1);
        logic.updateCreatures();
        logic.updatePlayersInCombat();
    }

    return 0;
}
