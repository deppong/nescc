#pragma once
#include "cpu.hh"
#include <string>

class Nes {

public:
    int load_from_file(std::string);
    const void update() {
        cpu.tick();
    };

private:
    Cpu cpu;
    // Ppu ppu;
};