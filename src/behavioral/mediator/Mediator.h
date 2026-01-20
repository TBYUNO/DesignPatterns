#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 中介者模式（Mediator）C++ 示例
// ------------------------------
// 示例 1：聊天室中介者
// 示例 2：对话框控件中介者（简化）

// ===== 示例 1：聊天室 =====

class ChatUser; // 前向声明

// 中介者接口
class IChatMediator {
public:
    virtual ~IChatMediator() = default;
    virtual void Send(const std::string& msg, ChatUser* sender) = 0;
};

// 同事类：用户
class ChatUser {
public:
    ChatUser(std::string name, std::shared_ptr<IChatMediator> mediator)
        : name_(std::move(name)), mediator_(std::move(mediator)) {}

    void Send(const std::string& msg) {
        std::cout << name_ << " sends: " << msg << std::endl;
        mediator_->Send(msg, this);
    }

    void Receive(const std::string& msg, const std::string& from) {
        std::cout << name_ << " receives from " << from << ": " << msg << std::endl;
    }

    const std::string& GetName() const { return name_; }

private:
    std::string name_;
    std::shared_ptr<IChatMediator> mediator_;
};

// 具体中介者：聊天室
class ChatRoomMediator : public IChatMediator, public std::enable_shared_from_this<ChatRoomMediator> {
public:
    void RegisterUser(const std::shared_ptr<ChatUser>& user) {
        users_.push_back(user);
    }

    void Send(const std::string& msg, ChatUser* sender) override {
        for (auto& u : users_) {
            if (u.get() != sender) {
                u->Receive(msg, sender->GetName());
            }
        }
    }

private:
    std::vector<std::shared_ptr<ChatUser>> users_;
};

inline void RunChatRoomMediatorDemo() {
    std::cout << "--- Chat Room Mediator Demo ---" << std::endl;

    auto mediator = std::make_shared<ChatRoomMediator>();

    auto alice = std::make_shared<ChatUser>("Alice", mediator);
    auto bob = std::make_shared<ChatUser>("Bob", mediator);
    auto cindy = std::make_shared<ChatUser>("Cindy", mediator);

    mediator->RegisterUser(alice);
    mediator->RegisterUser(bob);
    mediator->RegisterUser(cindy);

    alice->Send("Hello everyone!");
    bob->Send("Hi Alice!");
}

// ===== 示例 2：对话框控件中介者（简化） =====

class DialogMediator; // 前向声明

// 抽象控件
class Control {
public:
    explicit Control(DialogMediator* mediator) : mediator_(mediator) {}
    virtual ~Control() = default;

    virtual void Changed(); // 通知中介者控件状态改变

protected:
    DialogMediator* mediator_{};
};

// 中介者：负责协调控件
class DialogMediator {
public:
    void SetLoginButton(Control* btn) { loginButton_ = btn; }
    void SetUsernameBox(Control* box) { usernameBox_ = box; }

    void ControlChanged(Control* control) {
        // 简化逻辑：当用户名文本框变化时，决定是否启用登录按钮
        if (control == usernameBox_) {
            std::cout << "DialogMediator: username changed, enable login button" << std::endl;
        }
    }

private:
    Control* loginButton_{};
    Control* usernameBox_{};
};

inline void Control::Changed() {
    if (mediator_) {
        mediator_->ControlChanged(this);
    }
}

// 具体控件：按钮
class Button : public Control {
public:
    using Control::Control;

    void Click() {
        std::cout << "Button clicked" << std::endl;
        Changed();
    }
};

// 具体控件：文本框
class TextBox : public Control {
public:
    using Control::Control;

    void SetText(const std::string& text) {
        text_ = text;
        std::cout << "TextBox text set to: " << text_ << std::endl;
        Changed();
    }

private:
    std::string text_;
};

inline void RunDialogMediatorDemo() {
    std::cout << "\n--- Dialog Mediator Demo ---" << std::endl;

    DialogMediator dialogMediator;

    TextBox usernameBox(&dialogMediator);
    Button loginButton(&dialogMediator);

    dialogMediator.SetUsernameBox(&usernameBox);
    dialogMediator.SetLoginButton(&loginButton);

    usernameBox.SetText("admin");
    loginButton.Click();
}
