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
	date(unsigned int day, month month_name, unsigned int year);
	date(const date& other); 
	date();
	void changeDate(unsigned int day, month new_month, unsigned int year);
	unsigned int day() const;
	dates::month month() const;
	unsigned int year() const;

	date operator=(date arg);

private:
	unsigned int _day;
	dates::month _month_name; 
	unsigned int _year;
};

bool operator == (date& lhs, date& rhs);
bool operator <= (date& lhs, date& rhs);
bool operator >= (date& lhs, date& rhs);
bool operator <  (date& lhs, date& rhs);
bool operator >  (date& lhs, date& rhs);

bool is_leap_year(unsigned int year);
unsigned int days_in_month(dates::month month_name, unsigned int year);
std::string toString(dates::date& date_value);
}

