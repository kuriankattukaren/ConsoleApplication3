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


Action::Action(string act, dates::date& start_date, dates::date& due_date, person owner):
	_desc{ act }, _startDate{ start_date }, _dueDate{ due_date }, _owner{ owner }, _today{ today() } {
	_priority = priority::Medium;
	_currentStatus = status::notstarted;
}

string Action::description() const {
	return _desc;
}
void Action::changeDescription(string new_description) {
	_desc = new_description;
}
person Action::owner() const{
	return _owner;
}
void Action::changeOwner(person new_owner){
	_owner = new_owner;
}
priority Action::getPriority() const{
	return _priority;
}
void Action::setPriority(priority new_priority){
	_priority = new_priority;
}
dates::date Action::dueDate() const{
	return _dueDate;
}
void Action::changeDueDate(dates::date new_due_date) {
	_dueDate = new_due_date;
}
bool Action::due() {
	_today = today();
	//Completed tasks are not due.
	if (_currentStatus == status::completed) return false;
	if (_dueDate > _today) return false;
	else return true;
}
dates::date Action::startDate() const {
	return _startDate;
}
void Action::changeStartDate(dates::date new_start_date){
	_startDate = new_start_date;
}
status Action::actionStatus() const{
	return _currentStatus;
}
void Action::setStatus(status new_status){
	_currentStatus = new_status;
}


