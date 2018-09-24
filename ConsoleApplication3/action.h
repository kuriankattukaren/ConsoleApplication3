#pragma once
#include <string>
#include "dates.h"
#include "person.h"

enum class status{completed, ongoing, notstarted, hold};
enum class priority{Low, Medium, High};

class Action {
	string _desc;
	dates::date _dueDate, _startDate, today;
	status _currentStatus;
	person _owner;

public:
	Action(string act, dates::date start_date, dates::date due_date, person owner);
	
	dates::date dueDate();
	void changeDueDate(dates::date new_due_date);
	
	dates::date startDate();
	void changeStartDate(dates::date new_start_date);

	status actionStatus();

	string description();
	string changeDescription(string new_description);
};