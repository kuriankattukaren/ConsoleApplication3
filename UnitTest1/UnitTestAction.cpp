#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <random>

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
		vector<dates::date> end_dates{   dates::date(12, dates::month::December, 2018),
										 dates::date(3, dates::month::January, 2019),
										 dates::date(17, dates::month::August, 2022),
										 dates::date(12, dates::month::July, 2035),
										 dates::date(17, dates::month::March, 1979)};
		vector<person> owners{ person("Bill Clinton"),
							   person("Barack Obama"),
							   person("Donald Trump"),
							   person("Ronald Reagan"),
							   person("Franklin Roosevelt") };
		vector<Status> task_status{ Status::ongoing,
									Status::completed,
									Status::hold,
									Status::notstarted,
									Status::completed };
		vector<dates::date> task_status_dates{
										 dates::date(12, dates::month::November, 2018),
										 dates::date(3, dates::month::October, 2018),
										 dates::date(17, dates::month::August, 2018),
										 dates::date(12, dates::month::July, 2018),
										 dates::date(17, dates::month::March, 1979)};
		vector<Priority> task_priority{ Priority::High,
										Priority::Low,
										Priority::Medium,
										Priority::Low,
										Priority::High };
		vector<Action> actions;
		
		auto max = 5;
		for (auto i = 0; i < max; ++i) {
			auto act = Action(action_names.at(i), start_dates.at(i), end_dates.at(i), owners.at(i));
			act.set_status(task_status.at(i), task_status_dates.at(i));
			act.set_priority(task_priority.at(i));
			actions.push_back(act);
		}
		return actions;
	}
	
	/*
	 * Converts action status (completed, onhold, ongoing, started) to strings.
	 */
	string toString(Status actionStatus) {
		/*
		Converts action status to string.
		*/
		if (actionStatus == Status::completed)
			return "COMPLETED";
		else if (actionStatus == Status::hold)
			return "HOLD";
		else if (actionStatus == Status::notstarted)
			return "NOT_STARTED";
		else if (actionStatus == Status::ongoing)
			return "ONGOING";
	}

	/*
	 *  Returns today's date. This function computes the date today independently from 
	 *   today() define in Action.h.
	 */
	dates::date date_today() {
		char mbstr[100];
		std::time_t t = std::time(nullptr);
		auto localtime = std::localtime(&t);
		auto day_of_month = localtime->tm_mday;				//[1, 31]
		auto month = month_from_int(localtime->tm_mon + 1); //[0, 11] is the month index
		auto year = localtime->tm_year + 1900;				//1900 is the reference

		dates::date date_val{day_of_month, month, year};
		return date_val;
	}

	TEST_CLASS(HelperFunctions) {
	public:
		TEST_METHOD(Today){
			auto test_date = dates::toString(date_today());
			auto computed_date = dates::toString(today());
			Assert::IsTrue(date_today() == today());
		}
	};
	TEST_CLASS(Construction){
		string action_name{"Complete home work"};
		dates::date identified{1, dates::month::April, 2018 };
		dates::date due{1, dates::month::June, 2018 };
		person owner{"Kurian Kattukaren"};
		
	public:
		TEST_METHOD(Initialization)
		{
			Action act{action_name, identified, due, owner};
			
			Assert::AreEqual(action_name, act.description());
			Assert::IsTrue(owner == act.owner());
			Assert::IsTrue(identified == act.date_identified());
			Assert::IsTrue(due == act.date_due());
			Assert::IsTrue(Priority::Medium == act.priority());
			Assert::IsTrue(Status::notstarted == act.status());
			Assert::IsTrue(act.due());
		}
		TEST_METHOD(Invariant_IdentifiedDateLaterThanDueDate)
		{
			bool exception_thrown{false};
			try {
				dates::date identified{1, dates::month::April, 2018};
				dates::date due{1, dates::month::January, 2018};
				Action act{action_name, identified, due, owner};
			} catch (invalid_argument& e) { exception_thrown = true;}
			if (!exception_thrown){
				auto msg = std::string{"No exception raised."};
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(Invariant_IdentifiedDateLaterToday)
		{
			bool exception_thrown{false};
			try {
				dates::date start{1, dates::month::April, 2030};
				dates::date due{1, dates::month::January, 2031};
				Action act{action_name, start, due, owner };
			}
			catch (invalid_argument& e) { exception_thrown = true; }
			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
	};
	TEST_CLASS(Correctness) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{"Kurian Kattukaren"};
	public:
		TEST_METHOD(Due) {
			Action act{action_name, identified, due, owner};
			Assert::IsTrue(act.due());
		}
		TEST_METHOD(CompletedIsNotDue) {
			Action act{action_name, identified, due, owner};
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::completed, today());
			Assert::IsTrue(!act.due());
		}
		TEST_METHOD(OngoingWhichIsDue) {
			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, today());
			Assert::IsTrue(act.due());
		}
		
	};
	TEST_CLASS(Mutation) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(ChangeDescription) {
			Action act{ action_name, identified, due, owner };
			auto old_desc = act.description();
			act.set_description("Complete chores");
			auto new_desc = act.description();
			Assert::AreNotEqual(old_desc, new_desc);
		}
		TEST_METHOD(ChangeOwner) {
			string action_name{ "Complete home work" };
			dates::date start{ 1, dates::month::September, 2018 };
			dates::date end{ 18, dates::month::June, 2019 };
			person owner{ "Kurian Kattukaren" };
			Action act{ action_name, start, end, owner };

			person new_owner{ "James Bond" };
			act.set_owner(new_owner);
			Assert::IsTrue(new_owner == act.owner());
		}
		TEST_METHOD(ChangePriority) {
			string action_name{ "Complete home work" };
			dates::date start{ 1, dates::month::September, 2018 };
			dates::date end{ 18, dates::month::June, 2019 };
			person owner{ "Kurian Kattukaren" };
			Action act{ action_name, start, end, owner };

			Assert::IsTrue(Priority::Medium == act.priority());
			act.set_priority(Priority::High);
			Assert::IsFalse(act.due());
		}
		TEST_METHOD(ChangeDueDate) {
			Action act{action_name, identified, due, owner };
			Assert::IsTrue(act.due());
			dates::date new_date{ 18, dates::month::September, 2030 };
			act.set_date_due(new_date);
			Assert::IsFalse(act.due());
		}
		TEST_METHOD(InvalidDueDateRaisesException) {
			bool exception_thrown{false};
			Action act{action_name, identified, due, owner};
			dates::date due_date{18, dates::month::September, 1979};
			
			try{act.set_date_due(due_date);}
			catch(invalid_argument& e) {exception_thrown = true;}
			
			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
	};
	TEST_CLASS(ValidSetDateIdentified) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	
	public:
		TEST_METHOD(ActionNotStartedChangeDate) {
			Action act{ action_name, identified, due, owner };
			dates::date new_date_identified = dates::date{23, dates::month::December, 2017 };
			act.set_date_identified(new_date_identified);
			Assert::IsTrue(new_date_identified == act.date_identified());
		}
		TEST_METHOD(ActionOngoingChangeDate) {
			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, today());
			dates::date new_date_identified = dates::date{ 1, dates::month::May, 2018 };
			act.set_date_identified(new_date_identified);
			Assert::IsTrue(new_date_identified == act.date_identified());
		}
		TEST_METHOD(ActionOnHoldChangeDateIdentified) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());
			dates::date new_date_identified = dates::date{ 1, dates::month::May, 2018 };
			act.set_date_identified(new_date_identified);
			Assert::IsTrue(new_date_identified == act.date_identified());
		}
	};
	TEST_CLASS(InvalidSetDateIdentified) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };

	public:
		TEST_METHOD(ActionOngoingRaiseIdentifiedDatePastDueDate) {
			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, today());
			//Past due date
			dates::date new_identified_date = dates::date{ 1, dates::month::July, 2018 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionOngoingIdentifiedDatePastStartDate)
		
		{
			dates::date new_due_date{ 1, dates::month::July, 2040 };
			Action act{ action_name, identified, new_due_date, owner };
			act.set_status(Status::ongoing, today());
			//Past today//
			dates::date new_identified_date = dates::date{ 1, dates::month::July, 2022 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionOnHoldChangeDateIdentifiedPastDueDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());
			dates::date new_identified_date = dates::date{ 1, dates::month::July, 2018 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
				act.set_date_identified(new_identified_date);
				Assert::IsTrue(new_identified_date == act.date_identified());
			}
		}
		TEST_METHOD(ActionOnHoldChangeDateIdentifiedPastStartDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());
			dates::date new_date_identified = dates::date{ 1, dates::month::May, 2022 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_date_identified); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionOnHoldChangeDateIdentifiedPastHoldDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());
			dates::date new_identified_date = dates::date{ 1, dates::month::May, 2022 };
			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());

				act.set_date_identified(new_identified_date);
				Assert::IsTrue(new_identified_date == act.date_identified());
			}
		}
		TEST_METHOD(ActionCompletedChangeDateIdentifiedPastDueDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::completed, today());
			//Past due date
			dates::date new_identified_date = dates::date{ 1, dates::month::July, 2018 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionCompletedChangeDateIdentifiedPastStartDate) {
			dates::date new_due_date{ 1, dates::month::July, 2040 };
			Action act{ action_name, identified, new_due_date, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::completed, today());
			//Past today//
			dates::date new_identified_date = dates::date{ 1, dates::month::July, 2022 };

			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionCompletedChangeDateIdentifiedPastCompletedDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::completed, today());
			dates::date new_identified_date = dates::date{ 1, dates::month::May, 2022 };
			
			bool exception_thrown{ false };
			try { act.set_date_identified(new_identified_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());

				act.set_date_identified(new_identified_date);
				Assert::IsTrue(new_identified_date == act.date_identified());
			}
		}
	};
	TEST_CLASS(ValidSetStartDate) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(SetStartDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start_date = dates::date{ 23, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start_date);
			Assert::IsTrue(start_date == act.date_started());
			Assert::IsTrue(act.status() != Status::notstarted);
		}
		TEST_METHOD(ActionOnHoldChangeStartDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start_date = dates::date{ 23, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start_date);
			Assert::IsTrue(start_date == act.date_started());
			Assert::IsTrue(act.status() != Status::notstarted);
		}
		TEST_METHOD(ActionCompletedChangeStartDate) {
			dates::date start_date = dates::date{ 23, dates::month::May, 2018 };
			dates::date completed_date = dates::date{ 27, dates::month::May, 2018 };
			dates::date new_start_date = dates::date{ 12, dates::month::April, 2018 };
			
			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, start_date);
			act.set_status(Status::completed, completed_date);
			act.set_date_started(new_start_date);
			Assert::IsTrue(act.status() == Status::completed);
			Assert::IsTrue(act.date_started() == new_start_date);
			Assert::IsTrue(act.date_completed() == completed_date);
		}
		TEST_METHOD(ChangeStartDatePastDueDate) {
			dates::date start_date = dates::date{ 23, dates::month::May, 2018 };
			dates::date date_after_due_date = dates::date{ 12, dates::month::August, 2018 };

			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, start_date);
			
			act.set_date_started(date_after_due_date);
			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.date_started() == date_after_due_date);
		}
	};
	TEST_CLASS(InvalidSetStartDate) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(ActionCompletedChangeStartDatePastCompletedDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::completed, today());
			
			dates::date new_start_date = dates::date{1, dates::month::May, 2022};

			bool exception_thrown{ false };
			try { act.set_date_started(new_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ActionOnHoldChangeStartDatePastOnHoldDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());

			dates::date new_start_date = dates::date{ 1, dates::month::May, 2022 };

			bool exception_thrown{ false };
			try { act.set_date_started(new_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ChangeStartDateBeforeIdentifiedDate) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());

			dates::date new_start_date = dates::date{1, dates::month::May, 2017};

			bool exception_thrown{ false };
			try { act.set_date_started(new_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ChangeStartDateToTheFuture) {
			Action act{ action_name, identified, due, owner };
			dates::date start{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, start);
			act.set_status(Status::hold, today());
			dates::date new_start_date = dates::date{ 1, dates::month::May, 2022 };

			bool exception_thrown{ false };
			try { act.set_date_started(new_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
	};
	TEST_CLASS(InvalidSetOnHoldDate) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(StatusOngoing) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);

			dates::date valid_on_hold_date{ 10, dates::month::May, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_onhold(valid_on_hold_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(StatusNotStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date on_hold_date{ 10, dates::month::May, 2018 };
			
			bool exception_thrown{ false };
			try { act.set_date_onhold(on_hold_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(StatusCompleted) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			dates::date completed{ 1, dates::month::July, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::completed, completed);

			dates::date on_hold_date{ 10, dates::month::May, 2018 };
			bool exception_thrown{ false };
			try {act.set_date_onhold(on_hold_date);}
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(SetOnHoldBeforeDateIdentified) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::hold, today());

			dates::date on_hold_date{ 1, dates::month::January, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_onhold(on_hold_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}

		}
		TEST_METHOD(SetOnHoldAfterToday) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::hold, today());

			dates::date on_hold_date{ 1, dates::month::January, 2022 };
			bool exception_thrown{ false };
			try { act.set_date_onhold(on_hold_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(SetOnHoldBeforeStartDate) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::hold, today());

			dates::date before_start_date{ 25, dates::month::April, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_onhold(before_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
	};
	TEST_CLASS(ValidSetOnHoldDate) {
			string action_name{ "Complete home work" };
			dates::date identified{ 1, dates::month::April, 2018 };
			dates::date due{ 1, dates::month::June, 2018 };
			person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(SetOnHold) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::hold, today());

			dates::date new_hold_date{ 1, dates::month::August, 2018 };
			act.set_status(Status::hold, new_hold_date);
			
			Assert::IsTrue(act.status() == Status::hold);
			Assert::IsTrue(act.date_onhold() == new_hold_date);
			Assert::IsTrue(act.date_started() == started);
		}
	};
	TEST_CLASS(SetDateComplete) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
		
	public:
		TEST_METHOD(StatusOngoing) {
			Action act{ action_name, identified, due, owner };
			dates::date starting_date{ 1, dates::month::June, 2018 };
			act.set_status(Status::ongoing, starting_date);
			dates::date completed{ 10, dates::month::August, 2018 };
			
			bool exception_thrown{ false };
			try { act.set_date_completed(completed); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(StatusNotStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date completed{ 10, dates::month::August, 2018 };

			bool exception_thrown{ false };
			try { act.set_date_completed(completed); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(StatusOnHold) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			dates::date on_hold{ 1, dates::month::July, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::hold, on_hold);

			dates::date completed_date{ 10, dates::month::August, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_completed(completed_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(SetCompleteBeforeDateIdentified) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::completed, today());

			dates::date new_date{ 1, dates::month::January, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_completed(new_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(SetCompleteAfterToday) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::completed, today());

			dates::date future_date{ 1, dates::month::January, 2022 };
			bool exception_thrown{ false };
			try { act.set_date_completed(future_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(SetCompleteBeforeStartDate) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::completed, today());

			dates::date before_start_date{ 25, dates::month::April, 2018 };
			bool exception_thrown{ false };
			try { act.set_date_completed(before_start_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ValidSetComplete) {
			Action act{ action_name, identified, due, owner };
			dates::date started{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started);
			act.set_status(Status::completed, today());

			dates::date new_date{ 1, dates::month::August, 2018 };
			act.set_status(Status::completed, new_date);

			Assert::IsTrue(act.status() == Status::completed);
			Assert::IsTrue(act.date_completed() == new_date);
			Assert::IsTrue(act.date_started() == started);
		}
	};
	TEST_CLASS(SetStatus) {
		string action_name{ "Complete home work" };
		dates::date identified{ 1, dates::month::April, 2018 };
		dates::date due{ 1, dates::month::June, 2018 };
		person owner{ "Kurian Kattukaren" };
	public:
		TEST_METHOD(SetNotStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date new_identified_date{ 1, dates::month::March, 2018 };
			act.set_status(Status::notstarted, new_identified_date);

			Assert::IsTrue(act.status() == Status::notstarted);
			Assert::IsTrue(act.date_identified() == new_identified_date);
		}
		TEST_METHOD(SetOngoing) {
			Action act{ action_name, identified, due, owner };
			act.set_status(Status::ongoing, today());

			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.date_started() == today());
		}
		TEST_METHOD(SetOnHold) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::hold, today());

			Assert::IsTrue(act.status() == Status::hold);
			Assert::IsTrue(act.date_onhold() == today());
		}
		TEST_METHOD(SetCompleted) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::completed, today());

			Assert::IsTrue(act.status() == Status::completed);
			Assert::IsTrue(act.date_completed() == today());
			Assert::IsTrue(act.date_started() == started_date);
		}
		TEST_METHOD(ResetCompletedToNotStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::completed, today());
			act.set_status(Status::notstarted, act.date_identified());

			Assert::IsTrue(act.status() == Status::notstarted);
			Assert::IsTrue(act.date_identified() == identified);
			
			//We check for the optionals.
			if (act.date_completed().has_value()) Assert::Fail();
		}
		TEST_METHOD(ResetCompletedToOngoing) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::completed, today());
			act.set_status(Status::ongoing, started_date);

			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.date_started() == started_date);

			//We check for the optionals.
			if (act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
		}
		TEST_METHOD(ResetCompletedToOnHold) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::completed, today());
			act.set_status(Status::hold, today());

			Assert::IsTrue(act.status() == Status::hold);
			Assert::IsTrue(act.date_onhold() == today());

			//We check for the optionals.
			if (act.date_completed().has_value()) Assert::Fail();
			if (!act.date_onhold().has_value()) Assert::Fail();
		}
		TEST_METHOD(OnHoldToNotStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::hold, today());
			act.set_status(Status::notstarted, act.date_identified());

			Assert::IsTrue(act.status() == Status::notstarted);
			Assert::IsTrue(act.date_identified() == identified);

			//We check for the optionals.
			if (act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
			if (act.date_started().has_value()) Assert::Fail();
		}
		TEST_METHOD(OnHoldToStarted) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::hold, today());
			act.set_status(Status::ongoing, started_date);

			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.date_started() == started_date);

			//We check for the optionals.
			if (act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
			if (!act.date_started().has_value()) Assert::Fail();
		}
		TEST_METHOD(OnHoldToCompleted) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			act.set_status(Status::hold, today());
			act.set_status(Status::completed, today());

			Assert::IsTrue(act.status() == Status::completed);
			Assert::IsTrue(act.date_completed() == today());

			//We check for the optionals.
			if (!act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
			if (!act.date_started().has_value()) Assert::Fail();
		}
		TEST_METHOD(NotStartedToOnHoLD) {
			Action act{ action_name, identified, due, owner };
			
			dates::date on_hold_date{ 10, dates::month::August, 2018 };
			bool exception_thrown{ false };
			try { act.set_status(Status::hold, on_hold_date); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(NotStartedToCompleted) {
			Action act{ action_name, identified, due, owner };

			dates::date completed{ 10, dates::month::August, 2018 };
			bool exception_thrown{ false };
			try { act.set_status(Status::completed, completed); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
		}
		TEST_METHOD(ExceptionDoesNotChangeState) {
			Action act{ action_name, identified, due, owner };
			dates::date started_date{ 1, dates::month::May, 2018 };
			act.set_status(Status::ongoing, started_date);
			
			Assert::IsTrue(act.description() == action_name);
			Assert::IsTrue(act.date_identified() == identified);
			Assert::IsTrue(act.date_due() == due);
			Assert::IsTrue(act.owner() == owner);
			Assert::IsTrue(act.date_started() == started_date);
			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.priority() == Priority::Medium);

			if (act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
			
			dates::date completed{ 10, dates::month::August, 2017 };
			bool exception_thrown{ false };
			try { act.set_status(Status::completed, completed); }
			catch (invalid_argument& e) { exception_thrown = true; }

			if (!exception_thrown) {
				auto msg = std::string{ "No exception raised." };
				auto msg_w = std::wstring(msg.begin(), msg.end());
				Assert::Fail(msg_w.c_str());
			}
			
			Assert::IsTrue(act.description() == action_name);
			Assert::IsTrue(act.date_identified() == identified);
			Assert::IsTrue(act.date_due() == due);
			Assert::IsTrue(act.owner() == owner);
			Assert::IsTrue(act.date_started() == started_date);
			Assert::IsTrue(act.status() == Status::ongoing);
			Assert::IsTrue(act.priority() == Priority::Medium);

			if (act.date_completed().has_value()) Assert::Fail();
			if (act.date_onhold().has_value()) Assert::Fail();
		}
	};
	/*
	Tests if a collection of actions can be sorted.
	*/
	TEST_CLASS(Sorting) {
	public:
		TEST_METHOD(TestSortDueDates) {
		auto actions_to_sort = generate_actions(5);

		Assert::IsTrue(actions_to_sort.size() == 5);
		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_due_dates);
		dates::date expected{ 17, dates::month::March, 1979 };
		Assert::IsTrue(actions_to_sort.at(0).date_due() == expected);
	}
		TEST_METHOD(TestSortStartDates) {
		auto actions_to_sort = generate_actions(5);
		std::random_device rng;
		std::mt19937 urng(rng());
		std::shuffle(actions_to_sort.begin(), actions_to_sort.end(), urng);
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_identified_dates);
		dates::date expected{ 17, dates::month::February, 1969 };
		Assert::IsTrue(actions_to_sort.at(0).date_identified() == expected);
	}
		TEST_METHOD(TestSortDescription) {
		auto actions_to_sort = generate_actions(5);
		std::random_device rng;
		std::mt19937 urng(rng());
		std::shuffle(actions_to_sort.begin(), actions_to_sort.end(), urng);
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_descriptions);
		string expected{ "Call back friends." };
		Assert::IsTrue(actions_to_sort.at(0).description() == expected);
	}
		TEST_METHOD(TestSortOwner) {
		auto actions_to_sort = generate_actions(5);
		std::random_device rng;
		std::mt19937 urng(rng());
		std::shuffle(actions_to_sort.begin(), actions_to_sort.end(), urng);
		Assert::IsTrue(actions_to_sort.size() == 5);

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_owners);
		auto expected = person{ "Barack Obama" };
		Assert::IsTrue(actions_to_sort.at(0).owner() == expected);
	}
		TEST_METHOD(TestSortPriority) {
		auto actions_to_sort = generate_actions(5);
		std::random_device rng;
		std::mt19937 urng(rng());
		std::shuffle(actions_to_sort.begin(), actions_to_sort.end(), urng);
		Assert::IsTrue(actions_to_sort.size() == 5);
		if (actions_to_sort.at(0).priority() == Priority::High)
			Logger::WriteMessage("High");
		else if (actions_to_sort.at(0).priority() == Priority::Medium)
			Logger::WriteMessage("Medium");
		else if (actions_to_sort.at(0).priority() == Priority::Low)
			Logger::WriteMessage("Low");
		else
			Logger::WriteMessage("We don't know the priority.");

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_priority);
		Assert::IsTrue(actions_to_sort.at(0).priority() == Priority::Low);
		Assert::IsTrue(actions_to_sort.at(1).priority() == Priority::Low);
		Assert::IsTrue(actions_to_sort.at(2).priority() == Priority::Medium);
		Assert::IsTrue(actions_to_sort.at(3).priority() == Priority::High);
		Assert::IsTrue(actions_to_sort.at(4).priority() == Priority::High);
	}
		TEST_METHOD(TestSortStatus) {
		auto actions_to_sort = generate_actions(5);
		std::random_device rng;
		std::mt19937 urng(rng());
		std::shuffle(actions_to_sort.begin(), actions_to_sort.end(), urng);
		Assert::IsTrue(actions_to_sort.size() == 5);
		if (actions_to_sort.at(0).status() == Status::completed)
			Logger::WriteMessage("Action status: COMPLETED");
		else if (actions_to_sort.at(0).status() == Status::notstarted)
			Logger::WriteMessage("Action status: NOT_STARTED");
		else if (actions_to_sort.at(0).status() == Status::ongoing)
			Logger::WriteMessage("Action status: ONGOING");
		else if (actions_to_sort.at(0).status() == Status::hold)
			Logger::WriteMessage("Action status: HOLD");
		else
			Logger::WriteMessage("We don't know the status.");

		std::sort(actions_to_sort.begin(), actions_to_sort.end(), compare_status);
		auto first_status = actions_to_sort.at(1).status();
		if (first_status == Status::notstarted)
			Logger::WriteMessage("NOT STARTED");
		else if (first_status == Status::hold)
			Logger::WriteMessage("ON HOLD");
		else if (first_status == Status::ongoing)
			Logger::WriteMessage("ONGOING");
		else if (first_status == Status::completed)
			Logger::WriteMessage("COMPLETED");
		Assert::IsTrue(actions_to_sort.at(0).status() == Status::notstarted);
		Assert::IsTrue(actions_to_sort.at(1).status() == Status::hold);
		auto acstring = toString(actions_to_sort.at(1).status());
		Logger::WriteMessage(acstring.c_str());
		Assert::IsTrue(actions_to_sort.at(2).status() == Status::ongoing);
		Assert::IsTrue(actions_to_sort.at(3).status() == Status::completed);
		Assert::IsTrue(actions_to_sort.at(4).status() == Status::completed);
	}
	};

	/*
	Tests if a collection of actions can be searched.
	*/
	TEST_CLASS(Searching) {
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
	TEST_CLASS(Subsets) {
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
