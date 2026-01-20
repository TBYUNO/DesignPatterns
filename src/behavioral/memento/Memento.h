#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <string>

// 备忘录模式（Memento）C++ 示例
// ------------------------------
// 示例 1：文本编辑器撤销
// 示例 2：游戏存档（简化）

// ===== 示例 1：文本编辑器撤销 =====

class TextMemento {
public:
    explicit TextMemento(std::string text) : text_(std::move(text)) {}
    const std::string& GetText() const { return text_; }

private:
    std::string text_;
};

class TextEditor {
public:
    void SetText(const std::string& text) {
        text_ = text;
        std::cout << "TextEditor text set to: " << text_ << std::endl;
    }

    const std::string& GetText() const { return text_; }

    // 创建当前状态的备忘录
    std::shared_ptr<TextMemento> Save() const {
        return std::make_shared<TextMemento>(text_);
    }

    // 从备忘录恢复状态
    void Restore(const std::shared_ptr<TextMemento>& memento) {
        text_ = memento->GetText();
        std::cout << "TextEditor restored to: " << text_ << std::endl;
    }

private:
    std::string text_;
};

// 管理者：管理备忘录栈
class History {
public:
    void Push(const std::shared_ptr<TextMemento>& memento) {
        history_.push(memento);
    }

    std::shared_ptr<TextMemento> Pop() {
        if (history_.empty()) {
            return nullptr;
        }
        auto top = history_.top();
        history_.pop();
        return top;
    }

private:
    std::stack<std::shared_ptr<TextMemento>> history_;
};

inline void RunTextEditorMementoDemo() {
    std::cout << "--- Text Editor Memento Demo ---" << std::endl;

    TextEditor editor;
    History history;

    editor.SetText("Version 1");
    history.Push(editor.Save());

    editor.SetText("Version 2");
    history.Push(editor.Save());

    editor.SetText("Version 3");

    // 撤销到 Version 2
    auto m1 = history.Pop();
    if (m1) {
        editor.Restore(m1);
    }

    // 再撤销到 Version 1
    auto m2 = history.Pop();
    if (m2) {
        editor.Restore(m2);
    }
}

// ===== 示例 2：游戏存档 =====

class GameMemento {
public:
    GameMemento(int level, int hp) : level_(level), hp_(hp) {}

    int GetLevel() const { return level_; }
    int GetHp() const { return hp_; }

private:
    int level_;
    int hp_;
};

class Game {
public:
    void Play() {
        ++level_;
        hp_ -= 10;
        std::cout << "Game: level = " << level_ << ", hp = " << hp_ << std::endl;
    }

    std::shared_ptr<GameMemento> Save() const {
        return std::make_shared<GameMemento>(level_, hp_);
    }

    void Restore(const std::shared_ptr<GameMemento>& memento) {
        level_ = memento->GetLevel();
        hp_ = memento->GetHp();
        std::cout << "Game restored to level = " << level_ << ", hp = " << hp_ << std::endl;
    }

private:
    int level_{1};
    int hp_{100};
};

inline void RunGameMementoDemo() {
    std::cout << "\n--- Game Memento Demo ---" << std::endl;

    Game game;
    auto save1 = game.Save();

    game.Play();
    game.Play();

    // 恢复到最初状态
    game.Restore(save1);
}
