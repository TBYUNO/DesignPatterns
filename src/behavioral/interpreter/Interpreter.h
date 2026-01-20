#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// 解释器模式（Interpreter）C++ 示例
// ---------------------------------
// 示例 1：布尔表达式解释器
// 示例 2：简单算术表达式（仅演示结构）

// ===== 通用上下文 =====

class Context {
public:
    void SetBoolean(const std::string& name, bool value) {
        boolVars_[name] = value;
    }

    bool GetBoolean(const std::string& name) const {
        auto it = boolVars_.find(name);
        if (it == boolVars_.end()) {
            throw std::runtime_error("Variable not found: " + name);
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, bool> boolVars_;
};

// ===== 布尔表达式部分 =====

// 抽象表达式
class Expression {
public:
    virtual ~Expression() = default;
    virtual bool Interpret(const Context& ctx) const = 0;
};

// 终结符表达式：变量
class VariableExpression : public Expression {
public:
    explicit VariableExpression(std::string name) : name_(std::move(name)) {}

    bool Interpret(const Context& ctx) const override {
        return ctx.GetBoolean(name_);
    }

private:
    std::string name_;
};

// 非终结符：逻辑与
class AndExpression : public Expression {
public:
    AndExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    bool Interpret(const Context& ctx) const override {
        return left_->Interpret(ctx) && right_->Interpret(ctx);
    }

private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// 非终结符：逻辑或
class OrExpression : public Expression {
public:
    OrExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    bool Interpret(const Context& ctx) const override {
        return left_->Interpret(ctx) || right_->Interpret(ctx);
    }

private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
};

// 非终结符：逻辑非
class NotExpression : public Expression {
public:
    explicit NotExpression(std::shared_ptr<Expression> expr) : expr_(std::move(expr)) {}

    bool Interpret(const Context& ctx) const override {
        return !expr_->Interpret(ctx);
    }

private:
    std::shared_ptr<Expression> expr_;
};

inline void RunBooleanInterpreterDemo() {
    std::cout << "--- Boolean Interpreter Demo ---" << std::endl;

    Context ctx;
    ctx.SetBoolean("A", true);
    ctx.SetBoolean("B", false);
    ctx.SetBoolean("C", true);

    // 表达式：A AND (B OR C)
    auto a = std::make_shared<VariableExpression>("A");
    auto b = std::make_shared<VariableExpression>("B");
    auto c = std::make_shared<VariableExpression>("C");
    auto borc = std::make_shared<OrExpression>(b, c);
    auto expr = std::make_shared<AndExpression>(a, borc);

    bool result = expr->Interpret(ctx);
    std::cout << "Expression A AND (B OR C) = " << std::boolalpha << result << std::endl;
}

// ===== 简单算术表达式部分（示意） =====

// 算术表达式的抽象基类
class ArithmeticExpression {
public:
    virtual ~ArithmeticExpression() = default;
    virtual int Interpret() const = 0;
};

class NumberExpression : public ArithmeticExpression {
public:
    explicit NumberExpression(int value) : value_(value) {}
    int Interpret() const override { return value_; }

private:
    int value_;
};

class AddExpression : public ArithmeticExpression {
public:
    AddExpression(std::shared_ptr<ArithmeticExpression> left,
                  std::shared_ptr<ArithmeticExpression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int Interpret() const override { return left_->Interpret() + right_->Interpret(); }

private:
    std::shared_ptr<ArithmeticExpression> left_;
    std::shared_ptr<ArithmeticExpression> right_;
};

inline void RunArithmeticInterpreterDemo() {
    std::cout << "\n--- Arithmetic Interpreter Demo ---" << std::endl;

    // 表达式：1 + (2 + 3)
    auto one = std::make_shared<NumberExpression>(1);
    auto two = std::make_shared<NumberExpression>(2);
    auto three = std::make_shared<NumberExpression>(3);
    
    auto twoPlusThree = std::make_shared<AddExpression>(two, three);
    auto expr = std::make_shared<AddExpression>(one, twoPlusThree);

    std::cout << "Expression 1 + (2 + 3) = " << expr->Interpret() << std::endl;
}