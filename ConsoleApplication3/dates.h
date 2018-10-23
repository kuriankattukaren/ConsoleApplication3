#pragma once
#include <string>

namespace dates{

enum class month {
	January=1, February, March, April,
		May, June, July, August,
			September, October, November, December
	};

class date {
public:
	date(int day, dates::month month_name, int year);
	
	int day() const;
	dates::month month() const;
	int year() const;
	
//	void changeDate(int day, dates::month new_month, int year);
	void changeDate(const dates::date& new_date);
	date operator=( date arg);

private:
	int _day;
	dates::month _month_name; 
	int _year;
};

bool operator == (const date& lhs, const date& rhs);
bool operator <= (const date& lhs, const date& rhs);
bool operator >= (const date& lhs, const date& rhs);
bool operator <  (const date& lhs, const date& rhs);
bool operator >  (const date& lhs, const date& rhs);

bool is_leap_year(int year);
int days_in_month(dates::month month_name, int year);
std::string toString(dates::date& date_value);
}

