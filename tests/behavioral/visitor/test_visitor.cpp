#include "../../../src/behavioral/visitor/Visitor.h"
#include <gtest/gtest.h>

// 访问者模式测试套件

// 测试ConcreteElementA
TEST(VisitorTest, ConcreteElementA_GetValue) {
    ConcreteElementA elem(42);
    EXPECT_EQ(elem.GetValue(), 42);
}

// 测试ConcreteElementB
TEST(VisitorTest, ConcreteElementB_GetValue) {
    ConcreteElementB elem(100);
    EXPECT_EQ(elem.GetValue(), 100);
}

// 测试PrintVisitor访问ElementA
TEST(VisitorTest, PrintVisitor_VisitElementA) {
    ConcreteElementA elem(10);
    PrintVisitor visitor;
    
    EXPECT_NO_THROW(elem.Accept(visitor));
}

// 测试PrintVisitor访问ElementB
TEST(VisitorTest, PrintVisitor_VisitElementB) {
    ConcreteElementB elem(20);
    PrintVisitor visitor;
    
    EXPECT_NO_THROW(elem.Accept(visitor));
}

// 测试SumVisitor计算总和
TEST(VisitorTest, SumVisitor_CalculateSum) {
    SumVisitor visitor;
    
    ConcreteElementA elem1(10);
    ConcreteElementB elem2(20);
    ConcreteElementA elem3(5);
    
    elem1.Accept(visitor);
    elem2.Accept(visitor);
    elem3.Accept(visitor);
    
    EXPECT_EQ(visitor.GetSum(), 35);
}

// 测试ObjectStructure添加元素
TEST(VisitorTest, ObjectStructure_AddElements) {
    ObjectStructure obj;
    
    EXPECT_NO_THROW(obj.Add(std::make_shared<ConcreteElementA>(1)));
    EXPECT_NO_THROW(obj.Add(std::make_shared<ConcreteElementB>(2)));
}

// 测试ObjectStructure接受访问者
TEST(VisitorTest, ObjectStructure_AcceptVisitor) {
    ObjectStructure obj;
    obj.Add(std::make_shared<ConcreteElementA>(10));
    obj.Add(std::make_shared<ConcreteElementB>(20));
    
    PrintVisitor printVisitor;
    EXPECT_NO_THROW(obj.Accept(printVisitor));
}

// 测试ObjectStructure与SumVisitor
TEST(VisitorTest, ObjectStructure_WithSumVisitor) {
    ObjectStructure obj;
    obj.Add(std::make_shared<ConcreteElementA>(10));
    obj.Add(std::make_shared<ConcreteElementB>(20));
    obj.Add(std::make_shared<ConcreteElementA>(5));
    
    SumVisitor sumVisitor;
    obj.Accept(sumVisitor);
    
    EXPECT_EQ(sumVisitor.GetSum(), 35);
}

// 测试RunVisitorDemo
TEST(VisitorTest, RunVisitorDemo) {
    EXPECT_NO_THROW(RunVisitorDemo());
}

// 测试空对象结构
TEST(VisitorTest, ObjectStructure_Empty) {
    ObjectStructure obj;
    
    PrintVisitor printVisitor;
    EXPECT_NO_THROW(obj.Accept(printVisitor));
    
    SumVisitor sumVisitor;
    obj.Accept(sumVisitor);
    EXPECT_EQ(sumVisitor.GetSum(), 0);
}

// 测试多次访问同一元素
TEST(VisitorTest, MultipleVisits_SameElement) {
    ConcreteElementA elem(5);
    SumVisitor visitor;
    
    elem.Accept(visitor);
    elem.Accept(visitor);
    elem.Accept(visitor);
    
    EXPECT_EQ(visitor.GetSum(), 15);
}

// 测试通过基类指针访问
TEST(VisitorTest, BasePointer_Accept) {
    std::shared_ptr<Element> elem = std::make_shared<ConcreteElementA>(7);
    PrintVisitor visitor;
    
    EXPECT_NO_THROW(elem->Accept(visitor));
}
