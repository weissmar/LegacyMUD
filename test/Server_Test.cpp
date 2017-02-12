/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/30/2017
 * Last Modified: 2/11/2017
 * Course:        CS467, Winter 2017
 * Filename:      Server_Test.cpp
 *
 * Overview:
 *     Unit test for the Server class.
 ************************************************************************/

#include "Server.hpp"
#include "GameLogic.hpp"    // game logic stub
#include <string>
#include <iostream>

#include <gtest/gtest.h>

namespace {

/* Server test. */
TEST(ServerTest, InitializeServer) {
    legacymud::engine::GameLogic gl;
    legacymud::telnet::Server ts;
    int serverPort = 0;
    int playerCap = 10;
    int timeOut = 900;           // in seconds.
    bool err;

    /* Initialize the server. */
    err = ts.initServer(serverPort, playerCap, timeOut, &gl)
    
    EXPECT_EQ(false, err) 
            << "Expect false for invalid port.";
  
}



}
