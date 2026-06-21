#include <lpc214x.h>
#include <stdbool.h>

#define BUTTON   0
#define SEGMENT  16

const unsigned char segment_values[10] =
{
    0x3F,0x06,0x5B,0x4F,0x66,
    0x6D,0x7D,0x07,0x7F,0x6F
};

volatile unsigned char cnt = 0;
void delay_ms(unsigned int d)
{
    unsigned int i,j;

    for(i=0;i<d;i++)
        for(j=0;j<6000;j++);
}

int main(void)
{
    bool prev = 1, curr;

    /* Configure P0.0 as GPIO input */
    IODIR0 &= ~(1 << BUTTON);

    /* Configure P1.16-P1.23 as GPIO output */
    IODIR1 |= (0xFF << SEGMENT);

    /* Display initial value 0 */
    IOCLR1 = (0xFF << SEGMENT);
    IOSET1 = ((unsigned int)segment_values[cnt] << SEGMENT);

    while(1)
    {
        /* Read button state */
        curr = (IOPIN0 & (1 << BUTTON)) ? 1 : 0;

        /* Falling edge detection (button connected to GND) */
        if((prev == 1) && (curr == 0))
        {
            delay_ms(20);     /* Debounce */

            if((IOPIN0 & (1 << BUTTON)) == 0)
            {
                cnt++;

                if(cnt >= 10)
                    cnt = 0;

                IOCLR1 = (0xFF << SEGMENT);
                IOSET1 = ((unsigned int)segment_values[cnt] << SEGMENT);

                while((IOPIN0 & (1 << BUTTON)) == 0); /* Wait for release */
            }
        }

        prev = curr;
    }
}
