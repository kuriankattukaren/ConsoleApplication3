#pragma once
#include <string>
#include <vector>
#include <chrono>

using namespace std;


class person {
	/*
	 Represents a person who has a first name and a second name.

	 Precondition:
		Requires name to be a string containing two names: a first name and a second name, with each separated
		whitespace. For eg: "Kurian Kattukaren"
	 
	 Parameter:
		name: string
	 
	 Raises:
		std::invalid_arg exception if invalid name is provided on construction.

	 Returns:
		Name with all leading and trailing whitespaces removed and with first and second name
		separated by a single whitespace.
	*/

public:
	
	person(string name);
	string name();
	string firstName();
	string secondName();

	bool operator==(person name);
	bool operator<=(person name);
	bool operator>=(person name);
	bool operator<(person name);
	bool operator>(person name);

private:
	string _first_name;
	string _second_name;
};

vector<string> splitname(string str);
string remove_leading_whitespace(string name);
string remove_trailing_whitespace(string name);