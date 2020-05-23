#ifndef GMAIL_SERVER_H
#define GMAIL_SERVER_H

#include <sqlite3.h>


#define MAX_ALLOWED_USERS 5
#define MAX_USER_NAME_LENGTH 10;

class gmailServer {
	private:
		//variables
		int dbOpenStatus; //0 = good, 1 not so
		int dbCloseStatus;
		char choice;  //variable to hold user choice
		string userName;
		string passWord;
		string hashedPass;
		string sessionKey; //used for encryption and decryption

		//methods

		//gets user names and stores in gmail.db USER table
		void checkEndInput(string user_name);

	public:
		//methods

		//constructor
		gmailServer(); 

		//destructor
		~gmailServer();

		//SETTERS AND GETTERS METHODS BEGIN
		//----------------------------------


		//method to get value of private data member "choice";
		void setChoice();

		//method to get choice from user for either registering a user, sign-in, or quit 
		char getChoice();

		//method to get a user name during registration 
		void setUserName();

		//method to get user name
		string getUserName();

		//method to ser password for user name provided during registration
		void setPassword();

		//method to get a password provided during registration
		string getPassword();

		//method to create a Hash of a provided password
		void setHashPassword();

		//method to retrieve hashed password
		string getHashPassword();

		//method to generate session key for encryption and decryption of a message
		void setSessionKey();

		//method to retrieve session key
		string getSessionKey();
		
		//END OF SETTERS AND GETTERS METHODS....
		//------------------------------------


		//METHODS RELATED TO SQLITE3 DATABASE
		//-------------------------------------

		//method to connect to gmail server database
		int connectGmailDatabase();

		//method to close gmail server database
		void closeGmailDatabase();

		//END OF DATABASE METHODS
		//-------------------------------------


		//This method provides a means to register users
		//There is no limitation on how many users can register
		void userReg();

		// Once users are successfully registered, this method
		// allows to sign.  users are validated
		void userSignIn();

		//This method displays messges and counr
		void inbox();

		// This method encrypts the user message
		string genEncryption(string message, string passphrase);

		//This method decrypts user message
		string decryptMessage(string message, int mLength);

		// WriteMessage() methods allows signed in user to send a message to
		// anyother registred users in the mailbox
		// This method provides very rudimentary functionality
		// message content is encrypted
		void writeMessage();

		//This method allows to read inbox messages
		void readMessage();

		// This method is used witg the sqlite3()_exec() and called once for each row
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);

		// This method generates the hash
		string genHash(string pwd);


		sqlite3* DB;
		string sql;
		char *zErrMsg = 0;
};
#endif
