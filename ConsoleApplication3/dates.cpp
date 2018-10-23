#include "pch.h"
#include <string>
#include <vector>
#include <exception>
#include "dates.h"

using namespace std;

dates::date::date(int day, dates::month month_name, int year) {
	if (day > days_in_month(month_name, year))
		throw invalid_argument("Invalid day of month");
	if (day <= 0)
		throw invalid_argument("Day cannot be negative. Expected range is [1:31]");
	if (year < 0)
		throw invalid_argument("Year cannot be negative. Year should be > 1900");

	_day = day;
	_month_name = month_name;
	_year = year;
}
int dates::date::day()const 
{
	return _day;
}
dates::month dates::date::month()const 
{
	return _month_name;
}
int dates::date::year()const
{
	return _year;
}
/*
void dates::date::changeDate(int day, dates::month month_name, int year) {
	if (day <= 0 || day > days_in_month(month_name, year))
		throw invalid_argument("Invalid day of month");
	if (year < 1900)
		throw invalid_argument("Invalid year.");

	_day = day;
	_month_name = month_name;
	_year = year;
}
*/
void dates::date::changeDate( const dates::date& new_date)
{
	*this = new_date;
}

dates::date dates::date::operator=(dates::date arg)
{
	_day = arg.day();
	_month_name = arg.month();
	_year = arg.year();
	return *this;
}
bool dates::operator == (const date& lhs, const date& rhs)
{
	if (lhs.day() == rhs.day() && 
		lhs.month() == rhs.month() && 
		lhs.year() == rhs.year()) return true;
	else return false;
}
bool dates::operator <= (const date& lhs, const date& rhs) 
{
	if (lhs.year() < rhs.year()) return true;
	if (lhs.year() > rhs.year()) return false;
	else
		if (lhs.month() < rhs.month()) return true;
		else if (lhs.month() > rhs.month()) return false;
		else
			if (lhs.day() < lhs.day()) return true;
			if (lhs.day() > rhs.day()) return false;
	else return true;	// dates are equal
}
bool dates::operator >= (const date& lhs, const date& rhs) 
{
	if (lhs.year() < rhs.year()) return false;
	if (lhs.year() > rhs.year()) return true;
	else {
		if (lhs.month() < rhs.month()) return false;
		if (lhs.month() > rhs.month()) return true;
		else {
			if (lhs.day() < rhs.day()) return false;
			if (lhs.day() > rhs.day()) return true;
			else return true;	// dates are equal
		}
	}
}
bool dates::operator < (const date& lhs, const date& rhs) 
{
	if (lhs.year() < rhs.year()) return true;
	else if (lhs.year() > rhs.year()) return false;
	else {
		if (lhs.month() < rhs.month()) return true;
		if (lhs.month() > rhs.month()) return false;
		else {
			if (lhs.day() < rhs.day()) return true;
			if (lhs.day() > rhs.day()) return false;
			else return false;	// dates are equal
		}
	}
}
bool dates::operator>(const date& lhs, const date& rhs) 
{
	if (lhs.year() < rhs.year()) return false;
	else if (lhs.year() > rhs.year()) return true;
	else {
		if (lhs.month() < rhs.month()) return false;
		if (lhs.month() > rhs.month()) return true;
		else {
			if (lhs.day() < rhs.day()) return false;
			if (lhs.day() > rhs.day()) return true;
			else return false;	// dates are equal
		}
	}
}

bool dates::is_leap_year(int year) {
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
int dates::days_in_month(dates::month month_name, int year) {
	/*
	  Returns maximum days in month.
	 */
	vector<dates::month> months_with_31_days{ month::January, month::March, month::May,
												month::July, month::August, month::October,
													month::December };
	vector<dates::month> months_with_30_days{ month::April, month::June, month::September, month::November };

	if (find(months_with_31_days.begin(), months_with_31_days.end(), month_name) != months_with_31_days.end())
		return 31;
	else if (find(months_with_30_days.begin(), months_with_30_days.end(), month_name) != months_with_30_days.end())
		return 30;
	else
		if (is_leap_year(year)) return 29;
		else return 28;
}
string month_to_string(dates::month month)
{
	string month_name;
	switch (month) {
	case(dates::month::January): month_name = "January";  break;
	case(dates::month::February):month_name = "February"; break;
	case(dates::month::March):month_name = "March"; break;
	case(dates::month::April):month_name = "April"; break;
	case(dates::month::May):month_name = "May"; break;
	case(dates::month::June):month_name = "June"; break;
	case(dates::month::July):month_name = "July"; break;
	case(dates::month::August):month_name = "August"; break;
	case (dates::month::September): month_name = "September"; break;
	case (dates::month::October): month_name = "October"; break;
	case(dates::month::November):month_name = "November";break;
	case(dates::month::December):month_name = "December";break;
	}
	return month_name;
}
string dates::toString(dates::date & date_value) {
	/*
	 Converts dates::date to string formatted as "1 December 1979"
	*/
	auto day = date_value.day();
	auto year = date_value.year();
	auto month = date_value.month();

	string month_name{month_to_string(month)};
	
	auto day_string = std::to_string(day);
	auto year_string = std::to_string(year);
	string date_string{ day_string + " " + month_name + " " + year_string };
	return date_string;
}