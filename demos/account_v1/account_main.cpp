/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  2/12/2017
 * Last Modified: 2/14/2017
 * Course:        CS467, Winter 2017
 * Filename:      account_main.cpp
 *
 * Overview:
 *     This main function for for local testing of the account class.
 ************************************************************************/

#include <string>
#include <iostream> 
#include <Account.hpp>

int main() {
    std::string fileName = "legacy_mud_accounts.txt";
    legacymud::account::Account acc(fileName);
    std::string userName = "Keith";
    std::string password = "1234";
    bool isAdmin = true;
    int userId = 1;
    
    std::cout << "Welcome to account system testing." << std::endl;

    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    // Testing creating account, verifying account, verifying admin, change password, 
    // delete an account, unique user name.
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    
    // unique username
    if (acc.uniqueUsername(userName) ) 
        std::cout << userName << " is unique." << std::endl;
    else
        std::cout << userName << " is not unique." << std::endl;     
    
    // create account
    if (acc.createAccount(userName, password, isAdmin, userId ) ) 
        std::cout << "Account created." << std::endl;       
    
    // unique username
    if (acc.uniqueUsername(userName) ) 
        std::cout << userName << " is unique." << std::endl;
    else
        std::cout << userName << " is not unique." << std::endl;       
    
    // authorize account wrong password
    if (acc.verifyAccount(userName, "abc" )) 
        std::cout << userName << " is authorized." << std::endl;
    else
        std::cout << userName << " is NOT authorized." << std::endl;
    
    // authorize account correct password
    if (acc.verifyAccount(userName, password )) 
        std::cout << userName << " is authorized." << std::endl;
    else
        std::cout << userName << " is NOT authorized." << std::endl;  

    // authorize account wrong username
    if (acc.verifyAccount("Joe", password )) 
        std::cout << "Joe" << " is authorized." << std::endl;
    else
        std::cout << "Joe" << " is NOT authorized." << std::endl;   

    // authorize admin
    if (acc.verifyAdmin("Keith" )) 
        std::cout << "Keith" << " is an admin." << std::endl;
    else
        std::cout << "Keith" << " is NOT an admin." << std::endl;    

    // set admin
    acc.setAdmin("Keith", false ); 

    // authorize admin
    if (acc.verifyAdmin("Keith" )) 
        std::cout << "Keith" << " is an admin." << std::endl;
    else
        std::cout << "Keith" << " is NOT an admin." << std::endl;    

    // change password
    password = "9876";
    if (acc.changePassword("Keith", password )) 
        std::cout << "Keith's password is changed." << std::endl;
    else
        std::cout << "Keith's password is not changed." << std::endl;     

    // verify account correct password
    if (acc.verifyAccount(userName, password )) 
        std::cout << userName << " is authorized." << std::endl;
    else
        std::cout << userName << " is NOT authorized." << std::endl;      

    // delete
    if (acc.deleteAccount(userName )) 
        std::cout << userName << " account is deleted." << std::endl;
    else
        std::cout << userName << " account is not deleted." << std::endl;  

    // delete
    if (acc.deleteAccount("Joe" )) 
        std::cout << "Joe account is deleted." << std::endl;
    else
        std::cout << "Joe account is not deleted since it doesnt exist." << std::endl; 

    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    // Saving and loading data.
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    acc.createAccount("Keith", "myPassword", true, 1 );
    acc.createAccount("Sam", "12345", false, 2 );
    acc.createAccount("Linda", "butter", false, 3 );
    acc.saveToDisk();
    acc.initialize();
    
       // verify account correct password
    if (acc.verifyAccount("Linda", "butter" )) 
        std::cout << "Linda" << " is authorized." << std::endl;
    else
        std::cout << "Linda" << " is NOT authorized." << std::endl;
    
    
    return 0;
}
