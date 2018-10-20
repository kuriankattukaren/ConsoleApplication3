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
	TEST_CLASS(HelperFunctions) {
	public:
		TEST_METHOD(TestIfLeapYear) {
			vector<int> leap_years{ 1992, 1996, 2004, 2008, 2012 };
			vector<int> non_leap_years{ 1993, 2007, 2001 };

			for (auto year : leap_years) Assert::IsTrue(dates::is_leap_year(year));
			for (auto year : non_leap_years) Assert::IsFalse(dates::is_leap_year(year));
		}
		TEST_METHOD(TestDaysInFebLeapYear) 
		{
			Assert::IsTrue(days_in_month(dates::month::February, 1992) == 29);
		}
		TEST_METHOD(TestDaysInFebNonLeapYear) 
		{
			Assert::IsTrue(days_in_month(dates::month::February, 1993) == 28);
		}
		TEST_METHOD(TestDaysInSeptember) 
		{
			Assert::IsTrue(days_in_month(dates::month::September, 1993) == 30);
		}
		TEST_METHOD(TestDaysInOctober)
		{
			Assert::IsTrue(days_in_month(dates::month::October, 1993) == 31);
		}
		TEST_METHOD(TestDateToStringConversion)
		{
			dates::date date_to_test{ 1, dates::month::December, 1979 };
			string expected{ "1 December 1979" };
			auto returned_string{ dates::toString(date_to_test) };
			Logger::WriteMessage(returned_string.c_str());
			Assert::AreEqual(expected, dates::toString(date_to_test));
		}
	};
	TEST_CLASS(Construction){
	public:
		TEST_METHOD(Initialization) 
		{
			int day = 23;
			int year = 1970;

			dates::date date_of_birth{ day, dates::month::February, year };
			Assert::AreEqual(day, date_of_birth.day());
			if (dates::month::February != date_of_birth.month()) 
				Assert::Fail(L"Returned months are not the same.");
			Assert::AreEqual(year, date_of_birth.year());
		}
		TEST_METHOD(TestAssignmentOperator) 
		{
			dates::date birth{ 13 , dates::month::June, 1979 };
			auto copy = birth;
			
			Assert::IsTrue(birth == copy);
			Assert::IsTrue(birth.day() == copy.day());
			Assert::IsTrue(birth.month() == copy.month());
			Assert::IsTrue(birth.year() == copy.year());
		}
		TEST_METHOD(TestValidChangeDates) 
		{
			dates::date original_date{ 23, dates::month::July, 1979 };
			auto original_copy = original_date;
			original_date.changeDate({ 30, dates::month::September, 2018 });

			Assert::IsFalse(original_date == original_copy);
			Assert::IsFalse(original_date.day() == original_copy.day());
			Assert::IsFalse(original_date.month() == original_copy.month());
			Assert::IsFalse(original_date.year() == original_copy.year());
		}
		TEST_METHOD(TestInvalidChangeDates)
		{
			dates::date original_date{ 23, dates::month::July, 1979 };
			auto original_copy = original_date;
			try {
				original_date.changeDate({ 45, dates::month::September, 2018 });
			}
			catch (std::invalid_argument) {}
			catch (...) {
				auto error = string{ "Wrong exception raised." };
				Assert::Fail();
			}
		}
		TEST_METHOD(TestSortingDates) 
		{
			vector<dates::date> datesCollection{ dates::date(int{23}, dates::month::July, int{1979}),
												 dates::date(int{23}, dates::month::January, int{1979})};
			
			std::sort(datesCollection.begin(), datesCollection.end());
			auto expected_first_element = dates::date(int{ 23 }, dates::month::January, int{ 1979 });
			auto first_month = expected_first_element.month();
			auto sorted_month = datesCollection[0].month();
		}
	};
	TEST_CLASS(LogicalOperators) {
	public:
		TEST_METHOD(TestDatesEqual) {
			auto my_date_of_birth = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(int{ 5 }, dates::month::April, int{ 1981 });
			Assert::IsFalse(my_date_of_birth == my_sisters_date_of_birth);
			Assert::IsTrue(my_date_of_birth == my_date_of_birth);
		}
		TEST_METHOD(TestDatesLessThan) {
			auto my_date_of_birth = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(int{ 5 }, dates::month::April, int{ 1981 });
			Assert::IsTrue(my_date_of_birth < my_sisters_date_of_birth);
			Assert::IsFalse(my_sisters_date_of_birth < my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_date = dates::date(int{ 13 }, dates::month::July, int{ 1979 });
			Assert::IsTrue(earlier_date < later_date);
			Assert::IsFalse(later_date < earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_day = dates::date(int{ 15 }, dates::month::June, int{ 1979 });
			Assert::IsTrue(earlier_day < later_day);
			Assert::IsFalse(later_day < earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto same_date_2 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			Assert::IsFalse(same_date_1 < same_date_2);

			auto date_1 = dates::date(int{ 23 }, dates::month::July, int{ 1979 });
			auto date_2 = dates::date(int{ 23 }, dates::month::January, int{ 1979 });
			Assert::IsTrue(date_2 < date_1);
			Assert::IsFalse(date_2 > date_1);
		}
		TEST_METHOD(TestDatesGreaterThan) {
			auto my_date_of_birth = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(int{ 5 }, dates::month::April, int{ 1981 });
			Assert::IsFalse(my_date_of_birth > my_sisters_date_of_birth);
			Assert::IsTrue(my_sisters_date_of_birth > my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_date = dates::date(int{ 13 }, dates::month::July, int{ 1979 });
			Assert::IsFalse(earlier_date > later_date);
			Assert::IsTrue(later_date > earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_day = dates::date(int{ 15 }, dates::month::June, int{ 1979 });
			Assert::IsFalse(earlier_day > later_day);
			Assert::IsTrue(later_day > earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto same_date_2 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			Assert::IsFalse(same_date_1 > same_date_2);
		}
		TEST_METHOD(TestDatesLessThanEqual) {
			auto my_date_of_birth = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(int{ 5 }, dates::month::April, int{ 1981 });
			Assert::IsTrue(my_date_of_birth <= my_sisters_date_of_birth);
			Assert::IsFalse(my_sisters_date_of_birth <= my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_date = dates::date(int{ 13 }, dates::month::July, int{ 1979 });
			Assert::IsTrue(earlier_date <= later_date);
			Assert::IsFalse(later_date <= earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_day = dates::date(int{ 15 }, dates::month::June, int{ 1979 });
			Assert::IsTrue(earlier_day <= later_day);
			Assert::IsFalse(later_day <= earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto same_date_2 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			Assert::IsTrue(same_date_1 <= same_date_2);
		}
		TEST_METHOD(TestDatesGreaterThanEqual) {
			auto my_date_of_birth = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto my_sisters_date_of_birth = dates::date(int{ 5 }, dates::month::April, int{ 1981 });
			Assert::IsFalse(my_date_of_birth >= my_sisters_date_of_birth);
			Assert::IsTrue(my_sisters_date_of_birth >= my_date_of_birth);

			//Tests dates having the same day and year but different months.
			auto earlier_date = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_date = dates::date(int{ 13 }, dates::month::July, int{ 1979 });
			Assert::IsFalse(earlier_date >= later_date);
			Assert::IsTrue(later_date >= earlier_date);

			//Tests dates having the same month and year but different dates.
			auto earlier_day = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto later_day = dates::date(int{ 15 }, dates::month::June, int{ 1979 });
			Assert::IsFalse(earlier_day >= later_day);
			Assert::IsTrue(later_day >= earlier_day);

			//Tests dates having the same day, month and year.
			auto same_date_1 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			auto same_date_2 = dates::date(int{ 13 }, dates::month::June, int{ 1979 });
			Assert::IsTrue(same_date_1 >= same_date_2);
		}
	};
}