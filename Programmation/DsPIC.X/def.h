// FOSC
#pragma config FPR = XT_PLL16           // Primary Oscillator Mode (XT w/PLL 16x)
#pragma config FOS = FRC                // Oscillator Source (Internal Fast RC)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_OFF         // POR Timer Value (Timer Disabled)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = PGD                // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h>
#include <p30F2010.h>
#include "init.h"
#include "ISR.h"

#define Fcy 32000000                        //Fr�quence de l'horlogue interne
#define __delay_ms(x) __delay32(Fcy*14/x)   //Delay perso
#define LED_ON()        LATC = (1<<STATE)
#define LED_OFF()       LATC = 0x0000;
#define START_MOTOR()   PWMCON1 = 0x0777;
#define STOP_MOTOR()    PWMCON1 = 0x0700;

#define LIM_CUR         1000 //Limite de courant
#define LIM_VIT         20   //Limite de vitesse pour lancer le moteur
#define LIM_FREQ        1200 //Limite du mode pas � pas, 6000 tr/min * 20 ms (T3) * 10 (p�les) = 1200
#define PAS_RAMPE       9    //Pente de la rampe de vitesse, 0.6 s = 256*0.02*Incr�ment => pour une rampe en 0.6 s, il faut incr�menter par pas de 9

typedef enum {Idle, ErrRec, ErrCur} state; //Type de variable pour la LED
typedef enum {ComparateursOff, ComparateursOn} stateComp; //Type de variable pour les comparateurs