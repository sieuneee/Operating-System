#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "vm/vm.h"


void stack_growth(void* faultAddr, void* stackPtr){	
	bool writable = true;
	void *kpage = palloc_get_page (PAL_USER);

	page_install (faultAddr, palloc_get_page(PAL_USER), writable);
}

static bool page_install (void *upage, void *kpage, bool writable){
  struct thread *cur = thread_current ();

  return (pagedir_get_page (cur->pagedir, upage) == NULL
          && pagedir_set_page (cur->pagedir, upage, kpage, writable));
}

