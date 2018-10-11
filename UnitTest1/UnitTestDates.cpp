#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication3/testingfile.h"
#include "../ConsoleApplication3/dates.h"
#include <vector>
#include <algorithm>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestDates
{
	TEST_CLASS(UnitTestDates){
	public:
		TEST_METHOD(TestLeapYear) {
			vector<int> leap_years{ 1992, 1996, 2004, 2008, 2012};
			vector<int> non_leap_years{ 1993, 2007, 2001 };
			
			for (auto year : leap_years) Assert::IsTrue(dates::is_leap_year(year));
			for (auto year : non_leap_years) Assert::IsFalse(dates::is_leap_year(year));
		}
		TEST_METHOD(TestDatesInitValid) {
			unsigned int day{ 23 }, year{1970};
			auto month_born = dates::month::February;
			auto date_of_birth = dates::date(day, dates::month::February, year);
			Assert::AreEqual(day, date_of_birth.day());
			if (month_born != date_of_birth.month()) Assert::Fail(L"Returned months are not the same.");
			Assert::AreEqual(year, date_of_birth.year());
		}
		TEST_METHOD(TestDatesEqual) {
			auto my_date_of_birth = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(unsigned int{ 5 }, dates::month::April, unsigned int{ 1981 });
			Assert::IsFalse(my_date_of_birth == my_sisters_date_of_birth);
			Assert::IsTrue(my_date_of_birth == my_date_of_birth);
		}
		TEST_METHOD(TestDatesLessThan) {
			auto my_date_of_birth = dates::date(unsigned int{13}, dates::month::June, unsigned int{1979});
			auto my_sisters_date_of_birth = dates::date(unsigned int{5}, dates::month::April, unsigned int{1981});
			Assert::IsTrue(my_date_of_birth < my_sisters_date_of_birth);
			Assert::IsFalse(my_sisters_date_of_birth < my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(unsigned int{13}, dates::month::June, unsigned int{1979});
			auto later_date = dates::date(unsigned int{13}, dates::month::July, unsigned int{1979});
			Assert::IsTrue(earlier_date < later_date);
			Assert::IsFalse(later_date < earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_day = dates::date(unsigned int{ 15 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsTrue(earlier_day < later_day);
			Assert::IsFalse(later_day < earlier_day);
			
			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto same_date_2 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsFalse(same_date_1 < same_date_2);

			auto date_1 = dates::date(unsigned int{ 23 }, dates::month::July, unsigned int{ 1979 });
			auto date_2 = dates::date(unsigned int{ 23 }, dates::month::January, unsigned int{ 1979 });
			Assert::IsTrue(date_2 < date_1);
			Assert::IsFalse(date_2 > date_1);
		}
		TEST_METHOD(TestDatesGreaterThan) {
			auto my_date_of_birth = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(unsigned int{ 5 }, dates::month::April, unsigned int{ 1981 });
			Assert::IsFalse(my_date_of_birth > my_sisters_date_of_birth);
			Assert::IsTrue(my_sisters_date_of_birth > my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_date = dates::date(unsigned int{ 13 }, dates::month::July, unsigned int{ 1979 });
			Assert::IsFalse(earlier_date > later_date);
			Assert::IsTrue(later_date > earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_day = dates::date(unsigned int{ 15 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsFalse(earlier_day > later_day);
			Assert::IsTrue(later_day > earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto same_date_2 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsFalse(same_date_1 > same_date_2);
		}
		TEST_METHOD(TestDatesLessThanEqual) {
			auto my_date_of_birth = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(unsigned int{ 5 }, dates::month::April, unsigned int{ 1981 });
			Assert::IsTrue(my_date_of_birth <= my_sisters_date_of_birth);
			Assert::IsFalse(my_sisters_date_of_birth <= my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_date = dates::date(unsigned int{ 13 }, dates::month::July, unsigned int{ 1979 });
			Assert::IsTrue(earlier_date <= later_date);
			Assert::IsFalse(later_date <= earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_day = dates::date(unsigned int{ 15 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsTrue(earlier_day <= later_day);
			Assert::IsFalse(later_day <= earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto same_date_2 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsTrue(same_date_1 <= same_date_2);
		}
		TEST_METHOD(TestDatesGreaterThanEqual) {
			auto my_date_of_birth = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(unsigned int{ 5 }, dates::month::April, unsigned int{ 1981 });
			Assert::IsFalse(my_date_of_birth >= my_sisters_date_of_birth);
			Assert::IsTrue(my_sisters_date_of_birth >= my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_date = dates::date(unsigned int{ 13 }, dates::month::July, unsigned int{ 1979 });
			Assert::IsFalse(earlier_date >= later_date);
			Assert::IsTrue(later_date >= earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto later_day = dates::date(unsigned int{ 15 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsFalse(earlier_day >= later_day);
			Assert::IsTrue(later_day >= earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto same_date_2 = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			Assert::IsTrue(same_date_1 >= same_date_2);
		}
		TEST_METHOD(TestAssignmentOperator) {
			auto date_of_birth = dates::date(unsigned int{ 13 }, dates::month::June, unsigned int{ 1979 });
			auto date_var = date_of_birth;
			Assert::IsTrue(date_var == date_of_birth);
			Assert::IsTrue(date_var.day() == date_of_birth.day());
			Assert::IsTrue(date_var.month() == date_of_birth.month());
			Assert::IsTrue(date_var.year() == date_of_birth.year());
		}
		TEST_METHOD(TestChangeDates) {
			auto new_date = dates::date(unsigned int{ 23 }, dates::month::July, unsigned int{ 1979 });
			auto current_date = dates::date(unsigned int{ 22 }, dates::month::September, unsigned int{ 2018 })
				
				;
		}
		TEST_METHOD(TestSortingDates) {
			vector<dates::date> datesCollection{ dates::date(unsigned int{23}, dates::month::July, unsigned int{1979}),
												 dates::date(unsigned int{23}, dates::month::January, unsigned int{1979})};
			
			std::sort(datesCollection.begin(), datesCollection.end());
			auto expected_first_element = dates::date(unsigned int{ 23 }, dates::month::January, unsigned int{ 1979 });
			auto first_month = expected_first_element.month();
			auto sorted_month = datesCollection[0].month();
		}
	};
	TEST_CLASS(UnitTestDatesToString) {
		public:
			TEST_METHOD(TestDatesToString) {
				dates::date date_to_test{ 1, dates::month::December, 1979 };
				string expected{ "1 December 1979" };
				auto returned_string{ dates::toString(date_to_test) };
				Logger::WriteMessage(returned_string.c_str());
				Assert::AreEqual(expected, dates::toString(date_to_test));
			}
	};
}