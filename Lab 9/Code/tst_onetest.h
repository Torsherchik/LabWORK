#ifndef TST_TESTDIVISIONBYZERO_H
#define TST_TESTDIVISIONBYZERO_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lockcontroller.h"
using namespace testing;
using ::testing::Return;

class MockIKeypad : public IKeypad {
 public:
  MOCK_METHOD(bool, isActive, (), (override));
  MOCK_METHOD(void, wait, (), (override));
  MOCK_METHOD(PasswordResponse, requestPassword, (), (override));
};

class MockILatch : public ILatch {
 public:
  MOCK_METHOD(bool, isActive, (), (override));
  MOCK_METHOD(DoorStatus, open, (), (override));
  MOCK_METHOD(DoorStatus, close, (), (override));
  MOCK_METHOD(DoorStatus, getDoorStatus, (), (override));
};


TEST(TestProg, wait)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);
    EXPECT_CALL(keypad, wait())
            .Times(1);
    lc.wait();
}

TEST(TestProg, isDoorOpen_lock)
{
    MockIKeypad keypad;
       MockILatch latch;
       LockController lc = LockController(&keypad, &latch);


       EXPECT_CALL(latch, getDoorStatus())
               .Times(1)
               .WillOnce(Return(DoorStatus::CLOSE));
       bool res = lc.isDoorOpen();
       EXPECT_FALSE(res);
}

TEST(TestProg, isDoorOpen_unlock)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);


    EXPECT_CALL(latch, getDoorStatus())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));
    bool res = lc.isDoorOpen();
    EXPECT_TRUE(res);
}

TEST(TestProg, unlockDoor)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);


    EXPECT_CALL(latch, open())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    EXPECT_EQ(lc.unlockDoor(), DoorStatus::OPEN);
}

TEST(TestProg, lockDoor)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);

    EXPECT_CALL(latch, close())
            .Times(1)
            .WillOnce(Return(DoorStatus::CLOSE));

    EXPECT_EQ(lc.lockDoor(), DoorStatus::CLOSE);
}

TEST(TestProg, hardWareCheck_ok)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);

    EXPECT_CALL(keypad, isActive())
            .Times(1)
            .WillOnce(Return(true));

    EXPECT_CALL(latch, isActive())
            .Times(1)
            .WillOnce(Return(true));

    auto res = lc.hardWareCheck();
    EXPECT_EQ(res, HardWareStatus::OK);
}

TEST(TestProg, hardWareCheck_error)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(nullptr, &latch);

    EXPECT_CALL(keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    auto res = lc.hardWareCheck();
    EXPECT_EQ(res, HardWareStatus::ERROR);
}

TEST(TestProg, hardWareCheck_error_2)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);

    EXPECT_CALL(keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(false));

    auto res = lc.hardWareCheck();
    EXPECT_EQ(res, HardWareStatus::ERROR);
}

TEST(TestProg, isCorrectPassword)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);

    PasswordResponse ans;
    ans.status = PasswordResponse::Status::OK;
    ans.password = "0000";

    EXPECT_CALL(keypad, requestPassword())
            .Times(1)
            .WillOnce(Return(ans));

    auto res = lc.isCorrectPassword();
    EXPECT_TRUE(res);
}

TEST(TestProg, isCorrectPassword_error)
{
    MockIKeypad keypad;
    MockILatch latch;
    LockController lc = LockController(&keypad, &latch);

    PasswordResponse ans;
    ans.status = PasswordResponse::Status::OK;
    ans.password = "1234";

    EXPECT_CALL(keypad, requestPassword())
            .Times(1)
            .WillOnce(Return(ans));

    auto res = lc.isCorrectPassword();
    EXPECT_FALSE(res);
}

TEST(TestProg, resetPassword)
{
    MockIKeypad keypad;
       MockILatch latch;
       LockController contrl = LockController(&keypad,&latch);
       PasswordResponse oldPass{PasswordResponse::Status::OK,"0000"};
       PasswordResponse newPass{PasswordResponse::Status::OK,"2503"};
       EXPECT_CALL(keypad, requestPassword())
               .Times(2)
               .WillOnce(Return(oldPass))
               .WillOnce((Return(newPass)));
       contrl.resetPassword();
       EXPECT_CALL(keypad,requestPassword())
               .Times(1)

               .WillOnce(Return(newPass));
       bool Return = contrl.isCorrectPassword();
       EXPECT_TRUE(Return);
}

TEST(TestProg, resetPassword_2)
{
    MockIKeypad keypad;
        MockILatch latch;
        LockController contrl = LockController(&keypad,&latch);
        PasswordResponse constPass{PasswordResponse::Status::OK,"0000"};
        PasswordResponse oldPass{PasswordResponse::Status::OK,"2503"};
        PasswordResponse newPass{PasswordResponse::Status::OK,"2108"};

        EXPECT_CALL(keypad, requestPassword())
                .Times(2)
                .WillOnce(Return(constPass))
                .WillOnce((Return(oldPass)));
        contrl.resetPassword();

        EXPECT_CALL(keypad,requestPassword())
                .Times(1)
                .WillOnce(Return(oldPass));
        bool returned = contrl.isCorrectPassword();
        EXPECT_TRUE(returned);

        EXPECT_CALL(keypad,requestPassword())
                .Times(2)
                .WillOnce(Return(oldPass))
                .WillOnce(Return(newPass));
        contrl.resetPassword();
        EXPECT_CALL(keypad,requestPassword())
                .Times(1)
                .WillOnce(Return(newPass));
        returned =contrl.isCorrectPassword();
        EXPECT_TRUE(returned);
}


#endif // TST_TESTDIVISIONBYZERO_H
