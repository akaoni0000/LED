#include <SI_EFM8SB1_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"

//LEDライト点滅プログラム

sbit pin11 = P1^1;
void delay_ms (ms);

void SiLabs_Startup (void) {}
int main (void)
{
	enter_DefaultMode_from_RESET();
	enter_Mode2_from_DefaultMode();
	while(1){
	    pin11 = 0;
	    delay_ms(1000);
	    pin11 = 1;
	    delay_ms(1000);
	}
}

void delay_ms (ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TCON_TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON0 |= 0x04;                      // Timer0 counts SYSCLKs

   while (ms) {
      TCON_TR0 = 0;                         // Stop Timer0
      TH0 = ((-24500000/1000) >> 8);     // Overflow in 1ms
      TL0 = ((-24500000/1000) & 0xFF);
      TCON_TF0 = 0;                         // Clear overflow indicator
      TCON_TR0 = 1;                         // Start Timer0
      while (!TCON_TF0);                    // Wait for overflow
      ms--;                            // Update ms counter
   }

   TCON_TR0 = 0;                            // Stop Timer0
}
