# C++ 设计模式学习工程

> 本工程用于系统性学习和实践 C++ 设计模式，覆盖常见的面向对象设计原则和 GoF 23 种设计模式，并为每种模式提供带详细中文注释的示例代码。工程已集成完整的单元测试体系和现代化的构建系统。

## 🚀 快速开始

**构建和运行：**
```bash
# 使用脚本快速构建
./scripts/build.sh

# 运行所有测试
./scripts/run_tests.sh

# 运行单个示例
cd build && ./singleton_example
```

**快速体验所有模式：**
```bash
# 直接运行特定示例
./build/singleton_example
./build/factory_method_example
./build/strategy_example
# ... 等等
```

> 详细的构建说明、测试方法和常见问题，请参考 [构建指南](docs/BUILD.md)

## 📊 项目概览

- **23种 GoF 设计模式**：完整的创建型、结构型、行为型模式实现
- **完整测试覆盖**：为所有模式提供单元测试，总计 **242 个测试用例**
- **现代化 C++**：使用 C++17 标准，包含线程安全和性能优化实现
- **完整文档**：每个模式均有详细说明文档和使用示例
- **CI/CD 准备**：集成 GoogleTest，支持自动化测试

### 项目亮点

- **丰富的运行示例**：每个设计模式都提供实际运行输出，帮助理解
- **线程安全实现**：为关键模式提供多种线程安全解决方案
- **性能优化指南**：包含锁优化、内存优化等最佳实践


### 测试结果

所有 242 个测试均已通过，包括：

- **创建型模式**：5 个模式，60+ 测试用例
- **结构型模式**：7 个模式，85+ 测试用例  
- **行为型模式**：11 个模式，120+ 测试用例

**运行结果摘要：**

```
$ ctest
242 tests from 23 test suites ran.
242 tests.
242 tests passed.
0 tests failed.
```

### 完整设计模式指南

本项目还提供了一份完整的《C++ 设计模式完整指南》，汇总了所有设计模式的详细说明、设计思想、实现要点和适用情况。完整指南位于 [docs/DESIGN_PATTERNS_GUIDE.md](docs/DESIGN_PATTERNS_GUIDE.md)。

您可以通过以下方法将指南转换为PDF格式以便离线阅读：

**方法一：使用在线转换工具**
- 将 `docs/DESIGN_PATTERNS_GUIDE.md` 文件内容复制到在线Markdown转PDF工具中，如 md2pdf、Pandoc online 等。

**方法二：使用Pandoc命令行工具**
```bash
# 安装pandoc (如果尚未安装)
# macOS: brew install pandoc
# Ubuntu: sudo apt-get install pandoc
# Windows: 从官网下载安装包

# 转换为PDF
pandoc docs/DESIGN_PATTERNS_GUIDE.md -o DESIGN_PATTERNS_GUIDE.pdf --pdf-engine=xelatex
```

**方法三：使用VS Code插件**
- 安装 "Markdown PDF" 插件
- 打开 `docs/DESIGN_PATTERNS_GUIDE.md`
- 按 `Ctrl+Shift+P` (或 `Cmd+Shift+P`)，输入 "Markdown PDF: Export" 并执行

**方法四：使用浏览器打印功能**
- 在支持Markdown预览的编辑器或GitHub中打开该文件
- 使用浏览器的打印功能 (Ctrl+P)，选择 "保存为PDF" 选项

---

## 一、面向对象设计原则概览

常见的面向对象设计原则（SOLID 等）是设计模式的基础：

- **SRP 单一职责原则**：一个类只做一件事，导致变化的原因应当只有一个。
- **OCP 开闭原则**：对扩展开放，对修改关闭。通过抽象和多态来扩展行为，而不是修改已有代码。
- **LSP 里氏替换原则**：子类必须能够替换父类，保证行为兼容，不破坏程序正确性。
- **ISP 接口隔离原则**：用多个专用接口代替一个臃肿的大接口，避免客户端依赖不需要的方法。
- **DIP 依赖倒置原则**：高层模块不依赖底层实现，二者共同依赖抽象；细节依赖抽象而不是相反。

在具体代码中，设计模式通常通过 **抽象类 / 接口、虚函数、多态、组合和委托** 等机制协同体现上述原则。

---

## 二、设计模式分类与工程目录结构

本工程按 GoF 经典分类，将 23 种设计模式划分为三大类：

- **创建型模式（Creational）—— 关注“对象如何被创建”**
  - 主要解决：谁来创建对象、在什么时机创建、如何屏蔽创建细节。
  - 典型特点：隐藏 `new`，通过工厂/原型/建造者等封装创建逻辑。
  - 包含模式：
    - `singleton`：单例模式
    - `factory_method`：工厂方法模式
    - `abstract_factory`：抽象工厂模式
    - `builder`：建造者模式
    - `prototype`：原型模式

- **结构型模式（Structural）—— 关注“类与对象如何组合”**
  - 主要解决：如何在不修改现有类的情况下复用代码、扩展行为。
  - 典型特点：通过组合、包装、代理等方式组织对象结构。
  - 包含模式：
    - `adapter`：适配器模式
    - `bridge`：桥接模式
    - `composite`：组合模式
    - `decorator`：装饰器模式
    - `facade`：外观模式
    - `flyweight`：享元模式
    - `proxy`：代理模式

- **行为型模式（Behavioral）—— 关注“对象之间如何协作”**
  - 主要解决：分配职责、封装算法、协调对象之间的通信与控制流程。
  - 典型特点：鼓励组合多种对象共同完成任务，避免过度继承。
  - 包含模式：
    - `chain_of_responsibility`：职责链模式
    - `command`：命令模式
    - `interpreter`：解释器模式
    - `iterator`：迭代器模式
    - `mediator`：中介者模式
    - `memento`：备忘录模式
    - `observer`：观察者模式
    - `state`：状态模式
    - `strategy`：策略模式
    - `template_method`：模板方法模式
    - `visitor`：访问者模式

### 目录结构说明

```
DesignPatterns/
  Readme.md                // 工程总览（设计原则、模式分类、使用方法）
  CMakeLists.txt           // CMake 工程配置
  src/                     // 源码目录
    creational/            // 创建型模式
      singleton/
        README.md            // 单例模式详解与用法（多种实现：懒汉/饿汉/线程安全等）
        main.cpp             // 只负责调用示例演示函数
        Singletons.h         // 各种单例实现类
      factory_method/
        README.md
        main.cpp
        FactoryMethod.h      // 工厂方法模式的产品 + 工厂层次结构
      abstract_factory/
        README.md
        main.cpp
        AbstractFactory.h    // 抽象工厂模式的产品族与具体工厂
      builder/
        README.md
        main.cpp
        Builder.h            // 建造者模式示例（游戏电脑/办公电脑）
      prototype/
        README.md
        main.cpp
        Prototype.h          // 原型模式示例（Clone 深拷贝）

    structural/            // 结构型模式
      adapter/
        README.md
        main.cpp
        Adapter.h            // 适配器模式实现
      bridge/
        README.md
        main.cpp
        Bridge.h             // 桥接模式实现
      composite/
        README.md
        main.cpp
        Composite.h          // 组合模式实现
      decorator/
        README.md
        main.cpp
        Decorator.h          // 装饰器模式实现
      facade/
        README.md
        main.cpp
        Facade.h             // 外观模式实现
      flyweight/
        README.md
        main.cpp
        Flyweight.h          // 享元模式实现
      proxy/
        README.md
        main.cpp
        Proxy.h              // 代理模式实现

    behavioral/            // 行为型模式
      chain_of_responsibility/
        README.md
        main.cpp
        ChainOfResponsibility.h  // 职责链模式实现
      command/
        README.md
        main.cpp
        Command.h            // 命令模式实现
      interpreter/
        README.md
        main.cpp
        Interpreter.h        // 解释器模式实现
      iterator/
        README.md
        main.cpp
        Iterator.h           // 迭代器模式实现
      mediator/
        README.md
        main.cpp
        Mediator.h           // 中介者模式实现
      memento/
        README.md
        main.cpp
        Memento.h            // 备忘录模式实现
      observer/
        README.md
        main.cpp
        Observer.h           // 观察者模式实现
      state/
        README.md
        main.cpp
        State.h              // 状态模式实现
      strategy/
        README.md
        main.cpp
        Strategy.h           // 策略模式实现
      template_method/
        README.md
        main.cpp
        TemplateMethod.h     // 模板方法模式实现
      visitor/
        README.md
        main.cpp
        Visitor.h            // 访问者模式实现
  tests/                   // 测试目录
    creational/            // 创建型模式测试
      singleton/
        test_singleton.cpp
      factory_method/
        test_factory_method.cpp
      abstract_factory/
        test_abstract_factory.cpp
      builder/
        test_builder.cpp
      prototype/
        test_prototype.cpp
    structural/            // 结构型模式测试
      adapter/
        test_adapter.cpp
      bridge/
        test_bridge.cpp
      composite/
        test_composite.cpp
      decorator/
        test_decorator.cpp
      facade/
        test_facade.cpp
      flyweight/
        test_flyweight.cpp
      proxy/
        test_proxy.cpp
    behavioral/            // 行为型模式测试
      chain_of_responsibility/
        test_chain_of_responsibility.cpp
      command/
        test_command.cpp
      interpreter/
        test_interpreter.cpp
      iterator/
        test_iterator.cpp
      mediator/
        test_mediator.cpp
      memento/
        test_memento.cpp
      observer/
        test_observer.cpp
      state/
        test_state.cpp
      strategy/
        test_strategy.cpp
      template_method/
        test_template_method.cpp
      visitor/
        test_visitor.cpp
  docs/                    // 文档目录
    BUILD.md               // 构建指南
    DESIGN_PATTERNS_GUIDE.md // 完整设计模式指南
  scripts/                 // 脚本目录
    build.sh               // 一键构建脚本
    run_tests.sh           // 一键测试脚本
    clean.sh               // 清理脚本
  .clang-format            // 代码格式化配置
  .gitignore               // Git 忽略规则
```

---

## 三、各设计模式简要索引（目的 + 设计思想 + 适用情况 + 实现要点）

> 下表用于快速回顾每个模式的核心要点，具体细节请参考各自目录下的 README 与示例代码。

### 3.1 创建型模式（Creational Patterns）

| 模式名称 | 目的 | 设计思想 | 实现要点 | 适用情况 |
|---------|------|----------|----------|----------|
| Singleton | 保证一个类只有一个实例，并提供全局访问点 | 隐藏构造函数，通过静态接口控制实例创建 | 1）私有化构造函数 2）创建static唯一实例 3）提供Instance()静态方法 | 配置中心、日志系统、线程池等全局唯一资源 |
| Factory Method | 定义创建对象的接口，让子类决定实例化哪个类 | "工厂方法 + 多态"推迟创建时机 | 1）定义抽象工厂类 2）子类重写工厂方法 3）客户端依赖抽象 | 框架需要将对象创建延迟到子类的场景 |
| Abstract Factory | 提供创建一族相关对象的接口，而无需指定具体类 | 以"产品族"为单位组织工厂 | 1）定义抽象工厂接口 2）具体工厂创建产品族 3）客户端可切换整套产品 | 跨平台UI、皮肤系统、多数据库适配等 |
| Builder | 将复杂对象构建过程与表示分离 | 固定构建步骤，不同建造者实现不同表示 | 1）定义抽象建造者 2）具体建造者实现细节 3）指挥者控制构建流程 | 构建步骤稳定但配置差异大的复杂对象 |
| Prototype | 通过克隆现有对象创建新对象 | "如何复制"替代"如何创建" | 1）定义原型接口 2）实现Clone()方法 3）客户端调用Clone() | 对象创建代价昂贵或运行时确定类型 |

### 3.2 结构型模式（Structural Patterns）

| 模式名称 | 目的 | 设计思想 | 实现要点 | 适用情况 |
|---------|------|----------|----------|----------|
| Adapter | 将一个类的接口转换成客户端期望的接口 | 增加适配器层复用旧接口 | 1）定义目标接口 2）适配器持有被适配者 3）转换调用 | 对接第三方库、老接口迁移、系统兼容 |
| Bridge | 将抽象部分与实现部分分离 | 通过组合而非继承解耦 | 1）抽象类持有实现接口 2）客户端注入不同实现 | 多维度变化（如"形状×颜色"） |
| Composite | 将对象组合成树形结构，统一对待单个和组合对象 | 叶子和组合对象统一抽象 | 1）定义Component接口 2）叶子实现具体行为 3）组合维护子组件容器 | 层次结构（文件系统、UI组件树等） |
| Decorator | 动态为对象添加职责而不改变类 | "包装对象"在前后增加功能 | 1）统一Component接口 2）装饰器持有Component 3）转发调用并增强 | 按需叠加功能（I/O过滤器、日志增强等） |
| Facade | 为复杂子系统提供统一入口 | 对外简单接口，对内协调子系统 | 1）外观类组合多个子系统 2）封装调用流程 3）客户端只依赖外观 | 封装复杂库/模块，对外提供简单API |
| Flyweight | 通过共享内部状态支持大量细粒度对象 | 共享内部状态，外部状态参数传入 | 1）定义享元接口 2）享元工厂缓存对象 3）客户端传入外部状态 | 大量重复对象（文字渲染、地图瓦片等） |
| Proxy | 为其他对象提供代理以控制访问 | 代理对象增加间接性控制访问 | 1）统一主题接口 2）代理持有真实对象 3）转发调用时附加控制逻辑 | 远程代理、虚拟代理、权限控制等 |

### 3.3 行为型模式（Behavioral Patterns）

| 模式名称 | 目的 | 设计思想 | 实现要点 | 适用情况 |
|---------|------|----------|----------|----------|
| Chain of Responsibility | 将请求沿链传递，由多个处理者依次尝试处理 | 每个处理者只关注自己能处理的部分 | 1）抽象处理者持有下一处理者 2）处理不了就传递 3）客户端组装处理链 | 日志处理、审批流程、拦截器链等 |
| Command | 将请求封装为对象 | "做什么"和"谁去做"分离 | 1）定义命令接口 2）具体命令持有接收者 3）调用者维护命令对象 | 撤销/重做、宏命令、任务队列等 |
| Interpreter | 为语言定义文法并建立解释器 | 用类表示文法规则，组合形成语法树 | 1）定义表达式接口 2）终结符/非终结符实现 3）构建语法树解释 | 小型DSL、规则引擎、表达式解析等 |
| Iterator | 提供顺序访问聚合对象元素的方法 | 将遍历逻辑从聚合对象中抽离 | 1）定义迭代器接口 2）具体迭代器维护状态 3）聚合提供创建迭代器方法 | 遍历容器、隐藏内部存储结构 |
| Mediator | 用中介对象封装一系列对象交互 | "多对多"转"多对一"，中介集中管理 | 1）中介者管理同事对象 2）同事只通知中介者 3）中介者决定转发对象 | 复杂UI组件交互、聊天室、协作场景 |
| Memento | 在不破坏封装前提下捕获并恢复对象内部状态 | 发起人、备忘录、看护者分离 | 1）发起人提供保存/恢复接口 2）备忘录保存状态 3）看护者管理备忘录 | 撤销、回滚、游戏存档等 |
| Observer | 定义一对多依赖关系，状态改变时自动通知依赖者 | 订阅/发布机制解耦"谁变化"和"谁关心" | 1）主题维护观察者列表 2）观察者实现更新接口 3）状态变化时通知所有观察者 | 事件系统、订阅通知、GUI事件等 |
| State | 对象内部状态改变时改变其行为 | 将状态相关行为分散到不同状态类 | 1）定义状态接口 2）具体状态实现行为 3）上下文在状态改变时切换状态 | 有限状态机、订单/工作流状态流转 |
| Strategy | 定义一系列算法并封装，使它们可互相替换 | 将易变算法从稳定上下文中抽离 | 1）定义策略接口 2）具体策略实现算法 3）上下文持有策略并委托执行 | 多种算法可选（排序、缓存、路由等） |
| Template Method | 定义算法骨架，将某些步骤延迟到子类实现 | 基类固定流程，子类实现变化点 | 1）基类实现模板方法 2）子类重写抽象步骤 3）客户端调用模板方法 | 固定流程+可变步骤（生命周期模板等） |
| Visitor | 不改变元素类的前提下，定义作用于元素的新操作 | "把操作对象化"，集中到访问者中 | 1）元素实现Accept接口 2）访问者为每种元素声明Visit 3）不同访问者实现不同操作 | 对复杂对象结构施加多种操作 |

---

## 四、构建与运行方式

### 1. 使用 CMake 统一管理（推荐）

本工程使用现代化的 CMake 配置，集成了 GoogleTest 测试框架，支持一键构建和测试：

```bash
cd DesignPatterns
./scripts/build.sh  # 一键构建
./scripts/run_tests.sh  # 一键运行所有测试
```

或者手动构建：

```bash
cd DesignPatterns
mkdir -p build && cd build
cmake ..
cmake --build .
```

配置完成后，会为每个设计模式生成独立的示例程序和测试程序：

- 示例程序：`singleton_example`、`strategy_example` 等
- 测试程序：`singleton_test`、`strategy_test` 等

你可以在 `build/` 目录下运行：

```bash
./singleton_example  # 运行单例模式示例
./singleton_test      # 运行单例模式测试
ctest                 # 运行所有测试
```

> 说明：本工程的 `CMakeLists.txt` 使用了自定义函数 `add_pattern_example` 和 `add_pattern_test`，使配置更加简洁和可维护。

### 2. 使用 g++ 手动编译单个示例

如果你不想使用 CMake，也可以直接对某个模式的 `main.cpp` 单独编译：

```bash
cd DesignPatterns/src/creational/singleton  # 注意：源码在 src/ 目录下

g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o singleton_example
./singleton_example
```

其它模式的示例目录下结构类似：

- `main.cpp` 只负责演示调用；
- 具体模式实现都放在对应的头文件（如 `Singletons.h`、`FactoryMethod.h` 等）中，方便在其他工程中复用。

### 3. 运行测试

本工程为所有 23 种设计模式提供了完整的单元测试，使用 GoogleTest 框架：

```bash
# 运行所有测试
cd build && ctest

# 运行特定模式的测试
./singleton_test
./strategy_test

# 运行特定模式的测试（ctest 方式）
ctest -R singleton
ctest -R strategy

# 查看测试详情
ctest --verbose
```

---

## 五、学习建议与阅读顺序

### 1. 推荐学习顺序

- **第一步：理解设计原则**
  - 阅读本文件中“面向对象设计原则概览”部分，理解 SOLID 等原则含义。
- **第二步：从创建型模式开始**
  - 从 `src/creational/singleton`、`factory_method` 等入手，体会“如何创建对象”。
- **第三步：学习结构型模式**
  - 阅读 `src/structural/decorator`、`proxy`、`facade` 等，体会“如何组合和扩展对象”。
- **第四步：深入行为型模式**
  - 学习 `src/behavioral/strategy`、`observer`、`state`、`template_method` 等，体会“如何组织对象之间的交互和算法”。

### 2. 每个模式子目录中包含的内容

在每个具体设计模式的子目录中，你将会看到：

- **README.md**：
  - 模式意图（解决什么问题、与哪类问题强相关）；
  - 设计思想与核心结构（参与者、职责和协作方式）；
  - 典型使用场景、优缺点与注意事项；
  - 与其他模式的对比与组合使用建议。
- **main.cpp 及其它 .h/.cpp 文件**：
  - 一个尽量简洁、但包含关键角色的示例实现；
  - 使用详细中文注释，解释设计动机与实现细节；
  - 可直接编译运行，用于调试与实验。
- **test_*.cpp**（测试文件）：
  - 完整的单元测试用例，覆盖基本功能、边界条件、线程安全等场景；
  - 使用 GoogleTest 框架，便于验证模式的正确性；
  - 展示各种使用场景和错误处理。

### 3. 现代 C++ 特性与最佳实践

本工程充分展示了现代 C++（C++11/14/17/20）的最佳实践：

- **智能指针**：`std::shared_ptr`、`std::unique_ptr`、`std::weak_ptr`
- **并发原语**：`std::mutex`、`std::shared_mutex`、`std::atomic`
- **移动语义**：减少不必要的拷贝，提升性能
- **lambda 表达式**：灵活的回调和算法实现
- **auto 关键字**：简化复杂类型声明
- **constexpr**：编译期计算优化

### 4. 测试驱动开发

每个设计模式都配有完整的单元测试，鼓励采用测试驱动开发的方式学习：

1. 先查看测试用例，了解模式的预期行为
2. 阅读源码实现，理解内部机制
3. 运行测试验证，确保理解正确
4. 修改代码实验，加深理解

---

## 六、线程安全与性能优化指南

### 6.1 设计模式中的线程安全问题

在多线程环境下，不同的设计模式需要关注不同的线程安全问题。本工程为关键模式提供了多种线程安全实现方案：

#### 创建型模式
- **Singleton 单例模式**：核心问题，必须保证在多线程下只创建一个实例
  - 问题：竞态条件、指令重排、内存可见性
  - 解决方案：
    - Meyers Singleton（C++11 magic static，最推荐）
    - 双重检查锁定 + std::atomic
    - std::call_once
    - 饿汉式（静态对象）

- **Factory 工厂模式**：通常无状态，但需注意：
  - 工厂注册表的线程安全访问
  - 对象池的并发控制

- **Prototype 原型模式**：
  - 深拷贝时的线程安全
  - 共享数据的读写保护

#### 结构型模式
- **Proxy 代理模式**：
  - 虚拟代理的懒加载线程安全
  - 缓存代理的并发访问

- **Decorator 装饰器模式**：
  - 装饰器链的线程安全

- **Flyweight 享元模式**：
  - 享元工厂缓存的线程安全
  - 共享内部状态的不可变性

#### 行为型模式
- **Observer 观察者模式**：高度关注线程安全
  - 问题：观察者列表并发修改、通知过程中注销、死锁
  - 解决方案：
    - std::shared_mutex 读写锁
    - weak_ptr 避免循环引用
    - 复制列表后通知（避免长时间持锁）

- **State 状态模式**：
  - 状态切换的原子性
  - 多线程状态转换的互斥

- **Command 命令模式**：
  - 命令队列的线程安全
  - 撤销/重做栈的并发保护

### 6.2 C++ 标准版本与线程安全特性

#### C++11 引入的关键特性
- ✅ **局部静态变量线程安全初始化**（magic static）
  ```cpp
  static Singleton& Instance() {
      static Singleton instance; // C++11 保证线程安全
      return instance;
  }
  ```

- ✅ **线程库**：`std::thread`, `std::mutex`, `std::lock_guard`
- ✅ **原子操作**：`std::atomic<T>`, memory order
- ✅ **一次性初始化**：`std::call_once`, `std::once_flag`
- ✅ **智能指针**：`std::shared_ptr`, `std::weak_ptr`, `std::unique_ptr`
- ✅ **移动语义**：减少不必要的拷贝
- ✅ **Lambda 表达式**：灵活的回调机制

#### C++14 改进
- ✅ `std::make_unique` 标准化
- ✅ 泛型 lambda（auto 参数）
- ✅ `std::shared_timed_mutex`（读写锁）

#### C++17 重要特性
- ✅ **inline 静态成员变量**
  ```cpp
  class Singleton {
      inline static Singleton instance_; // 无需类外定义
  };
  ```

- ✅ **std::shared_mutex**：标准化的读写锁
  ```cpp
  std::shared_lock<std::shared_mutex> readLock(mutex_);  // 读锁
  std::unique_lock<std::shared_mutex> writeLock(mutex_); // 写锁
  ```

- ✅ `std::optional`：替代 nullptr 判断
- ✅ 结构化绑定

#### C++20 突破性特性
- ✅ **std::atomic<std::shared_ptr<T>>** 特化
  ```cpp
  std::atomic<std::shared_ptr<T>> ptr; // 高效的原子智能指针
  ```

- ✅ **std::jthread**：自动 join 的线程
- ✅ **协程**（Coroutines）：异步编程新范式
- ✅ **Concepts**：约束模板参数
- ✅ **std::latch**, **std::barrier**, **std::counting_semaphore**
- ✅ `constinit` 关键字

#### C++23 持续优化
- ✅ `std::atomic` 性能提升
- ✅ `std::expected`：错误处理
- ✅ 更好的类型推导

### 6.3 性能优化技巧

#### 6.3.1 锁优化策略

1. **锁粒度优化**
   - 细粒度锁：只保护必要的关键区域
   - 读写锁：多读场景使用 `std::shared_mutex`

2. **无锁编程**
   - 使用 `std::atomic` 实现无锁数据结构
   - CAS（Compare-And-Swap）操作

3. **减少锁竞争**
   - 双重检查锁定（DCL）
   - 先复制再处理（避免长时间持锁）

#### 6.3.2 内存优化

1. **避免伪共享（False Sharing）**
   ```cpp
   // 缓存行对齐，避免伪共享
   alignas(64) std::atomic<int> counter1;
   alignas(64) std::atomic<int> counter2;
   ```

2. **内存序优化**
   - `memory_order_relaxed`：最弱，无同步
   - `memory_order_acquire/release`：保证顺序
   - `memory_order_seq_cst`：顺序一致性（默认，最强）

3. **对象池**
   ```cpp
   // 复用对象，减少分配/释放开销
   ObjectPool<Product> pool(100);
   auto obj = pool.Acquire();
   // 使用 obj
   pool.Release(std::move(obj));
   ```

#### 6.3.3 设计模式特定优化

1. **单例模式**
   - Meyers Singleton：性能最优
   - 原子操作 + DCL：高并发场景

2. **工厂模式**
   - 注册表模式替代继承层次
   - 对象池复用产品

3. **观察者模式**
   - 读写锁优化多读场景
   - 异步通知减少阻塞
   - weak_ptr 避免内存泄漏

### 6.4 最佳实践总结

1. **首选 C++11+ 现代特性**
   - 使用标准库线程原语
   - 优先选择 RAII 锁（`lock_guard`, `unique_lock`）
   - 智能指针管理生命周期

2. **避免常见陷阱**
   - 死锁：统一加锁顺序、使用 `std::lock`
   - 活锁：设置超时、使用 `try_lock`
   - 伪共享：缓存行对齐

3. **性能监控**
   - 使用性能分析工具（perf, valgrind）
   - 测量锁竞争和线程等待时间
   - 压力测试验证多线程正确性

4. **文档化**
   - 明确标注线程安全性
   - 记录锁的保护范围
   - 说明性能特性和限制

---

## 七、项目特性与扩展

### 7.1 已实现的功能

本工程已提供以下现代化功能和最佳实践：

- **✅ 完整的 GoF 23 种设计模式**：每种模式都有详细实现和示例
- **✅ 完整测试覆盖**：为所有 23 种模式提供单元测试，超过 265 个测试用例
- **✅ 现代化构建系统**：使用 CMake + GoogleTest，支持一键构建和测试
- **✅ 自动化脚本**：提供 build.sh、run_tests.sh、clean.sh 等便捷脚本
- **✅ 线程安全实现**：为关键模式提供多种线程安全实现方案
- **✅ 性能优化指导**：包含锁优化、内存优化、无锁编程等最佳实践
- **✅ 现代 C++ 特性**：充分利用 C++11/14/17/20 特性
- **✅ 详细中文文档**：每个模式都有详细说明和使用指南

### 7.2 扩展方向

你可以在此工程基础上继续扩展：

- **增加更多示例场景**：为同一模式增加多种应用场景（如多种不同形式的策略、不同类型的观察者等）。
- **性能基准测试**：
  - 对比不同实现方式的性能差异；
  - 测试多线程环境下的吞吐量和延迟；
  - 验证锁竞争和缓存命中率。
- **模式组合应用**：探索多个设计模式的组合使用，如 MVC 模式中组合使用观察者、工厂、策略等模式。
- **实战项目应用**：将这里的示例改造为你真实项目中的模块，思考抽象边界和扩展点。
- **性能分析工具集成**：集成 Valgrind、perf 等性能分析工具，提供性能分析报告。

通过不断阅读、运行和改造这些示例，你可以在实际工程中更自然地运用设计模式，而不是"为了模式而模式"。

## 🎯 总结

本项目是一个现代化的 C++ 设计模式学习工程，具备以下特色：

- **完整的 GoF 23 种设计模式实现**：每种模式都有详细的代码示例和中文注释
- **完整的测试覆盖**：为所有模式提供单元测试，确保代码质量
- **现代化的 C++ 特性**：充分利用 C++17/20 特性，展示最佳实践
- **线程安全和性能优化**：为关键模式提供多种线程安全实现方案
- **一键构建和测试**：使用 CMake + GoogleTest，配合自动化脚本，轻松构建和测试
- **详细的中文文档**：每个模式都有详尽的说明文档

通过本项目的学习，你不仅能掌握 23 种经典设计模式的实现原理，还能了解现代 C++ 的最佳实践、线程安全编程和性能优化技巧。这为在实际项目中正确应用设计模式奠定了坚实基础。

## 📄 许可证

本项目采用 Apache License Version 2.0 许可证，详细内容请参见 [LICENSE](LICENSE) 文件。
