/*!
  \file     Account.cpp
  \author   Keith Adkins
  \created  2/12/2017
  \modified 2/14/2017
  \course   CS467, Winter 2017
 
  \details Implementation file for the Account class.
*/


#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>     // atoi
#include "Account.hpp"


namespace legacymud { namespace account {


/******************************************************************************
* Constructor: Account                        
*****************************************************************************/
Account::Account() { 
    /* Sets default private member variables. */
    _fileName = "legacy_mud_accounts.txt";      
}


/******************************************************************************
* Function:    initialize                 
*****************************************************************************/
bool Account::initialize() {
    
    std::ifstream inFile(_fileName);    // input stream for account file 
    std::string userName;               // userName string to be read in
    std::string isAdminStr;             // isAdmin string to be read in
    std::string userIdStr;              // userId string to be read in
    _UserInfo user;                     // user info struct for read in data

    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);  
    
    _userMap.clear();       // initialize the user map
    
    /* Load the account data from the file into the user map. */
    if (inFile.is_open() ) {
        
        /* Read the username. */
        while (getline(inFile, userName) ){           
        
            /* Read the password. */
            if (!getline(inFile, user.password) ) {       
                inFile.close();
                return false;     // error: data corrupt
            }

            /* Read isAdmin. */
            if (!getline(inFile, isAdminStr) ) {        
                inFile.close();
                return false;     // error: data corrupt
            }
            /* Convert isAdmin to a boolean. */
            if (isAdminStr == "true")
                user.isAdmin = true;
            else
                user.isAdmin = false;

            /* Read the userId. */
            if (!getline(inFile, userIdStr) ) {         
                inFile.close();
                return false;     // error: data corrupt
            }
            
            /* Convert userIdStr to an integer. */
            user.userId = atoi(userIdStr.c_str());      
            
            /* Put the user in the map. */
            _userMap[userName] = user;         
        }
 
        inFile.close();     // close the file
        
        return true;
    }
    else 
        return false;       // error opening file  
}   


/******************************************************************************
* Function:    saveToDisk                 
*****************************************************************************/
bool Account::saveToDisk() {
    
    std::ofstream outFile(_fileName);     // output stream for saving account data 
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);    
    
    /* Open the file and save the account data. */
    if (outFile.is_open() ) {
        
        /* Iterate through the user map writing all the data to disk. */
        for (auto user = _userMap.begin(); user != _userMap.end(); user++ ) {
            
            /* Store the username. */
            outFile << user->first << '\n';                    
            
            /* Store the password. */
            outFile << user->second.password << '\n';           
            
            /* Store the isAdmin setting. */
            if (user->second.isAdmin == true)                   
                outFile << "true\n";
            else
                outFile << "false\n"; 
            
            /* Store the userId. */
            outFile << user->second.userId << '\n';            
        }

        outFile.close();    // close file
        return true;
    }
    else 
        return false;       // error opening file
}


/******************************************************************************
* Function:    uniqueUsername                 
*****************************************************************************/
bool Account::uniqueUsername(std::string userName) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the userName is in the map, return false.  Otherwise return true. */
    return (user == _userMap.end());    // name is unique if end of map reached
       
}      


/******************************************************************************
* Function:    createAccount                 
*****************************************************************************/
bool Account::createAccount(std::string userName, std::string password, bool isAdmin, int userId ) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is not in the map, create the account. */
    if(user != _userMap.end()) 
        return false;   // user already listed
    else {
        _UserInfo newUser;
        newUser.password = password;
        newUser.isAdmin = isAdmin;
        newUser.userId = userId;
        _userMap[userName] = newUser;   // add the user           
        return true;        
    }   
}


/******************************************************************************
* Function:    changePassword                 
*****************************************************************************/
bool Account::changePassword(std::string userName, std::string password ) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is in the map, change the password. */
    if(user != _userMap.end()) { 
        user->second.password = password;
        return true;
    }
    else
        return false;
}        


/******************************************************************************
* Function:    verifyAccount                 
*****************************************************************************/
bool Account::verifyAccount(std::string userName, std::string password ) { 
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is in the map, return true if the passwords match. */
    if( (user != _userMap.end()) && (user->second.password == password) ) 
        return true;
    else
        return false;
}     


/******************************************************************************
* Function:    setAdmin                 
*****************************************************************************/
bool Account::setAdmin(std::string userName, bool isAdmin ) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is in the map, set the admin. */
    if(user != _userMap.end()) { 
        user->second.isAdmin = isAdmin;
        return true;
    }
    else
        return false;
}         


/******************************************************************************
* Function:    verifyAdmin                 
*****************************************************************************/
bool Account::verifyAdmin(std::string userName ) {
       
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is in the map, return true if the user is an admin.  Otherwise return false. */
    if(user != _userMap.end()) 
        return user->second.isAdmin;
    else
        return false;
}                


/******************************************************************************
* Function:    deleteAccount                 
*****************************************************************************/
bool Account::deleteAccount(std::string userName ) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Find the user. */
    auto user = _userMap.find(userName);
    
    /* If the user is in the map, delete account and return true. */
    if(user != _userMap.end()) {
        _userMap.erase(userName);
        return true;
    } 
    else
        return false;    
} 

/******************************************************************************
* Function:    getNumberOfAccounts                 
*****************************************************************************/
int Account::getNumberOfAccounts( ) {
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_userMap);   
    
    /* Return the number of accounts in the account system. */
    return _userMap.size();
}


}}