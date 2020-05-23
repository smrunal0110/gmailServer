#include <iostream>
#include <gcrypt.h>
#include <cstdlib>
#include <sqlite3.h>
using namespace std;

#include "gmail_server.h"

int main(int argc, char *argv[]) {
	gmailServer gs; // gs is an object of class gmailServer
	gs.connectGmailDatabase();
	gs.setSessionKey();
	gs.setChoice();

	while(1) {
	if (gs.getChoice() == 'r'){
		cout << "Registration Begins Now. " << endl;
		gs.userReg();
	} // if

	else if (gs.getChoice() == 's') {
		cout << "Sign-In Begins Now. " << endl;
		gs.userSignIn();
		gs.inbox();
		char inboxChoice;
		while(1) {
		cout << "Type 'r' to read message, 'w' to write a message, 'q' to quit." << endl;
		cin >> inboxChoice;
		if (inboxChoice == 'r')
			gs.readMessage();
		else if (inboxChoice == 'w')
			gs.writeMessage();
		else if (inboxChoice == 'q')
			//exit(2);
			break;
		} //innerwhile
	} //else if
	else if (gs.getChoice() == 'q'){
		cout << "User chose to exit immediatly. " << endl;
		exit(2);
	}

	gs.setChoice();
	} //top while
	gs.closeGmailDatabase();
	
	/*
	//gs.inbox();
	char choice;
	*/

	return 0;
} //main

		

