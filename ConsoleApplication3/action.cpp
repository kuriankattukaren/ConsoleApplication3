#include "pch.h"
#include "dates.h"
#include "action.h"
#include <chrono>
#include <ctime>
#include "../../DateTime/include/date/date.h"
#include "../../DateTime/include/date/tz.h"


dates::month month_from_int(int mon) {
	if (mon < 1 || mon > 12) {
		throw invalid_argument("Invalid month supplied.");
	}
	else {
		switch (mon) {
		case(1):
			return dates::month::January;
		case(2):
			return dates::month::February;
		case(3):
			return dates::month::March;
		case(4):
			return dates::month::April;
		case(5):
			return dates::month::May;
		case(6):
			return dates::month::June;
		case(7):
			return dates::month::July;
		case(8):
			return dates::month::August;
		case(9):
			return dates::month::September;
		case(10):
			return dates::month::October;
		case(11):
			return dates::month::November;
		case(12):
			return dates::month::December;
		default:
			throw invalid_argument("Invalid month supplied.");
		}
	}
}
dates::date today() {
		/*Local time should be used and not system time. System time is GMT
		  time and GMT can lag behind local time. If the program is executed 
		  in Australia, if system time is used there will be a delay in 
		  detecting due date as system time is GMT time which is behind local 
		  Australia time.
		 */
		auto rt = date::make_zoned(date::current_zone()->name(), std::chrono::system_clock::now());
		auto localtime = rt.get_local_time();
		
		date::year_month_day today = date::year_month_day{ date::floor<date::days>(localtime) };
		auto day = static_cast<int>(static_cast<unsigned int>(today.day()));
		auto month_integer = static_cast<int>(static_cast<unsigned int>(today.month()));
		auto month = month_from_int(month_integer);
		auto year = int(today.year());
		return dates::date{ day, month, year };
	
}

//Comparison functions.
bool compare_due_dates(const Action & lhs, const Action & rhs)
{
	return lhs.date_due() < rhs.date_due();
}
bool compare_identified_dates(const Action & lhs, const Action & rhs)
{
	return lhs.date_identified() < rhs.date_identified();
}
bool compare_priority(const Action & lhs, const Action & rhs)
{
	if (lhs.priority() == rhs.priority()) return false;
	else if (lhs.priority() == Priority::High) return false;
	else if (lhs.priority() == Priority::Medium) {
		if (rhs.priority() == Priority::High) return true;
		else return false;
	}
	else return true; //lhs.getPriority() == priority::Low
}
bool compare_owners(const Action & lhs, const Action & rhs)
{
	return lhs.owner() < rhs.owner();
}
bool compare_descriptions(const Action & lhs, const Action & rhs)
{
	return lhs.description() < rhs.description();
}
bool compare_status(const Action & lhs, const Action & rhs)
{
	auto lhs_status = lhs.status();
	auto rhs_status = rhs.status();

	if (lhs_status == rhs_status) return false;
	else if (lhs_status == Status::notstarted) return true;
	else if (lhs_status == Status::hold) {
		if (rhs_status == Status::notstarted) return false;
		else return true;
	}
	else if (lhs_status == Status::ongoing) {
		if (rhs_status == Status::notstarted || rhs_status == Status::hold)
			return false;
		else return true;
	}
	else return false; //lhs_status == status::completed
}


Action::Action(string act, dates::date& identified, dates::date& due, person owner):
	_desc{act}, _identified_date{identified}, _dueDate{due}, _owner{owner}, _today{today()}
{
	if (_identified_date > _dueDate) 
		throw invalid_argument("Date action was identified should be earlier than due date.");
	if (_identified_date > _today)
		throw invalid_argument("Date action was identified cannot be later than today.");
	_priority = Priority::Medium;
	_currentStatus = Status::notstarted;
}

bool Action::due() {
	_today = today();
	//Completed tasks are not due.
	if (_currentStatus == Status::completed) return false;
	if (_dueDate > _today) return false;
	else return true;
}
Status Action::status() const
{
	return _currentStatus;
}
Priority Action::priority() const 
{
	return _priority;
}
string Action::description() const 
{
	return _desc;
}
person Action::owner() const 
{
	return _owner;
}
dates::date Action::date_due() const 
{
	return _dueDate;
}
dates::date Action::date_identified() const 
{
	return _identified_date;
}
std::optional<dates::date>Action::date_started() const
{
	return _startDate;
}
std::optional<dates::date> Action::date_completed() const
{
	return _date_completed;
}
std::optional<dates::date> Action::date_onhold() const
{
	return _on_hold_date;
}
void Action::set_status(Status new_status, dates::date status_date) {
	auto previous_status = _currentStatus;
	if (new_status == Status::notstarted)
	{
		_on_hold_date = std::nullopt;
		_date_completed = std::nullopt;
		_startDate = std::nullopt;
		_currentStatus = new_status;
		try { this->set_date_identified(status_date); }
		catch (invalid_argument& e) { _currentStatus = previous_status; throw e; }
	}
	else if (new_status == Status::ongoing)
	{
		_currentStatus = new_status;
		try {this->set_date_started(status_date);}
		catch (invalid_argument& e) { _currentStatus = previous_status; throw e; }
		
		_on_hold_date = std::nullopt;
		_date_completed = std::nullopt;
	}
	else if (new_status == Status::hold)
	{
		if (!_startDate) {
			throw invalid_argument("Cannot set action to hold before starting the task.");
		}
		_currentStatus = new_status;
		try {this->set_date_onhold(status_date);}
		catch (invalid_argument& e) { _currentStatus = previous_status; throw e; }
			
		_date_completed = std::nullopt;
	}
	else if (new_status == Status::completed){
		if (!_startDate) {
			throw invalid_argument("Cannot set action to complete before starting the task.");
		}
		_currentStatus = new_status;
		try {this->set_date_completed(status_date);}
		catch (invalid_argument& e) { _currentStatus = previous_status; throw e; }

		_on_hold_date = std::nullopt;
	}
}
void Action::set_priority(Priority new_priority)
{
	_priority = new_priority;
}
void Action::set_description(string new_description)
{
	_desc = new_description;
}
void Action::set_owner(person new_owner)
{
	_owner = new_owner;
}
void Action::set_date_due(dates::date new_due_date) {
	if (new_due_date < _identified_date)
		throw invalid_argument("Due date cannot be earlier than identified date.");
	_dueDate = new_due_date;
}
void Action::set_date_identified(dates::date new_date)
{
	if (new_date > _dueDate)
		throw invalid_argument("Date action was identified should be earlier than due date.");
	else if(new_date > _today)
		throw invalid_argument("Date action was identified cannot be later than today.");
	
	switch (_currentStatus) {
		case (Status::notstarted): break;
		case (Status::ongoing):
			if (new_date > _startDate.value()) throw invalid_argument("Date action was idenfitied should be before start date.");
			break;
		case (Status::hold):
			if (new_date > _startDate.value()) throw invalid_argument("Date action was idenfitied should be before start date.");
			if (new_date > _on_hold_date.value()) throw invalid_argument("Date action was identified should be before.");
			break;
		case (Status::completed):
			if (new_date > _startDate.value()) throw invalid_argument("Date action was idenfitied should be before start date.");
			if (new_date > _date_completed.value()) throw invalid_argument("Date action was identified should be before completed date.");
			break;
		default: {}
	}
	_identified_date = new_date;
	}
void Action::set_date_started(dates::date new_start_date)
{
	if (new_start_date < _identified_date) 
		throw invalid_argument("Start date cannot be earlier than identified date");
	else if (new_start_date > today())
		throw invalid_argument("Action cannot be started in the future.");
	switch (_currentStatus) {
			case(Status::notstarted): {
				throw invalid_argument("Action cannot be started when the action is started.");
			}
			case (Status::completed): {
				if (new_start_date >= _date_completed.value())
					throw invalid_argument("Action cannot start after completion date.");
				break;
			}
			case (Status::hold): {
				if (new_start_date >= _on_hold_date.value())
					throw std::invalid_argument("Action cannot start after on hold date.");
				break;
			}
			default: {}
	}
	_startDate = dates::date{ new_start_date };
}
void Action::set_date_onhold(dates::date new_date)
{
	if(_currentStatus != Status::hold)
	{
		throw invalid_argument("Cannot set hold date when action not on hold.");
	}
	else 
	{
		if (new_date < _identified_date)
			throw invalid_argument("Hold date cannot be earlier than identified date");
		if (new_date > today())
			throw invalid_argument("Action cannot be put on hold in the future.");
		if (new_date < _startDate.value())
			throw invalid_argument("Hold date cannot be earlier than start date.");
		_on_hold_date = dates::date{new_date};
	}
}
void Action::set_date_completed(dates::date new_date)
{
	if (_currentStatus != Status::completed)
	{
		throw invalid_argument("Cannot set completed date when action status is not complete.");
	}
	else
	{
		if (new_date < _identified_date)
			throw invalid_argument("Completed date cannot be earlier than identified date");
		if (new_date > today())
			throw invalid_argument("Action cannot be completed in the future.");
		if (new_date < _startDate.value())
			throw invalid_argument("Completed date cannot be earlier than start date.");
		_date_completed = dates::date{new_date};
	}
}