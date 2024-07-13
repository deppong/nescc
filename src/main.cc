#include "nes.hh"

int main(int argc, char** argv) {
    // stupid?
    Nes nes = Nes();
    // nes.load_from_file(argv[1]);

    nes.update();
    return 0;
}