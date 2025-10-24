#include "pipe.h"
#include "sync.h"
#include <xc.h>

void create_pipe(pipe_t *p, uint8_t pipe_size)
{
    p->pipe_pos_read    = 0;
    p->pipe_pos_write   = 0;
    p->pipe_size        = pipe_size;
    
    p->pipe_data        = (char*) malloc(pipe_size * sizeof(char));
    
    // Inicializa os sem�foros de controle do pipe
    sem_init(&p->pipe_sem_read, 0);
    sem_init(&p->pipe_sem_write, pipe_size);
}

void read_pipe(pipe_t *p, char *buffer)
{
    di();
    
    // Testa o sem�foro de leitura
    sem_wait(&p->pipe_sem_read);
    
    *buffer = p->pipe_data[p->pipe_pos_read];
    p->pipe_pos_read = (p->pipe_pos_read + 1) % p->pipe_size;
    
    // Libera o sem�foro da escrita
    sem_post(&p->pipe_sem_write);
    
    ei();
}

void write_pipe(pipe_t *p, char buffer)
{
    di();
    
    // Testa o sem�foro de escrita
    sem_wait(&p->pipe_sem_write);
    
    p->pipe_data[p->pipe_pos_write] = buffer;
    p->pipe_pos_write = (p->pipe_pos_write + 1) % p->pipe_size;
    
    // Libera o sem�foro da leitura
    sem_post(&p->pipe_sem_read);
    
    ei();    
}

