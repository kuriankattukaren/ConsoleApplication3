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
	return lhs.dueDate() < rhs.dueDate();
}
bool compare_start_dates(const Action & lhs, const Action & rhs)
{
	return lhs.startDate() < rhs.startDate();
}
bool compare_priority(const Action & lhs, const Action & rhs)
{
	if (lhs.getPriority() == rhs.getPriority()) return false;
	else if (lhs.getPriority() == priority::High) return false;
	else if (lhs.getPriority() == priority::Medium) {
		if (rhs.getPriority() == priority::High) return true;
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
	auto lhs_status = lhs.actionStatus();
	auto rhs_status = rhs.actionStatus();

	if (lhs_status == rhs_status) return false;
	else if (lhs_status == status::notstarted) return true;
	else if (lhs_status == status::hold) {
		if (rhs_status == status::notstarted) return false;
		else return true;
	}
	else if (lhs_status == status::ongoing) {
		if (rhs_status == status::notstarted || rhs_status == status::hold)
			return false;
		else return true;
	}
	else return false; //lhs_status == status::completed
}

//Can we generalise it?
vector<string> get_descriptions(vector<Action>& actions) {
	vector<string> descriptions;
	for (const auto& action : actions) {
		descriptions.push_back(action.description());
	}
	return descriptions;
}
vector<dates::date> get_due_dates(vector<Action>& actions) {
	vector<dates::date> dueDates;
	for (const auto& action : actions) {
		dueDates.push_back(action.dueDate());
	}
	return dueDates;
}
vector<dates::date> get_start_dates(vector<Action>& actions){
	vector<dates::date> startDates;
	for (const auto& action : actions) {
		startDates.push_back(action.startDate());
	}
	return startDates;
}
vector<person> get_owners(vector<Action>& actions) {
	vector<person> owners;
	for (const auto& action : actions) {
		owners.push_back(action.owner());
	}
	return owners;
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
shared_ptr<dates::date>Action::date_started() const
{
	return _startDate;

}
shared_ptr<dates::date> Action::date_completed() const
{
	return _date_completed;
}
shared_ptr<dates::date> Action::date_onhold() const
{
	return _on_hold_date;
}
void Action::set_status(Status new_status) {
	if (new_status == Status::notstarted)
	{
		_on_hold_date = nullptr;
		_date_completed = nullptr;
		_startDate = nullptr;
		_currentStatus = new_status;
	}
	else if (new_status == Status::ongoing)
	{
		_on_hold_date = nullptr;
		_date_completed = nullptr;
		this->set_date_started(today());
		_currentStatus = new_status;
	}
	if (new_status == Status::hold)
	{
		_date_completed = nullptr;
		if (_startDate == nullptr)
			this->set_date_started(today());
		else
			this->set_date_onhold(today());
		_currentStatus = new_status;
	}
	if (new_status == Status::completed)
	{
		_on_hold_date = nullptr;
		_date_completed = make_shared<dates::date>(today());
		_currentStatus = new_status;
	}
	else
	{
		_on_hold_date = nullptr;
		_date_completed = nullptr;
		_currentStatus = new_status;
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
	if (_dueDate < _identified_date)
		throw invalid_argument("Due date cannot be earlier than identified date.");
	_dueDate = new_due_date;
}
void Action::set_date_identified(dates::date new_date)
{
	if (_currentStatus == Status::notstarted)
	{
		if (_identified_date > _dueDate)
			throw invalid_argument("Date action was identified should be earlier than due date.");
		if (_identified_date > _today)
			throw invalid_argument("Date action was identified cannot be later than today.");
		_identified_date = new_date;
	}
	if (_currentStatus == Status::ongoing)
	{
		if (_identified_date > *_startDate)
			throw invalid_argument("Date action was idenfitied should be before start date.");
	}
	if (_currentStatus == Status::hold)
	{
		if (_identified_date > *_startDate)
			throw invalid_argument("Date action was idenfitied should be before start date.");
		if (_identified_date > *_on_hold_date)
			throw invalid_argument("Date action was identified should be before.");
	}
	if (_currentStatus == Status::completed)
	{
		if (_identified_date > *_startDate)
			throw invalid_argument("Date action was idenfitied should be before start date.");
		if (_identified_date > *_date_completed)
			throw invalid_argument("Date action was identified should be before completed date.");
	}
	_identified_date = new_date;
}
void Action::set_date_started(dates::date new_start_date)
{
	if (new_start_date < _identified_date) 
		throw invalid_argument("Start date cannot be earlier than identified date");
	if (new_start_date > today())
		throw invalid_argument("Action cannot be started in the future.");
	if (_currentStatus == Status::completed)
	{
		if (new_start_date >= *_date_completed)
			throw invalid_argument("Action cannot start after completion date.");
	}
	if (_currentStatus == Status::hold)
	{
		if (new_start_date >= *_on_hold_date)
			throw std::invalid_argument("Action cannot start after on hold date.");
	}
	_startDate = make_shared<dates::date>(dates::date{ new_start_date });
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
		if (new_date < *_startDate)
			throw invalid_argument("Hold date cannot be earlier than start date.");
		_on_hold_date = make_shared<dates::date>(dates::date{ new_date });
	}
}