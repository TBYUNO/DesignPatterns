#include "../../../src/structural/flyweight/Flyweight.h"
#include <gtest/gtest.h>

// 享元模式测试套件

// 测试GlyphFactory创建新字符
TEST(FlyweightTest, GlyphFactory_CreateNewGlyph) {
    GlyphFactory factory;
    auto glyph = factory.GetGlyph('A');
    
    ASSERT_NE(glyph, nullptr);
}

// 测试GlyphFactory共享相同字符
TEST(FlyweightTest, GlyphFactory_SharesSameCharacter) {
    GlyphFactory factory;
    
    auto g1 = factory.GetGlyph('A');
    auto g2 = factory.GetGlyph('A');
    
    // 应该返回同一个实例
    EXPECT_EQ(g1.get(), g2.get());
}

// 测试GlyphFactory不同字符不共享
TEST(FlyweightTest, GlyphFactory_DifferentCharactersNotShared) {
    GlyphFactory factory;
    
    auto g1 = factory.GetGlyph('A');
    auto g2 = factory.GetGlyph('B');
    
    // 应该是不同的实例
    EXPECT_NE(g1.get(), g2.get());
}

// 测试CharacterGlyph绘制功能
TEST(FlyweightTest, CharacterGlyph_DrawFunctionality) {
    GlyphFactory factory;
    auto glyph = factory.GetGlyph('X');
    
    ASSERT_NE(glyph, nullptr);
    EXPECT_NO_THROW(glyph->Draw(10, 20));
}

// 测试多次请求同一字符
TEST(FlyweightTest, MultipleSameCharacterRequests) {
    GlyphFactory factory;
    
    auto g1 = factory.GetGlyph('C');
    auto g2 = factory.GetGlyph('C');
    auto g3 = factory.GetGlyph('C');
    
    // 所有实例应该指向同一个对象
    EXPECT_EQ(g1.get(), g2.get());
    EXPECT_EQ(g2.get(), g3.get());
}

// 测试混合请求多个字符
TEST(FlyweightTest, MixedCharacterRequests) {
    GlyphFactory factory;
    
    auto a1 = factory.GetGlyph('A');
    auto b1 = factory.GetGlyph('B');
    auto a2 = factory.GetGlyph('A');
    auto b2 = factory.GetGlyph('B');
    
    // 相同字符共享
    EXPECT_EQ(a1.get(), a2.get());
    EXPECT_EQ(b1.get(), b2.get());
    
    // 不同字符不共享
    EXPECT_NE(a1.get(), b1.get());
}

// 测试RunFlyweightBasicDemo
TEST(FlyweightTest, RunFlyweightBasicDemo) {
    EXPECT_NO_THROW(RunFlyweightBasicDemo());
}

// 测试RunFlyweightTextRenderDemo
TEST(FlyweightTest, RunFlyweightTextRenderDemo) {
    EXPECT_NO_THROW(RunFlyweightTextRenderDemo());
}

// 测试绘制字符串场景
TEST(FlyweightTest, RenderString_SharesGlyphs) {
    GlyphFactory factory;
    std::string text = "HELLO";
    
    std::vector<std::shared_ptr<Glyph>> glyphs;
    for (char ch : text) {
        glyphs.push_back(factory.GetGlyph(ch));
    }
    
    // 验证所有字形都创建成功
    for (const auto& g : glyphs) {
        ASSERT_NE(g, nullptr);
    }
    
    // 验证重复字符共享（两个'L'）
    EXPECT_EQ(glyphs[2].get(), glyphs[3].get());
}

// 测试大量字符的共享效率
TEST(FlyweightTest, LargeTextSharing) {
    GlyphFactory factory;
    std::string text = "AAABBBCCCDDD";
    
    std::vector<std::shared_ptr<Glyph>> glyphs;
    for (char ch : text) {
        glyphs.push_back(factory.GetGlyph(ch));
    }
    
    // 验证所有'A'共享同一实例
    EXPECT_EQ(glyphs[0].get(), glyphs[1].get());
    EXPECT_EQ(glyphs[1].get(), glyphs[2].get());
    
    // 验证所有'B'共享同一实例
    EXPECT_EQ(glyphs[3].get(), glyphs[4].get());
    EXPECT_EQ(glyphs[4].get(), glyphs[5].get());
    
    // 验证所有'C'共享同一实例
    EXPECT_EQ(glyphs[6].get(), glyphs[7].get());
    EXPECT_EQ(glyphs[7].get(), glyphs[8].get());
}

// 测试外部状态（位置）不影响共享
TEST(FlyweightTest, ExternalState_DoesNotAffectSharing) {
    GlyphFactory factory;
    
    auto g1 = factory.GetGlyph('Z');
    auto g2 = factory.GetGlyph('Z');
    
    // 即使在不同位置绘制，仍然是同一个享元
    EXPECT_NO_THROW(g1->Draw(0, 0));
    EXPECT_NO_THROW(g2->Draw(100, 200));
    
    EXPECT_EQ(g1.get(), g2.get());
}
