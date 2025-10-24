#ifndef USER_APP_H
#define	USER_APP_H

#include "os_config.h"
#include "types.h"

void config_app(void);

TASK controle_motores(void);
TASK leitura_sensores(void);
TASK monitoramento_bateria(void);
TASK controle_central(void);




#endif	/* USER_APP_H */

