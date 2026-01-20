#include "../../../src/creational/builder/Builder.h"
#include <gtest/gtest.h>

// 建造者模式测试套件

// 测试Director使用GamingComputerBuilder构建游戏电脑
TEST(BuilderTest, Director_ConstructsWithGamingComputerBuilder) {
    Director director;
    GamingComputerBuilder gamingBuilder;
    
    director.Construct(gamingBuilder);
    
    auto gamingPC = gamingBuilder.GetResult();
    ASSERT_NE(gamingPC, nullptr);
    
    // 验证游戏电脑对象的各个部分都被正确构建
    EXPECT_NO_THROW(gamingPC->Show());
}

// 测试Director使用OfficeComputerBuilder构建办公电脑
TEST(BuilderTest, Director_ConstructsWithOfficeComputerBuilder) {
    Director director;
    OfficeComputerBuilder officeBuilder;
    
    director.Construct(officeBuilder);
    
    auto officePC = officeBuilder.GetResult();
    ASSERT_NE(officePC, nullptr);
    
    // 验证办公电脑对象的各个部分都被正确构建
    EXPECT_NO_THROW(officePC->Show());
}

// 测试手动使用GamingComputerBuilder构建游戏电脑
TEST(BuilderTest, ManualGamingComputerConstruction) {
    GamingComputerBuilder builder;
    
    builder.BuildCPU();
    builder.BuildGPU();
    builder.BuildRAM();
    builder.BuildStorage();
    
    auto gamingPC = builder.GetResult();
    ASSERT_NE(gamingPC, nullptr);
    
    EXPECT_NO_THROW(gamingPC->Show());
}

// 测试手动使用OfficeComputerBuilder构建办公电脑
TEST(BuilderTest, ManualOfficeComputerConstruction) {
    OfficeComputerBuilder builder;
    
    builder.BuildCPU();
    builder.BuildGPU();
    builder.BuildRAM();
    builder.BuildStorage();
    
    auto officePC = builder.GetResult();
    ASSERT_NE(officePC, nullptr);
    
    EXPECT_NO_THROW(officePC->Show());
}

// 测试建造者多态性
TEST(BuilderTest, BuilderPolymorphism) {
    std::unique_ptr<ComputerBuilder> builder;
    
    // 使用GamingComputerBuilder
    builder = std::make_unique<GamingComputerBuilder>();
    ASSERT_NE(builder, nullptr);
    
    // 切换到OfficeComputerBuilder
    builder = std::make_unique<OfficeComputerBuilder>();
    ASSERT_NE(builder, nullptr);
}

// 测试Director复用
TEST(BuilderTest, DirectorReusability) {
    Director director;
    GamingComputerBuilder gamingBuilder;
    OfficeComputerBuilder officeBuilder;
    
    // 使用Director构建游戏电脑
    director.Construct(gamingBuilder);
    auto gamingPC = gamingBuilder.GetResult();
    ASSERT_NE(gamingPC, nullptr);
    
    // 使用同一个Director构建办公电脑
    director.Construct(officeBuilder);
    auto officePC = officeBuilder.GetResult();
    ASSERT_NE(officePC, nullptr);
}

// 测试部分构建
TEST(BuilderTest, PartialConstruction) {
    GamingComputerBuilder builder;
    
    // 只构建部分组件
    builder.BuildCPU();
    builder.BuildGPU();
    
    // 此时获取的结果可能不完整，但不应该崩溃
    auto partialPC = builder.GetResult();
    ASSERT_NE(partialPC, nullptr);
    
    EXPECT_NO_THROW(partialPC->Show());
}

// 测试建造者重置（通过创建新实例模拟）
TEST(BuilderTest, BuilderResetSimulation) {
    GamingComputerBuilder builder1;
    
    // 构建第一个对象
    builder1.BuildCPU();
    builder1.BuildGPU();
    auto pc1 = builder1.GetResult();
    ASSERT_NE(pc1, nullptr);
    
    // 创建新实例模拟重置并构建第二个对象
    GamingComputerBuilder builder2;
    builder2.BuildCPU();
    builder2.BuildGPU();
    builder2.BuildRAM();
    auto pc2 = builder2.GetResult();
    ASSERT_NE(pc2, nullptr);
    
    // 验证两个对象是不同的实例
    EXPECT_NE(pc1.get(), pc2.get());
}