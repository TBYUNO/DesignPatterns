#pragma once

#include <iostream>
#include <memory>
#include <string>

// 状态模式（State）C++ 示例
// -------------------------
// 示例 1：简单开关
// 示例 2：文本编辑器模式（插入/覆盖）

// ===== 示例 1：简单开关 =====

class Switch; // 前向声明

// 状态接口
class ISwitchState {
public:
    virtual ~ISwitchState() = default;
    virtual void Press(Switch& sw) = 0;
    virtual const char* Name() const = 0;
};

class Switch {
public:
    explicit Switch(std::shared_ptr<ISwitchState> state) : state_(std::move(state)) {}

    void SetState(std::shared_ptr<ISwitchState> state) { state_ = std::move(state); }

    void Press() {
        std::cout << "Switch is currently: " << state_->Name() << ", press..." << std::endl;
        state_->Press(*this);
    }

private:
    std::shared_ptr<ISwitchState> state_;
};

class OnState : public ISwitchState {
public:
    void Press(Switch& sw) override;
    const char* Name() const override { return "ON"; }
};

class OffState : public ISwitchState {
public:
    void Press(Switch& sw) override;
    const char* Name() const override { return "OFF"; }
};

inline void OnState::Press(Switch& sw) {
    std::cout << "Turning OFF" << std::endl;
    sw.SetState(std::make_shared<OffState>());
}

inline void OffState::Press(Switch& sw) {
    std::cout << "Turning ON" << std::endl;
    sw.SetState(std::make_shared<OnState>());
}

inline void RunSimpleStateDemo() {
    std::cout << "--- Simple State Demo ---" << std::endl;

    Switch sw(std::make_shared<OffState>());
    sw.Press();
    sw.Press();
}

// ===== 示例 2：文本编辑器模式 =====

class TextEditor; // 前向声明

class IEditMode {
public:
    virtual ~IEditMode() = default;
    virtual void InsertChar(TextEditor& editor, char ch) = 0;
    virtual const char* Name() const = 0;
};

class TextEditor {
public:
    explicit TextEditor(std::shared_ptr<IEditMode> mode) : mode_(std::move(mode)) {}

    void SetMode(std::shared_ptr<IEditMode> mode) { mode_ = std::move(mode); }

    void InsertChar(char ch) {
        mode_->InsertChar(*this, ch);
    }

    void MoveCursor(int pos) {
        cursor_ = pos;
        if (cursor_ < 0) cursor_ = 0;
        if (cursor_ > static_cast<int>(text_.size())) cursor_ = static_cast<int>(text_.size());
    }

    void Print() const {
        std::cout << "Text = '" << text_ << "', cursor = " << cursor_
                  << ", mode = " << mode_->Name() << std::endl;
    }

    // 以下方法供模式对象访问
    int GetCursor() const { return cursor_; }
    void SetCursor(int c) { cursor_ = c; }
    std::string& GetText() { return text_; }

private:
    std::string text_;
    int cursor_{0};
    std::shared_ptr<IEditMode> mode_;
};

class InsertMode : public IEditMode {
public:
    void InsertChar(TextEditor& editor, char ch) override;
    const char* Name() const override { return "Insert"; }
};

class OverwriteMode : public IEditMode {
public:
    void InsertChar(TextEditor& editor, char ch) override;
    const char* Name() const override { return "Overwrite"; }
};

inline void InsertMode::InsertChar(TextEditor& editor, char ch) {
    auto& text = editor.GetText();
    int cursor = editor.GetCursor();
    if (cursor < 0) cursor = 0;
    if (cursor > static_cast<int>(text.size())) cursor = static_cast<int>(text.size());

    text.insert(text.begin() + cursor, ch);
    editor.SetCursor(cursor + 1);
}

inline void OverwriteMode::InsertChar(TextEditor& editor, char ch) {
    auto& text = editor.GetText();
    int cursor = editor.GetCursor();
    if (cursor < 0) cursor = 0;
    if (cursor >= static_cast<int>(text.size())) {
        text.push_back(ch);
    } else {
        text[cursor] = ch;
    }
    editor.SetCursor(cursor + 1);
}

inline void RunTextEditorStateDemo() {
    std::cout << "\n--- TextEditor State Demo ---" << std::endl;

    TextEditor editor(std::make_shared<InsertMode>());
    editor.InsertChar('A');
    editor.InsertChar('B');
    editor.InsertChar('C');
    editor.Print();

    editor.SetMode(std::make_shared<OverwriteMode>());
    editor.MoveCursor(1);
    editor.InsertChar('X');
    editor.Print();
}
