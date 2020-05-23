//This program provides rudimetary
// email functionality


//Credits:
//refactor some of the codes from internet


#include <iostream>
#include <gcrypt.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdlib>
#include "sha256.h"
using namespace std;

#include "gmail_server.h"

//Constructor for class gmailServer
gmailServer::gmailServer()
{
	dbOpenStatus = 1;
	dbCloseStatus = 1;
	choice = 'a'; //dummy initialization
	userName = "mrunal"; // dummy userName initialization
	passWord = "mrunal"; //dummy pasword initialization
	hashedPass = "mrunal"; //dummy hashed password initiTION
	sessionKey = "mrunal"; //dummy initialization

} // constructor for class gmailServer

gmailServer::~gmailServer()
{

} //destructor for class gmailServer

// This method opens the gmail sever database(gmail.db)
// and returns the status
int gmailServer::connectGmailDatabase()
{

    	dbOpenStatus = sqlite3_open("gmail.db", &DB); //opens the gmail database
	
	if (dbOpenStatus == SQLITE_OK) {
        	//cout << "Gmail server database opened successfully!" << endl;
		return dbOpenStatus;
	} //if
	else {
        	cerr << "Error open DB.  exiting the program " << sqlite3_errmsg(DB) << endl;
        	exit(2);
	} //else

} // connectGmailDatabase()

// This method closes the gmail server database(gmail.db)
// and returns the status
void gmailServer::closeGmailDatabase()
{
	dbCloseStatus = sqlite3_close(DB);
	if (dbCloseStatus == SQLITE_OK) {
		//cout << "Gmail sever database closed successfully!" << endl;
	} //if
	else {
        	cerr << "Error closing DB.  exiting the program " << sqlite3_errmsg(DB) << endl;
        	exit(2);
	} //else

} //closeGmailDatabase()

void gmailServer::setChoice()
{
	choiceInput:
	cout << "enter 'r' for register new user, 's' to sign-in for existing user, 'q' to quit immediately" << endl;
    	cin >> choice;
	if (cin.fail()) {
		cout << "Invalid Choice. Choose valid input." << endl;
		cin.clear();
		cin.ignore();
		goto choiceInput;
	} //if
	else if ((choice != 's') && (choice !='r') && (choice != 'q')) {
		cout << "Invalid Choice. Choose valid input." << endl;
		cin.clear();
		cin.ignore();
		goto choiceInput;
	} //if

} //setChoice()

char gmailServer::getChoice()
{
	return choice;

} //getChoice()

void gmailServer::setUserName()
{
	userNameInput:
	cout << "Enter user name ino more than 10 characters and enter \"stop \" without quotes to end input" <<endl;
	cin >> userName;
	if (cin.fail()) {
		cout << "Invalid User Name " << endl;
		cin.clear();
		cin.ignore();
		goto userNameInput;
	} //if
	if (userName.size() > 10 )  {
		cout << "invalid input length in user name " << endl;
		cin.clear();
		cin.ignore();
		goto userNameInput;
	} //if

	//checkEndInput(userName);


} //setUserName()

string gmailServer::getUserName()
{
	return userName;

} //getUserName();

void gmailServer::setPassword()
{
	passwordInput:
	cout << "Enter password no more than 10 characters" <<endl;
	cin >> passWord;
	if (cin.fail()) {
		cout << "Invalid Password " << endl;
		cin.clear();
		cin.ignore();
		goto passwordInput;
	} //if
	if (passWord.size() > 10 )  {
		cout << "invalid password length." << endl;
		cin.clear();
		cin.ignore();
		goto passwordInput;
	} //if

} //setPassword();

string gmailServer::getPassword()
{
	return passWord;

} //getPassword()

void gmailServer:: setHashPassword()
{
	hashedPass = genHash(getPassword());
	
} //setHashPassword()

string gmailServer::getHashPassword()
{
	return hashedPass;

} //getHashPassword()

void gmailServer::setSessionKey()
{
	/*
	unsigned int randval;
    	FILE *f;
    	f = fopen("/dev/urandom", "r");
    	fread(&randval, sizeof(randval), 1, f);
    	fclose(f);
	*/
	unsigned long key = 560085637709877;
	sessionKey = to_string(key);

} //setSessionKey()

string gmailServer::getSessionKey()
{
	return sessionKey;

} //getSessionKey()


// This method registers uniqe users
void gmailServer::userReg() 
{
	//need to get user name and password
	//check if user already exists
	//if not insert user name and hash of password into gmail.db, USERS table
	// display a messahe that user was successfully registered
	

	startReg:
	setUserName();
	string user_name = getUserName();
	
    	sqlite3_stmt *selectStmt;
    	char selectQuery[] = "SELECT * FROM USERS";


    	sqlite3_prepare(DB, selectQuery, strlen(selectQuery)+1, &selectStmt, NULL);
    	while (1) {
        	int s;
        	s = sqlite3_step(selectStmt);
        	if (s == SQLITE_ROW) {
            		string dbUser = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,1)));
            		if(getUserName() == dbUser){
                		cout << "USER exists already" << endl;
        			sqlite3_finalize(selectStmt);
				goto startReg;
                		break;
            		} //if	
            
        	} //if
        	else if (s == SQLITE_DONE) {
			break;
        	} //else	
    	} //while

        sqlite3_step(selectStmt);


	setPassword();
	setHashPassword();
	string hashP = getHashPassword();

	sql = "INSERT INTO USERS (NAME, HASHPASS) values('" + user_name + "', '" + hashP + "')";

	int rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &zErrMsg);
	if (rc == SQLITE_OK) {
		cout << "registered user: "<<user_name<< " successfully." << endl;
		;
	} //
	else
		cout << "something went wrong with the sqlite3_exec statement above." << endl;

        sqlite3_finalize(selectStmt);
	//setChoice();

} //userReg()

void gmailServer::checkEndInput(string user_name)
{
	if (user_name == "stop" || user_name == "STOP" || user_name == "Stop"){
		cout << "Registration Ended." << endl;
		//setChoice();
	} //if

} // check_end_input()

string gmailServer::genHash(string pwd)
{
	string hashed_pwd;
  	// create a new hashing object
  	SHA256 sha256;

  	// hashing an std::string
	hashed_pwd = sha256(pwd);

	return hashed_pwd;


} // genHasg()


void gmailServer::userSignIn()
{

    	signStart:
	bool is_signIn_success = false;
    	int rc;
    
    
	input:
    	cout << "Please Enter Username: " << endl;
	cin >> userName;
	if (cin.fail()) {
		cout << "Invalid data " << endl;
		cin.clear();
		cin.ignore();
		goto input;
	} //if

	password:
    	cout << "Please Enter Password: " << endl;
	cin >> passWord;
	if (cin.fail()) {
		cout << "Invalid data " << endl;
		cin.clear();
		cin.ignore();
		goto password;
	} //if
    	hashedPass = genHash(passWord);

    	sqlite3_stmt *selectStmt;
    	char selectQuery[] = "SELECT * FROM USERS";


    	sqlite3_prepare(DB, selectQuery, strlen(selectQuery)+1, &selectStmt, NULL);
    	while (1) {
        	int s;
        	s = sqlite3_step(selectStmt);
        	if (s == SQLITE_ROW) {
            		string dbUser = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,1)));
            		string dbPass = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,2)));
            		if(userName==dbUser && hashedPass==dbPass){
                		cout << "Sign in successfuil" << endl;
				cout << endl << endl;
				is_signIn_success = true;
                		break;
            		} //if	
            
        	} //if
        	else if (s == SQLITE_DONE) {
            		cout <<"Invalid Username/Password" << endl;
            		sqlite3_finalize(selectStmt);
            		goto signStart;
        	} //else	

		if (is_signIn_success)
			break;
    	} //while
    	sqlite3_finalize(selectStmt);
	//setChoice();

} // userSignIn()


int gmailServer::callback(void *NotUsed, int argc, char **argv, char **azColName){

    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    for(int i = 0; i < argc; i++) {

        // Show column name, value, and newline
        cout << azColName[i] << ": " << argv[i] << endl;

    }

    // Insert a newline
    cout << endl;

    // Return successful
    return 0;
} // callback()

void gmailServer::inbox()
{

	int total = 0;
	const unsigned char* sender;

 	sqlite3_stmt *selectStmt;
	char str1[1000];
	char g[] = "';";
	strcpy(str1, "select count(receiver) from messages where receiver = '");
	strcat(str1, userName.c_str());
	strcat(str1, g);
	if (strlen(str1) > 1000) {
		cout << "query length greather than 1000" << endl;
		exit(2);
	} //if
	
    	sqlite3_prepare(DB, str1, strlen(str1)+1, &selectStmt, NULL);

	while (1) {
        	int s;
        	s = sqlite3_step(selectStmt);
        	if (s == SQLITE_ROW) {
            	total = sqlite3_column_int(selectStmt, 0);
            	//cout << "You have " << total << " total message(s). " << endl;
        	}
        	else if (s == SQLITE_DONE) {
            		break;

        	}
    	}
    	sqlite3_finalize(selectStmt);

	char str2[1000];
	strcpy(str2, "select sender from messages where receiver = '");
	strcat(str2, userName.c_str());
	strcat(str2, g);
	if (strlen(str2) > 1000) {
		cout << "query length greather than 1000" << endl;
		exit(2);
	} //if

	cout << "MESSAGES FROM:" << endl;
    	sqlite3_prepare(DB, str2, strlen(str2)+1, &selectStmt, NULL);

	while(1) {
        	int s;
        	s = sqlite3_step(selectStmt);
        	if (s == SQLITE_ROW) {
			//const unsigned char* sender;
			sender = sqlite3_column_text (selectStmt, 0);
			cout << sender << endl;

		} // if
		else if (s == SQLITE_DONE) {
			break;
		} //else`
	} //while
	//sql = "select NAME from USERS";

	//sqlite3_exec(DB, sql.c_str(), callback, 0, &zErrMsg);

    	sqlite3_finalize(selectStmt);
	cout <<endl << endl;
	cout << " User "<<userName << "\'s"<< " INBOX" << endl;
	cout << "------------------------------------" << endl;
	cout << "INBOX FOLDER: " << total << " messages" <<endl;
	cout <<endl<<endl;

	//sqlite3_close(DB);

} // inbox()

void gmailServer::readMessage()
{
   	sqlite3_stmt *selectStmt;
    	string readMessageFrom;
	string key = getSessionKey();

    	readMessageFrom:
	getline(cin,readMessageFrom); 
	if (cin.fail()) {
		cout << "Invalid message " << endl;
		cin.clear();
		cin.ignore();
		goto readMessageFrom;
	} //if

	cout << "Select a SENDER from the list to read message"<<endl;

	getline(cin,readMessageFrom); 
	if (cin.fail()) {
		cout << "Invalid message " << endl;
		cin.clear();
		cin.ignore();
		goto readMessageFrom;
	} //if

	// formulatiing SQL query
    	char str1[1000];
    	char a[] = "' and sender = '";
    	char g[] = "';'";
    	strcpy(str1,"select message,passphrase,mlength from messages where receiver = '");
    	strcat(str1,userName.c_str());
    	strcat(str1,a);
    	strcat(str1,readMessageFrom.c_str());
    	strcat(str1,g);
	if (strlen(str1) > 1000) {
		cout << "query length greather than 1000" << endl;
		exit(2);
	} //if

    	sqlite3_prepare(DB, str1, strlen(str1)+1, &selectStmt, NULL);

	while (1) {
        	int s;
        	s = sqlite3_step(selectStmt);
        	if (s == SQLITE_ROW) {
            		string message;
            		message = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 0)));
            		string passphrase = string(reinterpret_cast<const char*>(sqlite3_column_text(selectStmt,1)));
            		int mLength;
            		mLength = sqlite3_column_int(selectStmt, 2);
            		if(passphrase == genHash(key)){ //checking if session key matches 
                		cout << "Message from " << readMessageFrom << ":" << endl;
				string decrypt;
				decrypt = decryptMessage(message,mLength);
				cout << decrypt << endl;
            			} //if
			else{
                		cout << "Session Key does not match" << endl;
				exit(2);
            		} //else
        	} //if
        	else if (s == SQLITE_DONE) {
            		break;
        	} //else if
    	} //while
    	sqlite3_finalize(selectStmt);

} //readMessage()

string gmailServer::genEncryption(string message, string passphrase)
{
    	string encrypted = "";
    	string temp;
	temp = min(message, passphrase);
    	int min;
    	min = temp.size();
    
    	for (int i = 0; i < message.size(); i++){
        	encrypted += message[i] ^ ((passphrase[i % min]) );
    	} //for
	cout << "Encrypted Message = " << encrypted << endl;
	return encrypted;

} //genEncryption()

string gmailServer::decryptMessage(string message,int mLength)
{
        string decrypted = "";
	string key = getSessionKey();
        int lower;
        lower = min((int)(key.size()), mLength);
        for (int i = 0; i < mLength; i++){
                  decrypted += message[i] ^ ((key[i % lower]) );
        } //for

        //cout << decrypted << endl;
	return decrypted;

} //decryptMessage()

//This method provides rudimenrary functionality of
//sending a message to registred users
void gmailServer::writeMessage()
{
	
    	string sendMessageTo;
    	string message;
    	string passphrase;

    	getline(cin,sendMessageTo);
    	cout << "To: ";
    	getline(cin,sendMessageTo);

	message:
    	cout << "Message: ";
    	getline(cin,message);
	if (cin.fail()) {
		cout << "Invalid message " << endl;
		cin.clear();
		cin.ignore();
		goto message;
	} //if
	if (message.length() > 1000) {
		cout << "allowed message length exceeded." << endl;
		goto message;
	} //if

	//setSessionKey();
	passphrase = getSessionKey(); //get a session key
    	string hashPassphrase = genHash(passphrase); //hash of a session key

	string encrypt;
	encrypt = genEncryption(message, passphrase);

	//getting ready to format SQL query
    	int mLength = message.length();
    	char mLen[1000]; //check for the boundary
	if (strlen(mLen) > 1000) {
		cout << "mLen size ezceeded." << endl;
		exit(2);
	} //if
    	sprintf(mLen, "%d", mLength); //fix this as well

    	char c[] = "','";
    	char g[] = "');";
    	char a1[] = "',";
    	char a2[] = ",'";

	// format SQL query
	char str1[1000];
    	strcpy(str1,"insert into messages(sender,receiver,message,mlength,passphrase) values('");
    	strcat(str1,userName.c_str());
    	strcat(str1,c);
    	strcat(str1,sendMessageTo.c_str());
    	strcat(str1,c);
    	strcat(str1,encrypt.c_str());
    	strcat(str1,a1);
    	strcat(str1,mLen);
    	strcat(str1,a2);
    	strcat(str1,hashPassphrase.c_str());
    	strcat(str1,g);
    
   	int rc;
    	rc = sqlite3_exec(DB, str1, callback, 0, NULL);
	if( rc != SQLITE_OK ){
   		 fprintf(stderr, "SQL error: %s\n", "Message full for this user.");
      		 sqlite3_free(zErrMsg);
    	} //if
	else{
      		 cout << "Message Sent" << endl;
    	} 

} //writeMessage()


