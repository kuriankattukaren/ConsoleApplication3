#pragma once
#include <string>
#include "dates.h"
#include "person.h"
#include "../../DateTime/include/date/date.h"
#include "../../DateTime/include/date/tz.h"
enum class Status{notstarted, hold, ongoing, completed};
enum class Priority{Low, Medium, High};

class Action {
	string _desc;
	person _owner;
	Status _currentStatus;
	Priority _priority;
	dates::date _dueDate, _today, _identified_date;
	shared_ptr<dates::date> _on_hold_date{ nullptr }, _date_completed{ nullptr };
	shared_ptr<dates::date> _startDate{ nullptr };
public:
	Action(string act, dates::date& start_date, dates::date& due_date, person owner);
	
	/*
	 * Returns the date when the action was identified.
	 */
	dates::date date_identified() const;

	/*
	 * Changes the date identified.
	 */
	void set_date_identified(dates::date new_date);

	/*
	 * Returns the date the action is due.
	 */
	dates::date date_due() const;

	/*
	 * Changes the date the action is due.
	 */
	void set_date_due(dates::date new_due_date);
	
	/*
	 * Returns the date action was started.
	 * If action not started returns nullptr.
	 */
	shared_ptr<dates::date>date_started() const;

	/*
	 * Changes the date the action was started.
	 */
	void set_date_started(dates::date new_start_date);

	/*
	Returns pointer to date when action was marked completed.
	If action is not completed a nullptr is returned.
	*/
	shared_ptr<dates::date> date_completed() const;
	
	/*
	 Returns pointer to date when action was put on hold.
	 If not on hold returns nullptr.
	 */
	shared_ptr<dates::date> date_onhold() const;

	void set_date_onhold(dates::date new_date);

	void set_date_completed(dates::date new_date);

	/*
	 * Returns the current status of the action.
	 */
	Status status() const;

	/*
	 * Changes the status.
	 */
	void set_status(Status new_status);

	/*
	 * Returns the action description.
	 */
	string description() const;

	/*
	 * Changes the action description.
	 */
	void set_description(string new_description);

	/*
	 * Returns the owner of the action.
	 */
	person owner() const;

	/*
	 * Changes the owner of the action.
	 */
	void set_owner(person new_owner);

	/*
	 * Returns the priority of the action.
	 */
	Priority priority() const;

	/*
	 * Changes the priority of the action.
	 */
	void set_priority(Priority new_priority);

	/*
	 * Returns true if action is due. Action is due if
	 * it's not completed and due date has already passed.
	 */
	bool due();

};

dates::date today();
dates::month month_from_int(int mon);

bool compare_due_dates(const Action& lhs, const Action& rhs);
bool compare_start_dates(const Action& lhs, const Action& rhs);
bool compare_priority(const Action& lhs, const Action& rhs);
bool compare_owners(const Action&lhs, const Action& rhs);
bool compare_descriptions(const Action& lhs, const Action& rhs);
bool compare_status(const Action& lhs, const Action& rhs);

vector<string> get_descriptions(vector<Action>& actions);
vector<dates::date> get_due_dates(vector<Action>& actions);
vector<dates::date> get_start_dates(vector<Action>& actions);
vector<person> get_owners(vector<Action>& actions);

template<typename oper>
vector<Action> search_action(string search_string,  vector<Action>& actions, oper pred);
template<typename P>
vector<Action> find_actions(vector<Action>, P pred);

template<typename oper>
vector<Action> search_action(string search_string, vector<Action>& actions, oper pred) {
	vector<Action> search_results;
	for (const Action& action : actions) {
		if (pred(action))
			search_results.push_back(action);
	}
	return search_results;
}
template<typename P>
vector<Action> find_actions(vector<Action> actions, P pred) 
{
	vector<Action> matches;
	for (auto action : actions) { 
		if (pred(action)) matches.push_back(action);
	}
	return matches;
}

/*
Functors to find a search term in the Action.
*/
class search_owner {
	string val;
public:
	search_owner(string str) : val{ str } {}
	bool operator()(const Action& action) const 
	{
		if (action.owner().name().find(val) != string::npos) return true;
		else return false;
	}
};
class search_description {
	string search_string;
	public:
		search_description(string search_term) :search_string{ search_term } {}
		bool operator()(const Action& action) const
		{
			if (action.description().find(search_string) != string::npos) return true;
			else return false;
		}
};
class search_dueDate {
	string search_string;
public:
	search_dueDate(string search_term) :search_string{ search_term } {}
	bool operator()(const Action& action) const
	{
		if (action.description().find(search_string) != string::npos) return true;
		else return false;
	}
};

/*
Functors to check for status of action.
*/
class action_completed {
	/*
	Returns true if the action is completed.
	*/
public:
	action_completed(){}
	bool operator()(const Action& action) const
	{
		if (action.actionStatus() == status::completed) return true;
		else return false;
	}
};
class action_on_hold {
	/*
	Returns true if the action is completed.
	*/
public:
	action_on_hold() {}
	bool operator()(const Action& action) const
	{
		if (action.actionStatus() == status::hold) return true;
		else return false;
	}
};
class action_ongoing {
	/*
	Returns true if the action is completed.
	*/
public:
	action_ongoing() {}
	bool operator()(const Action& action) const
	{
		if (action.actionStatus() == status::ongoing) return true;
		else return false;
	}
};
class action_not_started {
	/*
	Returns true if the action is completed.
	*/
public:
	action_not_started() {}
	bool operator()(const Action& action) const
	{
		if (action.actionStatus() == status::notstarted) return true;
		else return false;
	}
};

/*
 Functors to check for priority.
*/
class priority_high {
public:
	priority_high() {}
	bool operator()(const Action& action) const
	{
		if (action.getPriority() == priority::High) return true;
		else return false;
	}
};
class priority_medium {
public:
	priority_medium() {}
	bool operator()(const Action& action) const
	{
		if (action.getPriority() == priority::Medium) return true;
		else return false;
	}
};
class priority_low {
public:	
	priority_low() {}
	bool operator()(const Action& action) const
	{
		if (action.getPriority() == priority::Low) return true;
		else return false;
	}
};