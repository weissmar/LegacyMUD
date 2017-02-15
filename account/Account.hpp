/*!
  \file     Account.hpp
  \author   Keith Adkins
  \created  2/12/2017
  \modified 2/14/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the Account class.
*/

 
#ifndef LEGACYMUD_ACCOUNT_HPP
#define LEGACYMUD_ACCOUNT_HPP

#include <map>
#include <mutex>


namespace legacymud { namespace account {


/*!
  \brief Provides a set of functions to maintain and validate a player's LegacyMUD account.
  
  This class provides a set of functions to maintain and validate a player's LegacyMUD account.
*/
class Account {
    public:
    
        Account();
        
        /*!
          \brief Initialize the account sytem loading the data from disk into memory.
          
          \param[in]  fileName        name of text file for account data
          \pre none
          \post Returns true if successful.  Otherwise false.
        */
        bool initialize(std::string fileName);   

        /*!
          \brief Saves all account sytem data to disk.
          
          This function saves all account sytem data to disk.  It is suggested to do this at the same
          time game data is saved so that data between the account system and the game stay
          aligned.
          
          \pre none
          \post Returns true if successful.  Otherwise false.
        */
        bool saveToDisk(); 

        /*!
          \brief Confirms that a proposed username is unique.  
          
          This function confirms that a proposed username is unique.  This would be used during
          account registration to ensure that all player usernames in the account system are unique.
          
          \param[in]  username        player's account user name,  must be unique.
          \pre none
          \post Returns true if the username is unique.  Otherwise false.
        */
        bool uniqueUsername(std::string username);          

        /*!
          \brief Creates a new user account. 
          
          This function creates a new user account.  This account will not be created if the username is not unique.
          
          \param[in]  username        player's account user name,  must be unique.
          \param[in]  password        player's password
          \param[in]  isAdmin         boolean that indicates if this player is an admin or not 
          \param[in]  userId          ID used to link this user to player game data          
          \pre The game engine sure make sure userId is unique.
          \post Returns true if the account is successfully created.  Returns false if the user is already listed.
        */
        bool createAccount(std::string username, std::string password, bool isAdmin, int userId );

        /*!
          \brief Changes a user's password.
          
          \param[in]  username        player's account user name
          \param[in]  password        player's password                 
          \pre none
          \post Returns true if the password is successfully changed.  Otherwise false.
        */
        bool changePassword(std::string username, std::string password );        

        /*!
          \brief Verifies an account by username and password.
          
          \param[in]  username        player's account user name
          \param[in]  password        player's password                 
          \pre none
          \post Returns true if the password matches.  Otherwise false.
        */
        bool verifyAccount(std::string username, std::string password );      

        /*!
          \brief Sets a user's admin status. 
          
          \param[in]  username        player's account user name
          \param[in]  isAdmin         boolean that indicates if this player is an admin or not                  
          \pre none
          \post Returns true if the admin status is set.  Returns false if the user was not found.
        */
        bool setAdmin(std::string username, bool isAdmin );         

        /*!
          \brief Checks if a player is an admin.
          
          \param[in]  username        player's account user name.                 
          \pre none
          \post Returns true if the user is an admin.  Otherwise false.
        */
        bool verifyAdmin(std::string username );                
        
        /*!
          \brief Deletes an account. 
          
          \param[in]  username        player's account user name               
          \pre none
          \post Returns true if the account is deleted.  Returns false if the user is not found.
        */
        bool deleteAccount(std::string username ); 

        /*!
          \brief Gets a user's userId that links to game data. 

          \param[in]  username        player's account user name           
          \pre none
          \post Returns the userId that links a user to game data. If the username is invalid, a 0 is returned.
        */
        int getUserId(std::string username);
        
        /*!
          \brief Gets the number of accounts in the account system. 
         
          \pre none
          \post Returns a the number of accounts in the account system.
        */
        int getNumberOfAccounts();
        
    private:     
        std::string _fileName;                      // name of file where account data is stored 
        struct _UserInfo {                          // struct user info struct
            std::string password;                       // password of user
            bool isAdmin;                               // flag that indicates if this user is an admin            
            int userId;                                 // user id that links to user's game data
        };  
        std::map<std::string, _UserInfo> _userMap;  // map of users and account info, key is the username
        std::mutex _mu_userMap;                     // mutex used for the player echo map       
};

}}

#endif