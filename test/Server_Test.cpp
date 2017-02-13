/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/30/2017
 * Last Modified: 2/12/2017
 * Course:        CS467, Winter 2017
 * Filename:      Server_Test.cpp
 *
 * Overview:
 *     Unit tests for the Server class.  
 ************************************************************************/
  
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread> 
#include <Server.hpp>
#include <GameLogic.hpp> 
          
#include <gtest/gtest.h>

namespace {


/* Server constructor and getter testing. */
TEST(ServerTest, ConstructServer) {
    legacymud::telnet::Server ts;

    /* Get server pause value. */
    EXPECT_EQ(false, ts.getServerPause() ) 
            << "Expect false for server pause default value.";

    /* Get server max player value. */
    EXPECT_EQ(0, ts.getMaxPlayers() ) 
            << "Expect 0 for max player default value.";            

    /* Get server player count value. */
    EXPECT_EQ(0, ts.getPlayerCount() ) 
            << "Expect 0 for player count starting value.";              

    /* Get server timeout value. */
    EXPECT_EQ(0, ts.getTimeOut() ) 
            << "Expect 0 for timeout default value.";

    /* Get server port value. */
    EXPECT_EQ(0, ts.getServerPort() ) 
            << "Expect 0 for server port default value.";

    /* Get game logic pointer value. */
    EXPECT_EQ(0, ts.getGameLogicPt() ) 
            << "Expect 0 for game logic pointer default value.";
            
}

/* Initialize server testing. */
TEST(ServerTest, InitializeServer) {
    legacymud::telnet::Server ts;
    legacymud::engine::GameLogic gl;
    int serverPort;
    int playerCap;
    int timeOut;           // timeout in seconds.
    
    srand(time(NULL));     // seed the random number generator

    /* All parameters are valid */
    playerCap = 1;
    timeOut = 1; 
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    EXPECT_EQ(true, ts.initServer(serverPort, playerCap, timeOut, &gl) ) 
            << "Expect true for all parameters being valid.  Could return false if port is already taken.";
    
    /* Shut the server down. */
    EXPECT_EQ(true, ts.shutdown() ) 
        << "Expect true that the server is shutdown.";
    
    /* Invalid server port of 999. */
    serverPort = 999;
    EXPECT_EQ(false, ts.initServer(serverPort, playerCap, timeOut, &gl) ) 
            << "Expect false for invalid port.";
   
    /* Invalid server port of 65536. */
    serverPort = 65536; 
    EXPECT_EQ(false, ts.initServer(serverPort, playerCap, timeOut, &gl) ) 
            << "Expect false for invalid port.";
            
    /* Invalid player cap. */
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    playerCap = 0;
    EXPECT_EQ(false, ts.initServer(serverPort, playerCap, timeOut, &gl) ) 
            << "Expect false for invalid player cap.  Could return false if port is already taken."; 


    /* Invalid timeout. */
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    timeOut = 0;
    EXPECT_EQ(false, ts.initServer(serverPort, playerCap, timeOut, &gl) ) 
            << "Expect false for invalid timeout.  Could return false if port is already taken.";             
}


/* Server listen testing. */
TEST(ServerTest, ListeningServer) {
    legacymud::telnet::Server ts;
    legacymud::engine::GameLogic gl;
    int serverPort;
    int playerCap=1;
    int timeOut=1;           // timeout in seconds.
    
    srand(time(NULL));     // seed the random number generator

    /* All parameters are valid */ 
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    
    ASSERT_EQ(ts.initServer(serverPort, playerCap, timeOut, &gl), true ) 
            << "Expect true for initializing server.  Could return false if port is already taken.";
    
    /* The following only runs if the previous passes. */
    
    /* Send listening off to it's own thread. */
    std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);   
    
    /* Shut the server down. */
    EXPECT_EQ(true, ts.shutdown() ) 
        << "Expect true that the server is shutdown.";  
        
    serverThread.join();          
}


/* Server player handling. */
TEST(ServerTest, HandlingPlayers) {
    legacymud::telnet::Server ts;
    legacymud::engine::GameLogic gl;
    int serverPort;
    int playerCap=1;
    int timeOut=1;           // timeout in seconds.
    int playerFd = 1;
    
    srand(time(NULL));     // seed the random number generator

    /* All parameters are valid */ 
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    
    ASSERT_EQ(ts.initServer(serverPort, playerCap, timeOut, &gl), true ) 
            << "Expect true for initializing server.  Could return false if port is already taken.";
    
    /* The following only runs if the previous passes. */
    
    /* Send listening off to it's own thread. */
    std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);   
    
    /* Disconnect a player.  There are no players so this should return false. */
    EXPECT_EQ(false, ts.disconnectPlayer(playerFd) ) 
        << "Expect false since there are no players.";

    /* Set a player's echo.  There are no players so this should return false. */
    EXPECT_EQ(false, ts.setPlayerEcho(playerFd, true) ) 
        << "Expect false since there are no players.";              
    
    /* Shut the server down. */
    EXPECT_EQ(true, ts.shutdown() ) 
        << "Expect true that the server is shutdown.";  
        
    serverThread.join();          
}

/* Testing server setters and getters. */
TEST(ServerTest, SettersandGetters) {
    legacymud::telnet::Server ts;
    legacymud::engine::GameLogic gl;
    int serverPort;
    int playerCap=1;
    int timeOut=1;           // timeout in seconds.
    
    srand(time(NULL));     // seed the random number generator

    /* All parameters are valid */ 
    serverPort = rand() % (65535-1000) + 1000;  // range 1000 to 65535
    
    ASSERT_EQ(ts.initServer(serverPort, playerCap, timeOut, &gl), true ) 
            << "Expect true for initializing server.  Could return false if port is already taken.";
    
    /* The following only runs if the previous passes. */
    
    /* Send listening off to it's own thread. */
    std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);   
    
    /* Set and get server pause. */
    ts.pause(true);
    EXPECT_EQ(true, ts.getServerPause() ) 
        << "Expect pause to be true.";
    ts.pause(false);
    EXPECT_EQ(false, ts.getServerPause() ) 
        << "Expect pause to be true.";        

    /* Set and get maxPlayers. */
    EXPECT_EQ(false, ts.setMaxPlayers(0) ) 
        << "Expect false since 0 is invalid.";          // invalid amount
    EXPECT_EQ(true, ts.setMaxPlayers(10) ) 
        << "Expect true since 10 is a valid amount.";   // valid amount        
    EXPECT_EQ(10, ts.getMaxPlayers() ) 
        << "Expect true the amount was set to 10.";       

    /* Set and get TimeOut. */
    EXPECT_EQ(false, ts.setTimeOut(0) ) 
        << "Expect false since 0 is invalid.";          // invalid amount
    EXPECT_EQ(true, ts.setTimeOut(900) ) 
        << "Expect true since 10 is a valid amount.";   // valid amount        
    EXPECT_EQ(900, ts.getTimeOut() ) 
        << "Expect true the amount was set to 900.";         
    
    /* Get the player count. */
    EXPECT_EQ(0, ts.getPlayerCount() ) 
        << "Expect 0 since there are no players on the server.";  

    /* Get the server port. */
    EXPECT_EQ(serverPort, ts.getServerPort() ) 
        << "Expect true.";   

    /* Get the the game logic pointer. */
    EXPECT_EQ(&gl, ts.getGameLogicPt() ) 
        << "Expect true.";         
    
    /* Shut the server down. */
    EXPECT_EQ(true, ts.shutdown() ) 
        << "Expect true that the server is shutdown.";  
        
    serverThread.join();          
}

}
