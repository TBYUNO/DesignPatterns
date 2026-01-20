# C++ 设计模式项目 - 构建指南

## 📋 项目结构

```
DesignPatterns/
├── src/                   # 源码目录
│   ├── creational/        # 创建型模式（5个）
│   │   ├── singleton/
│   │   │   ├── Singletons.h      # 7种单例实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── factory_method/
│   │   │   ├── FactoryMethod.h   # 工厂方法模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── abstract_factory/
│   │   │   ├── AbstractFactory.h # 抽象工厂模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── builder/
│   │   │   ├── Builder.h         # 建造者模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   └── prototype/
│   │       ├── Prototype.h       # 原型模式实现
│   │       └── main.cpp          # 演示程序
│   ├── structural/        # 结构型模式（7个）
│   │   ├── adapter/
│   │   │   ├── Adapter.h         # 适配器模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── bridge/
│   │   │   ├── Bridge.h          # 桥接模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── composite/
│   │   │   ├── Composite.h       # 组合模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── decorator/
│   │   │   ├── Decorator.h       # 装饰器模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── facade/
│   │   │   ├── Facade.h          # 外观模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   ├── flyweight/
│   │   │   ├── Flyweight.h       # 享元模式实现
│   │   │   └── main.cpp          # 演示程序
│   │   └── proxy/
│   │       ├── Proxy.h           # 代理模式实现
│   │       └── main.cpp          # 演示程序
│   └── behavioral/        # 行为型模式（11个）
│       ├── chain_of_responsibility/
│       │   ├── ChainOfResponsibility.h  # 责任链模式实现
│       │   └── main.cpp                 # 演示程序
│       ├── command/
│       │   ├── Command.h         # 命令模式实现
│       │   └── main.cpp          # 演示程序
│       ├── interpreter/
│       │   ├── Interpreter.h     # 解释器模式实现
│       │   └── main.cpp          # 演示程序
│       ├── iterator/
│       │   ├── Iterator.h        # 迭代器模式实现
│       │   └── main.cpp          # 演示程序
│       ├── mediator/
│       │   ├── Mediator.h        # 中介者模式实现
│       │   └── main.cpp          # 演示程序
│       ├── memento/
│       │   ├── Memento.h         # 备忘录模式实现
│       │   └── main.cpp          # 演示程序
│       ├── observer/
│       │   ├── Observer.h        # 观察者模式实现
│       │   └── main.cpp          # 演示程序
│       ├── state/
│       │   ├── State.h           # 状态模式实现
│       │   └── main.cpp          # 演示程序
│       ├── strategy/
│       │   ├── Strategy.h        # 策略模式实现
│       │   └── main.cpp          # 演示程序
│       ├── template_method/
│       │   ├── TemplateMethod.h  # 模板方法模式实现
│       │   └── main.cpp          # 演示程序
│       └── visitor/
│           ├── Visitor.h         # 访问者模式实现
│           └── main.cpp          # 演示程序
├── tests/                 # 测试目录
│   ├── creational/        # 创建型模式测试
│   │   ├── singleton/
│   │   │   └── test_singleton.cpp
│   │   ├── factory_method/
│   │   │   └── test_factory_method.cpp
│   │   ├── abstract_factory/
│   │   │   └── test_abstract_factory.cpp
│   │   ├── builder/
│   │   │   └── test_builder.cpp
│   │   └── prototype/
│   │       └── test_prototype.cpp
│   ├── structural/        # 结构型模式测试
│   │   ├── adapter/
│   │   │   └── test_adapter.cpp
│   │   ├── bridge/
│   │   │   └── test_bridge.cpp
│   │   ├── composite/
│   │   │   └── test_composite.cpp
│   │   ├── decorator/
│   │   │   └── test_decorator.cpp
│   │   ├── facade/
│   │   │   └── test_facade.cpp
│   │   ├── flyweight/
│   │   │   └── test_flyweight.cpp
│   │   └── proxy/
│   │       └── test_proxy.cpp
│   └── behavioral/        # 行为型模式测试
│       ├── chain_of_responsibility/
│       │   └── test_chain_of_responsibility.cpp
│       ├── command/
│       │   └── test_command.cpp
│       ├── interpreter/
│       │   └── test_interpreter.cpp
│       ├── iterator/
│       │   └── test_iterator.cpp
│       ├── mediator/
│       │   └── test_mediator.cpp
│       ├── memento/
│       │   └── test_memento.cpp
│       ├── observer/
│       │   └── test_observer.cpp
│       ├── state/
│       │   └── test_state.cpp
│       ├── strategy/
│       │   └── test_strategy.cpp
│       ├── template_method/
│       │   └── test_template_method.cpp
│       └── visitor/
│           └── test_visitor.cpp
├── scripts/               # 构建和测试脚本
│   ├── build.sh           # 一键构建脚本
│   ├── run_tests.sh       # 一键运行测试脚本
│   └── clean.sh           # 清理脚本
├── docs/                  # 文档目录
│   ├── BUILD.md           # 构建指南
│   └── DESIGN_PATTERNS_GUIDE.md   # 完整设计模式指南
├── CMakeLists.txt         # CMake配置文件
└── README.md              # 项目说明文档
```

## 🚀 快速开始

### 前置要求

- CMake >= 3.10
- C++17 兼容的编译器（GCC 7+, Clang 5+, MSVC 2017+）
- Git（用于下载GoogleTest）

### 方法一：使用脚本构建（推荐）

```bash
# 构建项目
./scripts/build.sh

# 运行所有测试
./scripts/run_tests.sh

# 清理构建产物
./scripts/clean.sh
```

### 方法二：手动构建

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译（使用多核加速）
make -j4

# 运行测试
ctest

# 或运行单个测试
./singleton_test
./observer_test
```

## 🎯 运行示例

每个设计模式都有对应的示例程序：

```bash
cd build

# 运行单例模式示例
./singleton_example

# 运行观察者模式示例
./observer_example

# 运行策略模式示例
./strategy_example

# 运行所有示例程序
for example in *_example; do
    echo "=== Running $example ==="
    ./$example
    echo ""
done
```

## 🧪 测试说明

项目包含完整的单元测试，使用GoogleTest框架：

```bash
# 运行所有测试
cd build && ctest

# 详细输出模式
ctest --output-on-failure --verbose

# 运行特定测试
ctest -R singleton
ctest -R observer

# 查看测试列表
ctest -N

# 运行特定测试并查看输出
ctest -R singleton -V
```

## 📝 开发说明

### 代码格式化

项目使用 `.clang-format` 配置统一代码风格：

```bash
# 格式化单个文件
clang-format -i src/creational/singleton/Singletons.h

# 格式化所有C++文件
find . -name "*.h" -o -name "*.cpp" | xargs clang-format -i
```

### 添加新模式

1. 在对应目录（src/creational/src/structural/src/behavioral）创建新目录
2. 添加头文件（实现）和 main.cpp（示例）
3. 在 tests/ 对应目录创建模式名称子目录并添加 test_*.cpp（测试）
4. 在 CMakeLists.txt 中注册：

```cmake
add_pattern_example(your_pattern src/path/to/pattern)
add_pattern_test(your_pattern tests/path/to/pattern_subdir)
```

## 🔧 CMake 配置选项

```bash
# Debug 模式
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release 模式
cmake -DCMAKE_BUILD_TYPE=Release ..

# 指定编译器
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# 导出编译命令（供IDE使用）
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# 使用系统安装的GoogleTest
cmake -DUSE_SYSTEM_GTEST=ON ..
```

## 📊 测试覆盖

项目为所有23个设计模式提供了完整的测试覆盖：

- ✅ 创建型模式：5个模式，60+ 测试用例
- ✅ 结构型模式：7个模式，85+ 测试用例  
- ✅ 行为型模式：11个模式，120+ 测试用例

总测试数量：**242个测试全部通过**

## 🐛 常见问题

### 1. CMake找不到

```bash
# macOS
brew install cmake

# Ubuntu/Debian
sudo apt-get install cmake

# CentOS/RHEL
sudo yum install cmake
```

### 2. 编译器版本过低

确保使用支持C++17的编译器：
- GCC >= 7.0
- Clang >= 5.0
- MSVC >= 2017

### 3. GoogleTest下载失败

检查网络连接，或使用系统安装的GoogleTest：
```bash
cmake -DUSE_SYSTEM_GTEST=ON ..
```

### 4. 头文件包含路径错误

如果遇到头文件找不到的问题，请确保使用正确的相对路径：
- 源文件包含（main.cpp中）：`#include "PatternName.h"` （同一目录）
- 测试文件包含：`#include "../../../src/category/pattern/PatternName.h"`

## 📚 参考资料

- [设计模式详细文档](../README.md)
- [GoogleTest文档](https://google.github.io/googletest/)
- [CMake官方文档](https://cmake.org/documentation/)
- [完整设计模式指南](DESIGN_PATTERNS_GUIDE.md)