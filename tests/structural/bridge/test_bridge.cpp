#include "../../../src/structural/bridge/Bridge.h"
#include <gtest/gtest.h>

// 桥接模式测试套件

// 测试红色圆形
TEST(BridgeTest, Circle_WithRedColor) {
    auto red = std::make_shared<RedColor>();
    Circle circle(5.0, red);
    
    EXPECT_NO_THROW(circle.Draw());
}

// 测试绿色矩形
TEST(BridgeTest, Rectangle_WithGreenColor) {
    auto green = std::make_shared<GreenColor>();
    Rectangle rect(3.0, 4.0, green);
    
    EXPECT_NO_THROW(rect.Draw());
}

// 测试运行时切换颜色
TEST(BridgeTest, Shape_DynamicColorChange) {
    auto red = std::make_shared<RedColor>();
    auto green = std::make_shared<GreenColor>();
    
    Circle circle(10.0, red);
    EXPECT_NO_THROW(circle.Draw());
    
    // 切换到绿色
    circle.SetColor(green);
    EXPECT_NO_THROW(circle.Draw());
}

// 测试多个形状共享颜色
TEST(BridgeTest, MultipleShapes_ShareColor) {
    auto red = std::make_shared<RedColor>();
    
    Circle circle(7.0, red);
    Rectangle rect(5.0, 6.0, red);
    
    EXPECT_NO_THROW(circle.Draw());
    EXPECT_NO_THROW(rect.Draw());
}

// 测试形状与颜色的独立扩展
TEST(BridgeTest, ShapeAndColor_IndependentExtension) {
    auto red = std::make_shared<RedColor>();
    auto green = std::make_shared<GreenColor>();
    
    // 两种形状
    Circle circle(1.0, red);
    Rectangle rect(2.0, 3.0, green);
    
    EXPECT_NO_THROW(circle.Draw());
    EXPECT_NO_THROW(rect.Draw());
    
    // 交换颜色
    circle.SetColor(green);
    rect.SetColor(red);
    
    EXPECT_NO_THROW(circle.Draw());
    EXPECT_NO_THROW(rect.Draw());
}

// 测试空颜色指针
TEST(BridgeTest, Shape_WithNullColor) {
    Circle circle(5.0, nullptr);
    EXPECT_NO_THROW(circle.Draw());
    
    Rectangle rect(3.0, 4.0, nullptr);
    EXPECT_NO_THROW(rect.Draw());
}

// 测试RunBasicBridgeDemo
TEST(BridgeTest, RunBasicBridgeDemo) {
    EXPECT_NO_THROW(RunBasicBridgeDemo());
}

// 测试RunDynamicBridgeDemo
TEST(BridgeTest, RunDynamicBridgeDemo) {
    EXPECT_NO_THROW(RunDynamicBridgeDemo());
}

// 测试通过基类指针使用
TEST(BridgeTest, Shape_ThroughBasePointer) {
    auto red = std::make_shared<RedColor>();
    
    std::unique_ptr<Shape> shape = std::make_unique<Circle>(8.0, red);
    EXPECT_NO_THROW(shape->Draw());
    
    auto green = std::make_shared<GreenColor>();
    shape->SetColor(green);
    EXPECT_NO_THROW(shape->Draw());
}

// 测试颜色独立使用
TEST(BridgeTest, Color_IndependentUsage) {
    RedColor red;
    GreenColor green;
    
    EXPECT_NO_THROW(red.ApplyColor());
    EXPECT_NO_THROW(green.ApplyColor());
}
