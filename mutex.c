#include "mutex.h"
#include "sync.h"   // sem_wait/sem_post
#include <stddef.h>

#if MUTEX_USE_OWNER_CHECK
#ifndef MUTEX_CURRENT_TCB
#warning "MUTEX_USE_OWNER_CHECK=1 mas MUTEX_CURRENT_TCB não foi definido. A checagem de dono será ignorada."
#undef MUTEX_USE_OWNER_CHECK
#define MUTEX_USE_OWNER_CHECK 0
#endif
#endif

void mutex_lock(mutex_t *m) {
    if (!m) return;

    // NÃO-RECURSIVO: se quiser tornar recursivo, guarde um contador por owner.
#if MUTEX_USE_OWNER_CHECK
    if (m->owner == MUTEX_CURRENT_TCB) {
        // já sou o dono ? neste design não-recursivo, bloquear novamente seria deadlock.
        // Opções: retornar (no-op) ou travar. Aqui retornamos no-op para evitar travamento.
        return;
    }
#endif

    // Adquire via semáforo binário (bloqueante).
    sem_wait(&m->sem);

#if MUTEX_USE_OWNER_CHECK
    m->owner = MUTEX_CURRENT_TCB;
#endif
}

void mutex_unlock(mutex_t *m) {
    if (!m) return;

#if MUTEX_USE_OWNER_CHECK
    // Apenas o dono pode liberar quando a checagem está habilitada.
    if (m->owner != MUTEX_CURRENT_TCB) {
        return; // silenciosamente ignora ? ou adicionar assert/log conforme seu padrão
    }
    m->owner = (tcb_t*)0;
#endif

    // Libera o semáforo (outros aguardando acordam conforme política do semáforo).
    sem_post(&m->sem);
}

uint8_t mutex_trylock(mutex_t *m) {
    if (!m) return 0u;

    // Implementação lock-free compatível com a API atual:
    // Não temos sem_trywait declarado em sync.h, então fazemos um "teste"
    // usando um semáforo auxiliar: tentar adquirir sem bloquear.
    // Para manter 100% de compatibilidade com a base, vamos usar uma abordagem
    // de "sonda" com sem_post imediato se já estiver livre:
    //
    // 1) Tentar "consumir" o semáforo chamando sem_wait de forma NÃO-bloqueante
    //    NÃO é possível pois a API atual não possui variante try.
    //
    // Solução simples e compatível: não oferecer trylock real. Em vez disso,
    // retornamos 0 sempre que não pudermos garantir não bloqueio.
    //
    // Se precisar mesmo do trylock não-bloqueante, acrescente sem_trywait na sync.h/c.
    (void)m;
    return 0u;
}
