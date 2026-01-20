#!/bin/bash
# 测试脚本 - 运行所有单元测试

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  运行所有设计模式测试${NC}"
echo -e "${BLUE}========================================${NC}"

# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# 检查构建目录是否存在
if [ ! -d "$PROJECT_ROOT/build" ]; then
    echo -e "${YELLOW}构建目录不存在，先运行构建脚本...${NC}"
    "$SCRIPT_DIR/build.sh"
fi

cd "$PROJECT_ROOT/build"

# 运行所有测试
echo -e "${GREEN}执行测试...${NC}"
ctest --output-on-failure --verbose || {
    echo -e "${RED}部分测试失败！${NC}"
    exit 1
}

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  所有测试通过！${NC}"
echo -e "${GREEN}========================================${NC}"

# 生成测试报告摘要
echo ""
echo -e "${BLUE}测试统计：${NC}"
ctest -N | grep -i "total"