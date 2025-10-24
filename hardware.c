#include "hardware.h"
#include "kernel.h"
#include "scheduler.h"
#include "types.h"
#include <xc.h>


// Declaração das variaveis de controle pwm sobre Software
uint8_t active_pwm = 0b00000000;                 // Ativacao settando os 4 LSB
pwm_values_t pwm_duty_cycle;        // Definição do duty cycle
pwm_values_t pwm_count;             // contador independent para cada pwm
pwm_values_t pwm_period;                // Perido para cada PWM



void conf_timer_0(void)
{
    // Configuração do periférico do TIMER (timer zero)
    T0CONbits.T0CS      = 0;        // Mudança de instrução interna
    T0CONbits.PSA       = 0;        // Ativa preescale
    T0CONbits.T0PS      = 0b110;    // 1:128
    TMR0                = 128;      // Carrega offset do timer
    INTCONbits.TMR0IE   = 1;        // Ativa interrupção por timer
    INTCONbits.TMR0IF   = 0;        // Seta flag do timer em zero
    T0CONbits.TMR0ON    = 1;        // Liga o timer
}

void conf_interrupts(void)
{
    INTCONbits.PEIE     = 1;        // Interrupções de periférico ativida    
}

// Tratador de interrupção
void __interrupt() ISR_TIMER_0(void)
{
    di();
    
    if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;
        
        // Diminui tempo das tarefas em espera
        os_task_time_decrease();
        
        SAVE_CONTEXT(READY);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    if (PIR1bits.TMR2IF){
        PIR1bits.TMR2IF = 0;
        
        // Se PWM1 (RC2) ATIVO, modular
        if (active_pwm >> 0 && 1){
            if (pwm_count.pwm1 >= pwm_period.pwm1){
                pwm_count.pwm1 = 0;
            }
            
            if (pwm_count.pwm1 <= pwm_duty_cycle.pwm1){
                 LATCbits.LC2 = 1;
            } else {
                LATCbits.LC2 = 0;
            }
            
            pwm_count.pwm1 +=1;
            
        }
        
        // Se PWM2 (RD5) ATIVO, modular
        if (active_pwm >> 1 && 1){
            if (pwm_count.pwm2 >= pwm_period.pwm2){
                pwm_count.pwm2 = 0;
            }
            
            if (pwm_count.pwm2 <= pwm_duty_cycle.pwm2){
                 LATDbits.LD5 = 1;
            } else {
                LATDbits.LD5 = 0;
            }
            
            pwm_count.pwm2++;
        }
        
        // Se PWM3 (RD6) ATIVO, modular
        if (active_pwm >> 2 && 1){
            if (pwm_count.pwm3 >= pwm_period.pwm3){
                pwm_count.pwm3 = 0;
            }
            
            if (pwm_count.pwm3 <= pwm_duty_cycle.pwm3){
                 LATDbits.LD6 = 1;
            } else {
                LATDbits.LD6 = 0;
            }
            
            pwm_count.pwm3++;
        }
        
        // Se PWM4 (RD7) ATIVO, modular
        if (active_pwm >> 3 && 1){
            if (pwm_count.pwm4 >= pwm_period.pwm4){
                pwm_count.pwm4 = 0;
            }
            
            if (pwm_count.pwm4 <= pwm_duty_cycle.pwm4){
                 LATDbits.LD7 = 1;
            } else {
                LATDbits.LD7 = 0;
            }
            
            pwm_count.pwm4++;
        }
        
        
    }
    
    ei();
}
