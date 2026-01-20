#!/bin/bash
# 构建脚本 - 编译所有示例和测试

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  C++ 设计模式项目构建脚本${NC}"
echo -e "${GREEN}========================================${NC}"

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# 创建构建目录
BUILD_DIR="$PROJECT_ROOT/build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}清理旧的构建目录...${NC}"
    rm -rf "$BUILD_DIR"
fi

echo -e "${GREEN}创建构建目录...${NC}"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 运行CMake配置
echo -e "${GREEN}运行 CMake 配置...${NC}"
cmake .. || {
    echo -e "${RED}CMake 配置失败！${NC}"
    exit 1
}

# 编译项目
echo -e "${GREEN}编译项目...${NC}"
NPROC=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
make -j${NPROC} || {
    echo -e "${RED}编译失败！${NC}"
    exit 1
}

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  构建成功！${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo -e "示例程序位于: ${YELLOW}$BUILD_DIR${NC}"
echo -e "运行测试请执行: ${YELLOW}cd $BUILD_DIR && ctest${NC}"
echo -e "或运行: ${YELLOW}$SCRIPT_DIR/run_tests.sh${NC}"