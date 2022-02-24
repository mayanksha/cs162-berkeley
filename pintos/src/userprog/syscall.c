#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler(struct intr_frame*);

void syscall_init(void) { intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall"); }

/* static void min(int a, int b) { */
  /* return (a < b) ? a: b; */
/* } */

static void syscall_handler(struct intr_frame* f UNUSED) {
  uint32_t* args = ((uint32_t*)f->esp);

  /*
   * The following print statement, if uncommented, will print out the syscall
   * number whenever a process enters a system call. You might find it useful
   * when debugging. It will cause tests to fail, however, so you should not
   * include it in your final submission.
   */

  // printf("System call number: %d\n", args[0]);

  if (args[0] == SYS_EXIT) {
    f->eax = args[1];
    printf("%s\n",thread_current()->name);
    printf("%s: exit(%d)\n", thread_current()->name, args[1]);
    thread_exit();
  } else if(args[0] == SYS_PRACTICE ){

    f->eax = (args[1] + 1);
    /* printf("before(%d) after(%d) \n",args[1],f->eax); */

  } else if (args[0] == SYS_WRITE) {
    /* dprintf()  */
    /* printf("args[1] = %d, args[2] = %s, args[3] = %d\n", args[1], args[2], args[3]); */
    int fd = args[1];
    void *buffer = args[2];
    unsigned int size = args[3];
    printf("%s",buffer);
    f->eax = size; 
  }
}


