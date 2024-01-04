#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestSnake
{
	TEST_CLASS(TestGame)
	{
	public:
		
		const COORD left = { -1, 0 };
		const COORD up = { 0, -1 };
		const COORD right = { 1, 0 };
		const COORD down = { 0, 1 };
		TEST_METHOD(Test_init_snake)
		{
			SNAKE snake = { 0 };

			init_snake(&snake);

			Assert::AreEqual(TRUE, snake.is_alive);
			Assert::AreEqual(FALSE, snake.has_eaten);
		}

		TEST_METHOD(Test_change_snake_direction) {
			SNAKE snake = { 0 };

			change_snake_direction(&snake, 'a');
			Assert::IsTrue(snake.direction.X == left.X && snake.direction.Y == left.Y);
			change_snake_direction(&snake, 'A');
			Assert::IsTrue(snake.direction.X == left.X && snake.direction.Y == left.Y);
			change_snake_direction(&snake, 'w');
			Assert::IsTrue(snake.direction.X == up.X && snake.direction.Y == up.Y);
			change_snake_direction(&snake, 'W');
			Assert::IsTrue(snake.direction.X == up.X && snake.direction.Y == up.Y);
			change_snake_direction(&snake, 'd');
			Assert::IsTrue(snake.direction.X == right.X && snake.direction.Y == right.Y);
			change_snake_direction(&snake, 'D');
			Assert::IsTrue(snake.direction.X == right.X && snake.direction.Y == right.Y);
			change_snake_direction(&snake, 's');
			Assert::IsTrue(snake.direction.X == down.X && snake.direction.Y == down.Y);
			change_snake_direction(&snake, 'S');
			Assert::IsTrue(snake.direction.X == down.X && snake.direction.Y == down.Y);
			change_snake_direction(&snake, 'k'); //random key, should not change
			Assert::IsTrue(snake.direction.X == down.X && snake.direction.Y == down.Y);
		}

		TEST_METHOD(Test_update_food) {
			SNAKE snake = { 0 };
			COORD size = { 5, 2 };
			COORD food = { 0 };
			
			// available space
			Assert::IsTrue(update_food(&food, size, &snake));
			snake.size = 10;
			// no available space
			Assert::IsFalse(update_food(&food, size, &snake));
		}
	};
}
