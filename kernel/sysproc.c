#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_sigalarm(void){
  int ticks;
  int handler;

  if (argint(0, &ticks) < 0) {
    panic("cant read ticks");
  }
  if (argint(1, &handler) < 0) {
    panic("cant read handler");
  }
  struct proc* p = myproc();
  p->alarm_ticks = ticks;
  p->handler = (uint64)handler;

  return 0;
}

uint64
sys_sigreturn(void) {
  struct proc* p = myproc();
  // uint64 sp = r_sp();
  // printf("sp = %p\n", sp);
  // printf("*sp = %p\n", (uint64)(*(uint64 *)sp));
  // for (int i = 280; i >= 0; i -= 8) {
  //   uint64 temp = (uint64)(*(uint64 *)sp);
  //   // *((uint64*)p->trapframe + i / 8) = *(uint64 *)sp;
  //   *((uint64*)p->trapframe + i / 8) = temp;
  //   printf("i = %d\n", i);
  //   printf("*sp = %p\n", (uint64)(*(uint64 *)sp));
  //   printf("sp = %p\n", sp);
  //   printf("temp = %p\n", temp);
  //   sp += 8;
  // }
  // intr_on();
  // intr_off();
  uint64* p_tf = (uint64*)&handler_saved_trapframe;
  
  //works copy bytes one by one
  // memmove(p->trapframe, p_tf, sizeof(struct trapframe));
  
  // uint64* tf = (uint64*)p->trapframe; 

  // //works
  // for (int i = 0; i <= 280; i += 8) {
  //   *tf = *p_tf;
  //   // printf("%d\n", i);
  //   // printf("%p\n", *tf);
  //   // printf("%p\n", *p_tf);
  //   tf++;
  //   p_tf++;
  // }

  // //works
  // for (int i = 0; i <= 280; i += 8) {
  //   *(tf + i / 8) = *(p_tf + i / 8);
  //   // printf("%d\n", i);
  //   // printf("%p\n", *(tf + i / 8));
  //   // printf("%p\n", *(p_tf + i / 8));
  // }

  // //dont work, why
  // // because write as "8 / i" !!!!!!!!!!!!
  // uint64* tf = (uint64*)p->trapframe;
  // // printf("tf = %p\n", tf);
  // for (int i = 0; i <= 280; i += 8) {
  //   // *((uint64*)p->trapframe + 8 / i) = *(p_tf + 8 / i);

  //   // uint64 temp = *(p_tf + 8 / i);
  //   // printf("p->trapframe = %p\n", p->trapframe);
  //   *(tf + 8 / i) = *(p_tf + 8 / i);
  // }

  //dont work, why
  for (int i = 0; i <= 280; i += 8) {
    // *((uint64*)p->trapframe + 8 / i) = *(p_tf + 8 / i);

    // // uint64 temp = *(p_tf + 8 / i);
    // printf("p->trapframe = %p\n", p->trapframe);
    // *(((uint64*)(p->trapframe)) + 8 / i) = (uint64)*(p_tf + 8 / i);

    *(((uint64*)(p->trapframe)) + i / 8) = *(p_tf + i / 8);
    
    // printf("temp = %p\n", temp);
    // *(tf + 8 / i) = *(p_tf + 8 / i);

    // if ((uint64)*((uint64*)p->trapframe + 8 / i) != (uint64)*(p_tf + 8 / i)) {
    //   panic("not equal");
    // }
    // printf("sigreturn\n");
    // printf("i = %d\n", i);
    // // *(p_tf + i / 8) = *((uint64*)(p->trapframe) + i / 8);
    // printf("p_tf + i / 8 =                    %p\n", p_tf + i / 8);
    // printf("(uint64*)(p->trapframe) + i / 8 = %p\n", (uint64*)(p->trapframe) + i / 8);

    // printf("*(p_tf + i / 8) =                    %p\n", *(p_tf + i / 8));
    
    // printf("*((uint64*)(p->trapframe) + i / 8) = %p\n", *((uint64*)(p->trapframe) + i / 8));
    // if (*((uint64*)p->trapframe + 8 / i) != *(p_tf + 8 / i)) {
      
    //   panic("not equal");
    // }
    // printf("\n");
  }
  // panic("stop");
  p->passed_ticks = 0;
  // intr_on();
  return 0;
}