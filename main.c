#include <xc.h>
#include "syscall.h"
#include "kernel.h"
#include "user_app.h"


int main()
{
    os_config();
    
    os_create_task(100, controle_motores, 3);
    os_create_task(101, leitura_sensores, 3);
    os_create_task(102, monitoramento_bateria, 3);
    os_create_task(103, controle_central, 3);
    
    os_start();
    
    while (1);
    
    return 0;
}


