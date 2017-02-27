/*!
  \file     TestServer.cpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017

  \details  Implementation file for the TestServer class.
*/


#include "TestServer.hpp"
#include <GameLogic.hpp>

#include <iostream>


namespace legacymud { namespace test {


TestServer::TestServer() {
    _gameLogicPt = nullptr;
    _player = -1;
    _disconnect.store(false);
}


bool TestServer::initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt) {
    _gameLogicPt = gameLogicPt;
    _waitForConnection.lock();
    _waitForDisconnect.lock();
    return true;
}


void TestServer::startListening() {
    _waitForConnection.lock();
    if (!_disconnect.load()) {
        _player = 0;
        _waitForReceive.lock();
        _waitForSend.lock();
        _thread = std::thread(&legacymud::engine::GameLogic::newPlayerHandler, _gameLogicPt, 0);
        _waitForDisconnect.lock();
        _thread.join();
    }
}


bool TestServer::disconnectPlayer(int playerFd) {
    if (_player != -1) {
        _disconnect.store(true);
        _waitForReceive.unlock();
        _player = -1;
        _waitForDisconnect.unlock();
        return true;
    }
    return false;
}


bool TestServer::shutDownServer() {
    _disconnect.store(true);
    if (_player == -1) {
        _waitForConnection.unlock();
    }
    else {
        _waitForReceive.unlock();
        _waitForSend.unlock();
    }
    _waitForDisconnect.unlock();
    return true;
}

std::string TestServer::getToPlayerMsg() {
    _waitForSend.lock();
    std::string input = _toPlayer;
    _toPlayer = std::string();
    return input;
}

std::string TestServer::tryGetToPlayerMsg() {
    if (_waitForSend.try_lock()) {
        std::string input = _toPlayer;
        _toPlayer = std::string();
        return input;
    }
    return std::string();
}

void TestServer::simulatePlayerInput(std::string input) {
    _fromPlayer = input;
    _waitForReceive.unlock();
}

void TestServer::simulatePlayerConnection() {
    _waitForConnection.unlock();
}

bool TestServer::sendQuestion(int playerFd, std::string outQuestion) {
    _toPlayer = outQuestion;
    _waitForSend.unlock();
    return true;
}


bool TestServer::sendMsg(int playerFd, std::string outMsg) {
    _toPlayer = outMsg;
    _waitForSend.unlock();
    return true;
}


bool TestServer::sendNewLine(int playerFd) {
    return sendMsg(playerFd, "");
}


bool TestServer::receiveMsg(int playerFd, std::string &inMsg ) {
    inMsg.clear();
    if (_player != -1) {
        _waitForReceive.lock();
        if (!_disconnect.load()) {
            inMsg = _fromPlayer;
            return true;
        }
        return false;
    }
    return false;
}


bool TestServer::listenForMsgs(int playerFd) {
    std::string inMsg;
    while (!_disconnect.load()) {
        if (receiveMsg(playerFd, inMsg)) {
            /* Send the message to the message handler. */
            _gameLogicPt->receivedMessageHandler(inMsg, playerFd);
        }
    }
    return false;
}

void TestServer::pause(bool pause) {
}

bool TestServer::setMaxPlayers(int maxPlayers) {
    return true;
}

bool TestServer::setTimeOut(int timeOut) {
    return true;
}

bool TestServer::setPlayerEcho(int playerFd, bool echo) {
    return true;
}


bool TestServer::getServerPause() const {
    return false;
}

int TestServer::getMaxPlayers() const {
    return 1;
}

int TestServer::getPlayerCount() const {
    return _player == -1 ? 0 : 1;
}

int TestServer::getTimeOut() const {
    return 0;
}

int TestServer::getServerPort() const {
    return 0;
}

engine::GameLogic* TestServer::getGameLogicPt() const {
    return _gameLogicPt;
}

}}

