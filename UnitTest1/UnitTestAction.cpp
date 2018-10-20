#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>

#include "../../DateTime/include/date/date.h"
#include "../../DateTime/include/date/tz.h"
#include "../ConsoleApplication3/action.h"
#include "../ConsoleApplication3/person.h"
#include "../ConsoleApplication3/dates.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestAction
{
	/*
	 * Generates a collection of actions.
	 */
	vector<Action> generate_actions(int number) {
		vector<string> action_names{ "Complete home work.",
									 "Do the washing.",
									 "Complete the exercise.",
									 "Call back friends.",
									 "Visit the doctor." };

		vector<dates::date> start_dates{ dates::date(23, dates::month::October,2018),
										 dates::date(1, dates::month::September, 2018),
										 dates::date(17, dates::month::July, 2018),
										 dates::date(12, dates::month::June, 2018),
										 dates::date(17, dates::month::February, 1969) };

		vector<dates::date> end_dates{ dates::date(12, dates::month::November, 2018),
										 dates::date(3, dates::month::October, 2018),
										 dates::date(17, dates::month::August, 2018),
										 dates::date(12, dates::month::July, 2018),
										 dates::date(17, dates::month::March, 1979) };

		vector<person> owners{ person("Bill Clinton"),
							   person("Barack Obama"),
							   person("Donald Trump"),
							   person("Ronald Reagan"),
							   person("Franklin Roosevelt") };

		vector<status> task_status{ status::ongoing,
									status::completed,
									status::hold,
									status::notstarted,
									status::completed };

		vector<priority> task_priority{ priority::High,
										priority::Low,
										priority::Medium,
										priority::Low,
										priority::High };

		vector<Action> actions;
		auto max = 5;
		for (auto i = 0; i < max; ++i) {
			auto act = Action(action_names.at(i), start_dates.at(i), end_dates.at(i), owners.at(i));
			act.setStatus(task_status.at(i));
			act.setPriority(task_priority.at(i));
			actions.push_back(act);
		}
		return actions;
	}
	
	/*
	 * Converts action status (completed, onhold, ongoing, started) to strings.
	 */
	string toString(status actionStatus) {
		/*
		Converts action status to string.
		*/
		if (actionStatus == status::completed)
			return "COMPLETED";
		else if (actionStatus == status::hold)
			return "HOLD";
		else if (actionStatus == status::notstarted)
			return "NOT_STARTED";
		else if (actionStatus == status::ongoing)
			return "ONGOING";
	}

	/*
	 *  Returns today's date. This function computes the date today independently from 
	 *   today() define in Action.h.
	 */
	dates::date date_today() {
		char mbstr[100];
		std::time_t t = std::time(nullptr);
		auto localtime = std::gmtime(&t);
		auto day_of_month = localtime->tm_mday;				//[1, 31]
		auto month = month_from_int(localtime->tm_mon + 1); //[0, 11] is the month index
		auto year = localtime->tm_year + 1900;				//1900 is the reference

		dates::date date_val{day_of_month, month, year};
		return date_val;
	}

	/*
	Tests Actions class initialisation and it's member functions.
	*/
	TEST_CLASS(UnitTestActionInit) {
public:
	TEST_METHOD(TestActionInit) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::April, 2019 };
		dates::date end{ 1, dates::month::June, 2019 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };

		Assert::AreEqual(action_name, act.description());
		Assert::IsTrue(owner == act.owner());
		Assert::IsTrue(start == act.startDate());
		Assert::IsTrue(end == act.dueDate());
		Assert::IsTrue(priority::Medium == act.getPriority());
		Assert::IsTrue(status::notstarted == act.actionStatus());
		Assert::IsFalse(act.due());
	}
	TEST_METHOD(TestTodayGenerator)
	{
		std::time_t t = std::time(nullptr);
		auto localtime = std::localtime(&t);
		auto day_of_month = localtime->tm_mday;				//[1, 31]
		auto month = month_from_int(localtime->tm_mon + 1); //[0, 11] is the month index
		auto year = localtime->tm_year + 1900;				//1900 is the reference
	
		Logger::WriteMessage(std::to_wstring(localtime->tm_hour).c_str());
		Logger::WriteMessage(std::to_wstring(localtime->tm_min).c_str());
		Logger::WriteMessage(std::to_wstring(localtime->tm_sec).c_str());
		Logger::WriteMessage(std::to_wstring(day_of_month).c_str());
		Logger::WriteMessage(std::to_wstring(localtime->tm_mon + 1).c_str());
		Logger::WriteMessage(std::to_wstring(localtime->tm_year + 1900).c_str());
		
		std::time_t gmt = std::time(nullptr);
		auto gmttime = std::gmtime(&gmt);
		auto gmday_of_month = gmttime->tm_mday;				//[1, 31]
		auto gmmonth = month_from_int(gmttime->tm_mon + 1); //[0, 11] is the month index
		auto gmyear = gmttime->tm_year + 1900;				//1900 is the reference
		Logger::WriteMessage(std::to_wstring(gmttime->tm_hour).c_str());
		Logger::WriteMessage(std::to_wstring(gmttime->tm_min).c_str());
		Logger::WriteMessage(std::to_wstring(gmttime->tm_sec).c_str());
		Logger::WriteMessage(std::to_wstring(gmday_of_month).c_str());
		Logger::WriteMessage(std::to_wstring(gmttime->tm_mon + 1).c_str());
		Logger::WriteMessage(std::to_wstring(gmttime->tm_year + 1900).c_str());

		auto rt = date::make_zoned(date::current_zone()->name(), std::chrono::system_clock::now());
		Logger::WriteMessage(std::to_wstring(rt.get_local_time().time_since_epoch().count()).c_str());
	}
	TEST_METHOD(TestToday) {
		auto test_date = dates::toString(date_today());
		auto computed_date = dates::toString(today());
		string warning{ "We're in test today" };
		Logger::WriteMessage(warning.c_str());
		Logger::WriteMessage(test_date.c_str());
		Logger::WriteMessage(computed_date.c_str());
		Assert::IsTrue(date_today() == today());
	}
	TEST_METHOD(TestTodayComparison) {
		dates::date end{ 1, dates::month::June, 2019 };
		dates::date tod{ today() };
		Assert::IsTrue(end > tod);
		Assert::IsFalse(end <= tod);
	}
	TEST_METHOD(TestActionDue) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };
		Assert::IsTrue(act.due());
	}
	TEST_METHOD(TestActionCompletedNotDue) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };
		act.setStatus(status::completed);
		Assert::IsFalse(act.due());

		//Non completed action is not due.

		act.setStatus(status::ongoing);
		Assert::IsTrue(act.due());
	}
	TEST_METHOD(TestChangeDescription) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };

		auto old_desc = act.description();
		act.changeDescription("Complete chores");
		auto new_desc = act.description();
		Assert::AreNotEqual(old_desc, new_desc);
	}
	TEST_METHOD(TestChangeStartDate) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2019 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };

		Assert::IsFalse(act.due());
		dates::date new_startdate{ 23, dates::month::December, 2018 };
		act.changeStartDate(new_startdate);
		Assert::IsTrue(new_startdate == act.startDate());
		Assert::IsFalse(act.due());
	}
	TEST_METHOD(TestChangeOwner) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2019 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };

		person new_owner{ "James Bond" };
		act.changeOwner(new_owner);
		Assert::IsTrue(new_owner == act.owner());
	}
	TEST_METHOD(TestChangePriority) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2019 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };

		Assert::IsTrue(priority::Medium == act.getPriority());
		act.setPriority(priority::High);
		Assert::IsFalse(act.due());
	}
	TEST_METHOD(TestChangeStatus) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };
		Assert::IsTrue(act.due());

		Assert::IsTrue(status::notstarted == act.actionStatus());
		act.setStatus(status::completed);
		Assert::IsFalse(act.due()); //The task is completed.
	}
	TEST_METHOD(TestChangeDueDate) {
		string action_name{ "Complete home work" };
		dates::date start{ 1, dates::month::September, 2018 };
		dates::date end{ 18, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		Action act{ action_name, start, end, owner };
		Assert::IsTrue(act.due());
		dates::date new_date{ 18, dates::month::September, 2030 };
		act.changeDueDate(new_date);
		Assert::IsFalse(act.due());
	}
	};

	/*
	Tests if a collection of actions can be sorted.
	*/
	TEST_CLASS(UnitTestSorting) {
	public:
		TEST_METHOD(TestSortDueDates) {
		auto actions_to_sort = generate_actions(5);

		Assert::IsTrue(actions_to_sort.size() == 5);
		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_due_dates);
		dates::date expected{ 17, dates::month::March, 1979 };
		Assert::IsTrue(actions_to_sort.at(0).dueDate() == expected);
	}
		TEST_METHOD(TestSortStartDates) {
		auto actions_to_sort = generate_actions(5);
		std::random_shuffle(actions_to_sort.begin(), actions_to_sort.end());
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_start_dates);
		dates::date expected{ 17, dates::month::February, 1969 };
		Assert::IsTrue(actions_to_sort.at(0).startDate() == expected);
	}
		TEST_METHOD(TestSortDescription) {
		auto actions_to_sort = generate_actions(5);
		std::random_shuffle(actions_to_sort.begin(), actions_to_sort.end());
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_descriptions);
		string expected{ "Call back friends." };
		Assert::IsTrue(actions_to_sort.at(0).description() == expected);
	}
		TEST_METHOD(TestSortOwner) {
		auto actions_to_sort = generate_actions(5);
		std::random_shuffle(actions_to_sort.begin(), actions_to_sort.end());
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_owners);
		auto expected = person{ "Barack Obama" };
		Assert::IsTrue(actions_to_sort.at(0).owner() == expected);
	}
		TEST_METHOD(TestSortPriority) {
		auto actions_to_sort = generate_actions(5);
		std::random_shuffle(actions_to_sort.begin(), actions_to_sort.end());
		Assert::IsTrue(actions_to_sort.size() == 5);
		if (actions_to_sort.at(0).getPriority() == priority::High)
			Logger::WriteMessage("High");
		else if (actions_to_sort.at(0).getPriority() == priority::Medium)
			Logger::WriteMessage("Medium");
		else if (actions_to_sort.at(0).getPriority() == priority::Low)
			Logger::WriteMessage("Low");
		else
			Logger::WriteMessage("We don't know the priority.");

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_priority);
		Assert::IsTrue(actions_to_sort.at(0).getPriority() == priority::Low);
		Assert::IsTrue(actions_to_sort.at(1).getPriority() == priority::Low);
		Assert::IsTrue(actions_to_sort.at(2).getPriority() == priority::Medium);
		Assert::IsTrue(actions_to_sort.at(3).getPriority() == priority::High);
		Assert::IsTrue(actions_to_sort.at(4).getPriority() == priority::High);
	}
		TEST_METHOD(TestSortStatus) {
		auto actions_to_sort = generate_actions(5);
		std::random_shuffle(actions_to_sort.begin(), actions_to_sort.end());
		Assert::IsTrue(actions_to_sort.size() == 5);
		if (actions_to_sort.at(0).actionStatus() == status::completed)
			Logger::WriteMessage("Action status: COMPLETED");
		else if (actions_to_sort.at(0).actionStatus() == status::notstarted)
			Logger::WriteMessage("Action status: NOT_STARTED");
		else if (actions_to_sort.at(0).actionStatus() == status::ongoing)
			Logger::WriteMessage("Action status: ONGOING");
		else if (actions_to_sort.at(0).actionStatus() == status::hold)
			Logger::WriteMessage("Action status: HOLD");
		else
			Logger::WriteMessage("We don't know the status.");

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_status);
		auto first_status = actions_to_sort.at(1).actionStatus();
		if (first_status == status::notstarted)
			Logger::WriteMessage("NOT STARTED");
		else if (first_status == status::hold)
			Logger::WriteMessage("ON HOLD");
		else if (first_status == status::ongoing)
			Logger::WriteMessage("ONGOING");
		else if (first_status == status::completed)
			Logger::WriteMessage("COMPLETED");
		Assert::IsTrue(actions_to_sort.at(0).actionStatus() == status::notstarted);
		Assert::IsTrue(actions_to_sort.at(1).actionStatus() == status::hold);
		auto acstring = toString(actions_to_sort.at(1).actionStatus());
		Logger::WriteMessage(acstring.c_str());
		Assert::IsTrue(actions_to_sort.at(2).actionStatus() == status::ongoing);
		Assert::IsTrue(actions_to_sort.at(3).actionStatus() == status::completed);
		Assert::IsTrue(actions_to_sort.at(4).actionStatus() == status::completed);
	}
	};

	/*
	Tests if a collection of actions can be searched.
	*/
	TEST_CLASS(UnitTestSearching) {
	 public:
		 TEST_METHOD(TestFindStringInOwner) 
		 {
			 auto actions = generate_actions(5);
			 string str_to_search{ "ill" };
			 
			 auto results = search_action(str_to_search, actions, search_owner{str_to_search});
			 Assert::AreEqual({ 1 }, results.size());
		 }
		 TEST_METHOD(TestFindStringInDescription) 
		 {
			 auto actions = generate_actions(5);
			 string str_to_search{ "Complete" };

			 auto results = search_action(str_to_search, actions, search_description{ str_to_search });
			 Assert::AreEqual({ 2 }, results.size());
		 }
		 TEST_METHOD(TestFindStringNotPresentInDescription) 
		 {
			 auto actions = generate_actions(5);
			 string str_to_search{ "idiot" };
			 auto results = search_action(str_to_search, actions, search_description{ str_to_search });
			 Assert::AreEqual({ 0 }, results.size());
		 }
	};

	/*
	Tests if a subset of actions satisfying a criteria can be determined.
	*/
	TEST_CLASS(UnitTestSubsets) {
	public:
		TEST_METHOD(TestFindCompletedActions) 
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, action_completed{});
			Assert::AreEqual({ 2 }, results.size());
		}
		TEST_METHOD(TestFindOngoingActions)
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, action_ongoing{});
			Assert::AreEqual({ 1 }, results.size());
		}
		TEST_METHOD(TestFindOnHoldActions) 
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, action_on_hold{});
			Assert::AreEqual({ 1 }, results.size());
		}
		TEST_METHOD(TestFindNotStartedActions)
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, action_not_started{});
			Assert::AreEqual({ 1 }, results.size());
		}
		TEST_METHOD(TestFindDueActions) 
		{
			//We use a lambda for this.
			auto actions = generate_actions(5);
			auto results = find_actions(actions, [](Action& action) {return action.due();});
			Assert::AreEqual({ 2 }, results.size()); //completed actions are not due.
		}
		TEST_METHOD(TestFindHighPriorityActions) 
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, priority_high{});
			Assert::AreEqual({ 2 }, results.size());
		}
		TEST_METHOD(TestFindLowPriorityActions)
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, priority_low{});
			Assert::AreEqual({ 2 }, results.size());
		}
		TEST_METHOD(TestFindMediumPriorityActions)
		{
			auto actions = generate_actions(5);
			auto results = find_actions(actions, priority_medium{});
			Assert::AreEqual({ 1 }, results.size());
		}
	};
}