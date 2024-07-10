#include "cpu.hh"

int main(void) {
    auto cpu = Cpu();

    cpu.tick();
    return 0;
}