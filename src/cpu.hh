#pragma once

#include <array>
#include <bitset>
#include <cstdint>

class Cpu {
public:
    Cpu();
    ~Cpu();

private:
    std::bitset<8> flags;

    enum status{
        Carry     = (1<<0),
        Zero      = (1<<1),
        Interrupt = (1<<2),
        // 3 and 4 unused!
        Decimal   = (1<<5),
        Overflow  = (1<<6),
        Negative  = (1<<7)
    };

    struct opcode {
        void (Cpu::*op)() ;
        uint8_t cycles;
    };

    std::array<opcode, 256> m_lookup = {
    };

    uint16_t PC;
    uint8_t X, Y, SP, A;
    uint16_t m_addr;

    uint8_t m_ram[2048];

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);

    void fetch_op(uint8_t opcode);
    void set_addr_mode();


public:
    void tick();
    
private:
    // get  ready for all these opcodes :!
    /*
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
    */

};
