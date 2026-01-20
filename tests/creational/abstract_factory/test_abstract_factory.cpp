#include "../../../src/creational/abstract_factory/AbstractFactory.h"
#include <gtest/gtest.h>

// 抽象工厂模式测试套件

// 测试Windows工厂创建Windows产品族
TEST(AbstractFactoryTest, WindowsFactory_CreatesWindowsProducts) {
    WindowsFactory factory;
    
    auto button = factory.CreateButton();
    auto checkbox = factory.CreateCheckbox();
    
    ASSERT_NE(button, nullptr);
    ASSERT_NE(checkbox, nullptr);
    
    EXPECT_NO_THROW(button->Paint());
    EXPECT_NO_THROW(checkbox->Paint());
}

// 测试Mac工厂创建Mac产品族
TEST(AbstractFactoryTest, MacFactory_CreatesMacProducts) {
    MacFactory factory;
    
    auto button = factory.CreateButton();
    auto checkbox = factory.CreateCheckbox();
    
    ASSERT_NE(button, nullptr);
    ASSERT_NE(checkbox, nullptr);
    
    EXPECT_NO_THROW(button->Paint());
    EXPECT_NO_THROW(checkbox->Paint());
}

// 测试通过抽象工厂多态性创建产品
TEST(AbstractFactoryTest, FactoryPolymorphism) {
    std::unique_ptr<GUIFactory> factory;
    
    // 使用Windows工厂
    factory = std::make_unique<WindowsFactory>();
    auto winButton = factory->CreateButton();
    auto winCheckbox = factory->CreateCheckbox();
    
    ASSERT_NE(winButton, nullptr);
    ASSERT_NE(winCheckbox, nullptr);
    
    // 切换到Mac工厂
    factory = std::make_unique<MacFactory>();
    auto macButton = factory->CreateButton();
    auto macCheckbox = factory->CreateCheckbox();
    
    ASSERT_NE(macButton, nullptr);
    ASSERT_NE(macCheckbox, nullptr);
}

// 测试RenderUI函数与Windows工厂
TEST(AbstractFactoryTest, RenderUI_WithWindowsFactory) {
    WindowsFactory factory;
    EXPECT_NO_THROW(RenderUI(factory));
}

// 测试RenderUI函数与Mac工厂
TEST(AbstractFactoryTest, RenderUI_WithMacFactory) {
    MacFactory factory;
    EXPECT_NO_THROW(RenderUI(factory));
}

// 测试产品族的一致性
TEST(AbstractFactoryTest, ProductFamily_Consistency) {
    WindowsFactory winFactory;
    MacFactory macFactory;
    
    // Windows工厂创建的产品应该是Windows风格
    auto winBtn1 = winFactory.CreateButton();
    auto winBtn2 = winFactory.CreateButton();
    ASSERT_NE(winBtn1, nullptr);
    ASSERT_NE(winBtn2, nullptr);
    
    // Mac工厂创建的产品应该是Mac风格
    auto macBtn1 = macFactory.CreateButton();
    auto macBtn2 = macFactory.CreateButton();
    ASSERT_NE(macBtn1, nullptr);
    ASSERT_NE(macBtn2, nullptr);
}

// 测试多次创建不同产品
TEST(AbstractFactoryTest, MultipleProductCreation) {
    WindowsFactory factory;
    
    for (int i = 0; i < 5; ++i) {
        auto button = factory.CreateButton();
        auto checkbox = factory.CreateCheckbox();
        
        ASSERT_NE(button, nullptr);
        ASSERT_NE(checkbox, nullptr);
    }
}