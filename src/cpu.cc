#include "cpu.hh"
#include <cstdint>

uint8_t Cpu::read(uint16_t addr) {
    return (addr <= 0xffff) ? m_ram[addr] : 0x0000;
}

void Cpu::write(uint16_t addr, uint8_t byte) {
    m_ram[addr] = (addr <= 0xffff) ? byte : m_ram[addr];
}

void Cpu::fetch_op(uint8_t opcode) {
    // uhm uh
}

// ---------------------------
/* CPU Instructions
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
    if(X==0) flags[status::Zero] = 1;
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
*/
