#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "../lib/user/syscall.h"
#include "../threads/vaddr.h"

void syscall_init (void);

void shutdown_power_off(void);
void halt(void);
void exit(int);
pid_t exec(const char *);

int fibonacci(int);
int max_of_four_int(int a, int b, int c, int d);

#endif /* userprog/syscall.h */
