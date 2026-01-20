#include "../../../src/behavioral/interpreter/Interpreter.h"
#include <gtest/gtest.h>

// 解释器模式测试套件

// 测试Context设置和获取Boolean
TEST(InterpreterTest, Context_SetAndGetBoolean) {
    Context ctx;
    ctx.SetBoolean("testVar", true);
    
    EXPECT_TRUE(ctx.GetBoolean("testVar"));
}

// 测试Context获取不存在的变量
TEST(InterpreterTest, Context_GetNonExistentVariable) {
    Context ctx;
    
    EXPECT_THROW(ctx.GetBoolean("nonExistent"), std::runtime_error);
}

// 测试变量表达式
TEST(InterpreterTest, VariableExpression_Interpret) {
    Context ctx;
    ctx.SetBoolean("A", true);
    
    VariableExpression expr("A");
    EXPECT_TRUE(expr.Interpret(ctx));
}

// 测试AND表达式 - true AND true
TEST(InterpreterTest, AndExpression_TrueAndTrue) {
    Context ctx;
    ctx.SetBoolean("A", true);
    ctx.SetBoolean("B", true);
    
    auto left = std::make_shared<VariableExpression>("A");
    auto right = std::make_shared<VariableExpression>("B");
    AndExpression expr(left, right);
    
    EXPECT_TRUE(expr.Interpret(ctx));
}

// 测试AND表达式 - true AND false
TEST(InterpreterTest, AndExpression_TrueAndFalse) {
    Context ctx;
    ctx.SetBoolean("A", true);
    ctx.SetBoolean("B", false);
    
    auto left = std::make_shared<VariableExpression>("A");
    auto right = std::make_shared<VariableExpression>("B");
    AndExpression expr(left, right);
    
    EXPECT_FALSE(expr.Interpret(ctx));
}

// 测试OR表达式 - false OR true
TEST(InterpreterTest, OrExpression_FalseOrTrue) {
    Context ctx;
    ctx.SetBoolean("A", false);
    ctx.SetBoolean("B", true);
    
    auto left = std::make_shared<VariableExpression>("A");
    auto right = std::make_shared<VariableExpression>("B");
    OrExpression expr(left, right);
    
    EXPECT_TRUE(expr.Interpret(ctx));
}

// 测试NOT表达式
TEST(InterpreterTest, NotExpression_Negate) {
    Context ctx;
    ctx.SetBoolean("A", true);
    
    auto inner = std::make_shared<VariableExpression>("A");
    NotExpression expr(inner);
    
    EXPECT_FALSE(expr.Interpret(ctx));
}

// 测试复杂表达式 - A AND (B OR C)
TEST(InterpreterTest, ComplexExpression_AndOr) {
    Context ctx;
    ctx.SetBoolean("A", true);
    ctx.SetBoolean("B", false);
    ctx.SetBoolean("C", true);
    
    auto a = std::make_shared<VariableExpression>("A");
    auto b = std::make_shared<VariableExpression>("B");
    auto c = std::make_shared<VariableExpression>("C");
    auto borc = std::make_shared<OrExpression>(b, c);
    auto expr = std::make_shared<AndExpression>(a, borc);
    
    EXPECT_TRUE(expr->Interpret(ctx));
}

// 测试RunBooleanInterpreterDemo
TEST(InterpreterTest, RunBooleanInterpreterDemo) {
    EXPECT_NO_THROW(RunBooleanInterpreterDemo());
}

// 测试数字表达式
TEST(InterpreterTest, NumberExpression_Interpret) {
    NumberExpression expr(42);
    EXPECT_EQ(expr.Interpret(), 42);
}

// 测试加法表达式
TEST(InterpreterTest, AddExpression_Interpret) {
    auto left = std::make_shared<NumberExpression>(5);
    auto right = std::make_shared<NumberExpression>(3);
    AddExpression expr(left, right);
    
    EXPECT_EQ(expr.Interpret(), 8);
}

// 测试嵌套加法表达式 - 1 + (2 + 3)
TEST(InterpreterTest, NestedAddExpression) {
    auto one = std::make_shared<NumberExpression>(1);
    auto two = std::make_shared<NumberExpression>(2);
    auto three = std::make_shared<NumberExpression>(3);
    
    auto twoPlusThree = std::make_shared<AddExpression>(two, three);
    AddExpression expr(one, twoPlusThree);
    
    EXPECT_EQ(expr.Interpret(), 6);
}

// 测试RunArithmeticInterpreterDemo
TEST(InterpreterTest, RunArithmeticInterpreterDemo) {
    EXPECT_NO_THROW(RunArithmeticInterpreterDemo());
}
