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
    // TODO: is m_data = read(m_addr); necessary?
    // (likely)
    #define LLHH(x) x | x<<8
    switch(o.mode) {
        case acc: m_data = A; break;
        case imp: m_data = 0; break;
        case imm:
            m_data = ROM[PC++];
            break;
        case zpg:
            m_addr = ROM[PC++];
            break;
        case zpx:
            m_addr = X + ROM[PC++];
            m_data = read(m_addr);
            break;
        case zpy:
            m_addr = Y + ROM[PC++];
            m_data = read(m_addr);
            break;
        case rel:
            m_addr = ROM[PC++] + PC;
            break;
        case abs:
            m_addr = LLHH(ROM[PC++]);
            m_data = read(m_addr);
            break;
        case abx:
            m_addr = (LLHH(ROM[PC++])) + X;
            m_data = read(m_addr);
            break;
        case aby:
            m_addr = (LLHH(ROM[PC++])) + Y;
            m_data = read(m_addr);
            break;
        case ind: 
            {uint16_t a = LLHH(ROM[PC++]);
            m_addr = read(a)|read(a+1)<<8;}
            break;
        case inx: 
            {uint16_t a = ROM[PC++] + X;
            m_addr = read(a)|read(a+1)<<8;
            m_data = read(m_addr);}
            break;
        case iny:
            {uint16_t a = ROM[PC++];
            m_addr = (read(a)|read(a+1)<<8) + Y;
            m_data = read(m_addr); }
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
        printf("%x %d ", i+j*16, m_lookup[i + j*16].cycles);
    }
    std::cout<<std::endl;
    }
    (this->*m_lookup[0xE8].op)();
    printf("sizeof mode: %d\n", m_lookup[0xE8].mode);
    printf("%d\n", X);
}

// ---------------------------
// CPU Instructions
void Cpu::LDA(){
    A = m_data;
    flags.set(Zero,A == 0);
    flags.set(Negative,A & 0x80);
};

void Cpu::LDX(){
    X = m_data;
    flags.set(Zero,X == 0);
    flags.set(Negative,X & 0x80);
};

void Cpu::LDY(){
    Y = m_data;
    flags.set(Zero,Y == 0);
    flags.set(Negative,Y & 0x80);
};

void Cpu::STA(){
    write(m_addr, A);
};

void Cpu::STX(){
    write(m_addr, X);
};

void Cpu::STY(){
    write(m_addr, Y);
};


void Cpu::TAX(){
    X = A;
    flags[Zero] = (X == 0);
    if (X & 0b10000000) flags[Negative] = 1;
    flags.set(Zero,Y == 0);
    flags.set(Negative,Y & 0x80);
};

void Cpu::TAY(){
    Y = A;
    flags.set(Zero,Y == 0);
    flags.set(Negative,Y & 0x80);

};

void Cpu::TXA(){
    A = X;
    flags.set(Zero,A == 0);
    flags.set(Negative,A & 0x80);
};

void Cpu::TYA(){
    A = Y;
    flags.set(Zero,A == 0);
    flags.set(Negative,A & 0x80);
};


void Cpu::TSX(){
    X = SP;
    flags.set(Zero,X == 0);
    flags.set(Negative,X & 0x80);
};

void Cpu::TXS(){
    SP = X;
    flags.set(Zero,SP == 0);
    flags.set(Negative,SP & 0x80);
};

void Cpu::PHA(){
    write(0x0100 + SP--, A);
};

void Cpu::PHP(){
    write(0x0100 + SP--, flags.to_ulong());
};

void Cpu::PLA(){
    A = read(0x0100 + SP++);
    flags.set(Zero,A == 0);
    flags.set(Negative,A & 0x80);
};

void Cpu::PLP(){
    flags = read(0x0100 + SP++);
};

void Cpu::AND(){
    A &= m_data;
    flags.set(Zero,A == 0);
    flags.set(Negative,A & 0x80);
};

void Cpu::EOR(){
    A ^= m_data;
    flags.set(Zero,A == 0);
    flags.set(Negative, A & 0x80);
};

void Cpu::ORA(){
    A |= m_data;
    flags.set(Zero,(A == 0));
    flags.set(Negative, (A & 0x80 )> 1);
};

void Cpu::BIT(){
    flags[Zero] = m_data & A;
    flags[Overflow] = (m_data | Overflow);
    flags[Negative] = (m_data | Negative);
};

// The NES Ricoh 2A03 Lacks decimal mode, so we ignore it. 
// https://stackoverflow.com/questions/29193303/6502-emulation-proper-way-to-implement-adc-and-sbc
void Cpu::ADC(){
    unsigned const sum = A + m_data + flags[Carry];
    flags.set(Carry, sum>0xFF);
    flags[Overflow] = ~(A ^ m_data) & (A ^ sum) & 0x80;
    A = sum;
    if (A == 0) flags.set(Zero);
    if (A & 0b10000000) flags.set(Negative);
};

// 2's complement makes this implementation simple
void Cpu::SBC(){
    m_data = ~m_data;
    Cpu::ADC();
};

void Cpu::CMP(){
    flags.set(Carry, A>=m_data);
    flags.set(Zero, A==m_data);
    flags.set(Negative, A & 0x80);
};

void Cpu::CPX(){
    flags.set(Carry, X>=m_data);
    flags.set(Zero, X==m_data);
    flags.set(Negative, X & 0x80);
};

void Cpu::CPY(){
    flags.set(Carry, Y>=m_data);
    flags.set(Zero, Y==m_data);
    flags.set(Negative, Y & 0x80);
};

void Cpu::INC(){
    uint8_t t = m_data + 1;
    write(m_addr, t);
    flags.set(Zero,t==0);
    flags.set(Negative, t & 0x80);
};

void Cpu::INX(){
    X++;
    flags.set(Zero,X==0);
    flags.set(Negative, X & 0x80);
};

void Cpu::INY(){
    Y++;
    flags.set(Zero,Y==0);
    flags.set(Negative, Y & 0x80);
};

void Cpu::DEC(){
    uint8_t t = m_data - 1;
    write(m_addr, t);
    flags.set(Zero,t==0);
    flags.set(Negative, t & 0x80);
};

void Cpu::DEX(){
    X--;
    flags.set(Zero,X==0);
    flags.set(Negative, X & 0x80);
};

void Cpu::DEY(){
    Y--;
    flags.set(Zero,Y==0);
    flags.set(Negative, Y & 0x80);
};

void Cpu::ASL(){
    flags.set(Carry, m_data & 0x80);
    m_data <<= 1;
    flags.set(Zero,A==0);
    flags.set(Negative, m_data & 0x80);
    if (m_lookup[current_op].mode != acc || m_lookup[current_op].mode != imp) 
        write(m_addr, m_data);
    else 
        A = m_data;
};

void Cpu::LSR(){
    flags.set(Carry, m_data & 0x00);
    m_data >>= 1;
    flags.set(Zero,A==0);
    flags.set(Negative, m_data & 0x80);
    if (m_lookup[current_op].mode != acc || m_lookup[current_op].mode != imp) 
        write(m_addr, m_data);
    else 
        A = m_data;
};

void Cpu::ROL(){
    flags.set(Carry, m_data & 0x80);
    m_data <<= 1;
    m_data |= flags[Carry];
    flags.set(Zero,A==0);
    flags.set(Negative, m_data & 0x80);
    if (m_lookup[current_op].mode != acc || m_lookup[current_op].mode != imp) 
        write(m_addr, m_data);
    else 
        A = m_data;
};

void Cpu::ROR(){
    flags.set(Carry, m_data & 1);
    m_data >>= 1;
    m_data |= flags[Carry]<<7;
    flags.set(Zero,A==0);
    flags.set(Negative, m_data & 0x80);
    if (m_lookup[current_op].mode != acc || m_lookup[current_op].mode != imp) 
        write(m_addr, m_data);
    else 
        A = m_data;

};

void Cpu::JMP(){
    PC = m_data;
};

void Cpu::JSR(){
    PC--;
    write(0x0100 + SP, (PC >> 8) & 0x00FF);
    SP--;
    write(0x0100 + SP, PC & 0x00FF);
    SP--;
};

void Cpu::RTS(){
    SP++;
    PC = read(0x0100 + SP);
    SP++;
    PC |= read(0x0100 + SP) >> 8;
    PC++;
};

void Cpu::BCC(){
    if (!flags[Carry]) {
        PC = m_data;
    }
};

void Cpu::BCS(){
    if (flags[Carry]) {
        PC = m_data;
    }
};

void Cpu::BEQ(){
    if (flags[Zero]) {
        PC = m_data;
    }

};

void Cpu::BMI(){
    if (flags[Negative]) {
        PC = m_data;
    }
};

void Cpu::BNE(){
    if (!flags[Zero]) {
        PC = m_data;
    }
};

void Cpu::BPL(){
    if (flags[Zero]) {
        PC = m_data;
    }
};

void Cpu::BVC(){
    if (!flags[Overflow]) {
        PC = m_data;
    }
};

void Cpu::BVS(){
    if (flags[Overflow]) {
        PC = m_data;
    }
};

void Cpu::CLC(){
    flags.reset(Carry);
};

void Cpu::CLD(){
    //Unused for Ricoh 2A03
};

void Cpu::CLI(){
    flags.reset(Interrupt);
};

void Cpu::CLV(){
    flags.reset(Overflow);
};

void Cpu::SEC(){
    flags.set(Carry);
};

void Cpu::SED(){
    flags.set(Decimal);
};

void Cpu::SEI(){
    flags.set(Interrupt);
};

void Cpu::BRK(){
    PC++;
    write(0x0100 + SP, (PC >> 8) & 0x00FF);
    SP--;
    write(0x0100 + SP, PC & 0x00FF);
    SP--;

    flags.set(Break);
    write(0x0100 + SP, flags.to_ulong());
    SP--;
    flags.set(Break, 0);
    PC = read(0xFFFE) | read(0xFFFF) << 8;
};

void Cpu::NOP(){
    // lol
};

void Cpu::RTI(){
    SP++;
    flags = read(0x0100 + SP);

    SP++;
    PC = read(0x0100 + SP);
    SP++;
    PC |= read(0x0100 + SP) << 8;
};
