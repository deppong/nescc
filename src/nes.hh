#pragma once
#include "cpu.hh"
#include <cstdio>
#include <string>
#include <stdint.h>

class Nes {

public:
    Nes() {cpu.reset();};
    const int load_from_file(std::string s) {
        FILE *fp = fopen(s.c_str(), "r");
        if (!fp) {
            fprintf(stderr, "failed to open file %s\n", s.c_str());
            return 1;
        }
        for (int i = 0; i < sizeof(cpu.ROM); ++i) {
            cpu.ROM[i] = fgetc(fp);
            if (feof(fp))
                break;
        }
        return 0;
    };
    const void update() {
        cpu.tick();
        printf("X:%d Y:%d A:%d \nSP:%#X PC:%#X CYCLES: %d\n", cpu.X, cpu.Y, cpu.A, cpu.SP, cpu.PC, cpu.cycles);
    };

private:
    Cpu cpu;
    // Ppu ppu;
};