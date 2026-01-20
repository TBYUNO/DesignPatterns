#include "../../../src/behavioral/command/Command.h"
#include <gtest/gtest.h>

// 命令模式测试套件

// 测试开灯命令
TEST(CommandTest, LightOnCommand_Execute) {
    auto light = std::make_shared<Light>();
    auto command = std::make_shared<LightOnCommand>(light);
    
    EXPECT_NO_THROW(command->Execute());
}

// 测试关灯命令
TEST(CommandTest, LightOffCommand_Execute) {
    auto light = std::make_shared<Light>();
    auto command = std::make_shared<LightOffCommand>(light);
    
    EXPECT_NO_THROW(command->Execute());
}

// 测试开灯命令撤销
TEST(CommandTest, LightOnCommand_Undo) {
    auto light = std::make_shared<Light>();
    auto command = std::make_shared<LightOnCommand>(light);
    
    command->Execute();
    EXPECT_NO_THROW(command->Undo());
}

// 测试关灯命令撤销
TEST(CommandTest, LightOffCommand_Undo) {
    auto light = std::make_shared<Light>();
    auto command = std::make_shared<LightOffCommand>(light);
    
    command->Execute();
    EXPECT_NO_THROW(command->Undo());
}

// 测试遥控器设置命令
TEST(CommandTest, RemoteControl_SetCommand) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    
    RemoteControl remote;
    remote.SetCommand(onCommand);
    
    EXPECT_NO_THROW(remote.PressButton());
}

// 测试遥控器按钮执行
TEST(CommandTest, RemoteControl_PressButton) {
    auto light = std::make_shared<Light>();
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    RemoteControl remote;
    remote.SetCommand(offCommand);
    
    EXPECT_NO_THROW(remote.PressButton());
}

// 测试遥控器撤销按钮
TEST(CommandTest, RemoteControl_PressUndo) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    
    RemoteControl remote;
    remote.SetCommand(onCommand);
    
    remote.PressButton();
    EXPECT_NO_THROW(remote.PressUndo());
}

// 测试遥控器切换命令
TEST(CommandTest, RemoteControl_SwitchCommands) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    RemoteControl remote;
    
    remote.SetCommand(onCommand);
    EXPECT_NO_THROW(remote.PressButton());
    
    remote.SetCommand(offCommand);
    EXPECT_NO_THROW(remote.PressButton());
}

// 测试RunSimpleCommandDemo
TEST(CommandTest, RunSimpleCommandDemo) {
    EXPECT_NO_THROW(RunSimpleCommandDemo());
}

// 测试宏命令添加命令
TEST(CommandTest, MacroCommand_AddCommand) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    MacroCommand macro;
    EXPECT_NO_THROW(macro.AddCommand(onCommand));
    EXPECT_NO_THROW(macro.AddCommand(offCommand));
}

// 测试宏命令执行
TEST(CommandTest, MacroCommand_Execute) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    MacroCommand macro;
    macro.AddCommand(onCommand);
    macro.AddCommand(offCommand);
    
    EXPECT_NO_THROW(macro.Execute());
}

// 测试宏命令撤销
TEST(CommandTest, MacroCommand_Undo) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    MacroCommand macro;
    macro.AddCommand(onCommand);
    macro.AddCommand(offCommand);
    
    macro.Execute();
    EXPECT_NO_THROW(macro.Undo());
}

// 测试宏命令多次执行和撤销
TEST(CommandTest, MacroCommand_MultipleExecuteAndUndo) {
    auto light = std::make_shared<Light>();
    auto onCommand = std::make_shared<LightOnCommand>(light);
    auto offCommand = std::make_shared<LightOffCommand>(light);
    
    MacroCommand macro;
    macro.AddCommand(onCommand);
    macro.AddCommand(offCommand);
    
    EXPECT_NO_THROW(macro.Execute());
    EXPECT_NO_THROW(macro.Undo());
    EXPECT_NO_THROW(macro.Execute());
    EXPECT_NO_THROW(macro.Undo());
}

// 测试RunMacroCommandDemo
TEST(CommandTest, RunMacroCommandDemo) {
    EXPECT_NO_THROW(RunMacroCommandDemo());
}

// 测试空宏命令
TEST(CommandTest, MacroCommand_Empty) {
    MacroCommand macro;
    EXPECT_NO_THROW(macro.Execute());
    EXPECT_NO_THROW(macro.Undo());
}

// 测试Light独立使用
TEST(CommandTest, Light_DirectUsage) {
    Light light;
    EXPECT_NO_THROW(light.On());
    EXPECT_NO_THROW(light.Off());
}
