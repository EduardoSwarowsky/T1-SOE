#include <xc.h>

#include "user_app.h"
#include "syscall.h"
#include "sync.h"
#include "pipe.h"
#include "mem.h"
#include "io.h"

//#if EXAMPLE_1 == YES
//// Exemplo 1
//
////sem_t semaforo_teste;
//pipe_t canal;
//
//void config_app(void)
//{
//    TRISDbits.RD0 = TRISDbits.RD1 = TRISDbits.RD2 = 0;
//    
//    TRISCbits.RC0 = TRISCbits.RC1                 = 0;             
//    
//    //sem_init(&semaforo_teste, 0);
//    create_pipe(&canal, 3);
//    
//    asm("GLOBAL _tarefa_1, _tarefa_2, _tarefa_3");
//}
//
//TASK tarefa_1(void)
//{    
//    char *dados = (char*)SRAMalloc(sizeof(char) * 3);
//    dados[0] = 'a';
//    dados[1] = 'b';
//    dados[2] = 'c';
//    
//    int index = 0;
//    while (1) {
//        LATDbits.LD0 = ~PORTDbits.RD0;
//        //sem_wait(&semaforo_teste);
//        write_pipe(&canal, dados[index]);
//        index = (index + 1) % 3;
//        os_delay(50);
//    }
//    
//    SRAMfree(dados);
//}
//
//TASK tarefa_2(void)
//{
//    char dado;
//    
//    while (1) {
//        LATDbits.LD1 = ~PORTDbits.RD1;
//        //os_delay(200);
//        read_pipe(&canal, &dado);
//        if (dado == 'a') {
//            LATCbits.LATC0 = 1;
//        }
//        else if (dado == 'b') {
//            LATCbits.LATC1 = 1;
//        }
//        else if (dado == 'c') {
//            LATCbits.LATC0 = 0;
//            LATCbits.LATC1 = 0;
//        }
//    }    
//}
//
//TASK tarefa_3(void)
//{
//    while (1) {
//        LATDbits.LD2 = ~PORTDbits.RD2;
//        //sem_post(&semaforo_teste);
//    }    
//}
//
//#elif EXAMPLE_2 == YES
//
//void config_app(void)
//{
//    set_channel(CHANNEL_0);
//    set_port(AN00);
//    config_adc(TAD12, FOSC4);
//    
//    TRISCbits.RC0 = TRISCbits.RC1 = 0;
//    TRISDbits.RD0 = 0;
//    
//    asm("GLOBAL _tarefa_1");
//}
//
//TASK tarefa_1(void)
//{
//    int pot = 0;
//    
//    adc_go(1);
//    
//    while (1) {
//        LATDbits.LD0 = ~PORTDbits.RD0;
//        
//        pot = adc_read();
//        
//        if (pot > 0 && pot < 500) {
//            LATCbits.LATC0 = 1;
//            LATCbits.LATC1 = 0;
//        }
//        else if (pot >= 500 && pot <= 1023) {
//            LATCbits.LATC1 = 1;
//            LATCbits.LATC0 = 0;
//        }
//    }
//}
//
//#endif

void config_app(void)
{
    set_channel(CHANNEL_0);
    set_port(AN05);
    config_adc(TAD12, FOSC4);
    
    // PWM Timer Period (unitary periode) | PR2 = 0xFF, Prescaler = 1| 1/39060s = 25us
    // Isto é, com a configuração abaixo definimos o periodo do pwm para 250 us
    config_pwm(0xFF, (pwm_values_t) {0,3,6,10},(pwm_values_t) {10,10,10,10}, 0xFF);
    
    asm("GLOBAL _controle_motores, _leitura_sensores");
}

TASK controle_motores(void){
    pwm_values_t motor; 
    
    motor.pwm1 = 20;
    motor.pwm2 = 40;
    motor.pwm3 = 60;
    motor.pwm4 = 100;
    
    while (1){
        set_pwm_duty_cycle_percent(motor);
        os_delay(50);
    }
    
}

TASK leitura_sensores(void){
    int acel_x = 0;
    int acel_y = 0;
    int acel_z = 0;
    int ang_x = 0;
    int ang_y = 0;
    int bat = 0;
    
    adc_go(1);
    
    
    while (1) {
        set_channel(CHANNEL_0);
        acel_x = adc_read();
        
        set_channel(CHANNEL_1);
        acel_y = adc_read();
        
        set_channel(CHANNEL_2);
        acel_z = adc_read();
        
        set_channel(CHANNEL_3);
        ang_x = adc_read();
        
        set_channel(CHANNEL_4);
        ang_y = adc_read();
        
        set_channel(CHANNEL_5);
        bat = adc_read();
        os_delay(50);
    }
}
    
    


TASK monitoramento_bateria(void){
    while (1) os_delay(50);;
}

TASK controle_central(void){
    while (1) os_delay(50);
}
