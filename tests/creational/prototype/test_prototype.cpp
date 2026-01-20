#include "../../../src/creational/prototype/Prototype.h"
#include <gtest/gtest.h>

// 原型模式测试套件

// 测试原型模式的基本功能
TEST(PrototypeTest, BasicFunctionality) {
    // 创建一个具体的原型实例
    auto prototype = std::make_unique<ConcretePrototype>("Initial Value", 42);
    
    // 克隆原型
    auto clone = prototype->Clone();
    
    // 验证克隆成功
    ASSERT_NE(clone, nullptr);
    
    // 修改克隆对象，验证深拷贝
    // (由于Clone返回unique_ptr，我们只能验证克隆本身)
    clone->Show();
}

// 测试克隆对象的功能
TEST(PrototypeTest, ClonedObject_WorksCorrectly) {
    auto original = std::make_unique<ConcretePrototype>("test", 100);
    auto cloned = original->Clone();
    
    ASSERT_NE(cloned, nullptr);
    EXPECT_NO_THROW(cloned->Show());
}

// 测试多次克隆
TEST(PrototypeTest, MultipleClones) {
    auto original = std::make_unique<ConcretePrototype>("multi", 99);
    
    auto clone1 = original->Clone();
    auto clone2 = original->Clone();
    auto clone3 = original->Clone();
    
    ASSERT_NE(clone1, nullptr);
    ASSERT_NE(clone2, nullptr);
    ASSERT_NE(clone3, nullptr);
    
    // 所有克隆应该是不同的实例
    EXPECT_NE(clone1.get(), clone2.get());
    EXPECT_NE(clone1.get(), clone3.get());
    EXPECT_NE(clone2.get(), clone3.get());
}

// 测试克隆的克隆
TEST(PrototypeTest, CloneOfClone) {
    auto original = std::make_unique<ConcretePrototype>("first", 1);
    auto firstClone = original->Clone();
    auto secondClone = firstClone->Clone();
    
    ASSERT_NE(firstClone, nullptr);
    ASSERT_NE(secondClone, nullptr);
    
    EXPECT_NE(original.get(), firstClone.get());
    EXPECT_NE(firstClone.get(), secondClone.get());
    EXPECT_NE(original.get(), secondClone.get());
}

// 测试通过基类指针克隆
TEST(PrototypeTest, CloneThroughBasePointer) {
    std::unique_ptr<Prototype> prototype = std::make_unique<ConcretePrototype>("base", 777);
    auto cloned = prototype->Clone();
    
    ASSERT_NE(cloned, nullptr);
    EXPECT_NO_THROW(cloned->Show());
}

// 测试原型模式演示函数
TEST(PrototypeTest, RunPrototypeDemo) {
    EXPECT_NO_THROW(RunPrototypeDemo());
}

// 测试原型对象的Show方法
TEST(PrototypeTest, Prototype_ShowMethod) {
    auto prototype = std::make_unique<ConcretePrototype>("display", 888);
    EXPECT_NO_THROW(prototype->Show());
    
    auto cloned = prototype->Clone();
    EXPECT_NO_THROW(cloned->Show());
}