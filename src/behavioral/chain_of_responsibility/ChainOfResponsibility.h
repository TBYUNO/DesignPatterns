#pragma once

#include <iostream>
#include <memory>
#include <string>

// 职责链模式（Chain of Responsibility）C++ 示例
// --------------------------------------------
// 示例 1：日志处理链
// 示例 2：请假审批链

// ===== 示例 1：日志处理链 =====

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

// 抽象日志处理者
class LogHandler {
public:
    explicit LogHandler(LogLevel level) : level_(level) {}
    virtual ~LogHandler() = default;

    void SetNext(std::shared_ptr<LogHandler> next) { next_ = std::move(next); }

    void Handle(LogLevel level, const std::string& message) {
        if (CanHandle(level)) {
            DoHandle(message);
        }
        if (next_) {
            next_->Handle(level, message);
        }
    }

protected:
    virtual bool CanHandle(LogLevel level) const = 0;
    virtual void DoHandle(const std::string& message) = 0;

    LogLevel level_;
    std::shared_ptr<LogHandler> next_;
};

// 控制台日志处理器
class ConsoleLogHandler : public LogHandler {
public:
    explicit ConsoleLogHandler(LogLevel minLevel) : LogHandler(minLevel) {}

protected:
    bool CanHandle(LogLevel level) const override {
        return static_cast<int>(level) >= static_cast<int>(level_);
    }

    void DoHandle(const std::string& message) override {
        std::cout << "[Console] " << message << std::endl;
    }
};

// "文件"日志处理器（示例中用控制台代替文件输出）
class FileLogHandler : public LogHandler {
public:
    explicit FileLogHandler(LogLevel minLevel) : LogHandler(minLevel) {}

protected:
    bool CanHandle(LogLevel level) const override {
        return static_cast<int>(level) >= static_cast<int>(level_);
    }

    void DoHandle(const std::string& message) override {
        std::cout << "[File] " << message << std::endl;
    }
};

inline void RunLogChainDemo() {
    std::cout << "--- Log Chain Demo ---" << std::endl;

    auto consoleHandler = std::make_shared<ConsoleLogHandler>(LogLevel::INFO);
    auto fileHandler = std::make_shared<FileLogHandler>(LogLevel::WARN);

    consoleHandler->SetNext(fileHandler);

    consoleHandler->Handle(LogLevel::DEBUG, "This is a debug message");
    consoleHandler->Handle(LogLevel::INFO, "This is an info message");
    consoleHandler->Handle(LogLevel::WARN, "This is a warn message");
    consoleHandler->Handle(LogLevel::ERROR, "This is an error message");
}

// ===== 示例 2：请假审批链 =====

struct LeaveRequest {
    std::string employeeName;
    int days{}; // 请假天数
};

// 抽象审批者
class Approver {
public:
    explicit Approver(std::string name) : name_(std::move(name)) {}
    virtual ~Approver() = default;

    void SetNext(std::shared_ptr<Approver> next) { next_ = std::move(next); }

    void HandleRequest(const LeaveRequest& request) {
        if (CanApprove(request.days)) {
            Approve(request);
        } else if (next_) {
            next_->HandleRequest(request);
        } else {
            std::cout << "No approver can handle this request\n";
        }
    }

protected:
    virtual bool CanApprove(int days) const = 0;
    virtual void Approve(const LeaveRequest& request) const {
        std::cout << name_ << " approved leave for " << request.employeeName
                  << " (" << request.days << " days)" << std::endl;
    }

    std::string name_;
    std::shared_ptr<Approver> next_;
};

// 组长：可审批 1 天以内
class TeamLeader : public Approver {
public:
    explicit TeamLeader(const std::string& name) : Approver(name) {}

protected:
    bool CanApprove(int days) const override { return days <= 1; }
};

// 经理：可审批 3 天以内
class Manager : public Approver {
public:
    explicit Manager(const std::string& name) : Approver(name) {}

protected:
    bool CanApprove(int days) const override { return days <= 3; }
};

// 总监：可审批 7 天以内
class Director : public Approver {
public:
    explicit Director(const std::string& name) : Approver(name) {}

protected:
    bool CanApprove(int days) const override { return days <= 7; }
};

inline void RunLeaveApprovalDemo() {
    std::cout << "\n--- Leave Approval Chain Demo ---" << std::endl;

    auto leader = std::make_shared<TeamLeader>("TeamLeader Zhang");
    auto manager = std::make_shared<Manager>("Manager Li");
    auto director = std::make_shared<Director>("Director Wang");

    leader->SetNext(manager);
    manager->SetNext(director);

    LeaveRequest r1{"Alice", 1};
    LeaveRequest r2{"Bob", 2};
    LeaveRequest r3{"Cindy", 5};

    leader->HandleRequest(r1);
    leader->HandleRequest(r2);
    leader->HandleRequest(r3);
}
