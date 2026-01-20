#include "../../../src/structural/proxy/Proxy.h"
#include <gtest/gtest.h>

// 代理模式测试套件

// 测试虚拟代理 - 延迟加载
TEST(ProxyTest, VirtualProxy_LazyLoading) {
    ProxyImage image("test.jpg");
    
    // 在第一次Display之前，RealImage不应被创建
    // 第一次Display时才创建
    EXPECT_NO_THROW(image.Display());
}

// 测试虚拟代理 - 多次Display
TEST(ProxyTest, VirtualProxy_MultipleDisplay) {
    ProxyImage image("picture.png");
    
    // 第一次Display创建RealImage
    EXPECT_NO_THROW(image.Display());
    
    // 后续Display复用RealImage
    EXPECT_NO_THROW(image.Display());
    EXPECT_NO_THROW(image.Display());
}

// 测试RealImage直接使用
TEST(ProxyTest, RealImage_DirectUsage) {
    RealImage realImage("direct.jpg");
    EXPECT_NO_THROW(realImage.Display());
}

// 测试RunVirtualProxyDemo
TEST(ProxyTest, RunVirtualProxyDemo) {
    EXPECT_NO_THROW(RunVirtualProxyDemo());
}

// 测试日志代理
TEST(ProxyTest, LoggingProxy_AddsLogging) {
    auto real = std::make_shared<RealService>();
    LoggingServiceProxy proxy(real);
    
    EXPECT_NO_THROW(proxy.DoWork("test-task"));
}

// 测试日志代理多次调用
TEST(ProxyTest, LoggingProxy_MultipleCalls) {
    auto real = std::make_shared<RealService>();
    LoggingServiceProxy proxy(real);
    
    EXPECT_NO_THROW(proxy.DoWork("task1"));
    EXPECT_NO_THROW(proxy.DoWork("task2"));
    EXPECT_NO_THROW(proxy.DoWork("task3"));
}

// 测试通过Service接口使用日志代理
TEST(ProxyTest, LoggingProxy_ThroughInterface) {
    auto real = std::make_shared<RealService>();
    std::unique_ptr<Service> svc = std::make_unique<LoggingServiceProxy>(real);
    
    EXPECT_NO_THROW(svc->DoWork("interface-task"));
}

// 测试RunLoggingProxyDemo
TEST(ProxyTest, RunLoggingProxyDemo) {
    EXPECT_NO_THROW(RunLoggingProxyDemo());
}

// 测试RealService直接使用
TEST(ProxyTest, RealService_DirectUsage) {
    RealService service;
    EXPECT_NO_THROW(service.DoWork("direct-task"));
}

// 测试多个代理实例
TEST(ProxyTest, MultipleProxyInstances) {
    ProxyImage image1("image1.jpg");
    ProxyImage image2("image2.png");
    ProxyImage image3("image3.gif");
    
    EXPECT_NO_THROW(image1.Display());
    EXPECT_NO_THROW(image2.Display());
    EXPECT_NO_THROW(image3.Display());
}

// 测试通过Image接口使用代理
TEST(ProxyTest, ProxyImage_ThroughInterface) {
    std::unique_ptr<Image> image = std::make_unique<ProxyImage>("poly.jpg");
    
    EXPECT_NO_THROW(image->Display());
    EXPECT_NO_THROW(image->Display());
}

// 测试嵌套代理（理论场景）
TEST(ProxyTest, NestedProxy_Concept) {
    auto realService = std::make_shared<RealService>();
    auto proxy1 = std::make_shared<LoggingServiceProxy>(realService);
    
    // 可以将一个代理传给另一个代理
    LoggingServiceProxy proxy2(proxy1);
    
    EXPECT_NO_THROW(proxy2.DoWork("nested-task"));
}