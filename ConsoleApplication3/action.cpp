#include "pch.h"
#include "action.h"

Action::Action(string act, dates::date start_date, dates::date due_date, person owner) {
	if (start_date > due_date) { throw invalid_argument("Start date earlier than due date.");}
	else if (start_date == due_date) { _due = true; }
	else { _due = false; }
	
	_desc = act;
	_startDate = start_date;
	_dueDate = due_date;
}

string Action::description() {
	return _desc;
}
string Action::changeDescription(string new_description) {
	_desc = new_description;
	return _desc;
}

dates::date Action::dueDate(){
	return _dueDate;
}
void Action::changeDueDate(dates::date new_due_date) {
	_dueDate = new_due_date;
	if (_dueDate >= today) { _due = true; }
	else { _due = false; }
}
bool Action::due(){
	return 
_due;
}

dates::date Action::startDate() {
	return _startDate;
}
dates::date Action::changeStartDate(dates::date new_start_date){
	return dates::date();
}




