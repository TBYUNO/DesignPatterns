#include "../../../src/structural/facade/Facade.h"
#include <gtest/gtest.h>

// 外观模式测试套件

// 测试计算机外观 - 启动
TEST(FacadeTest, ComputerFacade_Start) {
    ComputerFacade computer;
    EXPECT_NO_THROW(computer.Start());
}

// 测试计算机外观 - 关机
TEST(FacadeTest, ComputerFacade_Shutdown) {
    ComputerFacade computer;
    EXPECT_NO_THROW(computer.Shutdown());
}

// 测试计算机外观 - 完整流程
TEST(FacadeTest, ComputerFacade_FullCycle) {
    ComputerFacade computer;
    EXPECT_NO_THROW(computer.Start());
    EXPECT_NO_THROW(computer.Shutdown());
}

// 测试计算机外观 - 多次启动关机
TEST(FacadeTest, ComputerFacade_MultipleCycles) {
    ComputerFacade computer;
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_NO_THROW(computer.Start());
        EXPECT_NO_THROW(computer.Shutdown());
    }
}

// 测试家庭影院外观 - 观影
TEST(FacadeTest, HomeTheaterFacade_WatchMovie) {
    HomeTheaterFacade theater;
    EXPECT_NO_THROW(theater.WatchMovie("The Matrix"));
}

// 测试家庭影院外观 - 结束观影
TEST(FacadeTest, HomeTheaterFacade_EndMovie) {
    HomeTheaterFacade theater;
    EXPECT_NO_THROW(theater.EndMovie());
}

// 测试家庭影院外观 - 完整流程
TEST(FacadeTest, HomeTheaterFacade_FullCycle) {
    HomeTheaterFacade theater;
    EXPECT_NO_THROW(theater.WatchMovie("Inception"));
    EXPECT_NO_THROW(theater.EndMovie());
}

// 测试家庭影院外观 - 多部电影
TEST(FacadeTest, HomeTheaterFacade_MultipleMovies) {
    HomeTheaterFacade theater;
    
    std::vector<std::string> movies = {
        "Movie 1", "Movie 2", "Movie 3"
    };
    
    for (const auto& movie : movies) {
        EXPECT_NO_THROW(theater.WatchMovie(movie));
        EXPECT_NO_THROW(theater.EndMovie());
    }
}

// 测试RunComputerFacadeDemo
TEST(FacadeTest, RunComputerFacadeDemo) {
    EXPECT_NO_THROW(RunComputerFacadeDemo());
}

// 测试RunHomeTheaterFacadeDemo
TEST(FacadeTest, RunHomeTheaterFacadeDemo) {
    EXPECT_NO_THROW(RunHomeTheaterFacadeDemo());
}

// 测试CPU子系统
TEST(FacadeTest, CPU_IndependentUsage) {
    CPU cpu;
    EXPECT_NO_THROW(cpu.PowerOn());
    EXPECT_NO_THROW(cpu.Execute());
    EXPECT_NO_THROW(cpu.PowerOff());
}

// 测试Memory子系统
TEST(FacadeTest, Memory_IndependentUsage) {
    Memory memory;
    EXPECT_NO_THROW(memory.Load());
    EXPECT_NO_THROW(memory.Release());
}

// 测试Disk子系统
TEST(FacadeTest, Disk_IndependentUsage) {
    Disk disk;
    EXPECT_NO_THROW(disk.Read());
    EXPECT_NO_THROW(disk.Stop());
}

// 测试Amplifier子系统
TEST(FacadeTest, Amplifier_IndependentUsage) {
    Amplifier amp;
    EXPECT_NO_THROW(amp.On());
    EXPECT_NO_THROW(amp.SetVolume(15));
    EXPECT_NO_THROW(amp.Off());
}

// 测试DvdPlayer子系统
TEST(FacadeTest, DvdPlayer_IndependentUsage) {
    DvdPlayer dvd;
    EXPECT_NO_THROW(dvd.On());
    EXPECT_NO_THROW(dvd.Play("Test Movie"));
    EXPECT_NO_THROW(dvd.Stop());
    EXPECT_NO_THROW(dvd.Off());
}

// 测试Projector子系统
TEST(FacadeTest, Projector_IndependentUsage) {
    Projector projector;
    EXPECT_NO_THROW(projector.On());
    EXPECT_NO_THROW(projector.Off());
}

// 测试TheaterLights子系统
TEST(FacadeTest, TheaterLights_IndependentUsage) {
    TheaterLights lights;
    EXPECT_NO_THROW(lights.Dim());
    EXPECT_NO_THROW(lights.On());
}
