#include <common.h>
#include <klib.h>

intptr_t my_atomic_xchg(volatile intptr_t *addr, intptr_t newval) {
  intptr_t result;
asm volatile ("lock xchgl %0, %1":
    "+m"(*addr), "=a"(result) : "1"(newval) : "cc");
return result; 
}

intptr_t my_spin_lock(intptr_t locked)
{

  intptr_t value=1;
  //printf("I am here");
  while (1)
  {
    printf("%d\n",value);
   value=my_atomic_xchg(&locked,1);
   if(value==1)
    {
      break;
    }

  };
  return 0;
}

intptr_t my_spin_unlock(intptr_t locked)
{
 my_atomic_xchg(&locked, 0);
 return 0;
}



