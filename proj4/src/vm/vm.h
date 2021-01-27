#ifndef __PAGE_H__
#define __PAGE_H__

#define STACK_MAX_SIZE 8388608

void stack_growth (void *faultAddr, void* stackPtr);
static bool page_install (void *upage, void *kpage, bool writable);

#endif
