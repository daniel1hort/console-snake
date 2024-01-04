#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSnake
{
	TEST_CLASS(TestUtils)
	{
	public:

		CHAR_INFO buffer[5 * 5];
		COORD size = { 5, 5 };
		CHAR_INFO style = { ' ', BACKGROUND_GREEN };

		TEST_METHOD_INITIALIZE(init) {
			memset(buffer, 0, sizeof(CHAR_INFO) * 5 * 5);
		}

		TEST_METHOD(Test_no_rng) {
			int a, b;

			no_rng();
			a = rand();
			no_rng();
			b = rand();

			Assert::AreEqual(a, b);
		}

		TEST_METHOD(Test_set_coord) {
			COORD coord;

			set_coord(&coord, 6, 75);

			Assert::AreEqual(6, (int)coord.X);
			Assert::AreEqual(75, (int)coord.Y);
		}

		TEST_METHOD(Test_set_rect) {
			SMALL_RECT rect;
			
			set_rect(&rect, 46, 22, 79, 126);

			Assert::AreEqual(46, (int)rect.Left);
			Assert::AreEqual(22, (int)rect.Top);
			Assert::AreEqual(79, (int)rect.Right);
			Assert::AreEqual(126, (int)rect.Bottom);
		}

		TEST_METHOD(Test_rect_inside_boundaries) {
			Assert::AreEqual(TRUE, rect_inside_boundaries({ 0, 1, 2, 4 }, size));
			Assert::AreEqual(TRUE, rect_inside_boundaries({ 0, 0, 4, 4 }, size));
			Assert::AreEqual(FALSE, rect_inside_boundaries({ 3, 0, 2, 2 }, size)); //left bigger than right
			Assert::AreEqual(FALSE, rect_inside_boundaries({ 0, 3, 2, 2 }, size)); //top bigger than bottom
			Assert::AreEqual(FALSE, rect_inside_boundaries({ -6, 1, 2, 4 }, size)); //left less than 0
			Assert::AreEqual(FALSE, rect_inside_boundaries({ 1, -28, 2, 4 }, size)); //top less than 0
			Assert::AreEqual(FALSE, rect_inside_boundaries({ 1, 1, 12, 4 }, size)); //right bigger than size.X
			Assert::AreEqual(FALSE, rect_inside_boundaries({ 1, 1, 2, 56 }, size)); //bottom bigger than size.Y
			Assert::AreEqual(FALSE, rect_inside_boundaries({ -6, -28, -80, 56 }, size)); //random errors
		}

		TEST_METHOD(Test_draw_rect_center) {
			SMALL_RECT rect = { 2, 2, 2, 2 };
			BOOL result;
			int count = 0;

			result = draw_rect(buffer, size, rect, style);

			Assert::AreEqual(TRUE, result);
			Assert::AreEqual((int)style.Attributes, (int)buffer[5 * 2 + 2].Attributes);
			Assert::AreEqual(style.Char.AsciiChar, buffer[5 * 2 + 2].Char.AsciiChar);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(24, count);
		}

		TEST_METHOD(Test_draw_rect_in) {
			SMALL_RECT rect = { 1, 1, 3, 4 };
			BOOL result;
			int count = 0;

			result = draw_rect(buffer, size, rect, style);

			Assert::AreEqual(TRUE, result);
			for (int i = rect.Left; i <= rect.Right; i++) {
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * rect.Top + i].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * rect.Top + i].Char.AsciiChar);
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * rect.Bottom + i].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * rect.Bottom + i].Char.AsciiChar);
			}
			for (int i = rect.Top; i <= rect.Bottom; i++) {
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * i + rect.Left].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * i + rect.Left].Char.AsciiChar);
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * i + rect.Right].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * i + rect.Right].Char.AsciiChar);
			}
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(15, count);
		}

		TEST_METHOD(Test_draw_rect_border) {
			SMALL_RECT rect = { 0, 0, 4, 4 };
			BOOL result;
			int count = 0;

			result = draw_rect(buffer, size, rect, style);

			Assert::AreEqual(TRUE, result);
			for (int i = rect.Left; i <= rect.Right; i++) {
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * rect.Top + i].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * rect.Top + i].Char.AsciiChar);
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * rect.Bottom + i].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * rect.Bottom + i].Char.AsciiChar);
			}
			for (int i = rect.Top; i <= rect.Bottom; i++) {
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * i + rect.Left].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * i + rect.Left].Char.AsciiChar);
				Assert::AreEqual((int)style.Attributes, (int)buffer[5 * i + rect.Right].Attributes);
				Assert::AreEqual(style.Char.AsciiChar, buffer[5 * i + rect.Right].Char.AsciiChar);
			}
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(9, count);
		}

		TEST_METHOD(Test_draw_rect_error) {
			SMALL_RECT rect = { -1, 0, 0, 0 }; //outside de boundaries
			BOOL result;
			int count = 0;

			result = draw_rect(buffer, size, rect, style);

			Assert::AreEqual(FALSE, result);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(25, count);
		}

		TEST_METHOD(Test_fill_rect_center) {
			SMALL_RECT rect = { 2, 2, 2, 2 };
			BOOL result;
			int count = 0;

			result = fill_rect(buffer, size, rect, style);

			Assert::AreEqual(TRUE, result);
			Assert::AreEqual((int)style.Attributes, (int)buffer[5 * 2 + 2].Attributes);
			Assert::AreEqual(style.Char.AsciiChar, buffer[5 * 2 + 2].Char.AsciiChar);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(24, count);
		}

		TEST_METHOD(Test_fill_rect_in) {
			SMALL_RECT rect = { 1, 1, 3, 4 };
			BOOL result;
			int count = 0;

			result = fill_rect(buffer, size, rect, style);
			
			Assert::AreEqual(TRUE, result);
			for (int i = rect.Top; i <= rect.Bottom; i++)
				for (int j = rect.Left; j <= rect.Right; j++) {
					Assert::AreEqual((int)style.Attributes, (int)buffer[5 * i + j].Attributes);
					Assert::AreEqual(style.Char.AsciiChar, buffer[5 * i + j].Char.AsciiChar);
				}
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(13, count);
		}

		TEST_METHOD(Test_fill_rect_border) {
			SMALL_RECT rect = { 0, 0, 4, 4 };
			BOOL result;
			int count = 0;

			result = fill_rect(buffer, size, rect, style);

			Assert::AreEqual(TRUE, result);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(0, count);
		}

		TEST_METHOD(Test_fill_rect_error) {
			SMALL_RECT rect = { -1, 0, 0, 0 }; //outside de boundaries
			BOOL result;
			int count = 0;

			result = fill_rect(buffer, size, rect, style);

			Assert::AreEqual(FALSE, result);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (buffer[j * 5 + i].Attributes == 0 && buffer[j * 5 + i].Char.AsciiChar == '\0')
						count++;
			Assert::AreEqual(25, count);
		}
	};
}
