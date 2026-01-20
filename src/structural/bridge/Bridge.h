#pragma once

#include <iostream>
#include <memory>
#include <string>

// 桥接模式（Bridge）C++ 示例
// ---------------------------
// 本文件以“图形 × 颜色”为例演示桥接模式：
// - Color：实现接口（实现层次）
// - Shape：抽象接口（抽象层次），内部持有 Color 指针完成桥接
//
// 通过组合而非继承，使“图形种类”和“颜色种类”可以独立扩展。

// 实现接口：颜色
class Color {
public:
    virtual ~Color() = default;
    virtual void ApplyColor() const = 0; // 将颜色应用到图形上
};

// 具体实现：红色
class RedColor : public Color {
public:
    void ApplyColor() const override {
        std::cout << "red";
    }
};

// 具体实现：绿色
class GreenColor : public Color {
public:
    void ApplyColor() const override {
        std::cout << "green";
    }
};

// 抽象部分：图形
class Shape {
public:
    explicit Shape(std::shared_ptr<Color> color) : color_(std::move(color)) {}
    virtual ~Shape() = default;

    // 抽象接口：绘制图形
    virtual void Draw() const = 0;

    // 运行时切换颜色实现
    void SetColor(std::shared_ptr<Color> color) {
        color_ = std::move(color);
    }

protected:
    std::shared_ptr<Color> color_;
};

// 扩展抽象：圆形
class Circle : public Shape {
public:
    Circle(double radius, std::shared_ptr<Color> color)
        : Shape(std::move(color)), radius_(radius) {}

    void Draw() const override {
        std::cout << "Draw a circle with radius " << radius_ << " and color ";
        if (color_) {
            color_->ApplyColor();
        } else {
            std::cout << "(no color)";
        }
        std::cout << std::endl;
    }

private:
    double radius_{};
};

// 扩展抽象：矩形
class Rectangle : public Shape {
public:
    Rectangle(double width, double height, std::shared_ptr<Color> color)
        : Shape(std::move(color)), width_(width), height_(height) {}

    void Draw() const override {
        std::cout << "Draw a rectangle " << width_ << "x" << height_ << " with color ";
        if (color_) {
            color_->ApplyColor();
        } else {
            std::cout << "(no color)";
        }
        std::cout << std::endl;
    }

private:
    double width_{};
    double height_{};
};

// 基本示例：不同图形 × 不同颜色
inline void RunBasicBridgeDemo() {
    std::cout << "--- Basic Bridge Demo ---" << std::endl;

    auto red = std::make_shared<RedColor>();
    auto green = std::make_shared<GreenColor>();

    Circle redCircle(5.0, red);
    Rectangle greenRect(3.0, 4.0, green);

    redCircle.Draw();
    greenRect.Draw();
}

// 动态切换实现示例：在运行时给同一个图形切换颜色
inline void RunDynamicBridgeDemo() {
    std::cout << "\n--- Dynamic Bridge Demo ---" << std::endl;

    auto red = std::make_shared<RedColor>();
    auto green = std::make_shared<GreenColor>();

    Circle circle(10.0, red);
    circle.Draw();

    // 运行时切换到绿色实现
    circle.SetColor(green);
    circle.Draw();
}
