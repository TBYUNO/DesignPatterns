# cmake file content
cmake_minimum_required(VERSION 3.10)
project(CppDesignPatterns CXX)

# C++标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 导出编译命令供IDE使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 启用测试
enable_testing()

# 尝试使用 FetchContent 下载 GoogleTest，如果失败则寻找系统安装的版本
include(FetchContent)

# 设置一个选项让用户选择是否使用系统安装的 GoogleTest
option(USE_SYSTEM_GTEST "Use system installed GoogleTest instead of downloading" OFF)

if(USE_SYSTEM_GTEST)
    find_package(GTest REQUIRED)
    message(STATUS "Using system installed GoogleTest")
else()
    # 下载并配置 GoogleTest
    message(STATUS "Downloading GoogleTest...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # 对于 Windows: 防止覆盖父项目的编译器/链接器设置
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    message(STATUS "GoogleTest downloaded and configured")
endif()

# 包含 GoogleTest 的模块
include(GoogleTest)

# ===============================
# 辅助函数：添加示例程序
# ===============================
function(add_pattern_example pattern_name pattern_path)
    add_executable(${pattern_name}_example ${pattern_path}/main.cpp)
endfunction()

# ===============================
# 辅助函数：添加测试程序
# ===============================
function(add_pattern_test pattern_name pattern_path)
    add_executable(${pattern_name}_test ${pattern_path}/test_${pattern_name}.cpp)
    if(USE_SYSTEM_GTEST)
        target_link_libraries(${pattern_name}_test GTest::gtest_main)
    else()
        target_link_libraries(${pattern_name}_test GTest::gtest_main)
    endif()
    gtest_discover_tests(${pattern_name}_test)
endfunction()

# ===============================
# 创建型模式 (Creational Patterns)
# ===============================
add_pattern_example(singleton src/creational/singleton)
add_pattern_example(factory_method src/creational/factory_method)
add_pattern_example(abstract_factory src/creational/abstract_factory)
add_pattern_example(builder src/creational/builder)
add_pattern_example(prototype src/creational/prototype)

add_pattern_test(singleton tests/creational/singleton)
add_pattern_test(factory_method tests/creational/factory_method)
add_pattern_test(abstract_factory tests/creational/abstract_factory)
add_pattern_test(builder tests/creational/builder)
add_pattern_test(prototype tests/creational/prototype)

# ===============================
# 结构型模式 (Structural Patterns)
# ===============================
add_pattern_example(adapter src/structural/adapter)
add_pattern_example(bridge src/structural/bridge)
add_pattern_example(composite src/structural/composite)
add_pattern_example(decorator src/structural/decorator)
add_pattern_example(facade src/structural/facade)
add_pattern_example(flyweight src/structural/flyweight)
add_pattern_example(proxy src/structural/proxy)

add_pattern_test(adapter tests/structural/adapter)
add_pattern_test(bridge tests/structural/bridge)
add_pattern_test(composite tests/structural/composite)
add_pattern_test(decorator tests/structural/decorator)
add_pattern_test(facade tests/structural/facade)
add_pattern_test(flyweight tests/structural/flyweight)
add_pattern_test(proxy tests/structural/proxy)

# ===============================
# 行为型模式 (Behavioral Patterns)
# ===============================
add_pattern_example(chain_of_responsibility src/behavioral/chain_of_responsibility)
add_pattern_example(command src/behavioral/command)
add_pattern_example(interpreter src/behavioral/interpreter)
add_pattern_example(iterator src/behavioral/iterator)
add_pattern_example(mediator src/behavioral/mediator)
add_pattern_example(memento src/behavioral/memento)
add_pattern_example(observer src/behavioral/observer)
add_pattern_example(state src/behavioral/state)
add_pattern_example(strategy src/behavioral/strategy)
add_pattern_example(template_method src/behavioral/template_method)
add_pattern_example(visitor src/behavioral/visitor)

add_pattern_test(chain_of_responsibility tests/behavioral/chain_of_responsibility)
add_pattern_test(command tests/behavioral/command)
add_pattern_test(interpreter tests/behavioral/interpreter)
add_pattern_test(iterator tests/behavioral/iterator)
add_pattern_test(mediator tests/behavioral/mediator)
add_pattern_test(memento tests/behavioral/memento)
add_pattern_test(observer tests/behavioral/observer)
add_pattern_test(state tests/behavioral/state)
add_pattern_test(strategy tests/behavioral/strategy)
add_pattern_test(template_method tests/behavioral/template_method)
add_pattern_test(visitor tests/behavioral/visitor)