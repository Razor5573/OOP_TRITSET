#include <gtest\gtest.h>
#include "TritSet.h"

namespace googleTests {

    static const size_t LARGE_SET_SIZE = 999999;

    class TritSetFixtures : public ::testing::Test {
    protected:
        void SetUp() override {
            setA = new TernaryLogic::TritSet();
            setB = new TernaryLogic::TritSet();

            currentTritIndex = 0;
        }

        void TearDown() override {
            delete setA;
            delete setB;
        }

        TernaryLogic::TritSet *setA{};
        TernaryLogic::TritSet *setB{};

        size_t currentTritIndex{};
    };

    TEST_F(TritSetFixtures, ASSIGNMENT) {
        (*setA)[0] = TernaryLogic::Trit::False;
        (*setA)[1] = TernaryLogic::Trit::Unknown;

        (*setB)[0] = TernaryLogic::Trit::True;
        (*setB)[1] = TernaryLogic::Trit::True;

        (*setA)[0] = (*setB)[1];
        (*setB)[1] = (*setA)[1];

        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[0]);
        EXPECT_EQ(TernaryLogic::Trit::Unknown, (TernaryLogic::Trit) (*setB)[1]);
    }

    TEST_F(TritSetFixtures, AND) {
        (*setA)[0] = TernaryLogic::Trit::False;
        (*setA)[1] = TernaryLogic::Trit::True;
        (*setA)[2] = TernaryLogic::Trit::True;

        (*setB)[0] = TernaryLogic::Trit::Unknown;
        (*setB)[1] = TernaryLogic::Trit::True;
        (*setB)[2] = TernaryLogic::Trit::Unknown;

        *setA = *setA & *setB;

        EXPECT_EQ(TernaryLogic::Trit::False, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::Unknown, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
    }

    TEST_F(TritSetFixtures, OR) {
        (*setA)[0] = TernaryLogic::Trit::False;
        (*setA)[1] = TernaryLogic::Trit::True;
        (*setA)[2] = TernaryLogic::Trit::True;

        (*setB)[0] = TernaryLogic::Trit::Unknown;
        (*setB)[1] = TernaryLogic::Trit::False;
        (*setB)[2] = TernaryLogic::Trit::Unknown;

        *setA = *setA | *setB;

        EXPECT_EQ(TernaryLogic::Trit::Unknown, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
    }

    TEST_F(TritSetFixtures, NOT) {
        (*setA)[0] = TernaryLogic::Trit::False;
        (*setA)[1] = TernaryLogic::Trit::True;
        (*setA)[2] = TernaryLogic::Trit::Unknown;

        *setA = ~(*setA);

        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::False, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::Unknown, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
    }

    TEST_F(TritSetFixtures, SETTING_TRIT_VALUE) {
        (*setA)[0] = TernaryLogic::Trit::False;
        (*setA)[1] = TernaryLogic::Trit::True;
        (*setA)[2] = TernaryLogic::Trit::Unknown;

        EXPECT_EQ(TernaryLogic::Trit::False, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::True, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
        EXPECT_EQ(TernaryLogic::Trit::Unknown, (TernaryLogic::Trit) (*setA)[currentTritIndex++]);
    }

    TEST_F(TritSetFixtures, GETTING_TRIT_VALUE) {
        (*setA)[0] = TernaryLogic::Trit::False;
        auto tritValue = (TernaryLogic::Trit) (*setA)[0];
        EXPECT_EQ(TernaryLogic::Trit::False, tritValue);
    }

    TEST_F(TritSetFixtures, GETTING_SET_SIZE) {
        EXPECT_EQ(0, (*setA).getSize());
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        EXPECT_EQ(1, (*setA).getSize());
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::Unknown;
        EXPECT_EQ(1, (*setA).getSize());
        currentTritIndex = 8;
        (*setA)[currentTritIndex] = TernaryLogic::Trit::False;
        EXPECT_EQ(9, (*setA).getSize());
        (*setA)[currentTritIndex] = TernaryLogic::Trit::Unknown;
        EXPECT_EQ(9, (*setA).getSize());
    }

    TEST_F(TritSetFixtures, CAPACITY) {
        EXPECT_EQ(0, (*setA).capacity());
        (*setA)[currentTritIndex] = TernaryLogic::Trit::True;
        EXPECT_EQ(16, (*setA).capacity());
        currentTritIndex = 6;
        (*setA)[currentTritIndex] = TernaryLogic::Trit::False;
        EXPECT_EQ(16, (*setA).capacity());
        currentTritIndex = 20;
        (*setA)[currentTritIndex] = TernaryLogic::Trit::Unknown;
        EXPECT_EQ(16, (*setA).capacity());
        currentTritIndex = 25;
        (*setA)[currentTritIndex] = TernaryLogic::Trit::True;
        EXPECT_EQ(32, (*setA).capacity());
    }

    TEST_F(TritSetFixtures, LENGTH) {
        EXPECT_EQ(0, (*setA).length());
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        EXPECT_EQ(1, (*setA).length());
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::Unknown;
        EXPECT_EQ(1, (*setA).length());
        (*setA)[currentTritIndex] = TernaryLogic::Trit::False;
        EXPECT_EQ(3, (*setA).length());
    }

    TEST_F(TritSetFixtures, INDIVIDUAL_CARDINALITY) {
        EXPECT_EQ(0, (*setA).cardinality(TernaryLogic::Trit::True));
        EXPECT_EQ(0, (*setA).cardinality(TernaryLogic::Trit::False));
        EXPECT_EQ(0, (*setA).cardinality(TernaryLogic::Trit::Unknown));
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::Unknown;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::True));
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::False));
        EXPECT_EQ(1, (*setA).cardinality(TernaryLogic::Trit::Unknown));
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::Unknown;
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::True));
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::False));
        EXPECT_EQ(1, (*setA).cardinality(TernaryLogic::Trit::Unknown));
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::True));
        EXPECT_EQ(3, (*setA).cardinality(TernaryLogic::Trit::False));
        EXPECT_EQ(2, (*setA).cardinality(TernaryLogic::Trit::Unknown));
    }

    TEST_F(TritSetFixtures, SHRINK) {
        EXPECT_EQ(0, (*setA).cardinality(TernaryLogic::Trit::True));
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        (*setA).shrink();
        EXPECT_EQ(2, (*setA).getSize());
        currentTritIndex = 20;
        (*setA)[currentTritIndex] = TernaryLogic::Trit::False;
        (*setA).shrink();
        EXPECT_EQ(21, (*setA).getSize());
        (*setA)[currentTritIndex] = TernaryLogic::Trit::Unknown;
        (*setA).shrink();
        EXPECT_EQ(2, (*setA).getSize());

        size_t tempSetASize = 7;
        TernaryLogic::TritSet tempSetA(7);
        *setA = tempSetA;
        (*setA).shrink();
        EXPECT_EQ(7, (*setA).getSize());
    }

    TEST_F(TritSetFixtures, TRIM) {
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::True;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::Unknown;
        (*setA)[currentTritIndex++] = TernaryLogic::Trit::False;
        (*setA).trim(currentTritIndex);
        EXPECT_EQ(5, (*setA).getSize());
        currentTritIndex = 2;
        (*setA).trim(currentTritIndex);
        EXPECT_EQ(3, (*setA).getSize());
    }

    TEST_F(TritSetFixtures, LARGE_TEST) {
        TernaryLogic::TritSet largeSet(LARGE_SET_SIZE);
        (*setA) = largeSet;

        for (int currentTritIndex = 0; currentTritIndex < (*setA).getSize(); currentTritIndex++) {
            if (currentTritIndex < (*setA).getSize() / 3) {
                (*setA)[currentTritIndex] = TernaryLogic::Trit::True;
            } else if (currentTritIndex >= 2 * (*setA).getSize() / 3) {
                (*setA)[currentTritIndex] = TernaryLogic::Trit::False;
            } else {
                continue;
            }
        }

        EXPECT_EQ(LARGE_SET_SIZE / 3, (*setA).cardinality(TernaryLogic::Trit::True));
        EXPECT_EQ(LARGE_SET_SIZE / 3, (*setA).cardinality(TernaryLogic::Trit::False));
        EXPECT_EQ(LARGE_SET_SIZE / 3, (*setA).cardinality(TernaryLogic::Trit::Unknown));
    }

    TEST_F(TritSetFixtures, AS_CONST){
        (*setA)[10] = TernaryLogic::True;
        const TernaryLogic::TritSet& setC = *setA;
        EXPECT_EQ(TernaryLogic::True, setC[10]);
    }

    TEST_F(TritSetFixtures, COPY_SET_TO_SET) {
        for(int i = 0; i < 10; i++){
            (*setA)[i] = TernaryLogic::True;
        }
        TernaryLogic::TritSet setC = *setA;
        EXPECT_EQ(setC.cardinality(TernaryLogic::True), 10);
    }

    TEST(Iterator, RANGE_BASED1) {
        TernaryLogic::TritSet setA(1000);
        for (auto it: setA) {
            it = TernaryLogic::True;
        }

        EXPECT_EQ(setA.cardinality(TernaryLogic::True), 0);
    }

    TEST(Iterator, RANGE_BASED2) {
        TernaryLogic::TritSet setA(1000);
        for (auto &it: setA) {
            it = TernaryLogic::True;
        }

        EXPECT_EQ(setA.cardinality(TernaryLogic::True), 1000);
    }

    TEST(Iterator, RANGE_BASED3) {
        TernaryLogic::TritSet setA(1000);
        for (auto &&it: setA) {
            it = TernaryLogic::True;
        }

        EXPECT_EQ(setA.cardinality(TernaryLogic::True), 1000);
    }
}