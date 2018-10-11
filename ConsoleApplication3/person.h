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
		name: string eg: "Kurian Kattukaren"
	 
	 Raises:
		std::invalid_arg exception if invalid name is provided on construction.
     
	 Modifies:
	    Removes all leading and trailing whitespaces. The first and second names are separated by single white space.

	 Returns:
		Name with all leading and trailing whitespaces removed and with first and second name
		separated by a single whitespace.
	*/

public:
	person(string name);
	person(const person& other);
	string name();
	string firstName();
	string secondName();

	person operator=(person name);
	bool operator<=(person name);
	bool operator>=(person name);
	bool operator<(person name);
	bool operator>(person name);

private:
	string _first_name;
	string _second_name;
};

bool operator==(person &lhs, person& rhs);

vector<string> splitname(string str);
string remove_leading_whitespace(string name);
string remove_trailing_whitespace(string name);