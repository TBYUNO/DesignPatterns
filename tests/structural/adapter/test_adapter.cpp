#include "../../../src/structural/adapter/Adapter.h"
#include <gtest/gtest.h>
#include <sstream>

// 适配器模式测试套件

// 测试类适配器
TEST(AdapterTest, ClassAdapter_AdaptsInterface) {
    ClassAdapter adapter;
    ITarget* target = &adapter;
    
    ASSERT_NE(target, nullptr);
    EXPECT_NO_THROW(target->Print("Test message"));
}

// 测试类适配器的多态性
TEST(AdapterTest, ClassAdapter_Polymorphism) {
    ClassAdapter adapter;
    
    // 可以作为ITarget使用
    ITarget& target = adapter;
    EXPECT_NO_THROW(target.Print("Message 1"));
    
    // 也可以作为OldPrinter使用
    OldPrinter& oldPrinter = adapter;
    EXPECT_NO_THROW(oldPrinter.OldPrint("Message 2"));
}

// 测试对象适配器
TEST(AdapterTest, ObjectAdapter_AdaptsInterface) {
    auto oldPrinter = std::make_shared<OldPrinter>();
    ObjectAdapter adapter(oldPrinter);
    
    ITarget* target = &adapter;
    ASSERT_NE(target, nullptr);
    EXPECT_NO_THROW(target->Print("Test message"));
}

// 测试对象适配器使用nullptr
TEST(AdapterTest, ObjectAdapter_WithNullPtr) {
    ObjectAdapter adapter(nullptr);
    
    // 即使传入nullptr也不应崩溃
    EXPECT_NO_THROW(adapter.Print("Test"));
}

// 测试对象适配器多次调用
TEST(AdapterTest, ObjectAdapter_MultipleCalls) {
    auto oldPrinter = std::make_shared<OldPrinter>();
    ObjectAdapter adapter(oldPrinter);
    
    EXPECT_NO_THROW(adapter.Print("First call"));
    EXPECT_NO_THROW(adapter.Print("Second call"));
    EXPECT_NO_THROW(adapter.Print("Third call"));
}

// 测试RunClassAdapterDemo
TEST(AdapterTest, RunClassAdapterDemo) {
    EXPECT_NO_THROW(RunClassAdapterDemo());
}

// 测试RunObjectAdapterDemo
TEST(AdapterTest, RunObjectAdapterDemo) {
    EXPECT_NO_THROW(RunObjectAdapterDemo());
}

// 测试RunAdapterDemo
TEST(AdapterTest, RunAdapterDemo) {
    EXPECT_NO_THROW(RunAdapterDemo());
}

// 测试OldPrinter独立工作
TEST(AdapterTest, OldPrinter_WorksIndependently) {
    OldPrinter printer;
    EXPECT_NO_THROW(printer.OldPrint("Direct call"));
}

// 测试std::string到const char*的转换
TEST(AdapterTest, Adapter_StringConversion) {
    auto oldPrinter = std::make_shared<OldPrinter>();
    ObjectAdapter adapter(oldPrinter);
    
    std::string longString(1000, 'A');
    EXPECT_NO_THROW(adapter.Print(longString));
}
