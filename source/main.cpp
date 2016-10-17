/*
AdvancedInput
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	This tutorial illustrates the concept of Controll Handler Function,
	Standard Input/Output Streams,  String streams, std::exceptions class.
	The goal for this example is to ensure that every possible input does not
	lead to failure.
*/

#include <iostream>
#include <string>
#include <sstream>		//   https://msdn.microsoft.com/en-us/library/kb1es779.aspx
#include <exception>	//	 https://msdn.microsoft.com/en-us/library/w326ectb.aspx
#include <windows.h>	//  
using namespace std;

int getInt(string);		// declare function

const int MIN = 1;		// initialize a constant int type variable MIN, assign 1 to it
const int MAX = 9;		// initialize a constant int type variable MAX, assign 9 to it

// Registering a Control Handler Function
// http://msdn.microsoft.com/en-us/library/ms685049%28VS.85%29.aspx:
BOOL CtrlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType) {
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		Beep(750, 300);	// static void Beep (int frequence, int duration);
		return(TRUE);
	}
	return false;
}

void main(void) {
	// http://msdn.microsoft.com/en-us/library/ms685049%28VS.85%29.aspx
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
		cout << getInt("Please enter a value between 1 and 9: ") << endl;
	}

	system("pause");	// add
}

// originally adapted from http://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046996179&id=1043284385 
// and http://www.parashift.com/c++-faq/convert-string-to-num.html
// if you don't do the "!myStream.get(c)" (check for extra characters), 
//      return void* (http://www.parashift.com/c++-faq/istream-and-while.html)
//      (the void* explanation will also clarify why myStream>>i actually returns something)
//	(websites above seemed to have gone defunct since we originally wrote this)

// declare function stringToInt which returns to a bool type, 
// set two parameters: a reference to const string type and a reference to int type
bool stringToInt(const string &s, int &i) {
	istringstream myStream(s);					// input stream class to operate on strings; put s to class myStream
												// istringstream: http://www.cplusplus.com/reference/sstream/istringstream/
	char c;										// declare a character c
	return (myStream >> i) && !myStream.get(c);	// "myStream>>i": look into the stream and check if there is a number to get, if so store into i;
												// "myStream.get(c)": read the number character from myStream and assign it to c, if succeed, return true
												// if there is a number in the string, stringToInt return true
}


// declare function getInt which returns to a int type
// set parameter: string type variable prompt
int getInt(string prompt) {
	bool stopGettingVals = false;				// initialize the bool type variable stopGettingVals, puts false to it
	int val = MIN - 1;							// initialize the int type variable val, puts value of MIN-1 to it

	// keep prompting for a good value:
	while (!stopGettingVals) {

		// try to get a decent value:
		try {

			// set up input:
			string input;						// declare a string type variable input
			cout << prompt;						// output promote
			getline(cin, input);				// extract characters from what is input, and put into the input variable

			// if user tries to exit window (DOS), prevent it!
			// need to "reset" the input stream, though:
			if (cin.eof()) {					// check if it is the end of the file

				cin.clear();					// if true, clears the error flag
				throw new exception();			
			}

			// check if "non-convertable", else convert to int:	
			if (!stringToInt(input, val))		
				throw new exception();
			else if (input.find('.') != string::npos)
				throw new exception();
			else
				val = atoi(input.c_str());

			// is input [MIN,MAX]?
			if (val >= MIN && val <= MAX)
				stopGettingVals = true;			// if so, get out of while loop
			else
				cout << "Out of range!" << endl;// if not, output "Out of range!"

			// if stopGettingVals now true, we return
		}

		// something went wrong, likely with conversion or input:
		catch (...) {
			cout << "Wrong kind of input!" << endl;
		}
	}
		return val;
}

