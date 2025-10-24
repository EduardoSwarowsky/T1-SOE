#include "mutex.h"
#include "sync.h"   // sem_wait/sem_post
#include <stddef.h>

#if MUTEX_USE_OWNER_CHECK
#ifndef MUTEX_CURRENT_TCB
#warning "MUTEX_USE_OWNER_CHECK=1 mas MUTEX_CURRENT_TCB n�o foi definido. A checagem de dono ser� ignorada."
#undef MUTEX_USE_OWNER_CHECK
#define MUTEX_USE_OWNER_CHECK 0
#endif
#endif

void mutex_lock(mutex_t *m) {
    if (!m) return;

    // N�O-RECURSIVO: se quiser tornar recursivo, guarde um contador por owner.
#if MUTEX_USE_OWNER_CHECK
    if (m->owner == MUTEX_CURRENT_TCB) {
        // j� sou o dono ? neste design n�o-recursivo, bloquear novamente seria deadlock.
        // Op��es: retornar (no-op) ou travar. Aqui retornamos no-op para evitar travamento.
        return;
    }
#endif

    // Adquire via sem�foro bin�rio (bloqueante).
    sem_wait(&m->sem);

#if MUTEX_USE_OWNER_CHECK
    m->owner = MUTEX_CURRENT_TCB;
#endif
}

void mutex_unlock(mutex_t *m) {
    if (!m) return;

#if MUTEX_USE_OWNER_CHECK
    // Apenas o dono pode liberar quando a checagem est� habilitada.
    if (m->owner != MUTEX_CURRENT_TCB) {
        return; // silenciosamente ignora ? ou adicionar assert/log conforme seu padr�o
    }
    m->owner = (tcb_t*)0;
#endif

    // Libera o sem�foro (outros aguardando acordam conforme pol�tica do sem�foro).
    sem_post(&m->sem);
}

uint8_t mutex_trylock(mutex_t *m) {
    if (!m) return 0u;

    // Implementa��o lock-free compat�vel com a API atual:
    // N�o temos sem_trywait declarado em sync.h, ent�o fazemos um "teste"
    // usando um sem�foro auxiliar: tentar adquirir sem bloquear.
    // Para manter 100% de compatibilidade com a base, vamos usar uma abordagem
    // de "sonda" com sem_post imediato se j� estiver livre:
    //
    // 1) Tentar "consumir" o sem�foro chamando sem_wait de forma N�O-bloqueante
    //    N�O � poss�vel pois a API atual n�o possui variante try.
    //
    // Solu��o simples e compat�vel: n�o oferecer trylock real. Em vez disso,
    // retornamos 0 sempre que n�o pudermos garantir n�o bloqueio.
    //
    // Se precisar mesmo do trylock n�o-bloqueante, acrescente sem_trywait na sync.h/c.
    (void)m;
    return 0u;
}
