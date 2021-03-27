#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

string getFirstWord(string text);
string getLastWord(string text);
string extractWord(string& text);
bool isUppercase(string text);
string makeUppercase(string text);
bool hasMultipleExclamations(string text);
bool isGibberishWord(string text);
double countUppercase(string text);
int flaggedWords(string text);

int main() {

	const double capitalParameter = .50;
	const double capitalCheck = .90;
	int spam_emails = 0;
	int real_emails = 0;
	char choice = 'y';

	while (choice == 'y') {

		string subject_line = "";
		string body = "";
		string one_line = "";
		int spam_text = 0;

		cout << "Enter the subject line of the email: ";
		getline(cin, subject_line);

		if (getFirstWord(subject_line) != "") { //checks if subject_line has a letter
			if (countUppercase(subject_line) > capitalCheck) { //checks if subject_line has > 90% full capitalized words
				spam_text = spam_text + 30;
			}
		}

		string last = getLastWord(subject_line);
		if (isGibberishWord(last) == true) { //checks if there are more than 3 consecutive consonants in the last word of the string
			spam_text = spam_text + 40;
		}

		if (hasMultipleExclamations(subject_line) == true) { //checks for 3 consecutive '!'
			spam_text = spam_text + 20;
		}

		cout << "Enter the body of the email.  Press Enter on an empty line to finish.";
		cout << endl;

		while (getline(cin, body)) { //allows for the user to finish typing the body on an empty line
			if (body == "") {
				break;
			}
			one_line += body; //converts the body into one line
			one_line += " ";
		}

		if (getFirstWord(one_line) != "") {
			if (countUppercase(one_line) > capitalParameter) { //checks if body has > 50% full capitalized words
				spam_text = spam_text + 40;
			}
		}

		spam_text = spam_text + flaggedWords(one_line);  //checks string for key words and increments spam

		if (spam_text > 100) {
			cout << "This email is classified as spam, because its spam score is " << spam_text << ".";
			spam_emails++;
			cout << endl;
		}
		else {
			cout << "This email is classified as legitimate, because its spam score is " << spam_text << ".";
			real_emails++;
			cout << endl;
		}
		cout << "Would you like to classify another email (y or n)? ";
		cin >> choice;
		choice = tolower(choice);
		while (choice != 'y' && choice != 'n') {
			cout << "Please enter y or n.";
			cout << endl << "Would you like to classify another email (y or n)? ";
			cin >> choice;
		}
		cin.ignore(1000, '\n'); //allows for the subject line to be inputted in by the user
		//cin.ignore is necessary due to the cin in choice and the getline that retrieves the subject_line in the beginning of the while loop
	}
	if (choice == 'n') {
		cout << endl;
		cout << "Number of spam messages: " << spam_emails << endl;
		cout << "Number of legitimate messages: " << real_emails << endl;
		return 1;
	}
}

//function finds and returns first word (adjacent letters) in string while ignoring digits and characters
//digit or character in the middle of the first word will cut off the word and end before the digit/character
string getFirstWord(string text) {
	int start;
	int end = 0;
	for (int i = 0; i < text.size(); i++) {
		if (isalpha(text[i])) { //checks for alphabet so that characters and numbers are ignored
			start = i;
			for (int j = i; j < text.size() - 1; j++) {
				if (!isalpha(text[j])) {
					return text.substr(start, end);
				}
				end++;
			}
			end = text.size(); //in case string does not contain digits or characters, end the size of the string
			return text.substr(start, end);
		}
	}
	text = ""; //if string does not contain alphabet at all, creates empty string
	return text;
}

//function returns last word while ignoring anything besides letters after the last word
string getLastWord(string text) {
	int end;
	int start;
	int count;
	for (int i = text.size() - 1; i >= 0; i--) {
		if (isalpha(text[i])) {
			end = i;
			for (int j = i; j >= 0; j--) {
				if (!isalpha(text[j])) { //
					start = j + 1; //since j is position, add 1 so that start represents number of letters
					count = end - start;
					count++;
					return text.substr(start, count);
				}
			}
			start = 0; //if only alphabet in whole string
			count = end - start;
			count++;
			return text.substr(start, count);
		}
	}
	text = ""; //if string is empty or does not contain any letters
	return text;
}

//function returns the first word while removing it from the original string
string extractWord(string& text) {
	string firstWord;
	int start;
	int end = 0;
	for (int i = 0; i < text.size(); i++) {
		if (isalpha(text[i])) {
			start = i;
			for (int j = i; j != text.size(); j++) {
				if (!isalpha(text[j])) {
					firstWord = text.substr(start, end);
					text = text.substr(start + end, text.size() - end); //excludes the firstword
					return firstWord;
				}
				end++;
			}
			end = text.size(); //sets end equal to size if the string only contains letters past i
			firstWord = text.substr(start, end);
			text = "";
			return firstWord;
		}
	}
	text = ""; //if no letters, string is empty
	firstWord = "";
	return firstWord;
}

//function returns true (1) if string is entirely uppercase; otherwise, returns false (0)
bool isUppercase(string text) {
	for (int i = 0; i < text.size(); i++) {
		if (islower(text[i])) { //not necessary to check for anything except if that position in the string is lowercase
			return false;
		}
	}
	return true;
}

//function returns string entirely capitalized
//digits and characters are unaffected
string makeUppercase(string text) {
	for (int i = 0; i < text.size(); i++) {
		if (isalpha(text[i])) {
			text[i] = toupper(text[i]);
		}
	}
	return text;
}

//function returns true if three consecutive '!' are in the string
bool hasMultipleExclamations(string text) {
	for (int i = 0; i < text.size(); i++) {
		int count = 0;
		while (text[i] == '!') { //while position equals '!' i and count is incremented; if i != '!' function goes back to for loop and sets count to 0
			count++;
			i++;
			if (count >= 3) {
				return true;
			}
		}
	}
	return false;
}

//function returns true if more than 3 consonants are found adjacent to one another 
bool isGibberishWord(string text) {
	string copy = text;
	int consonants = 0;
	copy = makeUppercase(copy); //changes string to entirely capitalized letters; will not matter whether input was lowercase or uppercase
	for (int i = 0; i < text.size(); i++) {
		if (isalpha(copy[i])) { //if position is a letter and a consonant (A E I O U)
			if (copy[i] != 'A' && copy[i] != 'E' && copy[i] != 'I' && copy[i] != 'O' && copy[i] != 'U' && copy[i] != 'Y') {
				consonants++;
				if (consonants > 3) {
					return true;
				}
			}
			else { //makes sure the consonants are consecutive
				consonants = 0;
			}
		}
		else if (!isalpha(copy[i])) { //makes sure the consonants are consecutive
			consonants = 0;
		}
	}
	return false;
}

//function returns a double necessary to check spam conditions
//the double depends on the number of capitalized words in the string
double countUppercase(string text) {
	double countUpper = 0;
	double countLower = 0;
	double totalWords = 0;
	string copy = text;
	if (isUppercase(copy) == false) { //makes sure not all of the string is capitalized
		while (copy != "") { //while the string isn't empty
			string firstWord = extractWord(copy); //first Word in string is saved and string has first word removed from it
			if (isUppercase(firstWord) == true) {
				countUpper++;
			}
			else {
				countLower++;
			}
		}
		totalWords = countUpper / (countUpper + countLower);
		return totalWords;
	}
	else {
		totalWords = 1.0; //1.0 represents 100%; all of string is capitalized
		return totalWords;
	}
}

//function returns an integer to increment spam score if string contains key words
int flaggedWords(string text) {
	int spam = 0;
	string copy = text;
	while (copy != "") { //while string isn't empty
		string firstWord = extractWord(copy); //saves first word while removing it from string
		firstWord = makeUppercase(firstWord); //makes string uppercase so that input does not matter whether lower or uppercase
		if (firstWord == "BUY" || firstWord == "CHEAP" || firstWord == "CLICK" || firstWord == "DIPLOMA" || firstWord == "ENLARGE" || firstWord == "FREE" || firstWord == "LONELY" || firstWord == "MONEY" || firstWord == "NOW" || firstWord == "OFFER" || firstWord == "ONLY" || firstWord == "PILLS" || firstWord == "SEX") {
			spam = spam + 5;
		}
	}
	return spam;
}
