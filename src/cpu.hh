#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>

class Cpu {
public:
    Cpu();
    ~Cpu();
    void tick();

    // alas, I must use a shorthand to cut down on bulk. please use some form of  
    // documentation that I have shared to read more on address modes
    // the ill is for unimplemented illegal opcodes...
    enum addr_mode{
        acc, imp, imm, zpg, zpx, zpy,
        rel, abs, abx, aby, ind, inx, iny, ill
    };

    // this struct is what will be fetched. Just contains a func pointer
    // and some extra data about the particular opcode byte.
    // handily it also will handle the amount of cycles for each
    // different addressing mode.
    struct opcode {
        void (Cpu::*op)();
        uint8_t cycles;
        addr_mode mode;
    };

public:
    std::bitset<8> flags;

    enum status{
        Carry     = 0,
        Zero      = 1,
        Interrupt = 2,
        Decimal   = 3,
        Break     = 4,
        // unused
        Overflow  = 6,
        Negative  = 7
    };

    // Dear god... This is a massive initializer list of our lookup table
    // of all possible opcodes for the 6502 Microprocessor. Turns out
    // this is the way to go to avoid a massive switch statement or
    // massive template expansion/compression.
    const opcode m_lookup[256] = {
        {&Cpu::BRK, 7, imp},{&Cpu::ORA, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::ORA, 3, zpg},{&Cpu::ASL, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::PHP, 3, imp},{&Cpu::ORA, 2, imm},{&Cpu::ASL, 2, acc},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::ORA, 4, abs},{&Cpu::ASL, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BPL, 2, rel},{&Cpu::ORA, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::ORA, 4, zpx},{&Cpu::ASL, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::CLC, 2, imp},{&Cpu::ORA, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::ORA, 4, abx},{&Cpu::ASL, 7, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::JSR, 6, abs},{&Cpu::AND, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::BIT, 3, zpg},{&Cpu::AND, 3, zpg},{&Cpu::ROL, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::PLP, 4, imp},{&Cpu::AND, 2, imm},{&Cpu::ROL, 2, acc},{&Cpu::NOP, 0, ill},{&Cpu::BIT, 4, abs},{&Cpu::AND, 4, abs},{&Cpu::ROL, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BMI, 2, rel},{&Cpu::AND, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, imp},{&Cpu::AND, 4, zpx},{&Cpu::ROL, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::SEC, 2, imp},{&Cpu::AND, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::AND, 4, abx},{&Cpu::ROL, 7, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::RTI, 6, imp},{&Cpu::EOR, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, imp},{&Cpu::EOR, 3, zpg},{&Cpu::LSR, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::PHA, 3, imp},{&Cpu::EOR, 2, imm},{&Cpu::LSR, 2, acc},{&Cpu::NOP, 0, ill},{&Cpu::JMP, 3, abs},{&Cpu::EOR, 4, abs},{&Cpu::LSR, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BVC, 2, rel},{&Cpu::EOR, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, imp},{&Cpu::EOR, 4, zpx},{&Cpu::LSR, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::CLI, 2, imp},{&Cpu::EOR, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::EOR, 4, abx},{&Cpu::LSR, 7, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::RTS, 6, imp},{&Cpu::ADC, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, imp},{&Cpu::ADC, 3, zpg},{&Cpu::ROR, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::PLA, 4, imp},{&Cpu::ADC, 2, imm},{&Cpu::ROR, 2, acc},{&Cpu::NOP, 0, ill},{&Cpu::JMP, 5, ind},{&Cpu::ADC, 4, abs},{&Cpu::ROR, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BVS, 2, rel},{&Cpu::ADC, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, imp},{&Cpu::ADC, 4, zpx},{&Cpu::ROR, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::SEI, 2, imp},{&Cpu::ADC, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::ADC, 4, abx},{&Cpu::ROR, 7, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::NOP, 0, ill},{&Cpu::STA, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::STY, 3, zpg},{&Cpu::STA, 3, zpg},{&Cpu::STX, 3, zpg},{&Cpu::NOP, 0, ill},{&Cpu::DEY, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::TXA, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::STY, 4, abs},{&Cpu::STA, 4, abs},{&Cpu::STX, 4, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BCC, 2, rel},{&Cpu::STA, 6, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::STY, 4, zpx},{&Cpu::STA, 4, zpx},{&Cpu::STX, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::TYA, 2, imp},{&Cpu::STA, 5, aby},{&Cpu::TXS, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::STA, 5, abx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},
        {&Cpu::LDY, 2, imm},{&Cpu::LDA, 6, inx},{&Cpu::LDX, 2, imm},{&Cpu::NOP, 0, ill},{&Cpu::LDY, 3, zpg},{&Cpu::LDA, 3, zpg},{&Cpu::LDX, 3, zpg},{&Cpu::NOP, 0, ill},{&Cpu::TAY, 2, imp},{&Cpu::LDA, 2, imm},{&Cpu::TAX, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::LDY, 4, abs},{&Cpu::LDA, 4, abs},{&Cpu::LDX, 4, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BCS, 2, rel},{&Cpu::LDA, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::LDY, 4, zpx},{&Cpu::LDA, 4, zpx},{&Cpu::LDX, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::CLV, 2, imp},{&Cpu::LDA, 4, aby},{&Cpu::TSX, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::LDY, 4, abx},{&Cpu::LDA, 4, abx},{&Cpu::LDX, 4, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::CPY, 2, imm},{&Cpu::CMP, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::CPY, 3, zpg},{&Cpu::CMP, 3, zpg},{&Cpu::DEC, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::INY, 2, imp},{&Cpu::CMP, 2, imm},{&Cpu::DEX, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::CPY, 4, abs},{&Cpu::CMP, 4, abs},{&Cpu::DEC, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BNE, 2, rel},{&Cpu::CMP, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::CMP, 4, zpx},{&Cpu::DEC, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::CLD, 2, imp},{&Cpu::CMP, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::CMP, 4, abx},{&Cpu::DEC, 7, abx},{&Cpu::NOP, 0, ill},
        {&Cpu::CPX, 2, imm},{&Cpu::SBC, 6, inx},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::CPY, 4, zpg},{&Cpu::SBC, 3, zpg},{&Cpu::INC, 5, zpg},{&Cpu::NOP, 0, ill},{&Cpu::INX, 2, imp},{&Cpu::SBC, 2, imm},{&Cpu::NOP, 2, imp},{&Cpu::NOP, 0, ill},{&Cpu::CPX, 4, abs},{&Cpu::SBC, 4, abs},{&Cpu::INC, 6, abs},{&Cpu::NOP, 0, ill},
        {&Cpu::BEQ, 2, rel},{&Cpu::SBC, 5, iny},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::SBC, 4, zpx},{&Cpu::INC, 6, zpx},{&Cpu::NOP, 0, ill},{&Cpu::SED, 2, imp},{&Cpu::SBC, 4, aby},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::NOP, 0, ill},{&Cpu::SBC, 4, abx},{&Cpu::INC, 7, abx},{&Cpu::NOP, 0, ill}
    };

    uint16_t PC = 0;
    uint8_t X=0, Y=0, SP=0, A=0;
    uint16_t m_addr=0;
    uint8_t m_data=0;
    uint8_t current_op=0;
    uint32_t cycles;


    // external to the CPU 
    uint8_t m_ram[0xFFFF] = {0};

    uint8_t ROM[0xFFFF] = {0};

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);

    void fetch_op(uint8_t opcode);
    void set_addr_mode(opcode o);

    void reset();

public:
    // get  ready for all these opcodes :!

    void LDA(); void LDX(); void LDY(); void STA();
    void STX(); void STY(); void TAX(); void TAY();
    void TXA(); void TYA(); void TSX(); void TXS();

    void PHA(); void PHP(); void PLA(); void PLP(); 

    void AND(); void EOR(); void ORA(); void BIT(); 

    void ADC(); void SBC(); void CMP(); void CPX(); 
    void CPY(); void INC(); void INX(); void INY(); 
    void DEC(); void DEX(); void DEY(); 

    void ASL(); void LSR(); void ROL(); void ROR(); 

    void JMP(); void JSR(); void RTS(); 

    void BCC(); void BCS(); void BEQ(); void BMI(); 
    void BNE(); void BPL(); void BVC(); void BVS();

    void CLC(); void CLD(); void CLI(); void CLV(); 
    void SEC(); void SED(); void SEI(); 
    
    void BRK(); void NOP(); void RTI(); 
    

};
