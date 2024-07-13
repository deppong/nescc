#include "nes.hh"

int main(int argc, char** argv) {
    // stupid?
    Nes nes = Nes();
    nes.load_from_file(argv[1]);

    for (int i = 0; i < 50; i++ ) {
        nes.update();
    }
    return 0;
}