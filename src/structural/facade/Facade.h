#pragma once

#include <iostream>
#include <string>

// 外观模式（Facade）C++ 示例
// ---------------------------
// 示例 1：计算机启动/关机流程
// 示例 2：家庭影院一键观影

// ===== 示例 1：计算机子系统 =====

class CPU {
public:
    void PowerOn() { std::cout << "CPU: power on" << std::endl; }
    void Execute() { std::cout << "CPU: execute instructions" << std::endl; }
    void PowerOff() { std::cout << "CPU: power off" << std::endl; }
};

class Memory {
public:
    void Load() { std::cout << "Memory: load data" << std::endl; }
    void Release() { std::cout << "Memory: release data" << std::endl; }
};

class Disk {
public:
    void Read() { std::cout << "Disk: read boot sector" << std::endl; }
    void Stop() { std::cout << "Disk: stop spinning" << std::endl; }
};

// 外观：对外暴露简单的 Start / Shutdown 接口
class ComputerFacade {
public:
    void Start() {
        std::cout << "\n[ComputerFacade] Start computer" << std::endl;
        cpu_.PowerOn();
        disk_.Read();
        memory_.Load();
        cpu_.Execute();
    }

    void Shutdown() {
        std::cout << "\n[ComputerFacade] Shutdown computer" << std::endl;
        memory_.Release();
        disk_.Stop();
        cpu_.PowerOff();
    }

private:
    CPU cpu_;
    Memory memory_;
    Disk disk_;
};

// ===== 示例 2：家庭影院子系统 =====

class Amplifier {
public:
    void On() { std::cout << "Amplifier: on" << std::endl; }
    void Off() { std::cout << "Amplifier: off" << std::endl; }
    void SetVolume(int v) { std::cout << "Amplifier: set volume to " << v << std::endl; }
};

class DvdPlayer {
public:
    void On() { std::cout << "DvdPlayer: on" << std::endl; }
    void Off() { std::cout << "DvdPlayer: off" << std::endl; }
    void Play(const std::string& movie) { std::cout << "DvdPlayer: play movie '" << movie << "'" << std::endl; }
    void Stop() { std::cout << "DvdPlayer: stop" << std::endl; }
};

class Projector {
public:
    void On() { std::cout << "Projector: on" << std::endl; }
    void Off() { std::cout << "Projector: off" << std::endl; }
};

class TheaterLights {
public:
    void Dim() { std::cout << "TheaterLights: dim lights" << std::endl; }
    void On() { std::cout << "TheaterLights: lights on" << std::endl; }
};

// 家庭影院外观
class HomeTheaterFacade {
public:
    void WatchMovie(const std::string& movie) {
        std::cout << "\n[HomeTheaterFacade] Get ready to watch a movie" << std::endl;
        lights_.Dim();
        projector_.On();
        amp_.On();
        amp_.SetVolume(20);
        dvd_.On();
        dvd_.Play(movie);
    }

    void EndMovie() {
        std::cout << "\n[HomeTheaterFacade] Shutting movie theater down" << std::endl;
        dvd_.Stop();
        dvd_.Off();
        amp_.Off();
        projector_.Off();
        lights_.On();
    }

private:
    Amplifier amp_;
    DvdPlayer dvd_;
    Projector projector_;
    TheaterLights lights_;
};

// 演示：计算机外观
inline void RunComputerFacadeDemo() {
    ComputerFacade computer;
    computer.Start();
    computer.Shutdown();
}

// 演示：家庭影院外观
inline void RunHomeTheaterFacadeDemo() {
    HomeTheaterFacade theater;
    theater.WatchMovie("Design Patterns: The Movie");
    theater.EndMovie();
}
