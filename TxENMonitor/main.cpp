#include <p32xxxx.h>
#include <cstdint>

const uint32_t portBPinMask = 0b1110111110101100;
const uint32_t portATxEnable = 1;
const uint32_t portATxEnableClash = 2;

int main(int argc, char** argv)
{
    TRISB = 0xFFFF;
    
    LATACLR = portATxEnable | portATxEnableClash;
    TRISACLR = portATxEnable | portATxEnableClash;
    
    while(true)
    {
        const uint32_t tXEnableBits = PORTB & portBPinMask;
        
        uint32_t count = 0;
        
        for (uint32_t i = 0; i < 16; i++)
        {
            if (tXEnableBits & (1 << i)) count++;
        }
        
        if (tXEnableBits)
        {
            LATASET = portATxEnable;
        }
        else
        {
            LATACLR = portATxEnable;
        }
        
        if (count >= 2)
        {
            LATASET = portATxEnableClash;
        }
        else
        {
            LATACLR = portATxEnableClash;
        }
    }
    
    return 0;
}

