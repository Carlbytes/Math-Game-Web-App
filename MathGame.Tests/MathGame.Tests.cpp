#include "pch.h"
#include "CppUnitTest.h"

#define RUNNING_TESTS 

#include "C:\College Stuff\Year Three\Y3 Reposits\SoftwareDev3Agile\SoftwareDevelopment3GP\carlsMathGame\math_game.cpp" 
// ---------------------------------


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathGameTests
{
    TEST_CLASS(MathLogicTests)
    {
    public:

        // Your test for getRandomInt
        TEST_METHOD(TestRandomSystem_Utilities)
        {
            std::mt19937 test_rng(42);
            // Your discovered numbers:
            Assert::AreEqual(38, getRandomInt(test_rng, 1, 100));
            Assert::AreEqual(80, getRandomInt(test_rng, 1, 100));
            Assert::AreEqual(96, getRandomInt(test_rng, 1, 100));
        }

        // Your test for the Addition System
        TEST_METHOD(TestAdditionSystem)
        {
            std::mt19937 test_rng(42);
            MathProblem p = generateAdditionProblem(test_rng);
            // Your discovered numbers:
            Assert::AreEqual(8, p.num1);
            Assert::AreEqual(16, p.num2);
            Assert::AreEqual(24, p.correctAnswer);
            Assert::AreEqual('+', p.op);
        }

        // Your test for the Subtraction System
        TEST_METHOD(TestSubtractionSystem)
        {
            std::mt19937 test_rng(42);
            MathProblem p = generateSubtractionProblem(test_rng);
            // Your discovered numbers:
            Assert::AreEqual(8, p.num1);
            Assert::AreEqual(16, p.num2);
            Assert::AreEqual(-8, p.correctAnswer);
            Assert::AreEqual('-', p.op);
        }

        // Your test for the Multiplication System
        TEST_METHOD(TestMultiplicationSystem)
        {
            std::mt19937 test_rng(42);
            MathProblem p = generateMultiplicationProblem(test_rng);
            // Your discovered numbers:
            Assert::AreEqual(8, p.num1);
            Assert::AreEqual(16, p.num2);
            Assert::AreEqual(128, p.correctAnswer);
            Assert::AreEqual('*', p.op);
        }

        // Your test for the Division System
        TEST_METHOD(TestDivisionSystem)
        {
            std::mt19937 test_rng(42);
            MathProblem p = generateDivisionProblem(test_rng);
            // Your discovered numbers:
            Assert::AreEqual(6, p.num2);
            Assert::AreEqual(10, p.correctAnswer);
            Assert::AreEqual(60, p.num1);
            Assert::AreEqual('/', p.op);
        }

        // Your test for Orb Generation
        TEST_METHOD(TestRandomSystem_OrbGeneration)
        {
            std::mt19937 test_rng(42);
            std::vector<int> orbs = generateOrbs(test_rng, 100);
            // Your discovered numbers:
            Assert::AreEqual(3, (int)orbs.size());
            Assert::AreEqual(98, orbs[0]);
            Assert::AreEqual(104, orbs[1]);
            Assert::AreEqual(100, orbs[2]);
        }

        // Your test for the main dispatcher
        TEST_METHOD(TestRandomSystem_Dispatcher)
        {
            std::mt19937 test_rng(42);
            MathProblem p1 = generateProblem(test_rng);
            MathProblem p2 = generateProblem(test_rng);
            MathProblem p3 = generateProblem(test_rng);
            // Your discovered ops:
            Assert::AreEqual('-', p1.op);
            Assert::AreEqual('+', p2.op);
            Assert::AreEqual('*', p3.op);
        }
    };
}