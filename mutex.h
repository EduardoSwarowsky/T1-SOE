#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>
#include "types.h"   // fornece tcb_t, sem_t etc.
#include "sync.h"    // usa sem_init/sem_wait/sem_post já existentes

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Mutex binário, NÃO-RECURSIVO, implementado sobre semáforo binário.
 * Por padrão não faz checagem de "dono" (owner) para maximizar compatibilidade.
 * Se o kernel expõe um ponteiro da tarefa atual, você pode habilitar a checagem
 * definindo MUTEX_USE_OWNER_CHECK = 1 e informando MUTEX_CURRENT_TCB.
 *
 * Exemplo para habilitar checagem de dono:
 *   // em algum .c global do seu RTOS:
 *   extern tcb_t *current_task;
 *   #define MUTEX_USE_OWNER_CHECK 1
 *   #define MUTEX_CURRENT_TCB (current_task)
 */

#ifndef MUTEX_USE_OWNER_CHECK
#define MUTEX_USE_OWNER_CHECK 0
#endif

typedef struct {
    sem_t   sem;     // semáforo binário subjacente (1 = livre, 0 = ocupado)
#if MUTEX_USE_OWNER_CHECK
    tcb_t  *owner;   // dono atual do mutex (se checagem de dono estiver ativada)
#endif
} mutex_t;

/** Inicializa o mutex como "destravado" (livre). */
static inline void mutex_init(mutex_t *m) {
    if (!m) return;
    sem_init(&m->sem, 1u);
#if MUTEX_USE_OWNER_CHECK
    m->owner = (tcb_t*)0;
#endif
}

/** Bloqueia até adquirir o mutex (NÃO-RECURSIVO). */
void mutex_lock(mutex_t *m);

/** Desbloqueia o mutex (se a checagem de dono estiver ativa, apenas o dono pode liberar). */
void mutex_unlock(mutex_t *m);

/** Tenta adquirir sem bloquear. Retorna 1 se conseguiu, 0 se ocupado. */
uint8_t mutex_trylock(mutex_t *m);

#ifdef __cplusplus
}
#endif

#endif /* MUTEX_H */
