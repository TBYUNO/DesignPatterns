#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 命令模式（Command）C++ 示例
// ----------------------------
// 示例 1：简单命令（电灯开关）
// 示例 2：宏命令 + 撤销

// 命令接口
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;
    virtual void Undo() { /* 可选的撤销操作，默认空实现 */ }
};

// 接收者：电灯
class Light {
public:
    void On() { std::cout << "Light is ON" << std::endl; }
    void Off() { std::cout << "Light is OFF" << std::endl; }
};

// 具体命令：开灯
class LightOnCommand : public ICommand {
public:
    explicit LightOnCommand(std::shared_ptr<Light> light) : light_(std::move(light)) {}

    void Execute() override { light_->On(); }

    void Undo() override { light_->Off(); }

private:
    std::shared_ptr<Light> light_;
};

// 具体命令：关灯
class LightOffCommand : public ICommand {
public:
    explicit LightOffCommand(std::shared_ptr<Light> light) : light_(std::move(light)) {}

    void Execute() override { light_->Off(); }

    void Undo() override { light_->On(); }

private:
    std::shared_ptr<Light> light_;
};

// 调用者：遥控器，持有可配置的命令
class RemoteControl {
public:
    void SetCommand(std::shared_ptr<ICommand> command) { command_ = std::move(command); }

    void PressButton() {
        if (command_) {
            command_->Execute();
        }
    }

    void PressUndo() {
        if (command_) {
            command_->Undo();
        }
    }

private:
    std::shared_ptr<ICommand> command_;
};

inline void RunSimpleCommandDemo() {
    std::cout << "--- Simple Command Demo ---" << std::endl;

    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);

    RemoteControl remote;

    remote.SetCommand(onCommand);
    remote.PressButton();
    remote.PressUndo();

    remote.SetCommand(offCommand);
    remote.PressButton();
    remote.PressUndo();
}

// 宏命令：包含多个命令，一次执行
class MacroCommand : public ICommand {
public:
    void AddCommand(std::shared_ptr<ICommand> cmd) {
        commands_.push_back(std::move(cmd));
    }

    void Execute() override {
        for (auto& cmd : commands_) {
            cmd->Execute();
            history_.push_back(cmd);
        }
    }

    void Undo() override {
        // 按相反顺序撤销
        for (auto it = history_.rbegin(); it != history_.rend(); ++it) {
            (*it)->Undo();
        }
        history_.clear();
    }

private:
    std::vector<std::shared_ptr<ICommand>> commands_;
    std::vector<std::shared_ptr<ICommand>> history_;
};

inline void RunMacroCommandDemo() {
    std::cout << "\n--- Macro Command Demo ---" << std::endl;

    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);

    MacroCommand macro;
    macro.AddCommand(onCommand);
    macro.AddCommand(offCommand);

    macro.Execute();
    macro.Undo();
}
