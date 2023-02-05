#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include <random>

#include "../src/util/util.hpp"

namespace calculate
{
    using util::calculate;

    TEST(calculateTest, Multiplication)
    {
        EXPECT_EQ(7 * 6, calculate(7, '*', 6));
        EXPECT_EQ(129 * -77, calculate(129, '*', -77));
        EXPECT_EQ(9876 * 2, calculate(9876, '*', 2));
        EXPECT_EQ(-999 * -91, calculate(-999, '*', -91));
    }

    TEST(calculateTest, Addition)
    {
        EXPECT_EQ(7 + 6, calculate(7, '+', 6));
        EXPECT_EQ(129 + -77, calculate(129, '+', -77));
        EXPECT_EQ(9876 + 2, calculate(9876, '+', 2));
        EXPECT_EQ(-999 + -91, calculate(-999, '+', -91));
    }

    TEST(calculateTest, Substraction)
    {
        EXPECT_EQ(7 - 6, calculate(7, '-', 6));
        EXPECT_EQ(129 - -77, calculate(129, '-', -77));
        EXPECT_EQ(9876 - 2, calculate(9876, '-', 2));
        EXPECT_EQ(-999 - -91, calculate(-999, '-', -91));
    }

    TEST(calculateTest, Division)
    {
        EXPECT_EQ(7 / 6, calculate(7, '/', 6));
        EXPECT_EQ(129 / -77, calculate(129, '/', -77));
        EXPECT_EQ(9876 / 2, calculate(9876, '/', 2));
        EXPECT_EQ(-999 / -91, calculate(-999, '/', -91));
    }

    TEST(calculateTest, RandomisedInputAllOperators)
    {
        for (size_t i = 0; i < 1000; i++)
        {
            const int randomNumber1 = rand() % 100000 - 100000 / 2;
            int randomNumber2 = rand() % 100000 - 100000 / 2;

            EXPECT_EQ(randomNumber1 + randomNumber2, calculate(randomNumber1, '+', randomNumber2));
            EXPECT_EQ(randomNumber1 - randomNumber2, calculate(randomNumber1, '-', randomNumber2));
            EXPECT_EQ(randomNumber1 * randomNumber2, calculate(randomNumber1, '*', randomNumber2));

            if (randomNumber2 == 0)
            {
                randomNumber2 = 1;
            }
            EXPECT_EQ(randomNumber1 / randomNumber2, calculate(randomNumber1, '/', randomNumber2));
        }
    }
}

namespace split_string
{
    using util::split_string;

    std::string random_string(size_t length)
    {
        auto randchar = []() -> char
        {
            const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";

            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }

    std::string join(const std::vector<std::string> &v, char c)
    {
        std::string concatinatedStr;

        for (auto p = v.begin(); p != v.end(); ++p)
        {
            concatinatedStr += *p;
            if (p != v.end() - 1)
                concatinatedStr += c;
        }

        return concatinatedStr;
    }

    TEST(split_stringTest, RandomisedInput)
    {
        for (size_t i = 0; i < 20'000; i++)
        {
            std::vector<std::string> expectedResult;
            const int strCountNum = rand() % 10 + 5;
            expectedResult.resize(strCountNum);

            for (size_t strCount = 0; strCount < strCountNum; strCount++)
            {
                const int strLength = rand() % 10 + 5;
                const auto randString = random_string(strLength);
                expectedResult[strCount] = randString;
            }

            const std::string concatinatedString{join(expectedResult, ',')};

            ASSERT_THAT(split_string(concatinatedString, ','), testing::ContainerEq(expectedResult));
        }
    }

    TEST(split_stringTest, CommaSeparatedStrings)
    {
        ASSERT_THAT(split_string(",,,", ','), testing::ElementsAre("", "", "", ""));
        ASSERT_THAT(split_string("", ','), testing::ElementsAre(""));

        ASSERT_THAT(split_string("30,=C1+G31,0,90,", ','), testing::ElementsAre("30", "=C1+G31", "0", "90", ""));

        ASSERT_THAT(split_string("30,=C1+G31,0,90", ','), testing::ElementsAre("30", "=C1+G31", "0", "90"));
        ASSERT_THAT(split_string(",=C1+G31,0,90", ','), testing::ElementsAre("", "=C1+G31", "0", "90"));
    }
}