#include "../../../src/structural/composite/Composite.h"
#include <gtest/gtest.h>

// 组合模式测试套件

// 测试文件节点
TEST(CompositeTest, File_BasicFunctionality) {
    File file("test.txt", 10);
    
    EXPECT_EQ(file.GetSize(), 10);
    EXPECT_NO_THROW(file.Operation());
}

// 测试目录节点
TEST(CompositeTest, Directory_BasicFunctionality) {
    Directory dir("testDir");
    
    EXPECT_EQ(dir.GetSize(), 0);
    EXPECT_NO_THROW(dir.Operation());
}

// 测试目录添加文件
TEST(CompositeTest, Directory_AddFiles) {
    auto dir = std::make_shared<Directory>("myDir");
    
    dir->Add(std::make_shared<File>("file1.txt", 5));
    dir->Add(std::make_shared<File>("file2.txt", 3));
    
    // 总大小应该是两个文件之和
    EXPECT_EQ(dir->GetSize(), 8);
}

// 测试嵌套目录结构
TEST(CompositeTest, Directory_NestedStructure) {
    auto root = std::make_shared<Directory>("root");
    auto subDir = std::make_shared<Directory>("subDir");
    
    subDir->Add(std::make_shared<File>("inner.txt", 4));
    root->Add(subDir);
    root->Add(std::make_shared<File>("outer.txt", 6));
    
    // root包含subDir(4) + outer.txt(6) = 10
    EXPECT_EQ(root->GetSize(), 10);
}

// 测试深层嵌套
TEST(CompositeTest, Directory_DeepNesting) {
    auto level1 = std::make_shared<Directory>("level1");
    auto level2 = std::make_shared<Directory>("level2");
    auto level3 = std::make_shared<Directory>("level3");
    
    level3->Add(std::make_shared<File>("deep.txt", 1));
    level2->Add(level3);
    level1->Add(level2);
    
    EXPECT_EQ(level1->GetSize(), 1);
    EXPECT_NO_THROW(level1->Operation());
}

// 测试空目录
TEST(CompositeTest, Directory_Empty) {
    Directory emptyDir("empty");
    
    EXPECT_EQ(emptyDir.GetSize(), 0);
    EXPECT_NO_THROW(emptyDir.Operation());
}

// 测试BuildSampleFileTree
TEST(CompositeTest, BuildSampleFileTree) {
    auto root = BuildSampleFileTree();
    
    ASSERT_NE(root, nullptr);
    // src(4+2) + include(1) + docs(1) = 8
    EXPECT_EQ(root->GetSize(), 8);
}

// 测试RunCompositePrintDemo
TEST(CompositeTest, RunCompositePrintDemo) {
    EXPECT_NO_THROW(RunCompositePrintDemo());
}

// 测试RunCompositeSizeDemo
TEST(CompositeTest, RunCompositeSizeDemo) {
    EXPECT_NO_THROW(RunCompositeSizeDemo());
}

// 测试文件的Add方法（应该无效果）
TEST(CompositeTest, File_AddHasNoEffect) {
    File file("test.txt", 5);
    auto anotherFile = std::make_shared<File>("another.txt", 3);
    
    // 文件节点的Add应该不做任何事
    EXPECT_NO_THROW(file.Add(anotherFile));
    
    // 文件大小不应变化
    EXPECT_EQ(file.GetSize(), 5);
}

// 测试通过基类指针操作
TEST(CompositeTest, Component_ThroughBasePointer) {
    std::shared_ptr<Component> comp = std::make_shared<File>("poly.txt", 7);
    
    EXPECT_EQ(comp->GetSize(), 7);
    EXPECT_NO_THROW(comp->Operation());
    
    comp = std::make_shared<Directory>("polyDir");
    EXPECT_EQ(comp->GetSize(), 0);
    EXPECT_NO_THROW(comp->Operation());
}

// 测试复杂文件树
TEST(CompositeTest, ComplexFileTree) {
    auto root = std::make_shared<Directory>("project");
    
    auto src = std::make_shared<Directory>("src");
    src->Add(std::make_shared<File>("main.cpp", 10));
    src->Add(std::make_shared<File>("utils.cpp", 5));
    
    auto include = std::make_shared<Directory>("include");
    include->Add(std::make_shared<File>("utils.h", 2));
    
    auto tests = std::make_shared<Directory>("tests");
    tests->Add(std::make_shared<File>("test_main.cpp", 8));
    
    root->Add(src);
    root->Add(include);
    root->Add(tests);
    root->Add(std::make_shared<File>("README.md", 1));
    
    // 10+5+2+8+1 = 26
    EXPECT_EQ(root->GetSize(), 26);
    EXPECT_NO_THROW(root->Operation());
}
