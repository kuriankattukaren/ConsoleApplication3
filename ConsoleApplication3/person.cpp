#include "pch.h"
#include "person.h"
#include <string>
#include <exception>
#include <vector>

using namespace std;

person::person(string name) {
	auto names = splitname(name);
	if (names.size() == 2) {
		_second_name = names[1];
		_first_name = names[0];
	}
	else throw invalid_argument("Should have two names.");
}

string person::name() {
	string fullname = _first_name + " " + _second_name;
	return fullname;
}
string person::firstName() {
	return _first_name;
}
string person::secondName() {
	return _second_name;
}

person person::operator=(person& other){
	_first_name = other.firstName();
	_second_name = other.secondName();
	return *this;
}

bool operator == (person& lhs, person& rhs)
{
	if (lhs.firstName() == rhs.firstName() && (lhs.secondName() == rhs.secondName()))
		return true;
	else
		return false;
}
bool operator <= (person& lhs, person& rhs) 
{
	if (lhs.firstName() < rhs.firstName()) return true;
	else if (lhs.firstName() == rhs.firstName()) {
		if (lhs.secondName() < rhs.secondName()) return true;
		else if (lhs.secondName() == rhs.secondName()) return true;
		else return false;
	}
	else
		return false;
}
bool operator >= (person& lhs, person& rhs)
{
	if (lhs.firstName() > rhs.firstName()) return true;
	else if (lhs.firstName() == rhs.firstName()) {
		if (lhs.secondName() > rhs.secondName()) return true;
		else if (lhs.secondName() == rhs.secondName()) return true;
		else return false;
		}
	else return false;
}
bool operator <  (person& lhs, person& rhs)
{
	if (lhs.firstName() < rhs.firstName()) return true;
	else if (lhs.firstName() == rhs.firstName()) {
		if (lhs.secondName() < rhs.secondName()) return true;
		else return false;
	}
	else return false;
}
bool operator >  (person& lhs, person& rhs)
{
	if (lhs.firstName() > rhs.firstName()) return true;
	else if (lhs.firstName() == rhs.firstName()) {
		if (lhs.secondName() > rhs.secondName()) return true;
		else return false;
	}
	else return false;
}

string remove_leading_whitespace(string name) {
	const char whitespace{ ' ' };
	int pos = name.find_first_not_of(whitespace);

	if (pos != string::npos) return name.substr(pos);
	else return name;
}
string remove_trailing_whitespace(string name) {
	const char whitespace{ ' ' };
	int pos = name.find_last_not_of(whitespace);
	if (pos != string::npos) return name.substr(0, pos + 1);
	else return name;
}
vector<string> splitname(string name) {
	/*
	Splits a fullname containing both a first name and a second name into two names.
	For eg: Kurian Kattukaren is a full name.

	Precondition:
		name represents a string formated as "Kurian Kattukaren"
	Parameter:
		name:
	Returns:
		vector<string> where each element is a substring.
	*/
	vector<string> substrings;
	auto edited_name{ remove_trailing_whitespace(remove_leading_whitespace(name)) };

	auto whitespace_pos = edited_name.find(' ');
	if (whitespace_pos != string::npos) {
		auto first_name = edited_name.substr(0, whitespace_pos);
		auto second_name = remove_leading_whitespace(edited_name.substr(whitespace_pos));
		substrings.push_back(first_name);
		substrings.push_back(second_name);
	}
	else throw invalid_argument("Name cannot be split.");

	return substrings;
}