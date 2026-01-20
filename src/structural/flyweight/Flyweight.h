#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// 享元模式（Flyweight）C++ 示例
// ------------------------------
// 以字符绘制为例演示享元模式：
// - Glyph          ：享元接口
// - CharacterGlyph ：具体享元，内部存储字符本身等可共享信息
// - GlyphFactory   ：享元工厂，缓存并复用 CharacterGlyph 实例

// 享元接口
class Glyph {
public:
    virtual ~Glyph() = default;

    // 在给定坐标处绘制字符（x, y 为外部状态）
    virtual void Draw(int x, int y) const = 0;
};

// 具体享元：字符
class CharacterGlyph : public Glyph {
public:
    explicit CharacterGlyph(char ch) : ch_(ch) {
        std::cout << "Create CharacterGlyph for '" << ch_ << "'" << std::endl;
    }

    void Draw(int x, int y) const override {
        std::cout << "Draw '" << ch_ << "' at (" << x << ", " << y << ")" << std::endl;
    }

private:
    char ch_; // 内部状态：字符本身
};

// 享元工厂：负责创建/缓存共享的 CharacterGlyph
class GlyphFactory {
public:
    std::shared_ptr<Glyph> GetGlyph(char ch) {
        auto it = glyphs_.find(ch);
        if (it != glyphs_.end()) {
            return it->second;
        }
        auto glyph = std::make_shared<CharacterGlyph>(ch);
        glyphs_[ch] = glyph;
        return glyph;
    }

private:
    std::unordered_map<char, std::shared_ptr<Glyph>> glyphs_;
};

// 示例 1：多次请求同一字符，验证共享
inline void RunFlyweightBasicDemo() {
    std::cout << "--- Flyweight Basic Demo ---" << std::endl;
    GlyphFactory factory;

    auto g1 = factory.GetGlyph('A');
    auto g2 = factory.GetGlyph('A');
    auto g3 = factory.GetGlyph('B');

    std::cout << "Address of g1: " << g1.get() << std::endl;
    std::cout << "Address of g2: " << g2.get() << std::endl;
    std::cout << "Address of g3: " << g3.get() << std::endl;
}

// 示例 2：模拟绘制字符串
inline void RunFlyweightTextRenderDemo() {
    std::cout << "\n--- Flyweight Text Render Demo ---" << std::endl;
    GlyphFactory factory;

    std::string text = "ABABA";
    int x = 0;
    int y = 0;

    for (char ch : text) {
        auto glyph = factory.GetGlyph(ch);
        glyph->Draw(x, y); // 位置作为外部状态传入
        x += 10;           // 简单模拟横向排版
    }
}
