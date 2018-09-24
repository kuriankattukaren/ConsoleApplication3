#include "pch.h"
#include <string>
#include <vector>
#include <exception>
#include "dates.h"

using namespace std;

bool dates::is_leap_year(unsigned int year) {
	//Checks if leap year.
	if ((year % 4) == 0)
		if ((year % 100) == 0)
			if ((year % 400) == 0)
				return true;
			else
				return false;
		else
			return true;
	else
		return false;
}

unsigned int dates::days_in_month(dates::month month_name, unsigned int year) {
	//Returns maximum days in month.
	vector<dates::month> months_with_31_days{ month::January, month::March, month::May, 
												month::July, month::August, month::October, 
													month::December };
	vector<dates::month> months_with_30_days{ month::April, month::June, month::September, month::November};

	if (find(months_with_31_days.begin(), months_with_31_days.end(), month_name) != months_with_31_days.end()) 
		return 31;
	else if (find(months_with_30_days.begin(), months_with_30_days.end(), month_name) != months_with_30_days.end())
		return 30;
	else
		if (is_leap_year(year)) return 29;
		else return 28;
}

dates::date::date(unsigned int day, dates::month month_name, unsigned int year) {
	if (day > days_in_month(month_name, year))
		throw invalid_argument("Invalid day of month");

	_day = day;
	_month_name = month_name;
	_year = year;
}

dates::date::date(const date & other){
	_day = other.day();
	_month_name = other.month();
	_year = other.year();
}

void dates::date::changeDate(unsigned int day, dates::month month_name, unsigned int year) {
	if (day <= 0 || day > days_in_month(month_name, year))
		throw invalid_argument("Invalid day of month");

	_day = day;
	_month_name = month_name;
	_year = year;
}

unsigned int dates::date::day()const {
	return _day;
}

dates::month dates::date::month()const {
	return _month_name;
}

unsigned int dates::date::year()const 
{
	return _year;
}

dates::date dates::date::operator=(dates::date arg)
{
	return arg;
}

bool dates::date::operator==(date arg) {
	if (_day == arg.day() && _month_name == arg.month() && _year == arg.year())
		return true;
	else
		return false;
}

bool dates::date::operator<=(date arg) {
	if (_year < arg.year())
		return true;
	if (_year > arg.year())
		return false;
	else
		if (_month_name < arg.month())
			return true;
	if (_month_name > arg.month())
		return false;
	else
		if (_day < arg.day())
			return true;
	if (_day > arg.day())
		return false;
	else
		return true;	// dates are equal

}

bool dates::date::operator>=(date arg) {
	if (_year < arg.year())
		return false;
	if (_year > arg.year())
		return true;
	else
		if (_month_name < arg.month())
			return false;
	if (_month_name > arg.month())
		return true;
	else
		if (_day < arg.day())
			return false;
	if (_day > arg.day())
		return true;
	else
		return true;	// dates are equal
}

bool dates::date::operator<(date arg) {
	if (_year < arg.year()) return true;
	else if (_year > arg.year()) return false;
	else {
		if (_month_name < arg.month()) return true;
		if (_month_name > arg.month()) return false;
		else {
			if (_day < arg.day()) return true;
			if (_day > arg.day()) return false;
			else return false;	// dates are equal
		}
	}
}

bool dates::date::operator>(date arg) {
	if (_year < arg.year())
		return false;
	if (_year > arg.year())
		return true;
	else
		if (_month_name < arg.month())
			return false;
	if (_month_name > arg.month())
		return true;
	else
		if (_day < arg.day())
			return false;
	if (_day > arg.day())
		return true;
	else
		return false;	// dates are equal
}
