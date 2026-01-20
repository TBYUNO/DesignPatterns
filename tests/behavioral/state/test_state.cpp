#include "../../../src/behavioral/state/State.h"
#include <gtest/gtest.h>

// 状态模式测试套件

// 测试开关 - 初始状态OFF
TEST(StateTest, Switch_InitialOffState) {
    Switch sw(std::make_shared<OffState>());
    EXPECT_NO_THROW(sw.Press());
}

// 测试开关 - 初始状态ON
TEST(StateTest, Switch_InitialOnState) {
    Switch sw(std::make_shared<OnState>());
    EXPECT_NO_THROW(sw.Press());
}

// 测试开关 - 状态切换OFF->ON
TEST(StateTest, Switch_OffToOn) {
    Switch sw(std::make_shared<OffState>());
    EXPECT_NO_THROW(sw.Press()); // OFF -> ON
}

// 测试开关 - 状态切换ON->OFF
TEST(StateTest, Switch_OnToOff) {
    Switch sw(std::make_shared<OnState>());
    EXPECT_NO_THROW(sw.Press()); // ON -> OFF
}

// 测试开关 - 多次切换
TEST(StateTest, Switch_MultipleSwitches) {
    Switch sw(std::make_shared<OffState>());
    
    EXPECT_NO_THROW(sw.Press()); // OFF -> ON
    EXPECT_NO_THROW(sw.Press()); // ON -> OFF
    EXPECT_NO_THROW(sw.Press()); // OFF -> ON
    EXPECT_NO_THROW(sw.Press()); // ON -> OFF
}

// 测试RunSimpleStateDemo
TEST(StateTest, RunSimpleStateDemo) {
    EXPECT_NO_THROW(RunSimpleStateDemo());
}

// 测试文本编辑器 - 插入模式
TEST(StateTest, TextEditor_InsertMode) {
    TextEditor editor(std::make_shared<InsertMode>());
    
    editor.InsertChar('A');
    editor.InsertChar('B');
    editor.InsertChar('C');
    
    EXPECT_EQ(editor.GetText(), "ABC");
}

// 测试文本编辑器 - 覆盖模式
TEST(StateTest, TextEditor_OverwriteMode) {
    TextEditor editor(std::make_shared<InsertMode>());
    editor.InsertChar('A');
    editor.InsertChar('B');
    editor.InsertChar('C');
    
    editor.SetMode(std::make_shared<OverwriteMode>());
    editor.MoveCursor(1);
    editor.InsertChar('X');
    
    EXPECT_EQ(editor.GetText(), "AXC");
}

// 测试文本编辑器 - 模式切换
TEST(StateTest, TextEditor_SwitchMode) {
    TextEditor editor(std::make_shared<InsertMode>());
    
    editor.InsertChar('H');
    editor.InsertChar('I');
    
    editor.SetMode(std::make_shared<OverwriteMode>());
    editor.MoveCursor(0);
    editor.InsertChar('X');
    
    EXPECT_EQ(editor.GetText(), "XI");
}

// 测试文本编辑器 - 移动光标
TEST(StateTest, TextEditor_MoveCursor) {
    TextEditor editor(std::make_shared<InsertMode>());
    
    editor.InsertChar('A');
    editor.InsertChar('B');
    editor.MoveCursor(0);
    editor.InsertChar('X');
    
    EXPECT_EQ(editor.GetText(), "XAB");
}

// 测试文本编辑器 - Print方法
TEST(StateTest, TextEditor_Print) {
    TextEditor editor(std::make_shared<InsertMode>());
    editor.InsertChar('T');
    editor.InsertChar('E');
    editor.InsertChar('S');
    editor.InsertChar('T');
    
    EXPECT_NO_THROW(editor.Print());
}

// 测试RunTextEditorStateDemo
TEST(StateTest, RunTextEditorStateDemo) {
    EXPECT_NO_THROW(RunTextEditorStateDemo());
}

// 测试文本编辑器 - 光标边界处理
TEST(StateTest, TextEditor_CursorBoundary) {
    TextEditor editor(std::make_shared<InsertMode>());
    
    editor.MoveCursor(-10); // 负数
    EXPECT_EQ(editor.GetCursor(), 0);
    
    editor.InsertChar('A');
    editor.MoveCursor(100); // 超出范围
    EXPECT_EQ(editor.GetCursor(), 1);
}

// 测试覆盖模式在末尾插入
TEST(StateTest, OverwriteMode_InsertAtEnd) {
    TextEditor editor(std::make_shared<OverwriteMode>());
    
    editor.InsertChar('A');
    editor.InsertChar('B');
    
    EXPECT_EQ(editor.GetText(), "AB");
}
