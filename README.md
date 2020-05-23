# gmailServer
to compile: g++ main.cpp gmail_server.cpp -l sqlite3 -lgcrypt sha256.cpp

to run: ./a.out

I have provided sample ouputrun in ouputrun.txt

I have provided qa report in qareport.txt to see if there are any vulnerabilities.

I used cppcheck and the results are pretty good

I was not able to run rosecheckers since connection to sei seems down

I also noticed I am using strcat() to formulate the SQL query rather than strncat().

I am not able to thoroughly test the code.

gmail.db also contains some sample users from my testing
