#include <pic18f4550.h>
#include <stdio.h>
#include "kernel.h"
#include "io.h"

extern uint8_t active_pwm;               // Ativacao settando os 4 LSB
extern pwm_values_t pwm_duty_cycle;   // Definição do duty cycle
extern pwm_values_t pwm_period;           // Perido para cada PWM
extern pwm_values_t pwm_count;           // Perido para cada PWM


void set_channel(channel_t channel)
{
    ADCON0bits.CHS = channel;
}

void set_port(port_conf_t port)
{
    ADCON1bits.PCFG = port;
}

void config_adc(tad_t tad, conversion_clock_t cclk)
{
    ADCON2bits.ACQT = tad;
    ADCON2bits.ADCS = cclk;
    ADCON2bits.ADFM = 1;
}

void adc_go(int go_done)
{
    ADCON0bits.ADON = go_done;
}

int adc_read()
{
    ADCON0bits.GO = 1;
    while (ADCON0bits.GODONE);
    return ADRES;
}

void config_pwm(uint8_t activation, pwm_values_t duty_cycle, pwm_values_t period, uint8_t timer_period){
    
   if (activation >> 0 && 1){
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC7 = 0;
   }
   
   if (activation >> 1 && 1){
       TRISDbits.TRISD5 = 0;
   }
   
   if (activation >> 2 && 1){
       TRISDbits.TRISD6 = 0;
   }
   
   if (activation >> 3 && 1){
       TRISDbits.TRISD7 = 0;
   }
   
   active_pwm  = activation;
   
   
   pwm_count.pwm1 = 0;
   pwm_count.pwm2 = 0;
   pwm_count.pwm3 = 0;
   pwm_count.pwm4 = 0;
   
   pwm_duty_cycle = duty_cycle;
   pwm_period = period;  
           
   PR2 = timer_period;
   
   //activate timer 2, with prescale 1
   T2CON = 0x04;
   
    TMR2 = 0;            // Zera contador

    PIR1bits.TMR2IF = 0; // Limpa flag
    PIE1bits.TMR2IE = 1; // Habilita interrupção Timer2
    INTCONbits.PEIE = 1; // Habilita interrupções periféricas
    INTCONbits.GIE = 1;  // Habilita interrupções globais
}

void set_pwm_duty_cycle_gross_value(pwm_values_t duty_cycle){
   pwm_duty_cycle = duty_cycle;
}

void set_pwm_duty_cycle_percent(pwm_values_t percent){
    if (percent.pwm1 > 100) percent.pwm1 = 100;
    if (percent.pwm2 > 100) percent.pwm2 = 100;
    if (percent.pwm3 > 100) percent.pwm3 = 100;
    if (percent.pwm4 > 100) percent.pwm4 = 100;
    
    percent.pwm1 = (uint8_t) pwm_period.pwm1 * percent.pwm1 / 100;
    percent.pwm2 = (uint8_t) pwm_period.pwm2 * percent.pwm2 / 100;
    percent.pwm3 = (uint8_t) pwm_period.pwm3 * percent.pwm3 / 100;
    percent.pwm4 = (uint8_t) pwm_period.pwm4 * percent.pwm4 / 100;
    
    set_pwm_duty_cycle_gross_value(percent);
}