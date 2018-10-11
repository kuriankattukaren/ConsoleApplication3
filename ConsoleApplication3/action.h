#pragma once
#include <string>
#include "dates.h"
#include "person.h"

enum class status{notstarted, hold, ongoing, completed};
enum class priority{Low, Medium, High};

class Action {
	//bool _due;
	string _desc;
	person _owner;
	status _currentStatus;
	priority _priority;
	dates::date _dueDate, _startDate, _today;

public:
	Action(string act, dates::date& start_date, dates::date& due_date, person owner);
	
	dates::date dueDate() const;
	void changeDueDate(dates::date new_due_date);
	
	dates::date startDate() const;
	void changeStartDate(dates::date new_start_date);

	status actionStatus() const;
	void setStatus(status new_status);

	string description() const;
	void changeDescription(string new_description);

	person owner() const;
	void changeOwner(person new_owner);

	priority getPriority() const;
	void setPriority(priority new_priority);

	bool due();

};

dates::date today();
dates::month month_from_int(unsigned int mon);

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