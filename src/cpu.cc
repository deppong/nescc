#include "cpu.hh"
#include <cstdint>

// https://www.masswerk.at/6502/6502_instruction_set.html

Cpu::Cpu():
    X(0)
 {}
Cpu::~Cpu() {}

uint8_t Cpu::read(uint16_t addr) {
    return (addr <= 0xffff) ? m_ram[addr] : 0x0000;
}

void Cpu::write(uint16_t addr, uint8_t byte) {
    m_ram[addr] = (addr <= 0xffff) ? byte : m_ram[addr];
}

void Cpu::set_addr_mode(opcode o) {
    // even though we could use function pointers, and even though
    // this doesn't really need to be super performant, I feel
    // like we can just use a switch statement here as it saves 
    // on space, memory, and cpu cycles...

    // just a little macro to grab the next two bytes and put them
    // in little endian OPCODE LL HH -> 
    #define LLHH(x) x | x<<8
    switch(o.mode) {
        case acc: m_addr = A; break;
        case imp: m_addr = 0; break;
        case imm: m_addr = ROM[++PC]; break;
        case zpg: m_addr = read(ROM[++PC]); break;
        case zpx: m_addr = read(X + ROM[++PC]);break;
        case zpy: m_addr = read(Y + ROM[++PC]);break;
        case rel: m_addr = read(ROM[++PC] + PC+1);break;
        case abs: m_addr = read(LLHH(ROM[++PC])); break;
        case abx: m_addr = read((LLHH(ROM[++PC])) + X); break;
        case aby: m_addr = read((LLHH(ROM[++PC])) + Y); break;
        case ind: 
            {uint16_t a = LLHH(ROM[++PC]);
            m_addr = read(a)|read(a+1)<<8;}
            break;
        case inx: 
            {uint16_t a = ROM[++PC] + X;
            m_addr = read(read(a)|read(a+1)<<8);}
            break;
        case iny:
            {uint16_t a = ROM[++PC] + Y;
            m_addr = read(read(a)|read(a+1)<<8);}
            break;
        case ill: m_addr = 0;break;
        default: m_addr = 0; break;
    }
}

void Cpu::fetch_op(uint8_t opcode) {
}

void Cpu::tick() {
    for (int j = 0; j < 16; ++j) {
    for (int i = 0; i < 16; ++i) {
        printf("%d ", m_lookup[i + j*16].cycles);
    }
    std::cout<<std::endl;
    }
    (this->*m_lookup[0xE8].op)();
    (this->*m_lookup[0xE8].op)();
    (this->*m_lookup[0xE8].op)();
    (this->*m_lookup[0xE8].op)();
    printf("sizeof mode: %d\n", m_lookup[0xE8].mode);
    printf("%d\n", X);
}

// ---------------------------
// CPU Instructions
void Cpu::LDA(){

};

void Cpu::LDX(){

};

void Cpu::LDY(){

};

void Cpu::STA(){

};

void Cpu::STX(){

};

void Cpu::STY(){

};


void Cpu::TAX(){

};

void Cpu::TAY(){

};

void Cpu::TXA(){

};

void Cpu::TYA(){

};


void Cpu::TSX(){

};

void Cpu::TXS(){

};

void Cpu::PHA(){

};

void Cpu::PHP(){

};

void Cpu::PLA(){

};

void Cpu::PLP(){

};


void Cpu::AND(){
    A &= m_addr;
    if (A == 0) flags[status::Zero] = 1;
    if (A & 0b10000000) flags[status::Negative] = 1;
};

void Cpu::EOR(){
    A ^= m_addr;
    if (A == 0) flags[status::Zero] = 1;
    if (A & 0b10000000) flags[status::Negative] = 1;
};

void Cpu::ORA(){
    A |= m_addr;
    if (A == 0) flags[status::Zero] = 1;
    if (A & 0b10000000) flags[status::Negative] = 1;
};

void Cpu::BIT(){

};


void Cpu::ADC(){

};

void Cpu::SBC(){

};

void Cpu::CMP(){

};

void Cpu::CPX(){

};

void Cpu::CPY(){

};


void Cpu::INC(){

};

void Cpu::INX(){
    X++;
    flags[status::Zero] = (X==0);
};

void Cpu::INY(){

};

void Cpu::DEC(){

};

void Cpu::DEX(){

};

void Cpu::DEY(){

};


void Cpu::ASL(){
    flags[status::Carry] = A & 0b10000000;
    A = A << m_addr;
    if (A == 0) flags |= status::Zero;
    if (A & status::Negative) flags|= status::Negative;
};

void Cpu::LSR(){

};

void Cpu::ROL(){

};

void Cpu::ROR(){

};


void Cpu::JMP(){

};

void Cpu::JSR(){

};

void Cpu::RTS(){

};


void Cpu::BCC(){

};

void Cpu::BCS(){

};

void Cpu::BEQ(){

};

void Cpu::BMI(){

};

void Cpu::BNE(){

};

void Cpu::BPL(){

};

void Cpu::BVC(){

};

void Cpu::BVS(){

};


void Cpu::CLC(){

};

void Cpu::CLD(){

};

void Cpu::CLI(){

};

void Cpu::CLV(){

};

void Cpu::SEC(){

};

void Cpu::SED(){

};

void Cpu::SEI(){

};


void Cpu::BRK(){

};

void Cpu::NOP(){

};

void Cpu::RTI(){

};
