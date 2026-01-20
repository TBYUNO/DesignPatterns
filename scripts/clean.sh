#!/bin/bash
# 清理脚本 - 清理所有构建产物

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "清理构建产物..."

# 删除构建目录
if [ -d "$PROJECT_ROOT/build" ]; then
    echo "删除 $PROJECT_ROOT/build/ 目录"
    rm -rf "$PROJECT_ROOT/build"
fi

if [ -d "$PROJECT_ROOT/cmake-build-debug" ]; then
    echo "删除 $PROJECT_ROOT/cmake-build-debug/ 目录"
    rm -rf "$PROJECT_ROOT/cmake-build-debug"
fi

if [ -d "$PROJECT_ROOT/cmake-build-release" ]; then
    echo "删除 $PROJECT_ROOT/cmake-build-release/ 目录"
    rm -rf "$PROJECT_ROOT/cmake-build-release"
fi

# 删除CMake缓存文件
if [ -f "$PROJECT_ROOT/compile_commands.json" ]; then
    echo "删除 $PROJECT_ROOT/compile_commands.json"
    rm -f "$PROJECT_ROOT/compile_commands.json"
fi

# 删除CMake生成的临时文件
if [ -d "$PROJECT_ROOT/_deps" ]; then
    echo "删除 $PROJECT_ROOT/_deps/ 目录"
    rm -rf "$PROJECT_ROOT/_deps"
fi

echo "清理完成！"