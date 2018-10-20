#pragma once
#include <string>
#include <vector>
#include <chrono>

using namespace std;


class person {
	/*
	 Represents a person. A person is represented by a first name 
	 and a second name.

	 Invariant:
		Requires name to be a string containing two names: a first name and a second name,
		with each separated whitespace. For eg: "Kurian Kattukaren"
	 
	 Parameters:
		name: String object with a first name and second name. The
		      first and second names should be separated by whitespace.
			  Leading and trailing whitespaces are permitted.
	 
	 Raises:
		std::invalid_arg exception if invalid name is provided on construction.
     
	 Modifies:
	    name by removing all leading and trailing whitespaces. The first and second 
		names are separated by single white space.
	*/

public:
	person(string name);
	
	string name() const;
	string firstName() const;
	string secondName() const;

	person& operator=(person& name);
	
private:
	string _first_name;
	string _second_name;
};

bool operator == (const person& lhs, const person& rhs);
bool operator <= (const person& lhs, const person& rhs);
bool operator >= (const person& lhs, const person& rhs);
bool operator <  (const person& lhs, const person& rhs);
bool operator >  (const person& lhs, const person& rhs);

vector<string> splitname(string str);
string remove_leading_whitespace(string name);
string remove_trailing_whitespace(string name);