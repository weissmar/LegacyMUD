/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  2/12/2017
 * Last Modified: 2/14/2017
 * Course:        CS467, Winter 2017
 * Filename:      Account_Test.cpp
 *
 * Overview:
 *     Unit test for the Account class.
 ************************************************************************/

#include <Account.hpp>
#include <string>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

namespace {

std::string fileName = "legacy_mud_accounts.txt";       // txt file for account data

/* Testing with no accounts in the account system. */
TEST(AccountTest, NoAccounts) {

    legacymud::account::Account acc(fileName);

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are not any accounts."; 

    /* Save the account to disk.  Clearing any account data stored in the file. */
    EXPECT_TRUE(acc.saveToDisk() ) 
        << "Expect true since nothing should prevent data from being written to disk."; 

    /* Initialize loading all accounts from disk. */
    EXPECT_TRUE(acc.initialize() ) 
        << "Expect true since nothing should prevent data from being loaded from disk.";          
    
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are no accounts.";         
    
    /* Check if a username is unique. */
    EXPECT_TRUE(acc.uniqueUsername("Steve") )
        << "Expect true since this username is not the account system.";    

    /* Verify an account. */
    EXPECT_FALSE(acc.verifyAccount("Linda", "somepassword") ) 
        << "Expect false since this account is not in the system. ";  

    /* Verify admin status of a user. */
    EXPECT_FALSE(acc.verifyAdmin("Linda" ) ) 
        << "Expect false since this user is not in the system."; 
        
    /* Set the admin of a user. */
    EXPECT_FALSE(acc.setAdmin("Linda", false ) ) 
        << "Expect false since this user is not in the system."; 

    /* Changing password. */
    EXPECT_FALSE(acc.changePassword("China", "newPassword" ) ) 
        << "Expect false since this user is not in the system.";   

    /* Get UserId. */
    EXPECT_EQ(0, acc.getUserId("China" ) ) 
        << "Expect 0 since this user is not in the system.";        

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are no accounts.";

    remove(fileName.c_str());   // delete the account system file
}

/* Testing with one created account in the account system. */
TEST(AccountTest, OneAccount) {
    legacymud::account::Account acc(fileName);

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are not any accounts."; 
        
    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Linda", "validpassword", true, 1 ) ) 
        << "Expect true since this is a valid account creation.";

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(1, acc.getNumberOfAccounts() ) 
        << "Expect 1 since there is one account.";   
    
    /* Save the account to disk. */
    EXPECT_TRUE(acc.saveToDisk() ) 
        << "Expect true since nothing should prevent data from being written to disk.";        
    
    /* Initialize loading all accounts from disk. */
    EXPECT_TRUE(acc.initialize() ) 
        << "Expect true since nothing should prevent data from being loaded from disk.";  

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(1, acc.getNumberOfAccounts() ) 
        << "Expect 1 since there is one account.";         
        
    /* Try to create second account with the same username. */
    EXPECT_FALSE(acc.createAccount("Linda", "1234", false, 2 ) ) 
    << "Expect false since this username is not unique.";
    
    /* Check if a username is unique.  Testing with a username this is already in the account system. */
    EXPECT_FALSE(acc.uniqueUsername("Linda") )
        << "Expect false since this username is in the account system.";  

    /* Check if a username is unique. Testing with a username not in the account system.*/
    EXPECT_TRUE(acc.uniqueUsername("George") ) 
        << "Expect true since this username is not in the account system.";   

    /* Verify an account. Testing with a valid username and password.*/
    EXPECT_TRUE(acc.verifyAccount("Linda", "validpassword") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify an account. Testing with a valid username and invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Linda", "1234") ) 
        << "Expect false since the password is invalid.";  
 
    /* Verify an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.verifyAccount("Sally", "password4") ) 
        << "Expect false since this username is invalid.";  

    /* Verify admin status of a user. Testing with a valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Linda" ) ) 
        << "Expect true this user is an admin."; 
        
    /* Verify admin status of a user. Testing with invalid username. */
    EXPECT_FALSE(acc.verifyAdmin("Mike" ) ) 
        << "Expect false since this username is invalid."; 
        
    /* Set the admin of a user. Testing with a valid username. Setting admin to false. */
    EXPECT_TRUE(acc.setAdmin("Linda", false ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_FALSE(acc.verifyAdmin("Linda" ) )
        << "Expect false Linda is not an admin."; 

    /* Set the admin of a user. Testing with an invalid username. */
    EXPECT_FALSE(acc.setAdmin("George", false ) ) 
        << "Expect false since this username is invalid."; 

    /* Set the admin of a user. Testing with a valid username. Setting admin to true. */
    EXPECT_TRUE(acc.setAdmin("Linda", true ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Linda" ) ) 
        << "Expect true since this user is now an admin."; 

    /* Get userId. Testing with a valid username. */
    EXPECT_EQ(1, acc.getUserId("Linda" ) ) 
        << "Expect 1 since this is the correct user id."; 
        
    /* Get userId. Testing with invalid username. */
    EXPECT_EQ(0, acc.getUserId("Mike" ) ) 
        << "Expect 0 since this username is invalid.";        
        
    /* Changing password. Testing with valid username. */
    EXPECT_TRUE(acc.changePassword("Linda", "newPassword" ) ) 
        << "Expect true since this user a valid account.";         

    /* Verify account of new password. Testing with a valid password.*/
    EXPECT_TRUE(acc.verifyAccount("Linda", "newPassword") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify account of new password. Testing with old now invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Linda", "validpassword") ) 
        << "Expect false since the password is invalid.";        

    /* Changing password. Testing with invalid username. */
    EXPECT_FALSE(acc.changePassword("George", "newPassword" ) ) 
        << "Expect false since George is an invalid username."; 

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(1, acc.getNumberOfAccounts() ) 
        << "Expect 1 since there is only one account.";

    /* Delete an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.deleteAccount("Betty") ) 
        << "Expect false since the username is valid.";    
    
    /* Delete an account. Testing with a valid username.*/
    EXPECT_TRUE(acc.deleteAccount("Linda") ) 
        << "Expect true since the username is valid.";

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there is are no longer any accounts.";  

    remove(fileName.c_str());   // delete the account system file        
}


/* Testing with two created accounts in the account system. */
TEST(AccountTest, TwoAccounts) {
    legacymud::account::Account acc(fileName);

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are not any accounts."; 
        
    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Linda", "validpassword1", true, 1 ) ) 
        << "Expect true since this is a valid account creation.";
        
    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Kris", "validpassword2", false, 2 ) ) 
        << "Expect true since this is a valid account creation.";        

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(2, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are two accounts.";   
    
    /* Save the account to disk. */
    EXPECT_TRUE(acc.saveToDisk() ) 
        << "Expect true since nothing should prevent data from being written to disk.";        
    
    /* Initialize loading all accounts from disk. */
    EXPECT_TRUE(acc.initialize() ) 
        << "Expect true since nothing should prevent data from being loaded from disk.";  

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(2, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are two accounts.";         
        
    /* Try to create second account with the same username. */
    EXPECT_FALSE(acc.createAccount("Kris", "1234", false, 2 ) ) 
    << "Expect false since this username is not unique.";
    
    /* Check if a username is unique.  Testing with a username this is already in the account system. */
    EXPECT_FALSE(acc.uniqueUsername("Kris") )
        << "Expect false since this username is in the account system.";  

    /* Check if a username is unique. Testing with a username not in the account system.*/
    EXPECT_TRUE(acc.uniqueUsername("George") ) 
        << "Expect true since this username is not in the account system.";   

    /* Verify an account. Testing with a valid username and password.*/
    EXPECT_TRUE(acc.verifyAccount("Linda", "validpassword1") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify an account. Testing with a valid username and invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Linda", "1234") ) 
        << "Expect false since the password is invalid."; 

    /* Verify an account. Testing with a valid username and password.*/
    EXPECT_TRUE(acc.verifyAccount("Kris", "validpassword2") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify an account. Testing with a valid username and invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Kris", "invalidpassword") ) 
        << "Expect false since the password is invalid.";         
 
    /* Verify an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.verifyAccount("Sally", "password4") ) 
        << "Expect false since this username is invalid.";  

    /* Verify admin status of a user. Testing with a valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Linda" ) ) 
        << "Expect true this user is an admin."; 
        
    /* Verify admin status of a user. Testing with a valid username. */
    EXPECT_FALSE(acc.verifyAdmin("Kris" ) ) 
        << "Expect false since this user is not an admin.";         
        
    /* Verify admin status of a user. Testing with invalid username. */
    EXPECT_FALSE(acc.verifyAdmin("Mike" ) ) 
        << "Expect false since this username is invalid."; 
        
    /* Set the admin of a user. Testing with a valid username. Setting admin to false. */
    EXPECT_TRUE(acc.setAdmin("Linda", false ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_FALSE(acc.verifyAdmin("Linda" ) )
        << "Expect false since this user is not an admin."; 

    /* Set the admin of a user. Testing with a valid username. Setting admin to false. */
    EXPECT_TRUE(acc.setAdmin("Kris", true ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Kris" ) )
        << "Expect true since this user is now an admin.";
        
    /* Set the admin of a user. Testing with an invalid username. */
    EXPECT_FALSE(acc.setAdmin("George", false ) ) 
        << "Expect false since this username is invalid.";   

    /* Get userId. Testing with a valid username. */
    EXPECT_EQ(1, acc.getUserId("Linda" ) ) 
        << "Expect 1 since this is the correct user id."; 
        
    /* Get userId. Testing with a valid username. */
    EXPECT_EQ(2, acc.getUserId("Kris" ) ) 
        << "Expect 2 since this is the correct user id.";         
        
    /* Get userId. Testing with invalid username. */
    EXPECT_EQ(0, acc.getUserId("Mike" ) ) 
        << "Expect 0 since this username is invalid."; 
                
    /* Changing password. Testing with valid username. */
    EXPECT_TRUE(acc.changePassword("Linda", "newPassword1" ) ) 
        << "Expect true since this user a valid account.";         

    /* Changing password. Testing with valid username. */
    EXPECT_TRUE(acc.changePassword("Kris", "newPassword2" ) ) 
        << "Expect true since this user a valid account.";          
        
    /* Verify account of new password. Testing with a valid password.*/
    EXPECT_TRUE(acc.verifyAccount("Linda", "newPassword1") ) 
        << "Expect true since the username and password are valid.";  
    
    /* Verify account of new password. Testing with a valid password.*/
    EXPECT_TRUE(acc.verifyAccount("Kris", "newPassword2") ) 
        << "Expect true since the username and password are valid.";     

    /* Verify account of new password. Testing with old now invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Linda", "validpassword1") ) 
        << "Expect false since the password is invalid.";   

    /* Verify account of new password. Testing with old now invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Kris", "validpassword2") ) 
        << "Expect false since the password is invalid.";          

    /* Changing password. Testing with invalid username. */
    EXPECT_FALSE(acc.changePassword("George", "newPassword" ) ) 
        << "Expect false since George is an invalid username.";        

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(2, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are two accounts.";

    /* Delete an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.deleteAccount("Betty") ) 
        << "Expect false since the username is valid.";    
    
    /* Delete an account. Testing with a valid username.*/
    EXPECT_TRUE(acc.deleteAccount("Linda") ) 
        << "Expect true since the username is valid.";       

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(1, acc.getNumberOfAccounts() ) 
        << "Expect 1 since there is now one account.";    

    /* Verify account of deleted user. */
    EXPECT_FALSE(acc.verifyAccount("Linda", "newPassword1") ) 
        << "Expect false since this username is deleted from the system.";  

    /* Verify account of remaining user. */
    EXPECT_TRUE(acc.verifyAccount("Kris", "newPassword2") ) 
        << "Expect true since the username and password are valid.";   

    /* Delete an account. Testing with a valid username.*/
    EXPECT_TRUE(acc.deleteAccount("Kris") ) 
        << "Expect true since the username is valid.";  

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there no accounts.";         
        
    remove(fileName.c_str());   // delete the account system file        
}


/* Testing with Mutliple accounts in the account system. */
TEST(AccountTest, MultipleAccounts) {
    legacymud::account::Account acc(fileName);

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(0, acc.getNumberOfAccounts() ) 
        << "Expect 0 since there are not any accounts."; 
        
    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Linda", "validpassword1", true, 1 ) ) 
        << "Expect true since this is a valid account creation.";
        
    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Kris", "validpassword2", false, 2 ) ) 
        << "Expect true since this is a valid account creation.";   

    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Sue", "validpassword3", false, 3 ) ) 
        << "Expect true since this is a valid account creation."; 

    /* Create a valid account. */
    EXPECT_TRUE(acc.createAccount("Mike", "validpassword4", false, 4 ) ) 
        << "Expect true since this is a valid account creation.";         

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(4, acc.getNumberOfAccounts() ) 
        << "Expect 4 since there are four accounts.";   
    
    /* Save the account to disk. */
    EXPECT_TRUE(acc.saveToDisk() ) 
        << "Expect true since nothing should prevent data from being written to disk.";        
    
    /* Initialize loading all accounts from disk. */
    EXPECT_TRUE(acc.initialize() ) 
        << "Expect true since nothing should prevent data from being loaded from disk.";  

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(4, acc.getNumberOfAccounts() ) 
        << "Expect 4 since there are four accounts.";         
        
    /* Try to create second account with the same username. */
    EXPECT_FALSE(acc.createAccount("Mike", "1234", false, 2 ) ) 
    << "Expect false since this username is not unique.";
    
    /* Check if a username is unique.  Testing with a username this is already in the account system. */
    EXPECT_FALSE(acc.uniqueUsername("Kris") )
        << "Expect false since this username is in the account system.";  

    /* Check if a username is unique. Testing with a username not in the account system.*/
    EXPECT_TRUE(acc.uniqueUsername("George") ) 
        << "Expect true since this username is not in the account system.";   

    /* Verify an account. Testing with a valid username and password.*/
    EXPECT_TRUE(acc.verifyAccount("Mike", "validpassword4") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify an account. Testing with a valid username and invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Mike", "1234") ) 
        << "Expect false since the password is invalid."; 

    /* Verify an account. Testing with a valid username and password.*/
    EXPECT_TRUE(acc.verifyAccount("Sue", "validpassword3") ) 
        << "Expect true since the username and password are valid.";  

    /* Verify an account. Testing with a valid username and invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Sue", "invalidpassword") ) 
        << "Expect false since the password is invalid.";         
 
    /* Verify an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.verifyAccount("Sally", "password4") ) 
        << "Expect false since this username is invalid.";  

    /* Verify admin status of a user. Testing with a valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Linda" ) ) 
        << "Expect true this user is an admin."; 
        
    /* Verify admin status of a user. Testing with a valid username. */
    EXPECT_FALSE(acc.verifyAdmin("Mike" ) ) 
        << "Expect false since this user is not an admin.";         
        
    /* Verify admin status of a user. Testing with invalid username. */
    EXPECT_FALSE(acc.verifyAdmin("Larry" ) ) 
        << "Expect false since this username is invalid."; 
        
    /* Set the admin of a user. Testing with a valid username. Setting admin to false. */
    EXPECT_TRUE(acc.setAdmin("Linda", false ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_FALSE(acc.verifyAdmin("Linda" ) )
        << "Expect false since this user is not an admin."; 

    /* Set the admin of a user. Testing with a valid username. Setting admin to false. */
    EXPECT_TRUE(acc.setAdmin("Mike", true ) ) 
        << "Expect true since this is a valid username."; 
        
    /* Verify admin status of a user. Testing with valid username. */
    EXPECT_TRUE(acc.verifyAdmin("Mike" ) )
        << "Expect true since this user is now an admin.";
        
    /* Set the admin of a user. Testing with an invalid username. */
    EXPECT_FALSE(acc.setAdmin("Larry", false ) ) 
        << "Expect false since this username is invalid.";   
        
    /* Get userId. Testing with a valid username. */
    EXPECT_EQ(4, acc.getUserId("Mike" ) ) 
        << "Expect 4 since this is the correct user id."; 
        
    /* Get userId. Testing with a valid username. */
    EXPECT_EQ(3, acc.getUserId("Sue" ) ) 
        << "Expect 3 since this is the correct user id.";         
        
    /* Get userId. Testing with invalid username. */
    EXPECT_EQ(0, acc.getUserId("Larry" ) ) 
        << "Expect -1 since this username is invalid.";         
     
    /* Changing password. Testing with valid username. */
    EXPECT_TRUE(acc.changePassword("Mike", "newPassword4" ) ) 
        << "Expect true since this user a valid account.";         

    /* Changing password. Testing with valid username. */
    EXPECT_TRUE(acc.changePassword("Sue", "newPassword3" ) ) 
        << "Expect true since this user a valid account.";          
        
    /* Verify account of new password. Testing with a valid password.*/
    EXPECT_TRUE(acc.verifyAccount("Mike", "newPassword4") ) 
        << "Expect true since the username and password are valid.";  
    
    /* Verify account of new password. Testing with a valid password.*/
    EXPECT_TRUE(acc.verifyAccount("Sue", "newPassword3") ) 
        << "Expect true since the username and password are valid.";     

    /* Verify account of new password. Testing with old now invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Mike", "validpassword4") ) 
        << "Expect false since the password is invalid.";   

    /* Verify account of new password. Testing with old now invalid password.*/
    EXPECT_FALSE(acc.verifyAccount("Sue", "validpassword3") ) 
        << "Expect false since the password is invalid.";          

    /* Changing password. Testing with invalid username. */
    EXPECT_FALSE(acc.changePassword("George", "newPassword" ) ) 
        << "Expect false since George is an invalid username.";        

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(4, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are two accounts.";

    /* Delete an account. Testing with an invalid username.*/
    EXPECT_FALSE(acc.deleteAccount("Betty") ) 
        << "Expect false since the username is valid.";    
    
    /* Delete an account. Testing with a valid username.*/
    EXPECT_TRUE(acc.deleteAccount("Mike") ) 
        << "Expect true since the username is valid.";       

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(3, acc.getNumberOfAccounts() ) 
        << "Expect 3 since there are three accounts.";    

    /* Verify account of deleted user. */
    EXPECT_FALSE(acc.verifyAccount("Mike", "newPassword4") ) 
        << "Expect false since this username is deleted from the system.";  

    /* Verify account of other user. */
    EXPECT_TRUE(acc.verifyAccount("Sue", "newPassword3") ) 
        << "Expect true since the username and password are valid.";   

    /* Delete an account. Testing with a valid username.*/
    EXPECT_TRUE(acc.deleteAccount("Sue") ) 
        << "Expect true since the username is valid.";  

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(2, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are no accounts."; 

    /* Verify account of deleted user. */
    EXPECT_FALSE(acc.verifyAccount("Sue", "newPassword3") ) 
        << "Expect false since this user is no longer in the account system.";           
 
    /* Verify account of remaining user. */
    EXPECT_TRUE(acc.verifyAccount("Linda", "validpassword1") ) 
        << "Expect true since since this user is still in the account system."; 
        
    /* Verify account of remaining user. */
    EXPECT_TRUE(acc.verifyAccount("Kris", "validpassword2") ) 
        << "Expect true since since this user is still in the account system.";         

    /* Save the account data to disk. */
    EXPECT_TRUE(acc.saveToDisk() ) 
        << "Expect true since nothing should prevent data from being written to disk.";        
    
    /* Initialize loading all accounts from disk. */
    EXPECT_TRUE(acc.initialize() ) 
        << "Expect true since nothing should prevent data from being loaded from disk.";         

    /* Get the number of accounts in the account system. */
    EXPECT_EQ(2, acc.getNumberOfAccounts() ) 
        << "Expect 2 since there are two accounts.";  

    /* Verify account of remaining user. */
    EXPECT_TRUE(acc.verifyAccount("Linda", "validpassword1") ) 
        << "Expect true since since this user is still in the account system.";  

    /* Verify account of remaining user. */
    EXPECT_TRUE(acc.verifyAccount("Kris", "validpassword2") ) 
        << "Expect true since since this user is still in the account system."; 

    remove(fileName.c_str());   // delete the account system file        
}

}
