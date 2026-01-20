#include "../../../src/behavioral/template_method/TemplateMethod.h"
#include <gtest/gtest.h>

// 模板方法模式测试套件

// 测试Tea PrepareRecipe
TEST(TemplateMethodTest, Tea_PrepareRecipe) {
    Tea tea;
    EXPECT_NO_THROW(tea.PrepareRecipe());
}

// 测试Coffee PrepareRecipe
TEST(TemplateMethodTest, Coffee_PrepareRecipe) {
    Coffee coffee;
    EXPECT_NO_THROW(coffee.PrepareRecipe());
}

// 测试RunBeverageTemplateDemo
TEST(TemplateMethodTest, RunBeverageTemplateDemo) {
    EXPECT_NO_THROW(RunBeverageTemplateDemo());
}

// 测试CsvToJsonProcessor Process
TEST(TemplateMethodTest, CsvToJsonProcessor_Process) {
    CsvToJsonProcessor processor;
    EXPECT_NO_THROW(processor.Process());
}

// 测试LogFilterProcessor Process
TEST(TemplateMethodTest, LogFilterProcessor_Process) {
    LogFilterProcessor processor;
    EXPECT_NO_THROW(processor.Process());
}

// 测试RunDataProcessTemplateDemo
TEST(TemplateMethodTest, RunDataProcessTemplateDemo) {
    EXPECT_NO_THROW(RunDataProcessTemplateDemo());
}

// 测试通过基类指针调用模板方法
TEST(TemplateMethodTest, BasePointer_PrepareRecipe) {
    std::unique_ptr<CaffeineBeverage> beverage = std::make_unique<Tea>();
    EXPECT_NO_THROW(beverage->PrepareRecipe());
    
    beverage = std::make_unique<Coffee>();
    EXPECT_NO_THROW(beverage->PrepareRecipe());
}

// 测试通过基类指针调用数据处理
TEST(TemplateMethodTest, BasePointer_DataProcess) {
    std::unique_ptr<DataProcessor> processor = std::make_unique<CsvToJsonProcessor>();
    EXPECT_NO_THROW(processor->Process());
    
    processor = std::make_unique<LogFilterProcessor>();
    EXPECT_NO_THROW(processor->Process());
}

// 测试钩子方法的默认行为
TEST(TemplateMethodTest, HookMethod_DefaultBehavior) {
    class TestBeverage : public CaffeineBeverage {
    protected:
        void Brew() override {}
        void AddCondiments() override {}
    };
    
    TestBeverage bev;
    EXPECT_NO_THROW(bev.PrepareRecipe());
}
