# C++ 设计模式完整指南

本文档全面介绍了面向对象设计中的经典设计模式，包括创建型、结构型和行为型三大类共23种设计模式。每种模式都详细阐述了其意图、设计思想、核心问题、实现要点、注意事项和适用情况，并提供了C++代码示例和详细说明。本文档旨在帮助开发者深入理解设计模式的本质，掌握在实际项目中应用设计模式的最佳实践。

### 如何使用本指南

1. **初学者**：建议从设计原则开始，然后按创建型、结构型、行为型的顺序依次学习
2. **进阶者**：可以直接查找特定模式，重点关注实现要点和注意事项
3. **实践者**：参考最佳实践部分，了解模式选择和组合使用的技巧
4. **团队**：利用模式比较表快速选择适合的模式

## 目录
1. [设计原则](#设计原则)
2. [设计模式分类](#设计模式分类)
3. [创建型模式](#创建型模式)
4. [结构型模式](#结构型模式)
5. [行为型模式](#行为型模式)
6. [模式比较与选择](#模式比较与选择)
7. [最佳实践](#最佳实践)
8. [结论](#结论)

---

## 设计原则

### SOLID 原则

#### 单一职责原则 (Single Responsibility Principle, SRP)
一个类应该只有一个引起它变化的原因。即，一个类应该只负责一项职责。

**优点：**
- 降低类的复杂度
- 提高系统的可维护性
- 降低变更引起的风险

**示例：**
```cpp
// 不符合SRP的类
class BadEmployee {
private:
    string name;
    double salary;
public:
    void calculatePay();      // 计算薪资 - 职责1
    void saveToDatabase();    // 保存到数据库 - 职责2
    void generateReport();    // 生成报表 - 职责3
};

// 符合SRP的类
class Employee {
private:
    string name;
    double salary;
public:
    // 只负责员工基本信息
};

class PayCalculator {
public:
    double calculatePay(const Employee& emp);
};

class EmployeeRepository {
public:
    void save(const Employee& emp);
};

class ReportGenerator {
public:
    void generateReport(const Employee& emp);
};
```

**详细描述：**
上述示例展示了单一职责原则（SRP）的两个对比实现。第一个实现`BadEmployee`类承担了三个不同的职责：薪资计算、数据库保存和报表生成。这种设计违反了SRP，因为类有多个变化的原因。第二个实现将不同的职责分离到不同的类中：`Employee`类只负责员工的基本信息，`PayCalculator`类专门负责薪资计算，`EmployeeRepository`类负责数据持久化，`ReportGenerator`类专门负责报表生成。

**设计原因：**
1. **降低复杂度：** 每个类只负责一个功能，使得类更容易理解和维护
2. **提高可维护性：** 当需要修改薪资计算逻辑时，只需要修改`PayCalculator`类，不会影响其他功能
3. **减少耦合：** 各个类之间的依赖关系变得更清晰，降低了系统的耦合度
4. **增强可测试性：** 每个类的功能单一，更容易编写针对性的单元测试

#### 开闭原则 (Open-Closed Principle, OCP)
软件实体（类、模块、函数等）应该对扩展开放，对修改关闭。

**实现方式：**
- 使用抽象类和接口
- 使用策略模式、模板方法模式等
- 依赖注入

**示例：**
```cpp
// 抽象图形类
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override {
        return width * height;
    }
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

// 图形渲染器 - 对扩展开放，对修改关闭
class ShapeRenderer {
public:
    void render(const vector<unique_ptr<Shape>>& shapes) {
        for (const auto& shape : shapes) {
            cout << "Area: " << shape->area() << endl;
        }
    }
};
```

**详细描述：**
这个示例展示了开闭原则（OCP）的实际应用。代码定义了一个抽象的`Shape`基类，然后创建了`Rectangle`和`Circle`等具体形状类。`ShapeRenderer`类使用多态机制处理不同类型的形状对象，而无需修改其内部实现。当需要添加新的形状（如三角形、椭圆等）时，只需创建新的派生类并实现`area()`方法即可，无需修改`ShapeRenderer`类的代码。

**设计原因：**
1. **扩展性：** 可以轻松添加新的形状类型，只需继承`Shape`基类并实现`area()`方法
2. **稳定性：** 核心的渲染逻辑在`ShapeRenderer`中保持不变，减少了因添加新功能而引入错误的风险
3. **维护性：** 新增形状类与现有代码解耦，便于单独维护和测试
4. **复用性：** `ShapeRenderer`可以处理任意实现了`Shape`接口的对象，提高了代码复用性

#### 里氏替换原则 (Liskov Substitution Principle, LSP)
子类型必须能够替换掉它们的基类型。

**违反LSP的例子：**
```cpp
class Rectangle {
protected:
    int width, height;
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getArea() const { return width * height; }
};

// Square违反了LSP，因为它改变了setWidth/setHeight的行为
class Square : public Rectangle {
public:
    Square(int size) : Rectangle(size, size) {}
    
    // 这些重写违反了LSP
    void setWidth(int w) override {
        width = height = w;  // 改变了基类行为
    }
    
    void setHeight(int h) override {
        width = height = h;  // 改变了基类行为
    }
};
```

**详细描述：**
这个示例展示了里氏替换原则（LSP）的违反情况。`Square`类继承自`Rectangle`类，但在重写`setWidth()`和`setHeight()`方法时改变了基类的行为。在基类`Rectangle`中，设置宽度只会影响宽度属性，设置高度只会影响高度属性。然而，在`Square`类中，设置宽度或高度会同时改变宽度和高度，以保持正方形的特性。这种行为上的不一致会导致在使用基类指针或引用的地方，如果传入`Square`对象，可能会产生不符合预期的结果。

**设计原因：**
1. **行为一致性：** 子类不应改变基类的行为契约，否则会破坏程序的正确性
2. **可预测性：** 代码使用者应当能够预测方法调用的结果，即使在使用子类对象时也应如此
3. **安全性：** 遵循LSP可以确保程序在多态调用时的行为是安全的和可预期的
4. **替代性：** 子类对象应该能够在任何使用基类对象的地方无缝替换，而不会影响程序的正确性

#### 接口隔离原则 (Interface Segregation Principle, ISP)
客户端不应该被迫依赖于它们不使用的接口。

**示例：**
```cpp
// 不好的大接口
class IMachine {
public:
    virtual void print(Document& doc) = 0;
    virtual void scan(Document& doc) = 0;
    virtual void fax(Document& doc) = 0;
};

// 好的做法 - 小而专一的接口
class IPrinter {
public:
    virtual void print(Document& doc) = 0;
    virtual ~IPrinter() = default;
};

class IScanner {
public:
    virtual void scan(Document& doc) = 0;
    virtual ~IScanner() = default;
};

class IFax {
public:
    virtual void fax(Document& doc) = 0;
    virtual ~IFax() = default;
};

// 复合设备可以实现多个接口
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax {
public:
    void print(Document& doc) override { /* 实现 */ }
    void scan(Document& doc) override { /* 实现 */ }
    void fax(Document& doc) override { /* 实现 */ }
};
```

**详细描述：**
这个示例展示了接口隔离原则（ISP）的重要性。第一个设计`IMachine`定义了一个庞大的接口，包含了打印、扫描和传真三种功能。如果一个类只需要打印功能，却必须实现所有这三个方法（即使其他方法为空实现），这就违反了ISP。第二个设计将大接口拆分为三个小而专注的接口：`IPrinter`、`IScanner`和`IFax`。这样，只需要打印功能的类只需要实现`IPrinter`接口，多功能打印机可以同时实现多个接口。

**设计原因：**
1. **减少冗余：** 避免实现不需要的方法，减少代码冗余
2. **职责分离：** 每个接口专注于单一功能，符合单一职责原则
3. **灵活性：** 客户端只需依赖它们真正需要的接口
4. **可维护性：** 接口变更的影响范围更小，降低维护成本

#### 依赖倒置原则 (Dependency Inversion Principle, DIP)
高层模块不应该依赖于低层模块，两者都应该依赖于抽象。抽象不应该依赖于细节，细节应该依赖于抽象。

**示例：**
```cpp
// 低层模块
class MySQLDatabase {
public:
    void connect() { /* MySQL连接逻辑 */ }
    void query(const string& sql) { /* MySQL查询逻辑 */ }
};

// 高层模块直接依赖低层模块 - 不符合DIP
class UserServiceBad {
private:
    MySQLDatabase db;  // 直接依赖具体实现
public:
    void getUser(int id) {
        db.connect();
        db.query("SELECT * FROM users WHERE id = " + to_string(id));
    }
};

// 符合DIP的设计
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
};

class MySQLDatabase : public IDatabase {
public:
    void connect() override { /* MySQL连接逻辑 */ }
    void query(const string& sql) override { /* MySQL查询逻辑 */ }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void connect() override { /* PostgreSQL连接逻辑 */ }
    void query(const string& sql) override { /* PostgreSQL查询逻辑 */ }
};

class UserService {
private:
    shared_ptr<IDatabase> db;  // 依赖抽象
public:
    UserService(shared_ptr<IDatabase> database) : db(database) {}
    
    void getUser(int id) {
        db->connect();
        db->query("SELECT * FROM users WHERE id = " + to_string(id));
    }
};
```

**详细描述：**
这个示例展示了依赖倒置原则（DIP）的应用。第一部分展示了违反DIP的设计，`UserServiceBad`类直接依赖于具体的`MySQLDatabase`实现，这使得高层模块`UserServiceBad`与低层模块`MySQLDatabase`紧密耦合。第二部分展示了符合DIP的设计，通过引入`IDatabase`抽象接口，`UserService`类依赖于抽象而不是具体实现。这样，`UserService`可以与任何实现了`IDatabase`接口的数据库实现一起工作，无论是MySQL、PostgreSQL还是其他数据库系统。

**设计原因：**
1. **解耦合：** 高层模块不再依赖于低层模块的具体实现，降低了模块间的耦合度
2. **可扩展性：** 可以轻松添加新的数据库实现，而无需修改`UserService`代码
3. **可测试性：** 可以使用模拟对象（mock）来测试`UserService`，提高测试的灵活性
4. **可维护性：** 更换数据库实现变得简单，只需注入不同的实现即可

### 其他重要原则

#### 迪米特法则 (Law of Demeter, LoD)
一个对象应该对其他对象有最少的了解，只与直接朋友通信。

**示例：**
```cpp
class Address {
public:
    string street;
    string city;
    string zipCode;
};

class Employee {
private:
    Address addr;
public:
    const Address& getAddress() const { return addr; }  // 返回引用而不是指针
    // 不提供getAddress().getStreet()这样的链式调用
};

class Department {
private:
    vector<Employee> employees;
public:
    // 好的做法 - 只暴露必要的接口
    int getEmployeeCount() const { return employees.size(); }
    
    // 不好的做法 - 违反迪米特法则
    // Address getEmployeeAddress(int index) const { 
    //     return employees[index].getAddress(); 
    // }
};
```

**详细描述：**
这个示例展示了迪米特法则（LoD）的应用。迪米特法则也称为最少知识原则，它规定一个对象应该对其他对象有最少的了解。在示例中，`Department`类只与它的直接朋友（`Employee`类）通信，而不是深入到`Employee`内部去访问`Address`对象。如果`Department`直接返回`Employee`的`Address`（如注释中的代码所示），那么它就需要了解`Employee`的内部结构，这就违反了迪米特法则。

**设计原因：**
1. **降低耦合度：** 减少对象之间的依赖关系，使系统更加松散耦合
2. **提高模块化：** 每个对象只与少数几个直接朋友交互，提高了系统的模块化程度
3. **增强可维护性：** 当一个类的内部结构发生变化时，只有少数几个类会受到影响
4. **提高可理解性：** 对象之间的关系更简单明了，易于理解和维护

---

## 设计模式分类

### 模式概述

设计模式是针对软件设计中常见问题的可重用解决方案。GoF（Gang of Four）将23种设计模式分为三大类：

1. **创建型模式 (Creational)**：处理对象创建，抽象实例化过程
2. **结构型模式 (Structural)**：处理类和对象的组合，形成更大的结构
3. **行为型模式 (Behavioral)**：处理对象间的交互和职责分配

### 模式选择指南

| 问题领域 | 推荐模式 |
|---------|---------|
| 对象创建 | Factory Method, Abstract Factory, Builder, Prototype, Singleton |
| 对象组合 | Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy |
| 对象交互 | Chain of Responsibility, Command, Interpreter, Iterator, Mediator, Memento, Observer, State, Strategy, Template Method, Visitor |

---

## 创建型模式

### 1. 单例模式 (Singleton Pattern)

**意图：** 确保一个类只有一个实例，并提供全局访问点。

单例模式的核心意图是限制一个类只能创建一个实例，并提供一个全局访问点来获取该实例。这种模式在需要严格控制实例数量的场景中非常有用，特别是在系统中只需要一个对象来协调整个应用程序的行为时。通过强制类只有一个实例，可以确保数据一致性和避免重复的资源消耗。

**设计思想：** 控制实例的创建过程，防止外部创建新实例。

单例模式的设计思想是通过控制对象的创建过程来确保唯一性。通常通过将构造函数设为私有来阻止外部直接实例化，同时提供一个静态方法或静态变量来访问唯一的实例。这种设计将对象创建的责任从外部转移到类自身，从而实现了对实例化的精确控制。设计思想还强调了封装的重要性，隐藏了实例创建的复杂性，对外提供一个简单的接口。

**核心问题：** 如何确保类只存在一个实例，且该实例能被全局访问？

在实际开发中，某些对象只应该存在一个实例，比如日志管理器、配置中心、数据库连接池等。如果不加控制，可能会出现多个相同功能的实例，造成资源浪费、数据不一致等问题。同时，这个唯一的实例还需要被应用程序的各个部分访问，这就产生了既要保证唯一性又要提供全局访问的问题。在多线程环境中，这个问题变得更加复杂，需要考虑线程安全的实例创建。

**实现要点：**
1. **私有构造函数：** 将类的构造函数声明为私有，防止外部直接创建实例，这是确保唯一性的基础。
2. **静态实例：** 使用静态成员变量来存储唯一实例，静态变量的生命周期贯穿整个程序运行期间，确保实例在程序结束前一直存在。
3. **全局访问方法：** 提供公共的静态方法来获取实例，这是外界访问单例的唯一途径。
4. **防止拷贝：** 重载或删除拷贝构造函数和赋值运算符，防止通过拷贝创建新实例。
5. **线程安全：** 在多线程环境中，需要确保实例创建过程的线程安全性。

**注意事项：**
- **线程安全问题：** 在多线程环境下，多个线程可能同时尝试创建实例，需要使用适当的同步机制（如互斥锁）来确保线程安全。
- **内存泄漏风险：** 静态实例的生命周期与程序相同，需要谨慎处理内存释放，尤其是在程序结束时。
- **滥用导致紧耦合：** 过度使用单例模式可能导致代码过度依赖全局状态，增加模块间的耦合度，降低代码的可测试性和可维护性。
- **测试困难：** 单例模式的全局状态特性使其难以在单元测试中进行模拟和重置。
- **序列化问题：** 如果单例类需要被序列化，需要特别注意反序列化时可能会创建新实例的问题。

**适用情况：**
- **配置管理器：** 应用程序通常只需要一个配置管理器来读取和管理全局配置，确保所有组件使用相同的配置。
- **日志记录器：** 日志系统通常需要统一管理，避免多个日志记录器同时写入导致混乱，单例模式可以确保日志输出的一致性。
- **数据库连接池：** 连接池需要统一管理数据库连接，避免创建过多连接消耗资源，单例模式可以确保连接的集中管理。
- **线程池：** 应用程序通常只需要一个线程池来管理系统中的所有工作线程，便于统一调度和管理。
- **缓存管理器：** 缓存通常需要全局共享，以确保数据一致性和资源有效利用。
- **硬件接口管理：** 如打印机管理器，系统中通常只需要一个实例来管理打印队列。

#### 懒汉式单例（线程安全）
```cpp
class LazySingleton {
private:
    static shared_ptr<LazySingleton> instance;
    static mutex mtx;
    
    // 私有构造函数
    LazySingleton() = default;
    
public:
    // 删除拷贝构造和赋值操作
    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;
    
    static shared_ptr<LazySingleton> getInstance() {
        lock_guard<mutex> lock(mtx);
        if (!instance) {
            instance = make_shared<LazySingleton>();
        }
        return instance;
    }
};

// 静态成员定义
shared_ptr<LazySingleton> LazySingleton::instance = nullptr;
mutex LazySingleton::mtx;
```

**详细描述：**
懒汉式单例是一种延迟初始化的单例实现方式，只有在第一次调用`getInstance()`方法时才会创建实例。为了保证线程安全，使用了互斥锁`mutex`来防止多个线程同时创建实例。在`getInstance()`方法中，首先获取锁，然后检查实例是否已经存在，如果不存在则创建实例。这种方法确保了即使在多线程环境下也能保证只创建一个实例。

**设计原因：**
1. **延迟初始化：** 只在需要时才创建实例，节省系统资源
2. **线程安全：** 使用互斥锁确保多线程环境下的安全性
3. **唯一性：** 通过私有构造函数和静态实例确保全局唯一性
4. **资源控制：** 防止外部创建多个实例，有效控制资源使用

#### 饿汉式单例
```cpp
class EagerSingleton {
private:
    static shared_ptr<EagerSingleton> instance;
    
    EagerSingleton() = default;
    
public:
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;
    
    static shared_ptr<EagerSingleton> getInstance() {
        return instance;
    }
};

// 静态初始化
shared_ptr<EagerSingleton> EagerSingleton::instance = make_shared<EagerSingleton>();
```

**详细描述：**
饿汉式单例在程序启动时就会创建实例，而不是等到第一次使用时。静态成员变量`instance`在类加载时就被初始化，确保了实例的预先创建。这种方式天然就是线程安全的，因为实例在程序启动时就已经创建好了，后续的`getInstance()`调用只是返回已有的实例，无需额外的同步措施。

**设计原因：**
1. **线程安全：** 由于实例在程序启动时就创建，因此天然线程安全
2. **性能优势：** 获取实例时无需同步操作，性能较高
3. **简单实现：** 实现相对简单，无需复杂的同步机制
4. **内存预分配：** 提前分配内存，有助于内存管理

#### Meyers 单例（C++11线程安全）
```cpp
class MeyersSingleton {
private:
    MeyersSingleton() = default;
    
public:
    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;
    
    static MeyersSingleton& getInstance() {
        static MeyersSingleton instance;  // C++11 保证线程安全
        return instance;
    }
};
```

**详细描述：**
Meyers单例是利用C++11标准中局部静态变量的线程安全初始化特性实现的单例模式。在C++11及之后的标准中，首次访问局部静态变量时，编译器会自动确保其初始化过程是线程安全的。这种方法既实现了延迟初始化，又保证了线程安全，而且代码简洁，性能优秀。需要注意的是，返回类型是引用而不是指针，这可以避免空指针检查的需要。

**设计原因：**
1. **线程安全：** C++11标准保证局部静态变量初始化的线程安全性
2. **延迟初始化：** 实例只在首次调用时创建，节省资源
3. **性能优秀：** 无锁实现，性能高于加锁的懒汉式实现
4. **代码简洁：** 实现简单，易于理解和维护

#### 双重检查锁定单例
```cpp
class DoubleCheckedSingleton {
private:
    static atomic<shared_ptr<DoubleCheckedSingleton>> instance;
    static mutex mtx;
    
    DoubleCheckedSingleton() = default;
    
public:
    DoubleCheckedSingleton(const DoubleCheckedSingleton&) = delete;
    DoubleCheckedSingleton& operator=(const DoubleCheckedSingleton&) = delete;
    
    static shared_ptr<DoubleCheckedSingleton> getInstance() {
        shared_ptr<DoubleCheckedSingleton> tmp = instance.load(memory_order_relaxed);
        if (!tmp) {
            lock_guard<mutex> lock(mtx);
            tmp = instance.load(memory_order_relaxed);
            if (!tmp) {
                tmp = make_shared<DoubleCheckedSingleton>();
                instance.store(tmp, memory_order_relaxed);
            }
        }
        return tmp;
    }
};
```

**详细描述：**
双重检查锁定单例是一种高级的单例实现方式，旨在平衡线程安全和性能。它通过两次检查实例是否存在的条件来避免每次调用`getInstance()`时都需要获取锁。第一次检查是在没有同步的情况下进行的，只有在实例确实不存在时才进入同步块。进入同步块后，再次检查实例是否仍然不存在（因为可能在等待锁的过程中被其他线程创建），然后才创建实例。使用`std::atomic`确保了多线程环境下的内存可见性。

**设计原因：**
1. **性能优化：** 避免每次获取实例都进行同步操作，提高性能
2. **线程安全：** 通过双重检查和原子操作确保多线程环境下的安全性
3. **内存可见性：** 使用原子操作确保实例创建后的可见性
4. **减少锁竞争：** 只在必要时才使用锁，减少线程间的竞争

### 2. 工厂方法模式 (Factory Method Pattern)

**意图：** 定义创建对象的接口，让子类决定实例化哪个类。

工厂方法模式的核心意图是定义一个创建对象的接口，但由子类决定具体实例化哪个类。这种模式将对象的创建委托给子类，使得类可以推迟到子类中进行实例化。通过这种方式，父类可以专注于定义对象创建的通用接口和流程，而具体的创建细节则由子类来实现。这有助于在不修改现有代码的情况下扩展新的产品类型，符合开闭原则。

**设计思想：** 将对象的创建过程延迟到子类，实现创建逻辑与使用逻辑的解耦。

工厂方法模式的设计思想是通过将对象的创建过程延迟到子类来实现解耦。父类定义了一个抽象的工厂方法，声明了创建对象的接口，但并不实现具体的创建逻辑。子类继承父类并实现这个抽象的工厂方法，决定具体创建哪种类型的对象。这种设计将对象的创建与使用分离，客户端代码只需要依赖于抽象接口，而不需要知道具体的产品类型。这样，当需要添加新的产品时，只需创建新的具体工厂类，而不需要修改现有的客户端代码。

**核心问题：** 如何在不知道具体类的情况下创建对象？

在软件设计中，经常面临这样的问题：我们需要创建某个对象，但并不确定具体应该创建哪个类的实例，或者希望将对象创建的具体类型决策推迟到运行时。如果直接在代码中硬编码具体的类名，会导致代码与特定实现紧密耦合，难以扩展和维护。工厂方法模式解决这一问题，通过提供一个抽象的创建接口，允许子类决定具体创建哪种类型的对象，从而实现了对象创建的灵活性和可扩展性。

**实现要点：**
1. **定义抽象工厂接口：** 创建一个抽象基类或接口，声明工厂方法，该方法返回一个抽象产品类型的对象。
2. **创建具体工厂类：** 实现抽象工厂接口，提供具体的工厂方法实现，决定创建哪种具体的产品对象。
3. **客户端依赖抽象接口：** 客户端代码只依赖于抽象工厂和抽象产品接口，不依赖具体实现类。
4. **产品继承体系：** 定义一个产品抽象基类或接口，以及多个具体产品实现类。
5. **延迟绑定：** 将对象创建的具体类型决策延迟到运行时，通过具体工厂类来确定。

**注意事项：**
- **类层次结构复杂：** 工厂方法模式会增加系统的类数量，形成复杂的继承层次，增加了理解和维护的难度。
- **可能产生过多子类：** 每个具体产品通常需要对应一个具体工厂子类，当产品种类很多时，会产生大量的子类。
- **增加系统复杂度：** 对于简单对象的创建，使用工厂方法模式可能会显得过于复杂。
- **抽象层开销：** 需要维护抽象层，增加了系统的抽象复杂性。
- **运行时动态性：** 一旦选择了具体工厂类，创建的产品类型就固定了，无法在运行时动态切换。

**适用情况：**
- **创建对象需要复杂的初始化：** 当对象创建过程涉及复杂的初始化逻辑，或者需要根据参数动态决定创建哪种类型时，工厂方法可以将这些复杂性封装起来。
- **需要解耦具体类：** 当客户端代码不应该依赖于具体的产品类，而应该依赖于抽象接口时，工厂方法可以帮助实现这种解耦。
- **在运行时指定要创建的对象类型：** 当需要根据配置、用户输入或其他运行时因素来决定创建哪种对象时。
- **需要框架扩展性：** 当设计框架时，允许用户通过继承工厂类来添加新的产品类型。
- **平行类层次：** 当有平行的类层次结构时（如产品类和工厂类），工厂方法可以很好地管理这种关系。

```cpp
// 产品接口
class Product {
public:
    virtual ~Product() = default;
    virtual string getName() const = 0;
};

// 具体产品
class ConcreteProductA : public Product {
public:
    string getName() const override {
        return "Product A";
    }
};

class ConcreteProductB : public Product {
public:
    string getName() const override {
        return "Product B";
    }
};

// 工厂接口
class Creator {
public:
    virtual ~Creator() = default;
    virtual unique_ptr<Product> createProduct() = 0;
    
    void someOperation() {
        auto product = createProduct();
        cout << "Created: " << product->getName() << endl;
    }
};

// 具体工厂
class ConcreteCreatorA : public Creator {
public:
    unique_ptr<Product> createProduct() override {
        return make_unique<ConcreteProductA>();
    }
};

class ConcreteCreatorB : public Creator {
public:
    unique_ptr<Product> createProduct() override {
        return make_unique<ConcreteProductB>();
    }
};
```

**详细描述：**
工厂方法模式定义了一个创建对象的接口，让子类决定实例化哪个类。在这个示例中，`Product`是产品的抽象接口，`ConcreteProductA`和`ConcreteProductB`是具体的产品实现。`Creator`是工厂的抽象类，定义了创建产品的接口`createProduct()`，并包含一个使用产品的操作`somOperation()`。`ConcreteCreatorA`和`ConcreteCreatorB`是具体的工厂类，分别负责创建`ConcreteProductA`和`ConcreteProductB`。客户端代码只需要依赖于抽象的`Creator`类，而不需要知道具体的产品类型。

**设计原因：**
1. **解耦合：** 客户端与具体产品类解耦，只需要知道抽象接口
2. **可扩展性：** 可以轻松添加新的产品和对应的工厂类
3. **遵循开闭原则：** 添加新产品无需修改现有代码，只需添加新的子类
4. **灵活性：** 子类可以控制实例化对象的过程

### 3. 抽象工厂模式 (Abstract Factory Pattern)

**意图：** 提供创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。

抽象工厂模式的核心意图是提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。与工厂方法模式不同，抽象工厂关注的不是创建单个对象，而是创建一个产品族（一组相关的产品对象）。这种模式特别适用于需要创建多个相关对象的场景，确保创建出来的对象属于同一个产品变体或系列。通过抽象工厂，客户端代码可以从具体的实现中解耦，只需要与抽象接口交互即可获得一整套协调工作的对象。

**设计思想：** 提供一个创建对象家族的接口，而不必明确指定具体类。

抽象工厂模式的设计思想是提供一个创建对象家族的接口，使得客户端可以一次性获得一组相互关联或相互依赖的对象，而无需关心这些对象的具体实现。这种设计强调了对象之间的协调性，确保从同一个工厂创建出的对象能够很好地协同工作。抽象工厂定义了创建产品族的接口，具体工厂实现这些接口，负责创建特定产品族的全部对象。这种设计将产品族的概念显式化，使得系统能够轻松切换整个产品系列。

**核心问题：** 如何创建一组相关的对象，而无需指定具体的类？

在复杂的系统中，往往需要创建多个相关联的对象，这些对象之间存在内在的依赖关系，必须一起使用才能正常工作。如果分别创建这些对象，可能会导致对象之间不兼容的问题。例如，在GUI系统中，按钮、文本框、滚动条等控件需要属于同一个主题才能保持视觉一致性。抽象工厂模式解决了这一问题，通过提供一个统一的接口来创建整个产品族，确保创建出来的对象在风格、主题等方面保持一致。

**实现要点：**
1. **定义抽象工厂接口：** 声明创建产品族的抽象方法，每个方法负责创建一种类型的产品对象。
2. **创建具体工厂实现：** 实现抽象工厂接口，提供具体的创建方法，创建特定产品族的全部对象。
3. **客户端通过工厂接口创建对象：** 客户端代码依赖于抽象工厂接口，通过工厂创建所需的产品对象。
4. **产品族继承体系：** 为每类产品定义抽象基类或接口，并提供具体的实现类。
5. **产品族一致性：** 确保从同一具体工厂创建的对象属于同一个产品变体。

**注意事项：**
- **扩展困难：** 添加新的产品族相对容易，但添加新的产品类型则很困难，因为需要修改抽象工厂接口和所有具体工厂实现。
- **类层次复杂：** 抽象工厂模式会创建复杂的类层次结构，包含多个抽象类和实现类，增加了系统的复杂性。
- **维护成本高：** 需要维护多组相关的产品类和对应的工厂类，增加了维护的复杂性。
- **性能开销：** 抽象工厂引入了额外的间接层，可能会带来一定的性能开销。
- **初始化复杂：** 需要正确选择和配置合适的工厂实例，否则可能导致创建错误的产品族。

**适用情况：**
- **跨平台UI系统：** 需要为不同操作系统创建相应的UI控件，如Windows按钮、Linux按钮、Mac按钮等，确保同一平台上所有控件风格一致。
- **皮肤系统：** 在应用程序中实现主题或皮肤功能，如深色主题、浅色主题，每种主题包含一套协调的界面元素。
- **多数据库适配：** 为不同的数据库系统创建相应的连接、语句、结果集等对象，如MySQL系列、PostgreSQL系列、Oracle系列等。
- **游戏开发：** 创建具有特定风格的游戏元素，如奇幻风格的武器、盔甲、角色，科幻风格的武器、盔甲、角色等。
- **报告生成系统：** 生成不同格式的报告（PDF、HTML、Excel），每种格式需要相应的格式化器、渲染器等组件。

```cpp
// 抽象产品A
class AbstractProductA {
public:
    virtual ~AbstractProductA() = default;
    virtual string usefulFunctionA() const = 0;
};

class ConcreteProductA1 : public AbstractProductA {
public:
    string usefulFunctionA() const override {
        return "Result of ConcreteProductA1";
    }
};

class ConcreteProductA2 : public AbstractProductA {
public:
    string usefulFunctionA() const override {
        return "Result of ConcreteProductA2";
    }
};

// 抽象产品B
class AbstractProductB {
public:
    virtual ~AbstractProductB() = default;
    virtual string usefulFunctionB() const = 0;
    virtual string anotherUsefulFunctionB(const AbstractProductA& collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB {
public:
    string usefulFunctionB() const override {
        return "Result of ConcreteProductB1";
    }
    
    string anotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
        return "Result of B1 collaborating with " + collaborator.usefulFunctionA();
    }
};

class ConcreteProductB2 : public AbstractProductB {
public:
    string usefulFunctionB() const override {
        return "Result of ConcreteProductB2";
    }
    
    string anotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
        return "Result of B2 collaborating with " + collaborator.usefulFunctionA();
    }
};

// 抽象工厂
class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;
    virtual unique_ptr<AbstractProductA> createProductA() const = 0;
    virtual unique_ptr<AbstractProductB> createProductB() const = 0;
};

class ConcreteFactory1 : public AbstractFactory {
public:
    unique_ptr<AbstractProductA> createProductA() const override {
        return make_unique<ConcreteProductA1>();
    }
    
    unique_ptr<AbstractProductB> createProductB() const override {
        return make_unique<ConcreteProductB1>();
    }
};

class ConcreteFactory2 : public AbstractFactory {
public:
    unique_ptr<AbstractProductA> createProductA() const override {
        return make_unique<ConcreteProductA2>();
    }
    
    unique_ptr<AbstractProductB> createProductB() const override {
        return make_unique<ConcreteProductB2>();
    }
};
```

**详细描述：**
抽象工厂模式提供了一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。在这个示例中，我们有两个产品族：产品A族（`ConcreteProductA1`和`ConcreteProductA2`）和产品B族（`ConcreteProductB1`和`ConcreteProductB2`）。`AbstractFactory`定义了创建这两个产品族的接口。`ConcreteFactory1`创建产品族1（`ConcreteProductA1`和`ConcreteProductB1`），`ConcreteFactory2`创建产品族2（`ConcreteProductA2`和`ConcreteProductB2`）。这样可以确保创建出来的产品能够相互协作。

**设计原因：**
1. **产品族管理：** 统一管理一系列相关的产品，确保产品间的兼容性
2. **解耦合：** 客户端与具体产品类解耦，只需要知道抽象接口
3. **一致性：** 确保创建的产品属于同一个产品族，能够正确协作
4. **可扩展性：** 可以轻松添加新的产品族，而无需修改现有代码

### 4. 建造者模式 (Builder Pattern)

**意图：** 将一个复杂对象的构建与其表示分离，使得同样的构建过程可以创建不同的表示。

建造者模式的核心意图是将复杂对象的构建过程与其最终的表示分离，使得相同的构建过程可以创建不同的表示形式。这种模式特别适用于那些需要经过多个步骤才能完成创建的复杂对象。通过将构建过程封装在建造者中，可以对构建过程进行精细控制，同时保持对象表示的灵活性。客户端不必了解对象构建的复杂性，只需指定所需的构建类型，就能得到相应的产品。

**设计思想：** 将复杂对象的构建过程分解为多个简单的步骤，通过不同顺序的构建获得不同表示。

建造者模式的设计思想是将复杂的构建过程分解成一系列简单的步骤，并将这些步骤的执行过程与具体的实现分离。这种设计将构建过程抽象出来，通过指挥者（Director）类来控制构建的顺序和步骤，而具体的构建细节则由具体的建造者（Builder）来实现。这种分离使得可以使用相同的构建过程来创建不同的表示，只需更换不同的建造者即可。同时，客户端代码只需要与抽象接口交互，无需关心具体的构建细节。

**核心问题：** 如何分步骤构建复杂对象，且能灵活控制构建过程？

在软件开发中，经常会遇到需要创建复杂对象的场景，这些对象通常由多个部分组成，需要经过多个步骤才能完成创建。直接在客户端代码中实现这些构建逻辑会导致代码复杂、难以维护和扩展。此外，可能需要创建同一类对象的不同表示形式，但构建过程基本相同。建造者模式解决了这些问题，通过将构建过程封装在专门的建造者类中，使客户端能够以一致的方式构建复杂对象，同时保持构建过程的灵活性。

**实现要点：**
1. **定义建造者接口：** 定义一个抽象的建造者接口，声明构建对象各个部分的方法，以及获取最终产品的接口。
2. **实现具体建造者：** 实现建造者接口，提供具体的构建逻辑，定义如何构建对象的各个部分。
3. **使用指挥者控制构建过程：** 指挥者类负责调用建造者的各个构建方法，控制构建的顺序和流程。
4. **产品类：** 定义要构建的复杂对象，包含构成对象的各个部分。
5. **构建过程与表示分离：** 确保构建过程与最终对象的表示是分离的，相同的构建过程可以产生不同的表示。

**注意事项：**
- **代码量较大：** 建造者模式需要创建多个类（抽象建造者、具体建造者、指挥者、产品等），会增加代码量和系统复杂度。
- **适合构建复杂对象：** 对于简单的对象创建，使用建造者模式可能会过度设计，只适用于构建过程复杂的对象。
- **构建过程固定：** 如果构建过程需要频繁变化，指挥者类需要不断修改，影响系统的稳定性。
- **内存占用：** 在构建过程中可能需要临时存储中间状态，增加内存开销。
- **线程安全：** 建造者模式通常不是线程安全的，需要在多线程环境中特别注意。

**适用情况：**
- **构建复杂对象：** 当对象需要多个部分组成，且构建过程复杂时，如构建XML文档、HTML页面、复杂的数据对象等。
- **需要不同表示的相同构建过程：** 当需要使用相同的构建步骤创建不同表现形式的对象时，如构建不同样式的文档、不同主题的UI界面等。
- **对象构建过程需要精确控制：** 当构建过程需要按特定顺序执行，或需要根据不同条件执行不同构建步骤时。
- **配置对象：** 当需要根据不同的配置参数构建对象时，可以使用建造者模式来处理复杂的配置过程。
- **不可变对象：** 当需要创建不可变对象，且对象需要多个参数时，建造者模式可以简化构建过程。

```cpp
class Product {
private:
    vector<string> parts;
    
public:
    void addPart(const string& part) {
        parts.push_back(part);
    }
    
    string listParts() const {
        stringstream ss;
        ss << "Product parts: ";
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << parts[i];
        }
        return ss.str();
    }
};

class Builder {
public:
    virtual ~Builder() = default;
    virtual void producePartA() = 0;
    virtual void producePartB() = 0;
    virtual void producePartC() = 0;
    virtual unique_ptr<Product> getProduct() = 0;
};

class ConcreteBuilder : public Builder {
private:
    unique_ptr<Product> product;
    
public:
    ConcreteBuilder() {
        this->reset();
    }
    
    void reset() {
        product = make_unique<Product>();
    }
    
    void producePartA() override {
        product->addPart("PartA1");
    }
    
    void producePartB() override {
        product->addPart("PartB1");
    }
    
    void producePartC() override {
        product->addPart("PartC1");
    }
    
    unique_ptr<Product> getProduct() override {
        unique_ptr<Product> result = move(product);
        this->reset();
        return result;
    }
};

class Director {
private:
    Builder* builder;
    
public:
    void setBuilder(Builder* b) {
        this->builder = b;
    }
    
    void buildMinimalViableProduct() {
        builder->producePartA();
    }
    
    void buildFullFeaturedProduct() {
        builder->producePartA();
        builder->producePartB();
        builder->producePartC();
    }
};
```

**详细描述：**
建造者模式将一个复杂对象的构建与其表示分离，使得同样的构建过程可以创建不同的表示。在这个示例中，`Product`类代表最终要构建的复杂对象，`Builder`是抽象建造者，定义了构建各个部件的方法。`ConcreteBuilder`是具体建造者，实现了构建部件的具体逻辑。`Director`类控制构建过程，它知道如何使用建造者构建对象，但不关心具体的构建细节。客户端可以通过`Director`来构建最小可行产品或全功能产品，而具体如何构建由建造者决定。

**设计原因：**
1. **复杂对象构建：** 适用于构建具有多个组成部分的复杂对象
2. **构建过程控制：** 将构建过程与具体实现分离，便于控制构建流程
3. **多变表示：** 相同的构建过程可以创建不同的表示
4. **可扩展性：** 可以轻松添加新的建造者来创建不同类型的对象

### 5. 原型模式 (Prototype Pattern)

**意图：** 通过复制现有对象来创建新对象，而不是通过实例化类。

原型模式的核心意图是通过复制现有对象来创建新对象，而不是通过传统的类实例化方式。这种方式避免了复杂的初始化过程，特别是当对象创建需要大量资源或时间时，原型模式可以显著提高性能。该模式允许在运行时动态地确定要创建的对象类型，通过克隆机制创建新的实例，同时保留原有对象的属性和状态。原型模式特别适用于需要创建大量相似对象的场景。

**设计思想：** 通过克隆机制创建对象，避免复杂的初始化过程。

原型模式的设计思想是通过克隆现有对象来创建新对象，而不是通过调用构造函数。这种设计将对象的创建过程委托给对象本身，每个对象都可以作为创建新对象的"原型"。通过实现克隆接口，对象可以创建自身的副本。这种设计避免了复杂的初始化过程，特别是当对象的创建依赖于外部资源或复杂的计算过程时。设计思想还强调了对象的自描述能力，即对象可以描述如何创建自己的副本。

**核心问题：** 如何高效创建相似对象，避免重复初始化？

在某些情况下，创建对象的过程可能非常复杂和耗时，例如需要从数据库加载大量数据、执行复杂的计算或建立网络连接等。如果需要创建多个相似的对象，每次都经历完整的初始化过程会导致性能问题。原型模式解决了这一问题，通过复制现有对象（原型）来创建新对象，从而避免了重复的复杂初始化过程。这样不仅可以提高性能，还可以在运行时动态地确定对象的类型和初始状态。

**实现要点：**
1. **定义原型接口：** 定义一个包含克隆方法的原型接口，所有需要支持克隆的类都实现此接口。
2. **实现克隆方法：** 在具体类中实现克隆方法，通常使用拷贝构造函数或拷贝赋值操作符来创建对象副本。
3. **客户端通过克隆创建对象：** 客户端代码通过调用克隆方法来创建新对象，而不是直接实例化类。
4. **处理深拷贝与浅拷贝：** 根据对象的复杂程度，决定使用深拷贝还是浅拷贝来实现克隆。
5. **注册表机制：** 可选地实现原型注册表，允许在运行时动态注册和检索原型对象。

**注意事项：**
- **深拷贝vs浅拷贝问题：** 需要仔细考虑对象的克隆方式，浅拷贝只复制对象的值，而深拷贝还会复制对象所引用的其他对象，这可能会影响性能和内存使用。
- **需要实现拷贝构造函数：** 在C++中，需要正确实现拷贝构造函数以支持对象克隆，这可能比较复杂，尤其是对于包含动态分配内存的对象。
- **循环引用问题：** 如果对象图中存在循环引用，克隆过程可能会陷入无限递归。
- **权限控制：** 某些对象可能不应该被克隆，需要考虑访问控制机制。
- **性能权衡：** 对于简单对象，克隆可能不如直接构造高效。

**适用情况：**
- **对象创建代价昂贵：** 当对象的创建需要大量资源（如数据库连接、网络请求、复杂计算等）时，通过克隆现有对象可以避免重复的资源消耗。
- **运行时确定对象类型：** 当需要在运行时动态确定要创建的对象类型时，可以使用原型模式，从预先配置好的原型对象进行克隆。
- **需要创建相似对象：** 当需要创建多个相似的对象，且这些对象只在某些属性上有细微差别时，可以基于一个原型对象进行克隆并微调属性。
- **动态加载对象：** 当对象类型在编译时未知，需要在运行时动态加载和克隆对象时。
- **配置对象：** 当需要基于预设的配置模板创建多个配置对象时，原型模式可以简化创建过程。

```cpp
class Prototype {
public:
    virtual ~Prototype() = default;
    virtual unique_ptr<Prototype> clone() const = 0;
    virtual string getType() const = 0;
};

class ConcretePrototypeA : public Prototype {
private:
    string value;
    
public:
    ConcretePrototypeA(const string& v) : value(v) {}
    
    unique_ptr<Prototype> clone() const override {
        return make_unique<ConcretePrototypeA>(*this);
    }
    
    string getType() const override {
        return "ConcretePrototypeA: " + value;
    }
    
    // 拷贝构造函数用于克隆
    ConcretePrototypeA(const ConcretePrototypeA& other) : value(other.value) {}
};

class ConcretePrototypeB : public Prototype {
private:
    int number;
    
public:
    ConcretePrototypeB(int n) : number(n) {}
    
    unique_ptr<Prototype> clone() const override {
        return make_unique<ConcretePrototypeB>(*this);
    }
    
    string getType() const override {
        return "ConcretePrototypeB: " + to_string(number);
    }
    
    // 拷贝构造函数用于克隆
    ConcretePrototypeB(const ConcretePrototypeB& other) : number(other.number) {}
};
```

**详细描述：**
原型模式通过复制现有对象来创建新对象，而不是通过实例化类。在这个示例中，`Prototype`是抽象原型接口，定义了克隆方法。`ConcretePrototypeA`和`ConcretePrototypeB`是具体原型类，实现了自己的克隆方法。克隆方法使用拷贝构造函数来创建新对象，这避免了复杂的初始化过程。客户端可以通过调用`clone()`方法来复制现有对象，而不需要知道具体类的构造过程。

**设计原因：**
1. **高效创建：** 当创建对象的成本很高时，复制现有对象比重新创建更高效
2. **避免复杂初始化：** 避免重复复杂的对象初始化过程
3. **运行时配置：** 可以在运行时动态配置对象，然后复制这些配置好的对象
4. **独立于具体类：** 客户端无需知道对象的具体类型即可创建对象

---

## 结构型模式

### 1. 适配器模式 (Adapter Pattern)

**意图：** 将一个类的接口转换为客户希望的另一个接口。

适配器模式的核心意图是将一个类的接口转换为客户希望的另一个接口，使得原本由于接口不兼容而不能一起工作的类可以协同工作。这种模式就像现实世界中的电源适配器一样，将一种类型的插头转换为另一种类型，使其能够插入不同的插座。适配器模式在软件开发中经常用于整合第三方库、遗留代码或不兼容的接口，允许在不修改现有代码的情况下实现接口的兼容。

**设计思想：** 通过包装现有类，使其接口符合客户端需求。

适配器模式的设计思想是通过创建一个包装类（适配器）来包装不兼容的接口，将目标接口转换为客户端期望的形式。适配器充当中间层，它接收来自客户端的请求，然后将其转换为对被适配对象的适当调用。这种设计思想强调了接口的透明转换，客户端不需要知道也不关心底层的实际实现，只需要与适配器提供的目标接口交互。适配器模式有两种主要实现方式：对象适配器（使用组合）和类适配器（使用多重继承）。

**核心问题：** 如何让不兼容的接口能够协同工作？

在软件开发过程中，经常会遇到这种情况：有一个现有的类（被适配者）具有所需的功能，但其接口与客户端期望的接口不兼容。直接修改现有类可能不可行（例如，它是第三方库或遗留代码），而修改客户端代码可能代价高昂或不可能。适配器模式解决了这个问题，通过引入一个适配器类来协调这两种不兼容的接口，使得它们能够协同工作。适配器充当翻译器的角色，将客户端的请求转换为被适配者能够理解的格式。

**实现要点：**
1. **定义目标接口：** 明确客户端期望的接口形式，这通常是客户端代码直接依赖的接口。
2. **创建适配器类：** 实现目标接口，包含适配逻辑，将目标接口的调用转换为对被适配者的调用。
3. **适配器持有被适配对象：** 适配器通过组合的方式持有被适配者的实例（对象适配器），或通过继承被适配者（类适配器）。
4. **接口转换：** 实现具体的转换逻辑，将目标接口的方法调用映射到被适配者的方法。
5. **协议转换：** 如果需要，处理数据格式、通信协议等方面的转换。

**注意事项：**
- **对象适配器 vs 类适配器：** 对象适配器使用组合，更灵活但需要额外的组合对象；类适配器使用多重继承，更直接但受语言特性限制（如C++支持，Java不支持类继承多继承）。
- **接口转换可能丢失功能：** 在接口转换过程中，可能存在功能不完全对应的情况，导致某些功能无法适配或丢失。
- **性能开销：** 适配器增加了间接层，可能会带来一定的性能开销。
- **复杂性增加：** 引入适配器会增加系统的复杂性，需要维护额外的适配器类。
- **维护挑战：** 当被适配者的接口发生变化时，适配器也需要相应更新。

**适用情况：**
- **对接第三方库：** 当需要集成第三方库或框架，但其接口与现有系统不兼容时，可以使用适配器模式进行接口转换。
- **老接口迁移：** 在系统升级或重构过程中，为了保持向后兼容，可以使用适配器包装旧的接口。
- **系统兼容：** 在需要整合多个不同系统或模块时，适配器可以帮助解决接口不兼容的问题。
- **测试环境：** 在单元测试中，可以使用适配器来模拟某些外部依赖，提供可控的测试环境。
- **API标准化：** 当需要为不同来源的数据提供统一的访问接口时，适配器模式可以将各种不同的数据源适配到统一的接口。

```cpp
// 目标接口
class Target {
public:
    virtual ~Target() = default;
    virtual string request() const {
        return "Target: The default target's behavior.";
    }
};

// 被适配的类
class Adaptee {
public:
    string specificRequest() const {
        return ".eetpadA eht fo roivaheb laicepS";
    }
};

// 对象适配器
class Adapter : public Target {
private:
    unique_ptr<Adaptee> adaptee;
    
public:
    Adapter(unique_ptr<Adaptee> a) : adaptee(move(a)) {}
    
    string request() const override {
        string to_reverse = adaptee->specificRequest();
        reverse(to_reverse.begin(), to_reverse.end());
        return "Adapter: (TRANSLATED) " + to_reverse;
    }
};

// 类适配器（多重继承，在C++中可用）
class ClassAdapter : public Target, private Adaptee {
public:
    string request() const override {
        string to_reverse = specificRequest();
        reverse(to_reverse.begin(), to_reverse.end());
        return "ClassAdapter: (TRANSLATED) " + to_reverse;
    }
};
```

**详细描述：**
适配器模式将一个类的接口转换为客户希望的另一个接口。在这个示例中，`Target`是客户端期望的目标接口，`Adaptee`是现有的类，其接口与客户端期望的不兼容。`Adapter`是对象适配器，它持有`Adaptee`的实例，并实现了`Target`接口。当客户端调用`request()`方法时，适配器将请求转换为对`Adaptee`的`specificRequest()`方法的调用，并对结果进行处理。类适配器通过多重继承实现，同时继承`Target`接口和`Adaptee`类。

**设计原因：**
1. **接口兼容：** 使不兼容的接口能够协同工作
2. **复用现有类：** 无需修改现有类即可将其集成到系统中
3. **松耦合：** 减少客户端与被适配类之间的直接依赖
4. **可扩展性：** 可以轻松添加新的适配器来适配不同的类

### 2. 桥接模式 (Bridge Pattern)

**意图：** 将抽象部分与其实现部分分离，使它们都可以独立变化。

桥接模式的核心意图是将抽象部分与其实现部分分离，使它们都可以独立变化。这种模式通过组合而非继承的方式来连接抽象和实现，打破了传统继承结构中抽象和实现紧密耦合的问题。桥接模式允许在一个维度上扩展抽象类，在另一个维度上扩展实现类，避免了类层次的爆炸性增长。这种分离使得可以在不影响客户端代码的情况下修改或扩展实现，同时也可以独立地修改抽象层。

**设计思想：** 通过组合而非继承解耦抽象和实现，允许两者独立变化。

桥接模式的设计思想是通过组合关系来代替继承关系，将抽象部分和实现部分解耦。抽象部分定义高层控制逻辑，而实现部分定义底层操作。抽象类持有一个指向实现类的引用，通过这个引用调用实现部分的方法。这种设计使得抽象和实现可以独立地变化，而不会相互影响。设计思想强调了多维度变化的处理，通过将不同维度的变化分离到不同的继承层次中，实现了更好的可扩展性和可维护性。

**核心问题：** 如何避免类层次的指数级增长？

在传统的继承结构中，如果抽象和实现都在多个维度上变化，会导致类的数量呈指数级增长。例如，如果有3种形状和4种颜色，采用传统的继承方式会产生12个具体的子类。桥接模式解决了这个问题，通过将形状和颜色这两个变化维度分离到不同的继承层次中，只需要3个形状类和4个颜色实现类，总共7个类就可以实现相同的功能。这种分离大大减少了类的数量，提高了代码的可维护性。

**实现要点：**
1. **定义实现接口：** 定义实现部分的抽象接口，定义底层操作的方法签名。
2. **创建抽象类持有实现：** 创建抽象类，其中包含一个指向实现接口的引用，定义高层控制逻辑。
3. **扩展抽象类：** 根据需要扩展抽象类，添加更多的高层控制逻辑。
4. **实现具体实现类：** 实现实现接口，提供具体的实现细节。
5. **组合关系：** 确保抽象类通过组合关系而不是继承关系与实现类关联。

**注意事项：**
- **增加系统复杂度：** 桥接模式引入了额外的抽象层和组合关系，可能会使系统变得更加复杂，特别是对于简单的问题可能显得过度设计。
- **需要识别变化维度：** 正确识别哪些部分可能变化并将其分离到不同的维度是使用桥接模式的关键，错误的分离可能会导致设计不当。
- **运行时绑定：** 由于使用组合关系，实现的选择通常在运行时确定，可能会带来一定的性能开销。
- **接口设计：** 实现接口需要足够通用，以便适应不同的具体实现，但也不能过于宽泛而失去约束力。
- **初始设计：** 桥接模式最好在设计初期就考虑，后期重构引入可能会比较困难。

**适用情况：**
- **多维度变化：** 当系统中有两个或多个维度的变化需要独立处理时，如图形形状和绘制方式、设备类型和操作系统等。
- **平台无关性：** 当需要在多个平台上运行，且每个平台有特定的实现时，可以将平台相关的实现分离出来。
- **运行时切换实现：** 当需要在运行时动态切换不同的实现时，如在应用程序运行时切换不同的主题、数据库驱动等。
- **避免类爆炸：** 当使用继承会导致类层次结构过于庞大和复杂时。
- **组件重用：** 当需要将实现部分作为可重用的组件在多个抽象之间共享时。

```cpp
// 实现接口
class Implementation {
public:
    virtual ~Implementation() = default;
    virtual string operationImplementation() const = 0;
};

// 具体实现
class ConcreteImplementationA : public Implementation {
public:
    string operationImplementation() const override {
        return "ConcreteImplementationA: Here's the result on the platform A.";
    }
};

class ConcreteImplementationB : public Implementation {
public:
    string operationImplementation() const override {
        return "ConcreteImplementationB: Here's the result on the platform B.";
    }
};

// 抽象类
class Abstraction {
protected:
    shared_ptr<Implementation> implementation;
    
public:
    Abstraction(shared_ptr<Implementation> impl) : implementation(impl) {}
    
    virtual ~Abstraction() = default;
    
    virtual string operation() const {
        return "Abstraction: Base operation with:\n" +
               implementation->operationImplementation();
    }
};

// 扩展抽象类
class ExtendedAbstraction : public Abstraction {
public:
    ExtendedAbstraction(shared_ptr<Implementation> impl) : Abstraction(impl) {}
    
    string operation() const override {
        return "ExtendedAbstraction: Extended operation with:\n" +
               implementation->operationImplementation();
    }
};
```

**详细描述：**
桥接模式将抽象部分与其实现部分分离，使它们都可以独立变化。在这个示例中，`Implementation`是实现接口，定义了实现部分的接口。`ConcreteImplementationA`和`ConcreteImplementationB`是具体实现类，实现了不同的平台或实现方式。`Abstraction`是抽象类，持有对实现部分的引用，通过组合而非继承将抽象与实现连接起来。`ExtendedAbstraction`是扩展抽象类，继承自`Abstraction`，可以添加更多的抽象功能。这样，抽象部分和实现部分可以独立变化，比如可以添加新的抽象类或新的实现类。

**设计原因：**
1. **解耦合：** 将抽象与实现解耦，使它们可以独立变化
2. **多维度扩展：** 支持多个维度的扩展，避免类爆炸
3. **灵活性：** 可以在运行时动态切换实现
4. **遵循合成复用原则：** 使用组合而非继承，提高灵活性

### 3. 组合模式 (Composite Pattern)

**意图：** 将对象组合成树形结构以表示"部分-整体"的层次结构。

组合模式的核心意图是将对象组合成树形结构以表示"部分-整体"的层次结构。这种模式使得客户端可以统一处理单个对象和组合对象，无需区分它们是叶子节点还是分支节点。通过定义统一的接口，组合模式允许递归地构建复杂的树形结构，其中组合对象可以包含其他组合对象或叶子对象。这种模式特别适用于表示具有递归性质的层次结构，如文档中的段落包含句子，句子包含单词，或文件系统中的目录包含子目录和文件。

**设计思想：** 让叶对象和组合对象具有一致的接口，客户端可以统一处理。

组合模式的设计思想是通过定义一个统一的组件接口，使叶对象（叶子节点）和组合对象（容器节点）具有一致的接口。这种设计消除了客户端代码需要区分处理单个对象和组合对象的复杂性，客户端可以以相同的方式对待树形结构中的任何节点。设计思想强调了递归结构的自然表达，通过让组合对象和叶子对象实现相同的接口，形成了一个递归的结构。这种设计还体现了"开闭原则"，可以轻松地添加新的组件类型而无需修改现有代码。

**核心问题：** 如何处理树形结构，使客户端能一致地对待单个对象和组合对象？

在许多应用中，需要表示层次结构，如组织架构、文件系统、UI组件树等。这些结构的特点是上层对象可能包含下层对象，而下层对象本身也可能是容器。传统的做法是区分处理容器对象和叶子对象，但这会导致客户端代码变得复杂，需要不断地判断对象类型并采取不同的处理方式。组合模式解决了这个问题，通过统一的接口，客户端可以以一致的方式处理树形结构中的任何节点，无论它是叶子还是容器。

**实现要点：**
1. **定义组件接口：** 定义一个抽象的组件接口，声明所有具体组件（叶子和组合）都应该实现的操作，包括业务操作和结构操作（如添加、删除子节点）。
2. **实现叶子节点：** 实现组件接口，叶子节点是最基本的单元，不包含其他组件，通常只实现业务操作而不实现结构操作。
3. **实现组合节点：** 实现组件接口，组合节点包含其他组件（叶子或其他组合），实现业务操作时通常会递归调用子组件的操作。
4. **递归结构：** 确保组合节点可以包含其他组件，形成递归的树形结构。
5. **透明性与安全性：** 决定是否在接口层面提供添加/删除子节点的方法（透明性）或只在组合类中提供（安全性）。

**注意事项：**
- **安全性 vs 透明性权衡：** 在设计组件接口时需要权衡透明性和安全性。如果在接口中定义添加/删除子节点的方法，则所有组件都必须实现这些方法（即使叶子节点不需要），这提供了透明性但牺牲了安全性；反之，如果只在组合类中定义这些方法，则更安全但不够透明。
- **循环引用问题：** 需要注意防止在组合结构中形成循环引用，这会导致无限递归和栈溢出。
- **性能考虑：** 递归操作可能在深层树结构中导致性能问题，需要考虑性能优化策略。
- **内存管理：** 需要妥善处理树形结构中的内存管理，防止内存泄漏。
- **遍历复杂性：** 树形结构的遍历可能比线性结构更复杂，需要考虑遍历算法的实现。

**适用情况：**
- **文件系统：** 目录可以包含文件和其他目录，使用组合模式可以统一处理文件和目录的操作。
- **UI组件树：** 窗口包含面板，面板包含按钮、文本框等，所有组件都可以统一处理事件和布局。
- **组织架构：** 公司包含部门，部门包含小组，小组包含员工，可以用组合模式表示这种层级关系。
- **文档结构：** 文档包含章节，章节包含段落，段落包含文本等。
- **菜单系统：** 菜单包含子菜单和菜单项，可以用组合模式统一处理菜单操作。

```cpp
#include <vector>
#include <memory>

class Component {
public:
    virtual ~Component() = default;
    virtual void add(unique_ptr<Component>) {}
    virtual void remove(Component*) {}
    virtual bool isComposite() const { return false; }
    virtual string operation() const = 0;
};

class Leaf : public Component {
private:
    string name;
    
public:
    Leaf(string n) : name(n) {}
    
    string operation() const override {
        return "Leaf: " + name;
    }
};

class Composite : public Component {
private:
    vector<unique_ptr<Component>> children;
    string name;
    
public:
    Composite(string n) : name(n) {}
    
    void add(unique_ptr<Component> component) override {
        children.push_back(move(component));
    }
    
    void remove(Component* component) override {
        children.erase(
            remove_if(children.begin(), children.end(),
                [component](const unique_ptr<Component>& child) {
                    return child.get() == component;
                }),
            children.end()
        );
    }
    
    bool isComposite() const override { return true; }
    
    string operation() const override {
        stringstream result;
        result << "Composite (" << name << "): ";
        
        for (const auto& child : children) {
            if (child.get() == children.back().get()) {
                result << child->operation();
            } else {
                result << child->operation() << "+";
            }
        }
        
        return result.str();
    }
};
```

**详细描述：**
组合模式将对象组合成树形结构以表示"部分-整体"的层次结构。在这个示例中，`Component`是抽象组件接口，定义了叶子节点和组合节点的公共接口。`Leaf`是叶子节点，代表树形结构中的终端节点，没有子节点。`Composite`是组合节点，可以包含子节点（叶子节点或其他组合节点）。所有节点都实现了相同的接口，客户端可以统一处理单个对象和组合对象。这种模式特别适用于表示树形结构，如文件系统、UI组件树等。

**设计原因：**
1. **统一处理：** 客户端可以一致地处理单个对象和组合对象
2. **递归结构：** 支持任意深度的树形结构
3. **灵活性：** 可以轻松添加新的组件类型
4. **清晰结构：** 明确表达了"部分-整体"的关系

### 4. 装饰器模式 (Decorator Pattern)

**意图：** 动态地给对象添加一些额外的职责。

装饰器模式的核心意图是在不修改原有对象的基础上动态地给对象添加新的职责或功能。这种模式提供了一种比继承更加灵活的替代方案，允许在运行时动态地为对象添加功能，然后再动态地移除这些功能。装饰器模式通过创建一个包装对象（装饰器）来包裹原始对象，然后在调用原始对象方法的前后添加额外的功能。这种模式遵循开闭原则，即对扩展开放，对修改关闭，可以在不修改现有代码的情况下增加新功能。

**设计思想：** 通过包装对象来动态扩展功能，避免继承带来的问题。

装饰器模式的设计思想是通过包装（或称为“装饰”）现有对象来动态地添加功能，而不是使用静态的继承机制。装饰器对象持有对被装饰对象的引用，并实现与被装饰对象相同的接口。当客户端调用装饰器的方法时，装饰器可以选择性地执行额外的功能，然后将调用转发给被装饰的对象。这种设计允许通过组合多个装饰器来创建复杂的功能，形成装饰器链。设计思想强调了动态性和灵活性，使得功能的添加和移除可以在运行时进行，而不需要在编译时确定。

**核心问题：** 如何在不修改原有对象的情况下动态添加功能？

在软件开发中，经常需要为现有类添加新的功能，但传统的继承方式有几个缺点：1）功能的添加在编译时就固定了，不够灵活；2）多重继承会导致类层次结构复杂；3）一个类可能需要多种不同的功能组合，使用继承会产生大量子类。装饰器模式解决了这个问题，通过创建一个包装对象来动态地添加功能，而不需要修改原始类的代码。这种方法允许在运行时自由地组合功能，提供了极大的灵活性。

**实现要点：**
1. **定义组件接口：** 定义一个抽象的组件接口，所有具体组件和装饰器都要实现这个接口，确保装饰器可以透明地替换被装饰的对象。
2. **创建装饰器基类：** 创建一个装饰器基类，实现组件接口并持有一个对组件的引用，作为具体装饰器的基类。
3. **实现具体装饰器：** 创建具体的装饰器类，继承装饰器基类，在调用被装饰对象的方法前后添加额外的功能。
4. **保持接口一致：** 确保装饰器和被装饰对象具有相同的接口，以保证透明性。
5. **递归调用：** 装饰器在适当的时候调用被装饰对象的相应方法。

**注意事项：**
- **装饰器链可能很长：** 当多个装饰器嵌套使用时，会形成一个长的调用链，这可能导致调试困难和性能问题。
- **性能开销：** 每次方法调用都会经过多个装饰器层，可能带来额外的性能开销。
- **复杂性增加：** 过多的装饰器会使系统变得复杂，难以理解和维护。
- **对象标识：** 装饰器改变了对象的身份，可能影响依赖对象身份的代码。
- **初始化复杂性：** 构建装饰器链可能比直接创建对象更复杂。

**适用情况：**
- **I/O过滤器：** 在Java的I/O系统中广泛使用，如BufferedInputStream、DataInputStream等都是装饰器，可以组合使用来增强流的功能。
- **日志增强：** 为现有对象添加日志记录功能，而无需修改原对象的代码。
- **权限控制：** 为对象添加访问控制，检查用户权限后再调用原始对象的方法。
- **缓存机制：** 为数据访问对象添加缓存功能，先检查缓存再访问原始数据源。
- **事务管理：** 为业务对象添加事务处理功能。

```cpp
// 组件接口
class Component {
public:
    virtual ~Component() = default;
    virtual string operation() const = 0;
};

// 具体组件
class ConcreteComponent : public Component {
public:
    string operation() const override {
        return "ConcreteComponent";
    }
};

// 装饰器基类
class Decorator : public Component {
protected:
    shared_ptr<Component> component;
    
public:
    Decorator(shared_ptr<Component> c) : component(c) {}
    
    virtual string operation() const override {
        return component->operation();
    }
};

// 具体装饰器A
class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(shared_ptr<Component> c) : Decorator(c) {}
    
    string operation() const override {
        return "ConcreteDecoratorA(" + Decorator::operation() + ")";
    }
};

// 具体装饰器B
class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(shared_ptr<Component> c) : Decorator(c) {}
    
    string operation() const override {
        return "ConcreteDecoratorB(" + Decorator::operation() + ")";
    }
};
```

**详细描述：**
装饰器模式动态地给对象添加一些额外的职责，而不改变其结构。在这个示例中，`Component`是抽象组件接口，定义了基本操作。`ConcreteComponent`是具体组件，实现了基本功能。`Decorator`是装饰器基类，同样实现了`Component`接口，并持有一个对组件的引用。`ConcreteDecoratorA`和`ConcreteDecoratorB`是具体装饰器，它们通过包装组件来添加新的功能。装饰器可以在调用被包装组件的方法前后添加额外的功能。这种模式允许向对象添加功能，而无需修改其类。

**设计原因：**
1. **动态添加功能：** 可以在运行时动态地添加或移除功能
2. **避免子类爆炸：** 比使用继承更灵活，避免了大量的子类
3. **职责分离：** 将核心功能与附加功能分离
4. **透明性：** 装饰器对客户端来说是透明的，客户端无需知道对象是否被装饰

### 5. 外观模式 (Facade Pattern)

**意图：** 为子系统中的一组接口提供一个统一的接口。

外观模式的核心意图是为复杂的子系统提供一个简化的统一接口，隐藏子系统的复杂性，使子系统更容易使用。外观模式并不取代子系统的功能，而是作为一个更高层次的接口，将客户端与子系统的复杂性隔离开来。通过外观，客户端只需要与一个简单的接口交互，而不需要了解子系统内部的复杂工作流程。这种模式特别适用于那些需要与多个子系统组件协作完成复杂任务的场景，通过提供一个统一的入口点来简化客户端的使用。

**设计思想：** 简化复杂系统的接口，提供高层次的统一访问点。

外观模式的设计思想是通过引入一个外观类作为客户端与复杂子系统之间的中介，将复杂的子系统调用封装在简单的接口后面。外观类了解如何与子系统的各个组件协作来完成任务，它知道调用哪些子系统组件以及以什么顺序调用。这种设计体现了"最少知识原则"（也称为迪米特法则），客户端只需要与外观交互，而不需要了解子系统的内部细节。设计思想强调了简化和抽象，通过提供高层次的接口来掩盖底层的复杂性。

**核心问题：** 如何简化复杂系统的使用，提供统一的访问接口？

在大型系统中，通常包含多个子系统，每个子系统又有多个接口。当客户端需要使用这些子系统来完成某个任务时，可能需要了解子系统的内部结构、组件之间的依赖关系以及正确的调用顺序。这不仅增加了客户端的复杂性，还使系统变得难以维护和修改。外观模式解决了这个问题，通过提供一个统一的高层接口，将子系统的复杂性封装起来，客户端只需要通过外观来访问子系统，而不需要了解内部细节。

**实现要点：**
1. **识别子系统组件：** 识别需要简化访问的子系统组件，了解它们的功能和相互关系。
2. **创建外观类：** 创建一个外观类，提供简化的接口方法，这些方法封装了对子系统的调用。
3. **封装子系统调用：** 在外观类中实现对子系统组件的调用逻辑，包括调用顺序、参数传递和结果处理。
4. **保持子系统可访问性：** 外观不应该阻止对子系统组件的直接访问，应该允许高级用户绕过外观直接使用子系统。
5. **职责明确：** 外观类应该清楚地知道如何与子系统协作来完成特定任务。

**注意事项：**
- **可能成为上帝对象：** 外观类可能会变得过于庞大和复杂，承担过多的职责，成为一个"上帝对象"，违反了单一职责原则。
- **限制了子系统的灵活性：** 外观可能会限制对子系统功能的访问，某些高级功能可能无法通过外观访问。
- **性能开销：** 外观引入了额外的间接层，可能会带来轻微的性能开销。
- **维护负担：** 当子系统发生变化时，外观类也需要相应更新，增加了维护的复杂性。
- **抽象泄露：** 如果设计不当，子系统的复杂性可能会透过外观泄露给客户端。

**适用情况：**
- **封装复杂库/模块：** 当需要使用复杂的第三方库或框架时，可以创建外观来简化其使用。
- **提供简单API：** 为复杂的业务逻辑提供简化的接口，使API更易于使用和理解。
- **分层架构：** 在分层架构中，外观可以作为层之间的接口，隐藏底层的复杂性。
- **遗留系统集成：** 为复杂的遗留系统提供简化的访问接口。
- **测试简化：** 在测试中，外观可以用来简化对复杂系统的访问。

```cpp
// 子系统类
class Subsystem1 {
public:
    string operation1() const {
        return "Subsystem1: Ready!\n";
    }
    
    string operationN() const {
        return "Subsystem1: Go!\n";
    }
};

class Subsystem2 {
public:
    string operation1() const {
        return "Subsystem2: Get ready!\n";
    }
    
    string operationZ() const {
        return "Subsystem2: Fire!\n";
    }
};

// 外观类
class Facade {
private:
    unique_ptr<Subsystem1> subsystem1;
    unique_ptr<Subsystem2> subsystem2;
    
public:
    Facade() {
        this->subsystem1 = make_unique<Subsystem1>();
        this->subsystem2 = make_unique<Subsystem2>();
    }
    
    string operation() const {
        string result = "Facade initializes subsystems:\n";
        result += this->subsystem1->operation1();
        result += this->subsystem2->operation1();
        result += "Facade orders subsystems to perform the action:\n";
        result += this->subsystem1->operationN();
        result += this->subsystem2->operationZ();
        return result;
    }
};
```

**详细描述：**
外观模式为子系统中的一组接口提供一个统一的接口。在这个示例中，`Subsystem1`和`Subsystem2`代表复杂的子系统，它们各自有不同的接口和操作。`Facade`类提供了一个简单的接口，封装了子系统的复杂性。客户端只需要与`Facade`交互，而不需要了解子系统的内部工作细节。外观类协调子系统的工作，按适当的顺序调用它们的方法。这种模式简化了客户端的使用，同时隐藏了系统的复杂性。

**设计原因：**
1. **简化接口：** 为复杂子系统提供简单的接口，降低使用难度
2. **解耦合：** 客户端与子系统解耦，减少依赖关系
3. **分层架构：** 有助于实现清晰的分层架构
4. **提高可维护性：** 子系统内部的变化不会影响客户端

### 6. 享元模式 (Flyweight Pattern)

**意图：** 运用共享技术有效地支持大量细粒度的对象。

享元模式的核心意图是运用共享技术有效地支持大量细粒度的对象。这种模式通过尽可能地共享对象来减少内存使用和提高性能，特别适用于存在大量相似对象的场景。享元模式将对象的状态分为内部状态和外部状态，内部状态是可以共享的固有状态，而外部状态是随环境变化的状态。通过分离这两种状态，享元模式能够显著减少需要创建的对象数量，从而节省内存并提高系统性能。

**设计思想：** 通过共享内部状态减少对象数量，外部状态通过参数传递。

享元模式的设计思想是通过区分内部状态和外部状态来实现对象的共享。内部状态是对象的固有属性，可以被多个对象共享，存储在享元对象内部；外部状态是随环境变化的状态，不能被共享，需要在使用时通过参数传递给享元对象。享元工厂负责管理和缓存享元对象，确保具有相同内部状态的对象只会被创建一次。这种设计将不变的部分（内部状态）和可变的部分（外部状态）分离，实现了最大程度的对象共享。

**核心问题：** 如何在大量相似对象间共享状态以节省内存？

当系统中存在大量相似对象时，每个对象都占用一定的内存空间，如果这些对象有很多相同的属性，就会造成内存的浪费。例如，在文本编辑器中，每个字符对象都包含字体、大小、颜色等信息，如果每个字符都创建独立的对象，会占用大量内存。享元模式解决了这个问题，通过将共享的状态提取出来，让多个对象共享同一个状态，从而减少内存使用。

**实现要点：**
1. **区分内部状态和外部状态：** 仔细分析对象的状态，将可以共享的状态作为内部状态，将随环境变化的状态作为外部状态。
2. **创建享元工厂：** 创建享元工厂来管理和缓存享元对象，确保具有相同内部状态的对象只被创建一次。
3. **客户端传递外部状态：** 客户端在使用享元对象时，将外部状态作为参数传递给享元对象的方法。
4. **享元接口：** 定义享元接口，声明操作方法，接受外部状态作为参数。
5. **存储管理：** 使用适当的数据结构（如哈希表）来存储和管理享元对象。

**注意事项：**
- **线程安全问题：** 当多个线程同时访问共享的享元对象时，需要考虑线程安全问题，可能需要同步机制。
- **复杂性增加：** 需要区分内部状态和外部状态，增加了系统设计的复杂性。
- **内存与时间权衡：** 虽然节省了内存，但可能增加了计算时间，因为需要在运行时组合内部状态和外部状态。
- **适用范围限制：** 并非所有对象都适合使用享元模式，需要对象具备可共享的内部状态。
- **状态管理：** 需要仔细管理内部状态和外部状态，避免状态混淆。

**适用情况：**
- **文字渲染：** 在文本编辑器或排版系统中，字符的字体、大小、样式等属性可以共享，而位置信息则作为外部状态传递。
- **地图瓦片：** 在地图应用中，相同类型的瓦片（如道路、建筑物）可以共享样式信息，而位置坐标作为外部状态。
- **游戏精灵：** 在游戏中，相同类型的精灵（如敌人、道具）可以共享图像和行为模式，而位置、生命值等作为外部状态。
- **数据库连接池：** 连接的配置信息可以共享，而当前状态作为外部状态。
- **线程池：** 线程的基本配置可以共享，而执行的任务作为外部状态。

```cpp
#include <unordered_map>

struct SharedState {
    string brand;
    string model;
    string color;
    
    SharedState(const string& brand, const string& model, const string& color)
        : brand(brand), model(model), color(color) {}
    
    friend ostream& operator<<(ostream& os, const SharedState& ss) {
        return os << "[" << ss.brand << ", " << ss.model << ", " << ss.color << "]";
    }
};

struct UniqueState {
    string owner;
    string plates;
    
    UniqueState(const string& owner, const string& plates)
        : owner(owner), plates(plates) {}
    
    friend ostream& operator<<(ostream& os, const UniqueState& us) {
        return os << "[" << us.owner << ", " << us.plates << "]";
    }
};

class Flyweight {
private:
    SharedState shared_state_;

public:
    Flyweight(const SharedState& shared_state) : shared_state_(shared_state) {}

    const SharedState& getSharedState() const {
        return shared_state_;
    }

    void operation(const UniqueState& unique_state) const {
        cout << "Flyweight: Displaying shared (" << shared_state_ <<
            ") and unique (" << unique_state << ") state.\n";
    }
};

class FlyweightFactory {
private:
    unordered_map<string, shared_ptr<Flyweight>> flyweights_;

    string getKey(const SharedState& ss) const {
        return ss.brand + "_" + ss.model + "_" + ss.color;
    }

public:
    FlyweightFactory(initializer_list<SharedState> share_states) {
        for (const auto& ss : share_states) {
            flyweights_.insert({this->getKey(ss), make_shared<Flyweight>(ss)});
        }
    }

    shared_ptr<Flyweight> getFlyweight(const SharedState& shared_state) {
        string key = this->getKey(shared_state);
        if (flyweights_.find(key) == flyweights_.end()) {
            cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
            flyweights_.insert({key, make_shared<Flyweight>(shared_state)});
        } else {
            cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }
        return flyweights_.at(key);
    }

    void listFlyweights() const {
        size_t count = flyweights_.size();
        cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (pair<const string, shared_ptr<Flyweight>>& item : flyweights_) {
            cout << item.first << "\n";
        }
    }
};
```

**详细描述：**
享元模式运用共享技术有效地支持大量细粒度的对象。在这个示例中，汽车对象被分解为内部状态（Intrinsic State）和外部状态（Extrinsic State）。`SharedState`代表内部状态，包括品牌、型号和颜色，这些是可共享的。`UniqueState`代表外部状态，包括车主和车牌号，这些是不可共享的。`Flyweight`类封装了共享状态，`FlyweightFactory`负责管理和缓存享元对象。当请求一个享元对象时，工厂首先检查是否已存在具有相同内部状态的对象，如果存在则返回现有的对象，否则创建新的对象。这样可以显著减少内存使用。

**设计原因：**
1. **内存优化：** 通过共享内部状态大幅减少内存占用
2. **性能提升：** 减少对象创建和销毁的开销
3. **资源管理：** 有效管理大量相似对象
4. **可扩展性：** 支持大规模对象集合的处理

### 7. 代理模式 (Proxy Pattern)

**意图：** 为其他对象提供代理以控制对这个对象的访问。

代理模式的核心意图是为其他对象提供代理以控制对这个对象的访问。代理模式通过创建一个代理对象来代表真实的对象，客户端通过代理对象与真实对象交互。代理对象在客户端和真实对象之间起到中介的作用，可以在调用真实对象的方法前后添加额外的逻辑，如访问控制、延迟初始化、日志记录等。这种模式使得可以在不修改真实对象的情况下扩展其功能或控制其访问，是面向对象设计中一种重要的结构型模式。

**设计思想：** 通过代理对象控制对真实对象的访问，可以在访问前后添加额外逻辑。

代理模式的设计思想是通过一个代理对象来控制对真实对象的访问，代理对象和真实对象实现相同的接口，使得客户端可以透明地使用代理对象。这种设计体现了"保护代理"的思想，代理对象可以检查客户端是否有权限访问真实对象，也可以实现"虚拟代理"，延迟创建昂贵的对象，还可以实现"远程代理"，隐藏网络通信的复杂性。设计思想强调了间接访问的好处，通过增加一层间接性，可以在不影响客户端代码的情况下添加额外的功能。

**核心问题：** 如何在不改变真实对象的情况下控制对其的访问？

在某些情况下，我们不能或不想直接访问一个对象，可能是因为对象创建代价昂贵、对象位于远程服务器、或者需要对对象的访问进行控制。直接访问可能带来性能问题、安全风险或实现复杂性。代理模式解决了这个问题，通过引入一个代理对象来控制对真实对象的访问，使得可以在不修改真实对象的情况下添加访问控制、延迟加载、权限验证等功能。

**实现要点：**
1. **定义主题接口：** 定义一个接口或抽象类，代理对象和真实对象都实现这个接口，确保它们可以相互替换。
2. **创建真实对象：** 实现接口的具体业务逻辑类，提供实际的功能实现。
3. **创建代理对象：** 创建代理类实现相同接口，在内部持有对真实对象的引用，在调用真实对象方法前后执行额外逻辑。
4. **控制访问：** 代理对象可以在访问真实对象前进行权限检查、缓存验证等操作。
5. **透明性：** 代理对象应该对客户端透明，客户端不需要知道是在使用代理还是真实对象。

**注意事项：**
- **增加系统复杂度：** 引入代理对象会增加系统的复杂度，需要维护额外的类和关系。
- **可能影响性能：** 代理引入了额外的间接层，可能会对性能产生轻微影响。
- **内存开销：** 代理对象需要维护对真实对象的引用，可能会增加内存开销。
- **同步问题：** 在多线程环境中，需要考虑代理对象和真实对象的同步问题。
- **初始化时机：** 需要确定何时创建真实对象（延迟创建 vs 立即创建）。

**适用情况：**
- **远程代理：** 代表位于不同地址空间或网络中的真实对象，隐藏网络通信的复杂性。
- **虚拟代理：** 延迟创建昂贵的对象，只有在真正需要时才创建真实对象。
- **保护代理：** 控制对真实对象的访问权限，根据客户端的权限决定是否允许访问。
- **智能引用：** 在对象被访问时执行额外操作，如引用计数、日志记录等。
- **缓存代理：** 在代理中缓存真实对象的结果，提高访问性能。

```cpp
// Subject interface
class Subject {
public:
    virtual ~Subject() = default;
    virtual void request() const = 0;
};

// RealSubject
class RealSubject : public Subject {
public:
    void request() const override {
        cout << "RealSubject: Handling request.\n";
    }
};

// Proxy
class Proxy : public Subject {
private:
    shared_ptr<RealSubject> real_subject_;

    void logAccess() const {
        cout << "Proxy: Logging the time of request.\n";
    }

public:
    Proxy(shared_ptr<RealSubject> real_subject) : real_subject_(real_subject) {}

    void request() const override {
        if (this->checkAccess()) {
            real_subject_->request();
            this->logAccess();
        }
    }

private:
    bool checkAccess() const {
        cout << "Proxy: Checking access prior to firing a real request.\n";
        return true;
    }
};
```

**详细描述：**
代理模式为其他对象提供代理以控制对这个对象的访问。在这个示例中，`Subject`是接口，定义了真实对象和代理对象的公共接口。`RealSubject`是真实对象，实现实际的业务逻辑。`Proxy`是代理对象，它持有对真实对象的引用，并在调用真实对象的方法前后添加额外的控制逻辑。在示例中，代理在调用真实对象的方法之前检查访问权限，在调用之后记录日志。客户端与代理交互，而代理负责与真实对象通信，从而实现了对真实对象访问的控制。

**设计原因：**
1. **访问控制：** 在访问真实对象前后添加权限检查、日志记录等控制逻辑
2. **延迟加载：** 可以实现虚拟代理，延迟昂贵对象的创建
3. **远程访问：** 可以作为远程对象的本地代表
4. **增强功能：** 在不修改原始对象的情况下添加额外功能

---

## 行为型模式

### 1. 责任链模式 (Chain of Responsibility Pattern)

**意图：** 避免请求发送者与接收者耦合，让多个对象都有机会处理请求。

责任链模式的核心意图是避免请求发送者与接收者之间的紧耦合，让多个对象都有机会处理请求，直到有合适的对象处理为止。这种模式将这些对象连成一条链，并沿着这条链传递请求，直到有一个对象处理它。每个处理者都可以选择处理请求或将其传递给链上的下一个处理者。这种模式提供了一种动态指定处理对象的方式，使得处理者可以灵活地添加、移除或重新排序，而不会影响客户端代码。

**设计思想：** 将处理者组成一条链，沿着链传递请求直到有处理者处理。

责任链模式的设计思想是将多个处理者对象链接在一起形成一条处理链，请求沿着这条链传递，直到有处理者能够处理该请求。每个处理者都拥有对下一个处理者的引用，当一个处理者收到请求时，它可以选择处理请求或将其传递给下一个处理者。这种设计体现了单一职责原则，每个处理者只需关注自己能处理的请求类型，同时也体现了开闭原则，可以轻松地添加新的处理者或修改处理顺序，而不需要修改现有代码。

**核心问题：** 如何让多个对象有机会处理请求，而不需要知道具体处理者？

在复杂的系统中，一个请求可能需要经过多个处理步骤，每个步骤负责处理请求的不同方面。如果直接在代码中指定每个处理步骤，会导致发送者与接收者之间的紧耦合，使得系统难以维护和扩展。责任链模式解决了这个问题，通过将处理者组织成一条链，请求发送者只需要将请求发送给链的起点，而不需要知道具体哪个处理者会处理请求。这使得处理逻辑可以灵活配置，也使得系统更容易扩展。

**实现要点：**
1. **定义处理者接口：** 定义一个抽象的处理者接口，声明处理请求的方法，并包含对下一个处理者的引用。
2. **实现具体处理者：** 实现处理者接口，每个具体处理者实现自己的处理逻辑，并决定是否将请求传递给下一个处理者。
3. **组装处理链：** 将处理者按照一定的顺序链接起来，通常通过设置每个处理者的下一个处理者来实现。
4. **处理逻辑：** 每个处理者需要决定是处理请求还是将其传递给下一个处理者。
5. **终止条件：** 确定何时停止传递请求，可能是某个处理者处理了请求或到达链的末尾。

**注意事项：**
- **请求可能无法处理：** 如果链中的所有处理者都无法处理请求，请求可能会被忽略，需要考虑如何处理这种情况。
- **性能问题：** 如果链过长或处理逻辑复杂，可能会影响性能，需要权衡链的长度和处理效率。
- **调试困难：** 由于请求在链中传递，调试时可能难以追踪请求的处理过程。
- **链的维护：** 需要确保链的正确构建和维护，避免循环引用或断链。
- **处理顺序：** 处理者的顺序可能很重要，需要仔细设计处理顺序。

**适用情况：**
- **日志处理：** 不同级别的日志处理器可以组成一条链，如DEBUG、INFO、WARNING、ERROR处理器，每个处理器决定是否处理特定级别的日志。
- **审批流程：** 在工作流系统中，请求需要经过多个审批节点，如部门经理、总监、CEO等，每个节点决定是否批准或拒绝请求。
- **拦截器链：** 在Web框架中，请求需要经过多个拦截器，如认证拦截器、授权拦截器、日志拦截器等。
- **异常处理：** 不同的异常处理器可以组成链来处理不同类型的异常。
- **权限检查：** 多层权限检查可以组成责任链，每层检查不同的权限。

```cpp
class Handler {
protected:
    Handler* next_handler_;

public:
    Handler() : next_handler_(nullptr) {}
    
    void setNext(Handler* handler) {
        this->next_handler_ = handler;
    }
    
    virtual string handle(string request) {
        if (this->next_handler_) {
            return this->next_handler_->handle(request);
        }
        
        return {};
    }
};

class MonkeyHandler : public Handler {
public:
    string handle(string request) override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        } else {
            return Handler::handle(request);
        }
    }
};

class SquirrelHandler : public Handler {
public:
    string handle(string request) override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        } else {
            return Handler::handle(request);
        }
    }
};

class DogHandler : public Handler {
public:
    string handle(string request) override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + ".\n";
        } else {
            return Handler::handle(request);
        }
    }
};
```

**详细描述：**
责任链模式避免请求发送者与接收者耦合，让多个对象都有机会处理请求。在这个示例中，`Handler`是抽象处理者，定义了处理请求的接口，并持有下一个处理者的引用。`MonkeyHandler`、`SquirrelHandler`和`DogHandler`是具体处理者，每个处理者负责处理特定类型的请求。当一个处理者收到请求时，它首先检查自己是否能处理该请求，如果能处理则处理请求并返回结果，否则将请求传递给下一个处理者。客户端只需要将请求发送给链的第一个处理者，而不需要知道具体哪个处理者会处理请求。

**设计原因：**
1. **解耦合：** 发送者与接收者解耦，发送者不需要知道具体哪个对象处理请求
2. **动态指定处理者：** 可以在运行时动态指定处理请求的对象
3. **增强灵活性：** 可以灵活地添加、删除或调整处理者
4. **职责分离：** 每个处理者只需关注自己的处理逻辑

### 2. 命令模式 (Command Pattern)

**意图：** 将请求封装为对象，从而可以使用不同的请求对客户进行参数化。

命令模式的核心意图是将请求封装为对象，从而可以使用不同的请求对客户进行参数化。这种模式将操作（请求）及其参数封装在对象中，使得可以将操作作为对象来处理，可以存储、传递、排队、记录日志，甚至支持撤销和重做功能。通过将请求封装为命令对象，可以将发出请求的对象与执行请求的对象解耦，使得请求的发送者不需要知道请求的具体执行细节。这种模式特别适用于需要支持撤销操作、宏命令或任务队列的场景。

**设计思想：** 将操作封装为对象，支持撤销、重做、队列等功能。

命令模式的设计思想是将操作封装为对象，使操作具有对象的特性，如可存储、可传递、可组合等。这种设计将方法调用转换为一个命令对象，该对象包含执行操作所需的所有信息，包括接收者、方法和参数。通过这种方式，可以将命令对象存储在队列中、记录在日志中、传递给其他对象，甚至可以实现撤销和重做功能。设计思想强调了操作的可管理性，将操作的执行与操作的定义分离，使得系统更加灵活和可扩展。

**核心问题：** 如何将操作封装为对象，使其可以被存储、传递和执行？

在传统的编程模型中，操作通常通过方法调用直接执行，这使得操作难以存储、传递或管理。当我们需要实现撤销/重做功能、将操作放入队列中延迟执行、或者将操作记录下来供以后执行时，直接的方法调用就显得不够灵活。命令模式解决了这个问题，通过将操作封装为对象，使得操作可以像普通对象一样被处理，从而实现了操作的参数化和管理。

**实现要点：**
1. **定义命令接口：** 定义一个命令接口，声明执行操作的方法（如execute()），还可以包含撤销操作的方法（如undo()）。
2. **实现具体命令：** 实现命令接口，具体命令类包含对接收者的引用以及执行操作所需的所有参数。
3. **使用调用者执行命令：** 创建调用者类来持有和执行命令对象，调用者不需要知道具体执行什么操作。
4. **接收者：** 接收者是实际执行操作的对象，命令对象会将请求委派给接收者。
5. **参数存储：** 命令对象需要存储执行操作所需的所有参数。

**注意事项：**
- **增加类的数量：** 每个不同的操作都需要一个具体的命令类，这会增加系统的类数量，可能导致系统复杂性增加。
- **可能消耗内存：** 每个命令对象都需要存储执行操作所需的信息，如果命令对象需要长时间保存（如撤销历史），可能会消耗较多内存。
- **复杂性增加：** 对于简单的操作，使用命令模式可能会使系统变得过于复杂。
- **状态管理：** 对于支持撤销操作的命令，需要管理操作前后的状态，这可能增加实现的复杂性。
- **生命周期管理：** 需要妥善管理命令对象的生命周期，防止内存泄漏。

**适用情况：**
- **撤销/重做：** 文本编辑器、绘图软件等需要支持撤销和重做功能的应用。
- **宏命令：** 将一系列命令组合成一个宏命令，一次性执行多个操作。
- **任务队列：** 将命令对象放入队列中，按顺序执行或异步执行。
- **事务处理：** 在数据库操作中，将多个操作封装为一个命令对象，支持事务的提交和回滚。
- **日志记录：** 将命令记录下来，用于恢复系统状态或重放操作。

```cpp
// Receiver
class Light {
public:
    void turnOn() {
        cout << "The light is on\n";
    }
    
    void turnOff() {
        cout << "The light is off\n";
    }
};

// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Concrete Command
class LightOnCommand : public Command {
private:
    Light* light_;
    
public:
    LightOnCommand(Light* light) : light_(light) {}
    
    void execute() override {
        light_->turnOn();
    }
    
    void undo() override {
        light_->turnOff();
    }
};

class LightOffCommand : public Command {
private:
    Light* light_;
    
public:
    LightOffCommand(Light* light) : light_(light) {}
    
    void execute() override {
        light_->turnOff();
    }
    
    void undo() override {
        light_->turnOn();
    }
};

// Invoker
class RemoteControl {
private:
    unique_ptr<Command> on_command_;
    unique_ptr<Command> off_command_;
    
public:
    RemoteControl(unique_ptr<Command> on_cmd, unique_ptr<Command> off_cmd) 
        : on_command_(move(on_cmd)), off_command_(move(off_cmd)) {}
    
    void pressButton() {
        on_command_->execute();
    }
    
    void pressOffButton() {
        off_command_->execute();
    }
    
    void pressUndoButton() {
        on_command_->undo();
    }
};
```

**详细描述：**
命令模式将请求封装为对象，从而可以使用不同的请求对客户进行参数化。在这个示例中，`Light`是接收者，包含实际执行操作的方法。`Command`是命令接口，定义了执行和撤销操作的方法。`LightOnCommand`和`LightOffCommand`是具体命令，它们封装了对`Light`对象的操作，并实现了`execute()`和`undo()`方法。`RemoteControl`是调用者，持有命令对象，并在需要时调用命令的`execute()`方法。这种模式将请求的发起者和执行者解耦，使得命令可以排队、记录日志或撤销。

**设计原因：**
1. **解耦合：** 调用者与接收者解耦，调用者不需要知道具体执行什么操作
2. **支持撤销/重做：** 通过实现undo()方法，可以轻松实现撤销功能
3. **队列操作：** 可以将命令对象放入队列中，实现延迟执行
4. **参数化：** 可以将不同的命令对象传递给调用者，实现不同的行为

### 3. 解释器模式 (Interpreter Pattern)

**意图：** 给定一个语言，定义它的文法的一种表示，并定义一个解释器。

解释器模式的核心意图是给定一个语言，定义它的文法的一种表示，并定义一个解释器来解释该语言中的句子。这种模式主要用于定义语言的文法表示，并提供解释器来解释语言中的表达式或句子。解释器模式通过构建一个解释器来处理特定语言的表达式，通常会构建一个语法树来表示语言的结构。这种模式特别适用于需要解析和执行特定领域语言(DSL)的场景，如正则表达式、SQL查询、数学表达式等。通过解释器模式，可以将语言的语法表示和解释逻辑封装在对象中，使系统能够处理特定的语言结构。

**设计思想：** 为特定语言构建解释器，通过语法树解释语言表达式。

解释器模式的设计思想是为特定语言构建一个解释器，通过构建语法树来解释语言表达式。这种设计将语言的文法规则映射到对象结构中，每个文法规则对应一个类。终结符表达式对应文法中的终结符，非终结符表达式对应文法中的非终结符。通过组合终结符和非终结符表达式，可以构建复杂的语法树来表示复杂的语言结构。设计思想强调了递归结构的应用，解释器通过递归地解释子表达式来解释整个表达式。这种设计使得语言的扩展变得容易，只需添加新的表达式类即可支持新的语法规则。

**核心问题：** 如何为特定语言构建解释器，解释语言表达式？

在某些应用中，需要处理特定领域的语言或表达式，如数学公式、查询语言、配置文件等。直接在代码中硬编码解析逻辑会使代码复杂且难以维护。解释器模式解决了这个问题，通过将语言的文法规则映射到对象结构中，构建一个解释器来处理语言表达式。这样可以将语法解析和执行逻辑分离，使系统更加模块化和可扩展。

**实现要点：**
1. **定义表达式接口：** 定义抽象的表达式接口，声明解释方法，所有终结符和非终结符表达式都实现这个接口。
2. **实现终结符和非终结符：** 终结符表达式实现基本的解释逻辑，非终结符表达式包含其他表达式并实现复合的解释逻辑。
3. **构建语法树：** 通过组合终结符和非终结符表达式来构建语法树，表示语言的结构。
4. **上下文管理：** 管理解释过程中的上下文信息，如变量值、符号表等。
5. **递归解释：** 通过递归调用子表达式的解释方法来解释整个表达式。

**注意事项：**
- **复杂语法难以维护：** 当语言的文法变得复杂时，解释器的类层次结构也会变得复杂，难以维护和扩展。
- **性能问题：** 解释器模式可能比直接的解析器效率低，特别是对于复杂的表达式。
- **内存使用：** 构建语法树需要额外的内存空间，对于复杂的表达式可能消耗较多内存。
- **错误处理：** 需要仔细处理语法错误和运行时错误，提供有意义的错误信息。
- **扩展困难：** 修改文法规则可能需要修改多个类，扩展性有限。

**适用情况：**
- **小型DSL：** 适用于实现小型的特定领域语言，如配置脚本、规则定义语言等。
- **规则引擎：** 在业务规则引擎中，将业务规则表示为可解释的表达式。
- **表达式解析：** 解析和计算数学表达式、布尔表达式等。
- **查询语言：** 解析简单的查询语言或搜索条件。
- **配置解析：** 解析特定格式的配置文件或指令。

```cpp
class Expression {
public:
    virtual ~Expression() = default;
    virtual bool interpret(const string& context) = 0;
};

class TerminalExpression : public Expression {
private:
    string data_;
    
public:
    TerminalExpression(const string& data) : data_(data) {}
    
    bool interpret(const string& context) override {
        if (context.find(data_) != string::npos) {
            return true;
        }
        return false;
    }
};

class OrExpression : public Expression {
private:
    shared_ptr<Expression> expr1_;
    shared_ptr<Expression> expr2_;
    
public:
    OrExpression(shared_ptr<Expression> expr1, shared_ptr<Expression> expr2) 
        : expr1_(expr1), expr2_(expr2) {}
    
    bool interpret(const string& context) override {
        return expr1_->interpret(context) || expr2_->interpret(context);
    }
};

class AndExpression : public Expression {
private:
    shared_ptr<Expression> expr1_;
    shared_ptr<Expression> expr2_;
    
public:
    AndExpression(shared_ptr<Expression> expr1, shared_ptr<Expression> expr2) 
        : expr1_(expr1), expr2_(expr2) {}
    
    bool interpret(const string& context) override {
        return expr1_->interpret(context) && expr2_->interpret(context);
    }
};
```

**详细描述：**
解释器模式给定一个语言，定义它的文法的一种表示，并定义一个解释器。在这个示例中，`Expression`是抽象表达式接口，定义了解释方法。`TerminalExpression`是终结符表达式，代表文法中的终结符，它直接对上下文进行操作。`OrExpression`和`AndExpression`是非终结符表达式，它们包含其他表达式作为子表达式，并实现相应的逻辑运算。通过组合这些表达式，可以构建复杂的语法规则。例如，可以创建一个表达式来判断输入字符串是否包含某些关键词，或者是否满足复杂的条件组合。

**设计原因：**
1. **语法解析：** 适用于需要解析特定语法或表达式的场景
2. **灵活性：** 可以轻松扩展新的表达式类型
3. **易于修改：** 可以通过改变表达式组合来改变解释行为
4. **结构化：** 将语法结构映射到对象结构，便于理解和维护

### 4. 迭代器模式 (Iterator Pattern)

**意图：** 提供一种方法顺序访问一个聚合对象中的各个元素，而又不暴露其内部表示。

迭代器模式的核心意图是提供一种方法顺序访问一个聚合对象中的各个元素，而又不暴露其内部表示。这种模式为不同的聚合结构提供了一个统一的遍历接口，使得客户端代码可以以一致的方式遍历不同类型的聚合对象。迭代器模式将遍历的逻辑从聚合对象中分离出来，使得聚合对象可以专注于数据的存储和管理，而迭代器则专门负责数据的访问和遍历。这种模式特别适用于需要对聚合对象进行多种不同遍历方式的场景，如正向遍历、反向遍历、条件筛选遍历等。

**设计思想：** 将遍历逻辑从聚合对象中分离出来，提供统一的访问接口。

迭代器模式的设计思想是将聚合对象的遍历逻辑分离出来，通过一个专门的迭代器对象来管理遍历过程。这种设计将数据的存储和数据的访问分离，使得聚合对象和遍历算法可以独立变化。设计思想强调了统一接口的重要性，通过定义统一的迭代器接口，客户端代码可以使用相同的方法来遍历不同类型的聚合对象。这种设计还体现了封装性原则，聚合对象的内部结构对客户端是透明的，客户端只需要通过迭代器接口来访问数据。迭代器还提供了对遍历过程的精确控制，可以随时暂停、恢复或重新开始遍历。

**核心问题：** 如何提供统一的访问接口，而不暴露聚合对象的内部结构？

在面向对象设计中，经常会遇到需要遍历聚合对象中元素的需求，如数组、列表、树、图等。如果直接暴露聚合对象的内部结构，会导致客户端代码与聚合对象的具体实现紧密耦合，一旦聚合对象的内部结构发生变化，客户端代码也需要相应修改。此外，不同类型的聚合对象可能需要不同的遍历算法，如果在每个聚合对象中都实现遍历逻辑，会导致代码重复。迭代器模式解决了这个问题，通过将遍历逻辑封装在独立的迭代器对象中，客户端可以通过统一的接口来访问不同类型的聚合对象，而不需要了解它们的内部结构。

**实现要点：**
1. **定义迭代器接口：** 定义一个抽象的迭代器接口，包含遍历所需的基本方法，如`next()`、`hasNext()`、`first()`、`currentItem()`等。
2. **实现具体迭代器：** 为每种聚合结构实现具体的迭代器类，维护遍历过程中的状态信息，如当前位置、遍历方向等。
3. **聚合对象提供创建迭代器的方法：** 聚合对象应提供创建相应迭代器的方法，通常是`createIterator()`或`iterator()`方法。
4. **状态管理：** 迭代器需要维护遍历过程中的状态，确保能够正确地访问下一个元素。
5. **有效性检查：** 提供方法检查迭代器是否仍然有效，避免访问无效元素。

**注意事项：**
- **需要考虑并发修改：** 在遍历过程中，如果聚合对象被修改，可能会导致迭代器失效或产生不一致的结果，需要考虑实现快照迭代器或抛出异常。
- **可能影响性能：** 迭代器引入了额外的间接层，可能会对性能产生一定影响，特别是在高频访问的场景中。
- **内存开销：** 迭代器对象需要维护遍历状态，会占用额外的内存空间。
- **生命周期管理：** 需要确保迭代器的生命周期不超过其所遍历的聚合对象。
- **一致性问题：** 在多线程环境中，需要考虑迭代器状态的一致性问题。

**适用情况：**
- **遍历容器：** 当需要以统一方式遍历不同类型的容器时，如列表、集合、映射等。
- **隐藏内部存储结构：** 当希望隐藏聚合对象的内部实现细节，只暴露遍历接口时。
- **统一访问接口：** 当需要为不同类型的聚合对象提供统一的访问方式时。
- **复杂遍历逻辑：** 当遍历逻辑比较复杂，如树的前序、中序、后序遍历时。
- **延迟计算：** 当需要实现惰性求值的遍历，即只在需要时才计算下一个元素时。

```cpp
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual T next() = 0;
    virtual bool hasNext() const = 0;
};

template<typename T>
class Container {
public:
    virtual ~Container() = default;
    virtual unique_ptr<Iterator<T>> createIterator() = 0;
};

template<typename T>
class NumberList : public Container<T> {
private:
    vector<T> numbers_;
    
public:
    void addNumber(T number) {
        numbers_.push_back(number);
    }
    
    unique_ptr<Iterator<T>> createIterator() override {
        return make_unique<NumberListIterator<T>>(*this);
    }
    
    vector<T>& getNumbers() {
        return numbers_;
    }
};

template<typename T>
class NumberListIterator : public Iterator<T> {
private:
    NumberList<T>& list_;
    size_t position_ = 0;
    
public:
    NumberListIterator(NumberList<T>& list) : list_(list) {}
    
    T next() override {
        T value = list_.getNumbers()[position_];
        position_++;
        return value;
    }
    
    bool hasNext() const override {
        return position_ < list_.getNumbers().size();
    }
};
```

**详细描述：**
迭代器模式提供了一种方法顺序访问聚合对象中的各个元素，而又不暴露其内部表示。在这个示例中，`Iterator`是迭代器接口，定义了访问和遍历元素的方法(`next()`和`hasNext()`)。`Container`是容器接口，提供创建迭代器的方法。`NumberList`是具体容器类，内部使用vector存储数据，并实现了创建迭代器的方法。`NumberListIterator`是具体迭代器类，持有一个对容器的引用和当前位置索引，通过`hasNext()`判断是否还有下一个元素，通过`next()`返回当前元素并将位置前移。这种设计将遍历逻辑从容器中分离出来，使得可以有不同的遍历策略，同时也隐藏了容器的内部实现。

**设计原因：**
1. **统一接口：** 为不同类型的聚合对象提供统一的遍历接口
2. **解耦：** 将遍历逻辑与聚合对象分离，降低耦合度
3. **多遍历策略：** 可以为同一聚合对象实现不同的迭代器，支持多种遍历方式
4. **简化客户端：** 客户端无需了解聚合对象的内部结构即可遍历元素

### 5. 中介者模式 (Mediator Pattern)

**意图：** 用一个中介对象来封装一系列对象之间的交互。

中介者模式的核心意图是用一个中介对象来封装一系列对象之间的交互。这种模式将对象之间的直接交互转换为通过中介者进行的间接交互，从而减少对象之间的直接依赖关系。中介者模式将网状的依赖关系转换为星型结构，所有对象都只依赖于中介者，而不再直接依赖其他对象。这种模式特别适用于对象之间存在复杂交互关系的场景，通过中介者可以集中管理这些交互逻辑，使系统更容易理解和维护。

**设计思想：** 集中管理对象间的复杂交互，减少对象间的直接依赖。

中介者模式的设计思想是集中管理对象间的复杂交互，将原本分散在多个对象中的交互逻辑集中到一个中介者对象中。这种设计将对象间的通信从点对点的直接通信转变为通过中介者的间接通信。设计思想强调了集中控制的好处，通过中介者可以统一处理对象间的交互逻辑，避免了对象之间的紧耦合。中介者扮演了协调者的角色，了解各个对象的职责和交互方式，并负责协调它们之间的协作。这种设计还体现了单一职责原则，每个对象只需要关注自己的业务逻辑，而不需要了解其他对象的细节。

**核心问题：** 如何减少对象间的紧耦合，避免网状依赖关系？

在复杂的系统中，对象之间可能存在大量的直接交互，导致对象之间形成网状的依赖关系。这种网状结构使得系统难以理解、维护和扩展，因为任何一个对象的改变都可能影响到多个其他对象。此外，对象间的直接依赖关系使得它们难以独立地变化和复用。中介者模式解决了这个问题，通过引入中介者对象，将对象间的直接交互转换为通过中介者的间接交互，从而将网状结构简化为星型结构。

**实现要点：**
1. **定义中介者接口：** 定义中介者接口，声明协调对象交互的方法，通常包括接收通知和触发响应的方法。
2. **实现具体中介者：** 实现中介者接口，具体中介者了解所有同事对象，并负责协调它们之间的交互。
3. **同事对象持有中介者引用：** 同事对象持有对中介者的引用，通过中介者与其他对象通信。
4. **交互协议：** 定义同事对象与中介者之间的通信协议，通常通过事件或消息传递。
5. **状态管理：** 中介者可能需要维护系统状态，以便正确处理对象间的交互。

**注意事项：**
- **中介者可能变得过于复杂：** 由于中介者需要了解所有同事对象并协调它们的交互，随着系统复杂性的增加，中介者本身可能变得非常复杂和庞大。
- **可能成为性能瓶颈：** 所有对象间的交互都要通过中介者，这可能成为系统的性能瓶颈。
- **过度集中：** 中介者可能过度集中了系统的交互逻辑，违反了单一职责原则。
- **测试困难：** 由于中介者协调多个对象，对其进行单元测试可能比较困难。
- **调试复杂：** 由于交互逻辑集中在中介者中，调试时可能需要跟踪多个对象之间的交互流程。

**适用情况：**
- **复杂UI组件交互：** 在复杂的用户界面中，多个组件之间需要相互通信，如按钮点击影响文本框显示、下拉列表选择影响其他控件等。
- **聊天室：** 在聊天室系统中，用户之间的消息传递需要通过服务器中介进行转发。
- **协作场景：** 在协同工作系统中，多个参与者需要协调他们的操作，如文档协作编辑、多人游戏等。
- **工作流管理：** 在工作流系统中，不同步骤之间需要协调和通信。
- **模块间通信：** 在模块化系统中，不同模块之间需要通信但又不能直接耦合。

```cpp
class Colleague;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void notify(Colleague* sender, const string& event) = 0;
};

class Colleague {
protected:
    Mediator* mediator_;
    
public:
    Colleague(Mediator* mediator = nullptr) : mediator_(mediator) {}
    
    void setMediator(Mediator* mediator) {
        this->mediator_ = mediator;
    }
};

class Component1 : public Colleague {
public:
    Component1(Mediator* mediator = nullptr) : Colleague(mediator) {}
    
    void doA() {
        cout << "Component 1 does A.\n";
        this->mediator_->notify(this, "A");
    }
    
    void doB() {
        cout << "Component 1 does B.\n";
        this->mediator_->notify(this, "B");
    }
};

class Component2 : public Colleague {
public:
    Component2(Mediator* mediator = nullptr) : Colleague(mediator) {}
    
    void doC() {
        cout << "Component 2 does C.\n";
        this->mediator_->notify(this, "C");
    }
    
    void doD() {
        cout << "Component 2 does D.\n";
        this->mediator_->notify(this, "D");
    }
};

class ConcreteMediator : public Mediator {
private:
    shared_ptr<Component1> component1_;
    shared_ptr<Component2> component2_;
    
public:
    ConcreteMediator(shared_ptr<Component1> c1, shared_ptr<Component2> c2) 
        : component1_(c1), component2_(c2) {
        component1_->setMediator(this);
        component2_->setMediator(this);
    }
    
    void notify(Colleague* sender, const string& event) override {
        if (event == "A") {
            cout << "Mediator reacts on A and triggers following operations:\n";
            component2_->doC();
        }
        if (event == "D") {
            cout << "Mediator reacts on D and triggers following operations:\n";
            component1_->doB();
            component2_->doC();
        }
    }
};
```

**详细描述：**
中介者模式用一个中介对象来封装一系列对象之间的交互。在这个示例中，`Mediator`是中介者接口，定义了接收同事对象通知的方法。`Colleague`是抽象同事类，持有对中介者的引用。`Component1`和`Component2`是具体同事类，它们不直接相互通信，而是通过中介者进行交互。`ConcreteMediator`是具体中介者，了解所有同事对象，并负责协调它们之间的交互。当一个同事对象发生特定事件时，它会通知中介者，中介者根据情况决定如何响应，可能会触发其他同事对象的动作。

**设计原因：**
1. **减少耦合：** 同事对象之间不需要相互了解，降低了对象间的耦合度
2. **集中控制：** 将对象间的交互逻辑集中在中介者中，便于管理和维护
3. **可扩展性：** 添加新的同事对象时，只需修改中介者即可
4. **复用性：** 同事对象可以更容易地被复用，因为它们不依赖于其他特定对象

### 6. 备忘录模式 (Memento Pattern)

**意图：** 在不破坏封装性的条件下，捕获一个对象的内部状态，并在该对象之外保存这个状态。

备忘录模式的核心意图是在不破坏封装性的条件下，捕获一个对象的内部状态，并在该对象之外保存这个状态。这样以后就可以将该对象恢复到原先保存的状态。备忘录模式允许在不破坏对象封装性的前提下保存和恢复对象的状态，特别适用于需要支持撤销操作、回滚操作或快照功能的场景。这种模式通过创建一个备忘录对象来存储发起人的内部状态，使得发起人的状态可以在未来某个时间点被恢复到之前的状态，同时保持了对象的封装性。

**设计思想：** 保存和恢复对象状态，支持撤销操作。

备忘录模式的设计思想是将对象的状态保存和对象的业务逻辑分离，通过引入备忘录对象来保存发起人的内部状态。这种设计将状态的保存和恢复操作委托给备忘录对象，发起人只负责创建备忘录和从备忘录恢复状态，而看护者负责管理备忘录的生命周期。设计思想强调了封装性的保护，通过窄接口和宽接口的设计，确保只有发起人能够访问备忘录的内部状态，而看护者只能管理备忘录对象但不能访问其内容。这种设计还体现了职责分离的原则，将状态管理的职责从发起人分离到备忘录和看护者中。

**核心问题：** 如何在不破坏封装性的前提下保存和恢复对象状态？

在面向对象设计中，有时候需要保存对象的当前状态以便将来能够恢复到该状态，如实现撤销功能、事务回滚或快照功能。但如果直接访问对象的私有成员来保存状态，就会破坏对象的封装性。备忘录模式解决了这个问题，通过引入一个专门的备忘录对象来保存发起人的状态，发起人可以创建备忘录来保存自己的状态，也可以从备忘录恢复状态，而看护者则负责管理备忘录的生命周期，但无法访问备忘录的内容。

**实现要点：**
1. **定义备忘录类：** 创建备忘录类来存储发起人的内部状态，通常只允许发起人访问其内容。
2. **发起人管理状态：** 发起人负责创建备忘录以保存当前状态，并可以从备忘录恢复状态。
3. **看护者管理备忘录：** 看护者负责管理备忘录的生命周期，但不能访问备忘录的内容。
4. **窄接口与宽接口：** 备忘录通常对外提供窄接口，只对发起人提供宽接口。
5. **状态管理：** 发起人需要能够将自己的状态完整地保存到备忘录中，并能从备忘录中恢复状态。

**注意事项：**
- **内存消耗：** 保存多个状态快照可能会消耗大量内存，特别是当对象状态很复杂或需要保存很多历史状态时。
- **破坏封装性：** 如果实现不当，可能会通过备忘录暴露对象的内部结构，破坏封装性。
- **生命周期管理：** 需要妥善管理备忘录的生命周期，防止内存泄漏。
- **性能影响：** 频繁创建和恢复状态快照可能会影响性能。
- **一致性问题：** 如果对象状态包含外部引用或指针，恢复状态时可能出现一致性问题。

**适用情况：**
- **撤销操作：** 在编辑器、IDE或其他需要撤销功能的应用程序中，保存用户操作前的状态以便撤销操作。
- **游戏存档：** 在游戏中保存游戏进度，允许玩家从之前的存档点继续游戏。
- **回滚操作：** 在事务处理系统中，保存事务开始前的状态，以便在事务失败时回滚到之前的状态。
- **版本控制：** 在文档编辑系统中保存文档的历史版本。
- **配置备份：** 在系统配置管理中保存配置的备份，以便在配置出错时恢复。

```cpp
class Memento {
private:
    string state_;
    
public:
    Memento(const string& state) : state_(state) {}
    
    const string& getState() const {
        return this->state_;
    }
};

class Originator {
private:
    string state_;
    
public:
    Originator(const string& state) : state_(state) {
        cout << "Originator: My initial state is: " << this->state_ << "\n";
    }
    
    void doSomething() {
        cout << "Originator: I'm doing something important.\n";
        this->state_ = this->generateRandomString(30);
        cout << "Originator: and my state has changed to: " << this->state_ << "\n";
    }
    
private:
    string generateRandomString(int length = 10) {
        const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string result;
        for (int i = 0; i < length; i++) {
            result += charset[rand() % (sizeof(charset) - 1)];
        }
        return result;
    }
    
public:
    unique_ptr<Memento> save() {
        return make_unique<Memento>(this->state_);
    }
    
    void restore(unique_ptr<Memento> memento) {
        if (!memento) {
            return;
        }
        this->state_ = memento->getState();
        cout << "Originator: My state has changed to: " << this->state_ << "\n";
    }
};

class Caretaker {
private:
    vector<unique_ptr<Memento>> mementos_;
    Originator* originator_;
    
public:
    Caretaker(Originator* originator) : originator_(originator) {
        this->mementos_ = {};
    }
    
    void backup() {
        cout << "\nCaretaker: Saving Originator's state...\n";
        this->mementos_.push_back(this->originator_->save());
    }
    
    void undo() {
        if (!this->mementos_.size()) {
            return;
        }
        const unique_ptr<Memento> memento = move(this->mementos_.back());
        this->mementos_.pop_back();
        cout << "Caretaker: Restoring state to: " << memento->getState() << "\n";
        try {
            this->originator_->restore(move(memento));
        } catch (...) {
            this->undo();
        }
    }
    
    void showHistory() const {
        cout << "Caretaker: Here's the list of mementos:\n";
        for (const unique_ptr<Memento> memento : this->mementos_) {
            cout << memento->getState() << "\n";
        }
    }
};
```

**详细描述：**
备忘录模式在不破坏封装性的条件下，捕获一个对象的内部状态，并在该对象之外保存这个状态。在这个示例中，`Memento`类负责存储`Originator`对象的内部状态，它是一个窄接口，只允许创建它的`Originator`访问其内部状态。`Originator`是发起人，它创建备忘录以记录当前状态，并可以从备忘录恢复状态。`Caretaker`是看护者，负责管理备忘录，但不能访问备忘录的内容。这种设计确保了发起人的内部状态可以在不影响封装性的前提下被保存和恢复。

**设计原因：**
1. **封装性保护：** 在保存和恢复状态时不破坏对象的封装性
2. **撤销功能：** 提供了强大的撤销和回滚功能
3. **状态管理：** 有效地管理对象的多个历史状态
4. **职责分离：** 将状态保存和管理的职责分离给不同的对象

### 7. 观察者模式 (Observer Pattern)

**意图：** 定义对象间的一对多依赖关系，当一个对象改变状态时，所有依赖它的对象都会得到通知。

观察者模式的核心意图是定义对象间的一对多依赖关系，当一个对象（主题或可观察者）的状态发生改变时，所有依赖于它的对象（观察者）都会得到通知并自动更新。这种模式实现了发布-订阅机制，使得主题对象可以将状态变化推送给所有订阅的观察者，而无需知道观察者的具体类型或数量。观察者模式提供了一种松耦合的通信机制，主题对象不需要了解观察者的具体实现，观察者也不需要知道主题对象的内部细节，它们只需要遵循约定的接口即可。

**设计思想：** 建立发布-订阅机制，实现松耦合的通信。

观察者模式的设计思想是建立一种发布-订阅机制，通过将对象间的关系从紧耦合转变为松耦合。主题对象（发布者）维护一个观察者列表，当其状态发生变化时，自动通知所有注册的观察者（订阅者）。这种设计将主题和观察者的依赖关系解耦，使得它们可以独立地变化和扩展。设计思想强调了事件驱动的编程模型，通过事件通知机制实现对象间的通信，避免了直接的函数调用和硬编码的依赖关系。这种设计还体现了依赖倒置原则，高层模块（主题）和低层模块（观察者）都依赖于抽象（观察者接口）。

**核心问题：** 如何建立松耦合的通信机制，使对象状态变化时自动通知依赖对象？

在传统的编程模型中，当一个对象的状态发生变化时，如果需要通知其他对象，通常需要在代码中硬编码对这些对象的引用，这导致了对象间的紧耦合。当需要添加新的依赖对象或修改通知逻辑时，需要修改主题对象的代码，这违反了开闭原则。观察者模式解决了这个问题，通过引入观察者接口，主题对象只需要维护一个观察者接口的列表，而不需要知道具体观察者的类型，实现了松耦合的通信机制。

**实现要点：**
1. **定义观察者接口：** 定义一个观察者接口，声明更新方法，所有具体的观察者类都需要实现这个接口。
2. **主题管理观察者列表：** 主题类维护一个观察者列表，提供添加、删除和通知观察者的方法。
3. **状态变化时通知所有观察者：** 当主题的状态发生变化时，自动调用通知方法，遍历观察者列表并调用每个观察者的更新方法。
4. **观察者注册：** 观察者需要向主题注册自己，以便接收状态更新通知。
5. **避免循环引用：** 在C++中使用weak_ptr等智能指针来避免循环引用导致的内存泄漏。

**注意事项：**
- **内存泄漏风险：** 如果观察者没有正确地从主题中注销，可能会导致内存泄漏，特别是在使用智能指针时需要注意循环引用。
- **通知顺序不确定：** 观察者接收通知的顺序通常是不确定的，不能依赖特定的通知顺序。
- **性能问题：** 如果观察者数量很多或更新逻辑复杂，通知过程可能会消耗较多时间。
- **意外更新：** 如果观察者在更新过程中修改主题的状态，可能会导致意外的连锁反应。
- **调试困难：** 由于通知是自动发生的，调试时可能难以追踪状态变化的源头。

**适用情况：**
- **事件系统：** 在事件驱动的系统中，当特定事件发生时需要通知所有感兴趣的监听器。
- **GUI事件处理：** 当用户界面组件的状态发生变化时（如按钮点击、窗口大小调整），需要通知相关的处理程序。
- **消息通知：** 在分布式系统或应用程序中，当某个状态或数据发生变化时需要通知相关的组件或服务。
- **模型-视图架构：** 在MVC或MVVM架构中，当模型数据发生变化时，需要通知视图进行更新。
- **数据绑定：** 在现代UI框架中，当数据模型发生变化时自动更新UI元素。

```cpp
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& message_from_subject) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(shared_ptr<Observer> observer) = 0;
    virtual void detach(shared_ptr<Observer> observer) = 0;
    virtual void notify() = 0;
};

class ConcreteSubject : public Subject {
private:
    vector<weak_ptr<Observer>> observers_;
    string state_;
    
public:
    string getState() const {
        return this->state_;
    }
    
    void setState(const string& state) {
        this->state_ = state;
        this->notify();
    }
    
    void attach(shared_ptr<Observer> observer) override {
        this->observers_.push_back(observer);
    }
    
    void detach(shared_ptr<Observer> observer) override {
        observers_.erase(
            remove_if(observers_.begin(), observers_.end(),
                [&observer](const weak_ptr<Observer>& obs) {
                    auto locked_obs = obs.lock();
                    return locked_obs && (locked_obs == observer);
                }),
            observers_.end()
        );
    }
    
    void notify() override {
        cout << "Subject: Notifying observers...\n";
        for (auto& obs_weak : observers_) {
            if (auto obs = obs_weak.lock()) {
                obs->update(this->state_);
            }
        }
    }
};

class ConcreteObserverA : public Observer {
public:
    void update(const string& message_from_subject) override {
        cout << "ConcreteObserverA: Reacted to '" << message_from_subject << "'\n";
    }
};

class ConcreteObserverB : public Observer {
public:
    void update(const string& message_from_subject) override {
        cout << "ConcreteObserverB: Reacted to '" << message_from_subject << "'\n";
    }
};
```

**详细描述：**
观察者模式定义对象间的一对多依赖关系，当一个对象改变状态时，所有依赖它的对象都会得到通知。在这个示例中，`Observer`是观察者接口，定义了更新方法。`Subject`是主题接口，定义了添加、删除和通知观察者的方法。`ConcreteSubject`是具体主题，维护观察者列表并管理状态。当状态发生变化时，`setState()`方法会自动通知所有观察者。`ConcreteObserverA`和`ConcreteObserverB`是具体观察者，它们实现更新方法来响应主题的状态变化。使用`weak_ptr`是为了避免循环引用导致的内存泄漏。

**设计原因：**
1. **松耦合：** 主题和观察者之间保持松散耦合关系
2. **动态关联：** 可以在运行时动态添加或删除观察者
3. **广播通信：** 实现了一对多的通知机制
4. **符合开闭原则：** 添加新的观察者无需修改现有代码

### 8. 状态模式 (State Pattern)

**意图：** 允许对象在其内部状态改变时改变它的行为。

状态模式的核心意图是允许对象在其内部状态改变时改变它的行为。这种模式将对象的行为封装在不同的状态对象中，当对象的状态发生改变时，其行为也随之改变。状态模式将与特定状态相关的行为局部化到专门的状态类中，并将状态转换的责任委托给代表不同状态的类。这种模式特别适用于对象的行为取决于其状态，并且在运行时需要根据状态改变其行为的场景。通过状态模式，可以消除大量的条件语句，使代码更易于理解和维护。

**设计思想：** 将状态相关的行为封装到专门的状态类中，使状态转换显式化。

状态模式的设计思想是将对象的状态相关行为封装到专门的状态类中，通过将状态转换的逻辑分布在各个状态类中，使得状态转换变得显式和清晰。设计思想强调了行为与状态的绑定，每个状态类封装了对象在该状态下特有的行为。这种设计将条件分支逻辑转换为对象的组合和委托，使得系统更容易扩展和维护。设计思想还体现了单一职责原则，每个状态类只负责处理对象在特定状态下的行为。上下文对象委托当前状态对象来处理请求，当需要状态转换时，上下文改变其持有的状态对象。

**核心问题：** 如何根据对象的内部状态改变其行为？

在面向对象设计中，经常会遇到对象的行为依赖于其内部状态的场景。传统的解决方案是使用大量的条件语句（if-else或switch-case）来根据对象的状态执行不同的行为，这种方法不仅使代码难以理解和维护，而且当需要添加新的状态或修改状态转换逻辑时，需要修改大量的代码。状态模式解决了这个问题，通过将每个状态封装为一个类，将状态相关的行为移到对应的状态类中，使得状态转换变得清晰和易于管理。

**实现要点：**
1. **定义状态接口：** 定义一个状态接口，声明所有具体状态类需要实现的方法，这些方法定义了对象在该状态下的行为。
2. **实现具体状态：** 为每个状态实现一个具体的状态类，每个类封装了对象在该状态下的特定行为和状态转换逻辑。
3. **上下文管理状态转换：** 上下文类持有对当前状态对象的引用，并将请求委托给当前状态对象处理，同时负责在适当的时候切换状态。
4. **状态转换：** 状态转换可以发生在状态类内部，也可以在上下文类中进行。
5. **双向引用：** 有时需要状态对象访问上下文的信息，这时可以在状态类中持有对上下文的引用。

**注意事项：**
- **状态类数量可能很多：** 对于状态复杂的对象，可能需要创建大量的状态类，这会增加系统的复杂性。
- **可能引入循环依赖：** 当状态类之间需要相互引用或状态转换逻辑复杂时，可能会引入循环依赖。
- **性能开销：** 状态模式引入了额外的对象创建和方法调用，可能会带来一定的性能开销。
- **状态转换逻辑：** 状态转换的逻辑需要仔细设计，避免状态转换的死循环或非法转换。
- **内存使用：** 每个状态对象都需要占用内存，对于状态频繁转换的场景需要考虑内存管理。

**适用情况：**
- **有限状态机：** 适用于实现有限状态机，如解析器、协议实现等。
- **订单状态流转：** 在电子商务系统中，订单从创建到完成经历多个状态，如待支付、已支付、发货中、已完成等。
- **工作流管理：** 在工作流系统中，任务或流程在不同阶段有不同的处理逻辑。
- **用户界面控件：** 如按钮在不同状态（正常、按下、禁用）下有不同的外观和行为。
- **游戏角色状态：** 游戏中角色在不同状态（行走、攻击、防御）下有不同的行为模式。

```cpp
class Context;

class State {
public:
    virtual ~State() = default;
    virtual void handle(Context& context) = 0;
    virtual string getStateName() const = 0;
};

class Context {
private:
    shared_ptr<State> state_;
    
public:
    Context(shared_ptr<State> state) : state_(state) {
        cout << "Context: New state is " << state_->getStateName() << ".\n";
    }
    
    void transitionTo(shared_ptr<State> state) {
        state_ = state;
        cout << "Context: Transition to " << state_->getStateName() << ".\n";
    }
    
    void request() {
        state_->handle(*this);
    }
    
    shared_ptr<State> getState() const {
        return state_;
    }
};

class ConcreteStateA : public State {
public:
    string getStateName() const override {
        return "State A";
    }
    
    void handle(Context& context) override {
        cout << "ConcreteStateA handles the request.\n";
        cout << "ConcreteStateA wants to change the state of the context.\n";
        context.transitionTo(make_shared<ConcreteStateB>());
    }
};

class ConcreteStateB : public State {
public:
    string getStateName() const override {
        return "State B";
    }
    
    void handle(Context& context) override {
        cout << "ConcreteStateB handles the request.\n";
        cout << "ConcreteStateB wants to change the state of the context.\n";
        context.transitionTo(make_shared<ConcreteStateA>());
    }
};
```

**详细描述：**
状态模式允许对象在其内部状态改变时改变它的行为。在这个示例中，`State`是状态接口，定义了处理请求的方法。`Context`是上下文类，维护当前状态并委托状态对象处理请求。`ConcreteStateA`和`ConcreteStateB`是具体状态类，它们实现特定状态的行为，并可以在适当时机改变上下文的状态。当上下文收到请求时，它将请求委托给当前状态对象处理，状态对象可以执行相应的行为并可能改变上下文的状态。这种模式将与特定状态相关的行为封装在专门的状态类中，使得状态转换变得显式和清晰。

**设计原因：**
1. **消除条件语句：** 避免了使用大量if-else或switch语句来处理不同状态
2. **增强可扩展性：** 可以轻松添加新的状态而无需修改现有代码
3. **状态封装：** 将特定状态的行为封装在专门的类中
4. **简化逻辑：** 使状态转换逻辑更加清晰和易于理解

### 9. 策略模式 (Strategy Pattern)

**意图：** 定义一系列算法，把它们一个个封装起来，并且使它们可互相替换。

策略模式的核心意图是定义一系列算法，将它们一个个封装起来，并且使它们可以互相替换。这种模式将算法的实现与使用算法的上下文分离，使得算法的变化不会影响使用算法的客户端代码。策略模式允许在运行时根据需要选择不同的算法实现，使系统具有更好的灵活性和可扩展性。通过将算法封装在独立的策略类中，可以独立地改变算法的实现而不影响其他部分，这符合开闭原则。

**设计思想：** 将算法从使用算法的环境中分离出来，使算法可以独立变化。

策略模式的设计思想是将算法的实现与使用算法的上下文分离，通过定义一个策略接口，将不同的算法实现封装在各自的策略类中。这种设计使得算法可以独立于使用它的上下文而变化，客户端代码通过策略接口与算法交互，而不需要知道具体算法的实现细节。设计思想强调了算法的可替换性，通过依赖注入的方式将具体的策略对象注入到上下文中，使得可以在运行时动态地切换算法。这种设计还体现了依赖倒置原则，高层模块（上下文）依赖于抽象（策略接口），而不是具体实现。

**核心问题：** 如何在运行时选择和切换算法？

在软件开发中，经常会遇到一个问题：一个类需要支持多种算法或行为，这些算法在不同的情况下使用。传统的解决方案是使用条件语句（if-else或switch）来选择不同的算法，但这会导致代码复杂、难以维护和扩展。每当需要添加新的算法时，都需要修改原有的代码，违反了开闭原则。策略模式解决了这个问题，通过将每种算法封装在独立的策略类中，使得可以在运行时动态地选择和切换算法，而不需要修改使用算法的上下文代码。

**实现要点：**
1. **定义策略接口：** 定义一个策略接口，声明算法的公共接口，所有具体策略类都实现这个接口。
2. **实现具体策略：** 实现策略接口，每个具体策略类封装一种特定的算法实现。
3. **上下文持有策略对象：** 创建上下文类，持有对策略对象的引用，通过策略接口与算法交互。
4. **策略注入：** 通过构造函数或setter方法将具体策略对象注入到上下文中。
5. **策略切换：** 提供在运行时切换策略的能力，通常通过setter方法实现。

**注意事项：**
- **客户端需要了解不同策略：** 客户端需要知道不同策略之间的差异，以便选择合适的策略，这可能增加客户端的复杂性。
- **增加类的数量：** 每种算法都需要一个独立的策略类，这会增加系统的类数量，可能使系统变得复杂。
- **性能开销：** 策略模式引入了额外的间接层，可能会带来轻微的性能开销。
- **策略选择逻辑：** 需要确定策略选择的逻辑，有时这本身可能变得复杂。
- **内存使用：** 每个策略对象都需要占用内存，如果策略对象有状态，可能需要考虑状态管理。

**适用情况：**
- **多种算法可选：** 当一个系统需要支持多种算法或行为，且这些算法在不同情况下使用时。
- **需要在运行时切换算法：** 当需要在程序运行时根据配置、用户选择或其他条件动态切换算法时。
- **算法变体：** 当有多种算法的变体，且这些变体可以相互替换时。
- **业务规则变化：** 当业务规则可能会变化，需要能够灵活地替换算法实现时。
- **消除条件语句：** 当代码中存在大量的条件语句来选择不同的行为时，可以使用策略模式来消除这些条件语句。

```cpp
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual string doAlgorithm(const vector<string>& data) const = 0;
};

class ConcreteStrategyA : public Strategy {
public:
    string doAlgorithm(const vector<string>& data) const override {
        string result = "Strategy A: ";
        vector<string> sorted_data = data;
        sort(sorted_data.begin(), sorted_data.end());
        for (const string& element : sorted_data) {
            result += element + ",";
        }
        return result;
    }
};

class ConcreteStrategyB : public Strategy {
public:
    string doAlgorithm(const vector<string>& data) const override {
        string result = "Strategy B: ";
        vector<string> sorted_data = data;
        sort(sorted_data.rbegin(), sorted_data.rend());
        for (const string& element : sorted_data) {
            result += element + ",";
        }
        return result;
    }
};

class Context {
private:
    shared_ptr<Strategy> strategy_;
    
public:
    Context(shared_ptr<Strategy> strategy) : strategy_(strategy) {}
    
    void setStrategy(shared_ptr<Strategy> strategy) {
        strategy_ = strategy;
    }
    
    string executeStrategy(const vector<string>& data) {
        return strategy_->doAlgorithm(data);
    }
};
```

**详细描述：**
策略模式定义一系列算法，把它们一个个封装起来，并且使它们可互相替换。在这个示例中，`Strategy`是策略接口，定义了算法的公共接口。`ConcreteStrategyA`和`ConcreteStrategyB`是具体策略类，它们实现了不同的算法。`Context`是上下文类，持有对策略对象的引用，并将算法请求委托给策略对象执行。客户端可以动态地选择和切换不同的策略，而无需修改上下文类的代码。这种模式将算法的实现与使用算法的上下文分离，使算法可以独立变化。

**设计原因：**
1. **算法可替换：** 可以在运行时动态选择和替换算法
2. **避免条件语句：** 消除了大量的条件判断语句
3. **职责分离：** 将算法的实现与使用算法的上下文分离
4. **可扩展性：** 可以轻松添加新的算法策略

### 10. 模板方法模式 (Template Method Pattern)

**意图：** 定义一个操作中的算法骨架，而将一些步骤延迟到子类中。

模板方法模式的核心意图是在一个方法中定义算法的骨架，而将一些步骤延迟到子类中实现。这种模式使得子类可以在不改变算法结构的前提下重新定义算法的某些特定步骤。模板方法定义了算法的执行顺序和框架，但将一些具体的实现留给子类来完成。通过这种方式，可以在基类中复用算法的通用部分，同时允许子类自定义特定的实现细节。模板方法模式结合了代码复用和灵活性，是面向对象设计中一个重要的模式。

**设计思想：** 在基类中定义算法框架，子类实现具体步骤。

模板方法模式的设计思想是在抽象基类中定义算法的整体结构和执行流程，而将具体的实现细节交给子类来完成。这种设计将算法的不变部分放在基类中实现，而将可变的部分声明为抽象方法，由子类实现。模板方法通常使用final关键字来防止子类重写，确保算法结构的完整性。设计思想强调了代码复用和扩展性的平衡，通过钩子方法（Hook Methods）允许子类在特定点介入算法流程，同时保持算法的整体结构不变。

**核心问题：** 如何在不改变算法结构的情况下重新定义算法的某些步骤？

在软件开发中，经常会遇到这样的情形：一个算法的总体结构是固定的，但某些步骤的具体实现可能因情况而异。如果为每种情况都实现一个完整的算法，会导致代码重复；如果使用条件语句来处理不同的实现，会使代码变得复杂且难以维护。模板方法模式解决了这个问题，通过将算法的结构定义在基类中，而将可变的部分延迟到子类实现，这样既保证了算法结构的一致性，又提供了足够的灵活性来处理不同的情况。

**实现要点：**
1. **定义抽象基类：** 创建一个抽象基类，包含算法的通用部分和需要子类实现的抽象方法。
2. **实现模板方法：** 在基类中实现模板方法，定义算法的执行顺序和框架，调用具体步骤和钩子方法。
3. **子类实现具体步骤：** 创建具体子类，实现基类中定义的抽象方法，提供特定的算法实现。
4. **钩子方法：** 在基类中提供钩子方法，允许子类在特定点介入算法流程，但不强制要求实现。
5. **保护算法结构：** 使用合适的方法修饰符（如final）来保护算法的整体结构不被破坏。

**注意事项：**
- **子类不能改变算法结构：** 模板方法模式限制了子类对算法结构的修改，子类只能改变特定步骤的实现，而不能改变算法的整体流程。
- **可能导致继承层次复杂：** 过度使用模板方法模式可能导致复杂的继承层次，增加系统的复杂性。
- **违反里氏替换原则：** 如果子类的实现与父类的预期相差很大，可能违反里氏替换原则。
- **扩展困难：** 当需要添加新的算法步骤时，需要修改基类，这可能影响所有子类。
- **性能考虑：** 由于使用了多态机制，可能会有一定的性能开销。

**适用情况：**
- **算法结构固定：** 当算法的整体结构是固定的，但某些步骤的具体实现可能变化时。
- **算法步骤可变：** 当需要为同一个算法提供不同的实现，而算法的总体流程保持一致时。
- **代码复用：** 当多个类有相似的功能，可以通过提取共同的算法框架来减少代码重复时。
- **框架开发：** 在开发框架时，为用户提供可定制的算法模板，同时保证框架的核心逻辑不变。
- **流程控制：** 在需要控制执行流程的场景中，如数据处理流水线、请求处理管道等。

```cpp
class AbstractClass {
public:
    // 模板方法定义算法骨架
    void templateMethod() const {
        this->baseOperation1();
        this->requiredOperations1();
        this->baseOperation2();
        this->hook1();
        this->requiredOperation2();
        this->baseOperation3();
        this->hook2();
    }
    
protected:
    void baseOperation1() const {
        cout << "AbstractClass says: I am doing the bulk of the work\n";
    }
    
    void baseOperation2() const {
        cout << "AbstractClass says: But I let subclasses override some operations\n";
    }
    
    void baseOperation3() const {
        cout << "AbstractClass says: But I am doing the bulk of the work anyway\n";
    }
    
    // 抽象方法，子类必须实现
    virtual void requiredOperations1() const = 0;
    virtual void requiredOperation2() const = 0;
    
    // 钩子方法，子类可以选择性实现
    virtual void hook1() const {}
    virtual void hook2() const {}
};

class ConcreteClass1 : public AbstractClass {
protected:
    void requiredOperations1() const override {
        cout << "ConcreteClass1 says: Implemented Operation1\n";
    }
    
    void requiredOperation2() const override {
        cout << "ConcreteClass1 says: Implemented Operation2\n";
    }
};

class ConcreteClass2 : public AbstractClass {
protected:
    void requiredOperations1() const override {
        cout << "ConcreteClass2 says: Implemented Operation1 differently\n";
    }
    
    void requiredOperation2() const override {
        cout << "ConcreteClass2 says: Implemented Operation2 differently\n";
    }
    
    void hook1() const override {
        cout << "ConcreteClass2 says: Overridden hook1\n";
    }
};
```

**详细描述：**
模板方法模式定义一个操作中的算法骨架，而将一些步骤延迟到子类中。在这个示例中，`AbstractClass`是抽象类，它定义了`templateMethod()`作为算法的骨架，这个方法按特定顺序调用了多个其他方法。`baseOperation1()`、`baseOperation2()`和`baseOperation3()`是具体的操作，由基类实现。`requiredOperations1()`和`requiredOperation2()`是抽象方法，必须由子类实现。`hook1()`和`hook2()`是钩子方法，子类可以选择性地重写它们。`ConcreteClass1`和`ConcreteClass2`是具体子类，它们实现了抽象方法并可以根据需要重写钩子方法。这样，算法的整体结构保持不变，但具体的实现步骤可以在子类中定制。

**设计原因：**
1. **代码复用：** 算法的公共部分在基类中实现，避免重复代码
2. **算法控制：** 基类控制算法的整体结构和执行顺序
3. **灵活性：** 子类可以定制算法的特定步骤
4. **一致性：** 确保所有子类遵循相同的算法结构

### 11. 访问者模式 (Visitor Pattern)

**意图：** 表示一个作用于某对象结构中的各元素的操作，它使你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。

访问者模式的核心意图是表示一个作用于某对象结构中的各元素的操作，它使你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。这种模式将数据结构和作用于结构上的操作解耦，使得可以在不修改元素类的情况下添加新的操作。访问者模式特别适用于数据结构相对稳定的系统，因为在这种情况下，添加新的操作比添加新的元素更常见。通过访问者模式，可以将与特定操作相关的代码集中到一个访问者类中，而不是分散在各个元素类中，从而提高代码的可维护性和可扩展性。

**设计思想：** 将数据结构和作用于结构上的操作解耦，使操作可以独立变化。

访问者模式的设计思想是将数据结构和作用于结构上的操作分离，通过双分派机制实现操作与结构的解耦。元素类负责接受访问者，而访问者类负责对元素执行特定的操作。这种设计使得可以在不修改元素类的情况下添加新的操作，只需创建新的访问者类即可。设计思想强调了操作的聚集性，将与特定操作相关的所有逻辑封装在一个访问者类中，而不是分散在各个元素类中。这种设计还体现了访问者与元素之间的协作关系，元素向访问者暴露自己，访问者对元素执行操作。

**核心问题：** 如何在不修改对象结构的情况下添加新的操作？

在面向对象设计中，经常会遇到这样的问题：有一个稳定的数据结构，但是需要对这个结构中的元素执行多种不同的操作。如果将所有的操作都放在元素类中，会导致元素类变得臃肿，且每次添加新操作都需要修改元素类，违反了开闭原则。访问者模式解决了这个问题，通过引入访问者角色，将操作从元素类中分离出来，使得可以独立地添加新操作而不影响现有的元素类。

**实现要点：**
1. **定义访问者接口：** 定义访问者接口，为每种类型的元素定义一个访问方法，方法名称通常以visit开头。
2. **元素类接受访问者：** 元素类实现accept方法，该方法接受一个访问者对象，并调用访问者对该元素类型的访问方法。
3. **访问者为每种元素提供访问方法：** 具体访问者实现访问者接口，为每种元素类型提供具体的访问逻辑。
4. **双分派机制：** 通过元素的accept方法和访问者的visit方法实现双分派，确保调用正确的访问方法。
5. **对象结构：** 通常还需要一个对象结构来管理元素集合，如组合模式中的组件。

**注意事项：**
- **违反了开闭原则（添加新元素困难）：** 虽然访问者模式对添加新操作开放，但对添加新元素关闭，每添加一个新的元素类型都需要修改访问者接口和所有具体访问者类。
- **增加系统复杂度：** 访问者模式引入了额外的类层次结构，增加了系统的复杂度，特别是对于简单的操作可能显得过度设计。
- **访问权限：** 访问者需要访问元素的内部状态，这可能需要元素暴露一些本来应该是私有的数据。
- **双分派开销：** 双分派机制可能会带来一定的性能开销。
- **类型依赖：** 访问者与元素类型紧密耦合，访问者必须了解所有元素类型。

**适用情况：**
- **对复杂对象结构施加多种操作：** 当有一个复杂的数据结构（如树或图）并且需要对其中的元素执行多种不同的操作时。
- **编译器设计：** 在编译器中，抽象语法树（AST）的遍历和处理，如类型检查、代码生成等。
- **XML/HTML解析：** 解析文档对象模型（DOM）并执行不同的处理操作。
- **报表生成：** 对数据结构执行不同的统计和汇总操作。
- **数据转换：** 将一种数据结构转换为另一种表示形式。

```cpp
class ConcreteElementA;
class ConcreteElementB;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visitElementA(const ConcreteElementA* element) = 0;
    virtual void visitElementB(const ConcreteElementB* element) = 0;
};

class Element {
public:
    virtual ~Element() = default;
    virtual void accept(Visitor& visitor) = 0;
};

class ConcreteElementA : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visitElementA(this);
    }
    
    string operationA() const {
        return "A";
    }
};

class ConcreteElementB : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visitElementB(this);
    }
    
    string operationB() const {
        return "B";
    }
};

class ConcreteVisitor1 : public Visitor {
public:
    void visitElementA(const ConcreteElementA* element) override {
        cout << element->operationA() << "+ConcreteVisitor1\n";
    }
    
    void visitElementB(const ConcreteElementB* element) override {
        cout << element->operationB() << "+ConcreteVisitor1\n";
    }
};

class ConcreteVisitor2 : public Visitor {
public:
    void visitElementA(const ConcreteElementA* element) override {
        cout << element->operationA() << "+ConcreteVisitor2\n";
    }
    
    void visitElementB(const ConcreteElementB* element) override {
        cout << element->operationB() << "+ConcreteVisitor2\n";
    }
};
```

**详细描述：**
访问者模式表示一个作用于某对象结构中的各元素的操作，它使你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。在这个示例中，`Element`是抽象元素接口，定义了接受访问者的方法。`ConcreteElementA`和`ConcreteElementB`是具体元素，它们实现`accept`方法，该方法调用访问者对应的访问方法。`Visitor`是访问者接口，为每种类型的元素定义了访问方法。`ConcreteVisitor1`和`ConcreteVisitor2`是具体访问者，它们实现了对不同类型元素的操作。当元素接受访问者时，它会调用访问者对应的方法，从而实现了双分派机制。这种模式将操作与对象结构分离，使我们可以为现有对象结构添加新操作而无需修改其代码。

**设计原因：**
1. **操作与结构分离：** 将作用于结构上的操作与结构本身分离
2. **易于添加新操作：** 可以轻松添加新的访问者来定义新操作
3. **访问私有数据：** 访问者可以访问元素的私有数据
4. **一次访问多次操作：** 可以在一次遍历中执行多种操作

---

## 模式比较与选择

### 创建型模式比较

| 模式名称 | 核心意图 | 主要优势 | 潜在劣势 |
|----------|----------|----------|----------|
| **单例模式** | 确保类只有一个实例 | 全局访问点，节省资源 | 可能造成紧耦合，难以测试 |
| **工厂方法** | 定义创建接口，子类决定实例化 | 降低耦合度，支持扩展 | 增加类层次复杂度 |
| **抽象工厂** | 创建相关对象族 | 保证产品族一致性 | 扩展新产品族较为困难 |
| **建造者模式** | 分步骤构建复杂对象 | 精确控制构建过程 | 代码量较大，复杂度高 |
| **原型模式** | 通过复制创建新对象 | 避免重复初始化开销 | 深浅拷贝问题需注意 |

### 结构型模式比较

| 模式名称 | 核心意图 | 主要优势 | 潜在劣势 |
|----------|----------|----------|----------|
| **适配器模式** | 接口转换，使不兼容接口协同工作 | 复用现有类，提高兼容性 | 可能增加系统复杂度 |
| **桥接模式** | 解耦抽象与实现，使二者独立变化 | 实现灵活扩展，支持多维度变化 | 初始设计复杂度较高 |
| **组合模式** | 构建树形结构，统一处理单个对象和组合对象 | 简化客户端代码，易于扩展 | 限制了多态性 |
| **装饰器模式** | 动态添加功能，提供比继承更灵活的扩展 | 遵循开闭原则，灵活扩展 | 可能产生过多小对象 |
| **外观模式** | 简化复杂子系统的接口 | 降低耦合度，提供简单接口 | 可能成为性能瓶颈 |
| **享元模式** | 共享细粒度对象，减少内存使用 | 显著节省内存资源 | 引入共享状态管理复杂度 |
| **代理模式** | 控制对对象的访问 | 增强安全性，提供间接访问 | 可能增加请求处理时间 |

### 行为型模式比较

| 模式名称 | 核心意图 | 主要优势 | 潜在劣势 |
|----------|----------|----------|----------|
| **责任链模式** | 将请求传递给一系列处理者直到被处理 | 解耦发送者和接收者，支持动态组合 | 请求可能未被处理，性能难以预测 |
| **命令模式** | 将请求封装为对象，支持撤销重做 | 支持撤销/重做，参数化请求，排队处理 | 可能导致命令类数量激增 |
| **解释器模式** | 定义语言的文法表示并解释执行 | 易于扩展语法，遵循开闭原则 | 复杂语法导致类数量爆炸 |
| **迭代器模式** | 提供统一方式遍历聚合对象 | 统一访问接口，支持多种遍历方式 | 可能引入性能开销 |
| **中介者模式** | 封装对象间的复杂交互 | 降低对象间耦合度，简化交互管理 | 中介者可能变得过于复杂 |
| **备忘录模式** | 保存和恢复对象状态 | 支持状态回滚，封装性好 | 可能消耗大量内存资源 |
| **观察者模式** | 定义一对多依赖关系，自动通知变化 | 松耦合，支持广播通信 | 可能导致意外更新 |
| **状态模式** | 根据状态改变对象行为 | 消除条件语句，易于扩展新状态 | 状态类数量可能较多 |
| **策略模式** | 定义算法族，可互换使用 | 算法可自由切换，遵循开闭原则 | 客户端需了解不同策略差异 |
| **模板方法模式** | 定义算法骨架，子类实现特定步骤 | 代码复用，控制算法结构 | 子类对父类依赖性强 |
| **访问者模式** | 对对象结构中的元素执行新操作 | 易于添加新操作，符合单一职责 | 增加类数量，违反开闭原则 |

---

## 最佳实践

### 模式选择指南

1. **不要为了模式而模式**
   - 选择模式是因为它解决了实际问题
   - 简单问题用简单方案

2. **考虑团队熟悉度**
   - 选择团队熟悉的模式
   - 过度使用复杂模式会降低可维护性

3. **评估复杂度**
   - 模式增加了系统复杂度
   - 权衡复杂度与收益

4. **遵循SOLID原则**
   - 用模式实现SOLID原则
   - 避免违反SOLID原则的模式滥用

### 组合使用模式

常见模式组合：
- **MVC**：Observer + Command + Strategy
- **GUI框架**：Observer + Command + Composite
- **编译器**：Composite + Visitor + Iterator
- **游戏引擎**：Observer + Command + State

### 注意事项

1. **避免过度设计**
   - 不要提前引入不必要的模式
   - 重构时再考虑引入模式

2. **文档化**
   - 记录为什么选择某个模式
   - 说明模式的使用方式

3. **性能考量**
   - 某些模式可能带来性能开销
   - 在性能敏感场景谨慎使用

4. **测试**
   - 确保模式的正确实现
   - 覆盖各种使用场景

---

## 结论

设计模式是面向对象编程的重要组成部分，它们提供了经过验证的解决方案来解决常见的设计问题。通过合理使用设计模式，我们可以构建出更灵活、可维护和可扩展的软件系统。

### 关键要点

1. **理解本质**
   - 深入理解每个模式的意图和适用场景
   - 不要盲目套用，要根据实际需求选择

2. **适度使用**
   - 避免过度设计和模式滥用
   - 简单问题用简单解决方案

3. **持续学习**
   - 不断实践和反思模式的使用
   - 关注新的设计思想和模式

4. **团队协作**
   - 与团队成员分享模式知识
   - 建立共同的设计理念

设计模式不是银弹，但它们是优秀的工具。正确使用设计模式可以使代码更优雅、更易维护，但错误使用也可能使系统变得复杂难懂。关键是理解其背后的原理，并在合适的情境中应用。

掌握设计模式需要时间和实践，希望这份指南能帮助您在面向对象设计的道路上走得更远。