#include "../../../src/behavioral/chain_of_responsibility/ChainOfResponsibility.h"
#include <gtest/gtest.h>

// 责任链模式测试套件

// 测试日志处理链 - DEBUG级别
TEST(ChainOfResponsibilityTest, LogChain_DebugLevel) {
    auto consoleHandler = std::make_shared<ConsoleLogHandler>(LogLevel::DEBUG);
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::DEBUG, "Debug message"));
}

// 测试日志处理链 - INFO级别
TEST(ChainOfResponsibilityTest, LogChain_InfoLevel) {
    auto consoleHandler = std::make_shared<ConsoleLogHandler>(LogLevel::INFO);
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::INFO, "Info message"));
}

// 测试日志处理链 - 链式传递
TEST(ChainOfResponsibilityTest, LogChain_ChainedHandlers) {
    auto consoleHandler = std::make_shared<ConsoleLogHandler>(LogLevel::INFO);
    auto fileHandler = std::make_shared<FileLogHandler>(LogLevel::WARN);

    consoleHandler->SetNext(fileHandler);
    
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::INFO, "Info message"));
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::WARN, "Warn message"));
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::ERROR, "Error message"));
}

// 测试RunLogChainDemo
TEST(ChainOfResponsibilityTest, RunLogChainDemo) {
    EXPECT_NO_THROW(RunLogChainDemo());
}

// 测试请假审批链 - 1天
TEST(ChainOfResponsibilityTest, LeaveApproval_OneDayRequest) {
    auto leader = std::make_shared<TeamLeader>("Leader");
    LeaveRequest request{"Alice", 1};

    EXPECT_NO_THROW(leader->HandleRequest(request));
}

// 测试请假审批链 - 2天
TEST(ChainOfResponsibilityTest, LeaveApproval_TwoDaysRequest) {
    auto leader = std::make_shared<TeamLeader>("Leader");
    auto manager = std::make_shared<Manager>("Manager");

    leader->SetNext(manager);

    LeaveRequest request{"Bob", 2};
    EXPECT_NO_THROW(leader->HandleRequest(request));
}

// 测试请假审批链 - 5天
TEST(ChainOfResponsibilityTest, LeaveApproval_FiveDaysRequest) {
    auto leader = std::make_shared<TeamLeader>("Leader");
    auto manager = std::make_shared<Manager>("Manager");
    auto director = std::make_shared<Director>("Director");

    leader->SetNext(manager);
    manager->SetNext(director);

    LeaveRequest request{"Cindy", 5};
    EXPECT_NO_THROW(leader->HandleRequest(request));
}

// 测试请假审批链 - 超过最大天数
TEST(ChainOfResponsibilityTest, LeaveApproval_ExceedsMaxDays) {
    auto leader = std::make_shared<TeamLeader>("Leader");
    auto manager = std::make_shared<Manager>("Manager");
    auto director = std::make_shared<Director>("Director");

    leader->SetNext(manager);
    manager->SetNext(director);

    LeaveRequest request{"David", 10};
    EXPECT_NO_THROW(leader->HandleRequest(request));
}

// 测试RunLeaveApprovalDemo
TEST(ChainOfResponsibilityTest, RunLeaveApprovalDemo) {
    EXPECT_NO_THROW(RunLeaveApprovalDemo());
}

// 测试单个处理者（无后继）
TEST(ChainOfResponsibilityTest, SingleHandler_NoSuccessor) {
    auto consoleHandler = std::make_shared<ConsoleLogHandler>(LogLevel::INFO);
    
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::WARN, "Warning"));
    EXPECT_NO_THROW(consoleHandler->Handle(LogLevel::DEBUG, "Debug"));
}

// 测试链的动态修改
TEST(ChainOfResponsibilityTest, DynamicChainModification) {
    auto handler1 = std::make_shared<ConsoleLogHandler>(LogLevel::INFO);
    auto handler2 = std::make_shared<FileLogHandler>(LogLevel::WARN);
    auto handler3 = std::make_shared<ConsoleLogHandler>(LogLevel::ERROR);
    
    handler1->SetNext(handler2);
    EXPECT_NO_THROW(handler1->Handle(LogLevel::INFO, "Message 1"));
    
    handler2->SetNext(handler3);
    EXPECT_NO_THROW(handler1->Handle(LogLevel::ERROR, "Message 2"));
}