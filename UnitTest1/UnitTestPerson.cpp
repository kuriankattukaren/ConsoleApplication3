#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication3/testingfile.h"
#include "../ConsoleApplication3/person.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestPerson
{		
	TEST_CLASS(UnitTestPerson)
	{
	public:
		TEST_METHOD(TestRemoveLeadingWhiteSpace) {
			std::string name{ "    Kurian Kattukaren" };
			auto edited_name{remove_leading_whitespace(name)};
			Assert::AreEqual(edited_name, string("Kurian Kattukaren"));
		}
		TEST_METHOD(TestRemoveLeadingWhiteSpaceNoLeadingWhiteSpace) {
			std::string name{ "Kurian Kattukaren" };
			auto edited_name{remove_leading_whitespace(name)};
			Assert::AreEqual(edited_name, name);
		}
		TEST_METHOD(TestRemoveTrailingWhiteSpace) {
			std::string name{ "Kurian Kattukaren   " };
			std::string expected_name{ "Kurian Kattukaren" };
			auto edited_name{remove_trailing_whitespace(name)};
			Assert::AreEqual(expected_name, edited_name);
		}
		TEST_METHOD(TestRemoveTrailingWhiteSpaceNoTrailingWhiteSpace) {
			std::string name{ "Kurian Kattukaren" };
			auto edited_name{remove_trailing_whitespace(name) };
			Assert::AreEqual(edited_name, name);
		}
		TEST_METHOD(TestNameSplit)
		{
			std::string name{ "Kurian Kattukaren" };
			std::string first_name{ "Kurian" };
			std::string second_name{ "Kattukaren" };

			auto results = splitname(name);
			Assert::AreEqual(first_name, std::string(results[0]));
			Assert::AreEqual(second_name, std::string(results[1]));
		}
		TEST_METHOD(TestPersonClassValidInit) {
			//Expected Input
			std::string mothers_name{ "Rosamma Joseph" };
			std::string mothers_first_name{ "Rosamma" };
			std::string mothers_second_name{ "Joseph" };
			auto mother = person(mothers_name);
			Assert::AreEqual(mothers_name, mother.name());
			Assert::AreEqual(mothers_first_name, mother.firstName());
			Assert::AreEqual(mothers_second_name, mother.secondName());

			//Tests names with leading and trailing whitespaces.
			std::string sister_name{ "    Rosen    Joseph    " };
			std::string expected_name{ "Rosen Joseph" };
			std::string sister_first_name{ "Rosen" };
			std::string sister_second_name{ "Joseph" };

			auto sister = person(sister_name);
			Assert::AreEqual(expected_name, sister.name());
			Assert::AreEqual(sister_first_name, sister.firstName());
			Assert::AreEqual(sister_second_name, sister.secondName());

			//Tests when only single name is provided
			const wchar_t* message = L"Did not raise invalid_argument exception.";

			try {
				std::string invalid_name{ "   Rosen  " };
				auto name = person{ invalid_name };
			}
			catch (std::invalid_argument) { Logger::WriteMessage("Exception occurred."); }
			catch (...) {
				Assert::Fail(message);
			}
		}
		TEST_METHOD(TestPersonInitNameWithLeadingTrailingSpaces) {
			//Tests names with leading and trailing whitespaces.
			std::string sister_name{ "    Rosen    Joseph    " };
			std::string expected_name{ "Rosen Joseph" };
			std::string sister_first_name{ "Rosen" };
			std::string sister_second_name{ "Joseph" };

			auto sister = person(sister_name);
			Assert::AreEqual(expected_name, sister.name());
			Assert::AreEqual(sister_first_name, sister.firstName());
			Assert::AreEqual(sister_second_name, sister.secondName());
		}
		TEST_METHOD(TestPersonInvalidName) {
			//Tests when only single name is provided
			const wchar_t* message = L"Did not raise invalid_argument exception.";

			try {
				std::string invalid_name{ "   Rosen  " };
				auto name = person{ invalid_name };
			}
			catch (std::invalid_argument) { Logger::WriteMessage("Exception occurred."); }
			catch (...) {
				Assert::Fail(message);
			}
		}
		TEST_METHOD(TestPersonClassEquality) {
			std::string fathers_name_1{ "Jose Kurian" };
			std::string fathers_name_2{ "Jose Kurian" };
			std::string mothers_name{ "Rosamma Joseph" };

			auto father_1 = person(fathers_name_1);
			auto father_2 = person(fathers_name_2);
			auto mother = person(mothers_name);

			Assert::IsTrue(father_1 == father_2);
			Assert::IsFalse(father_1 == mother);
		}
		TEST_METHOD(TestPersonClassLessThan) {
			std::string fathers_name{ "Jose Kurian" };
			std::string mothers_name{ "Rosamma Joseph" };

			auto father = person(fathers_name);
			auto mother = person(mothers_name);

			Assert::IsTrue(father < mother);
			Assert::IsFalse(mother < father);
			Assert::IsFalse(father < father);
		}
		TEST_METHOD(TestPersonClassLessThanEqual){
			std::string fathers_name{ "Jose Kurian" };
			std::string mothers_name{ "Rosamma Joseph" };

			auto father = person(fathers_name);
			auto mother = person(mothers_name);

			Assert::IsTrue(father <= mother);
			Assert::IsFalse(mother <= father);
			Assert::IsTrue(father <= father);
			}
		TEST_METHOD(TestPersonClassGreaterThan) {
			std::string fathers_name{ "Jose Kurian" };
			std::string mothers_name{ "Rosamma Joseph" };

			auto father = person(fathers_name);
			auto mother = person(mothers_name);

			Assert::IsTrue(mother > father);
			Assert::IsFalse(father > mother);
			Assert::IsFalse(father > father);
		}
		TEST_METHOD(TestPersonClassGreatherThanEqual) {
			std::string fathers_name{ "Jose Kurian" };
			std::string mothers_name{ "Rosamma Joseph" };

			auto father = person(fathers_name);
			auto mother = person(mothers_name);

			Assert::IsTrue(mother >= father);
			Assert::IsFalse(father >= mother);
		}
	};
}