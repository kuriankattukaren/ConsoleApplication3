#include "pch.h"
#include <string>
#include <vector>
#include <exception>
#include "dates.h"

using namespace std;

bool dates::is_leap_year(unsigned int year) {
	/*
	  Checks whether year is a leap year.
	*/
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
	/*
	  Returns maximum days in month.
	 */
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
string dates::toString(dates::date & date_value){
	/*
	 Converts dates::date to string formatted as "1 December 1979"
	*/
	auto day = date_value.day();
	auto year = date_value.year();
	auto month = date_value.month();
	string month_name;

	switch (month) {
		case(dates::month::January):
			month_name = "January" ;
		case(dates::month::February):
			month_name = "February";
		case(dates::month::March):
			month_name = "March" ;
		case(dates::month::April):
			month_name = "April";
		case(dates::month::May):
			month_name = "May";
		case(dates::month::June) :
			month_name = "June";
		case(dates::month::July):
			month_name = "July";
		case(dates::month::August):
			month_name = "August";
		case (dates::month::September):
			month_name = "September";
		case (dates::month::October):
			month_name = "October";
		case(dates::month::November):
			month_name = "November";
		case(dates::month::December):
			month_name = "December";
	}
	auto day_string = std::to_string(day);
	auto year_string = std::to_string(year);
	string date_string{day_string + " " + month_name + " " + year_string};
	return date_string;
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

dates::date::date(){
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
	//auto new_date = dates::date{ arg.day(), arg.month(), arg.year() };
	_day = arg.day();
	_month_name = arg.month();
	_year = arg.year();
	return *this;
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
