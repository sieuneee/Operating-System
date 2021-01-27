#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
	lock_init(&file_lock);
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
  int n1, n2, n3, n4;
  int k;

  //if(!is_user_vaddr(fault_addr)) exit(-1);
  //if(!user) exit(-1);
  
  switch(*(uint32_t*)(f->esp)){
		case SYS_HALT:
			halt();
			break;
		case SYS_EXIT:
			check_addr(f->esp+4);
			exit(*(uint32_t *)(f->esp+4));
			break;
		case SYS_EXEC:
			check_addr(f->esp+4);
			f->eax = exec((const char *)*(uint32_t*)(f->esp+4));
			break;
		case SYS_WAIT:
			check_addr(f->esp+4);
			f->eax = wait(*(uint32_t*)(f->esp+4));
			break;
		case SYS_READ:
			check_addr(f->esp+4);
			check_addr(f->esp+8);
			check_addr(f->esp+12);
			f->eax = read((int)*(uint32_t *)(f->esp+4), (void *)*(uint32_t *)(f->esp+8), (unsigned)*((uint32_t *)(f->esp+12)));
			break;
		case SYS_WRITE:
			check_addr(f->esp+4);
			check_addr(f->esp+8);
			check_addr(f->esp+12);
			f->eax = write((int)*(uint32_t *)(f->esp+4), (void *)*(uint32_t *)(f->esp+8), (unsigned)*((uint32_t *)(f->esp+12)));
			break;
		case SYS_CREATE:
			check_addr(f->esp+4);
			check_addr(f->esp+8);
			f->eax = create((const char*)*(uint32_t *)(f->esp+4), (unsigned)*(uint32_t*)(f->esp+8));
			break;
		case SYS_REMOVE:
			check_addr(f->esp+4);
			f->eax = remove((const char *)*(uint32_t *)(f->esp+4));
			break;
		case SYS_OPEN:
			check_addr(f->esp+4);
			f->eax = open((const char*)*(uint32_t*)(f->esp+4));
			break;
		case SYS_FILESIZE:
			check_addr(f->esp+4);
			f->eax = filesize((int)*(uint32_t *)(f->esp+4));
			break;
		case SYS_SEEK:
			check_addr(f->esp+4);
			check_addr(f->esp+8);
			seek((int)*(uint32_t*)(f->esp+4), (unsigned)*(uint32_t*)(f->esp+8));
			break;
		case SYS_TELL:
			check_addr(f->esp+4);
			f->eax = tell((int)*(uint32_t*)(f->esp+4));
			break;
		case SYS_CLOSE:
			check_addr(f->esp+4);
			close((int)*(uint32_t*)(f->esp+4));
			break;
		case SYS_FIBO:
			check_addr(f->esp+4);
			k = (int)*(uint32_t*)(f->esp +4);
			f->eax = fibonacci(k);
			break;
		case SYS_FMAX:
			check_addr(f->esp+24); check_addr(f->esp+28); check_addr(f->esp+32); check_addr(f->esp+26);
			n1 = (int)*(uint32_t*)(f->esp + 24);
			n2 = (int)*(uint32_t*)(f->esp + 28);
			n3 = (int)*(uint32_t*)(f->esp + 32);
			n4 = (int)*(uint32_t*)(f->esp + 36);
			f->eax = max_of_four_int(n1,n2,n3,n4);
			break;
  }

  //thread_exit ();
}

void check_addr(const void *addr){
	if(!is_user_vaddr(addr)) exit(-1);
}

int fibonacci(int n){
	int n1=1, n2=1;
	int tmp;

	if(n==1 || n==2) return 1;
	else{
		for(int i=2;i<n;i++){
			tmp = n1+n2;
			n1 = n2;
			n2 = tmp;
		}
		return n2;
	}
}

int max_of_four_int(int a, int b, int c, int d){
	int max1 = (a > b) ? a : b;
	int max2 = (c > d) ? c : d;
	return max1 > max2 ? max1: max2;
}

void halt(void){
	shutdown_power_off();
}

void exit(int status){

	struct thread *tmp= thread_current();
	char file_name[128];
	//tmp->exit_status = status;
	memset(file_name, 0x00, sizeof(file_name));

	int i = 0;
	for(i=0;i<strlen(tmp->name);i++){
		if(tmp->name[i]=='\0' || tmp->name[i]==' ') break;
		file_name[i] = tmp->name[i];
	}
	file_name[i]='\0';

	printf("%s: exit(%d)\n", file_name, status);
	tmp->exit_status = status;

	for(int i=3;i<128;i++){
		if(thread_current()->fd[i]!=NULL) close(i);
	}

	struct thread* temp = NULL;
	struct list_elem *tmp_elem = NULL;
	for(tmp_elem = list_begin(&thread_current()->child);tmp_elem !=list_end(&thread_current()->child); tmp_elem=list_next(tmp_elem)){
		temp = list_entry(tmp_elem, struct thread, child_elem);
		process_wait(temp->tid);
	}

	thread_exit();	
}

pid_t exec (const char *cmd){

	struct file *fp = NULL;
	int i=0;
	char tmp[128];

	while(cmd[i]!=' ' && cmd[i]!='\0'){
		tmp[i] = cmd[i];
		i++;
	}
	tmp[i]='\0';
	fp = filesys_open(tmp);

	if(fp==NULL) return -1;

	return (pid_t)process_execute(cmd);

}

int wait(pid_t pid){
	return process_wait((tid_t)pid);
}


int read(int f, void *tmp, unsigned size){
	int i=0;
	int re=-1;

	check_addr(tmp);
	lock_acquire(&file_lock);

	if(f!=0){
		if(thread_current()->fd[f]==NULL) {
			lock_release(&file_lock);
			exit(-1);
		}
		re = file_read(thread_current()->fd[f], tmp, size);
	}
	else{
		for(i=0;i<size;i++){
			if(input_getc()=='\0') break;
		}
	}

	lock_release(&file_lock);
	return re;

}

int write(int f, const void *tmp, unsigned size){
	int re=-1;

	check_addr(tmp);

	lock_acquire(&file_lock);
	if(f==1){
		putbuf(tmp, size);
		re = size;
	}
	else{
		if(thread_current()->fd[f]==NULL) {
			lock_release(&file_lock);
			exit(-1);
		}
		if(thread_current()->fd[f]->deny_write) file_deny_write(thread_current()->fd[f]);
		else file_allow_write(thread_current()->fd[f]);
		re = file_write(thread_current()->fd[f], tmp, size);
	}
	lock_release(&file_lock);
	return re;
	//return -1;
}

//project2
bool create (const char *fp, unsigned size) {
	if(fp==NULL) exit(-1);
	return filesys_create(fp, size);
}

bool remove (const char *fp) {
	if(fp==NULL) exit(-1);
	return filesys_remove(fp);
}

int open (const char *fp) {
	int re=-1;
	if(fp==NULL) exit(-1);

	lock_acquire(&file_lock);
	struct file* tmp = filesys_open(fp);

	if (tmp == NULL) {
		re = -1;
	} 
	else {
		for (int i = 3; i < 128; i++) {
			if (thread_current()->fd[i] == NULL) {
				if(strcmp(thread_current()->name, fp)==0) file_deny_write(tmp);
				else file_allow_write(tmp);
				thread_current()->fd[i] = tmp;
				re = i; break;
			}
		}
	}
	lock_release(&file_lock);
	return re;
}

int filesize (int i) {
	if(thread_current()->fd[i]==NULL) exit(-1);
	return file_length(thread_current()->fd[i]);
}

void seek (int i, unsigned pos) {
	if(thread_current()->fd[i]==NULL) exit(-1);
	file_seek(thread_current()->fd[i], pos);
}

unsigned tell (int i) {
	if(thread_current()->fd[i]==NULL) exit(-1);
	return file_tell(thread_current()->fd[i]);
}

void close (int i) {
	if(thread_current()->fd[i]==NULL) exit(-1);
	struct file *fp = thread_current()->fd[i];
	thread_current()->fd[i] = NULL;
	file_close(fp);
}

