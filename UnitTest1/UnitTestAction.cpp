#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../ConsoleApplication3/action.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestDates
{
	TEST_CLASS(UnitTestDates) {
		public:
			TEST_METHOD(TestActionInit) {
				string act_desc{ "Complete homework." };
				dates::date start_date(unsigned int(23), dates::month::September, unsigned int(2018));
				dates::date due_date(unsigned int(30), dates::month::October, unsigned int(2018));
				person owner{ "Kurian Kattukaren" };

				auto task = Action(act_desc, start_date, due_date, owner);

				Assert::AreEqual(act_desc, task.description());
				Assert::AreEqual(start_date, task.dueDate());
				Assert::AreEqual(due_date, task.startDate());
				Assert::IsFalse(task.due());
			}
	};
}
