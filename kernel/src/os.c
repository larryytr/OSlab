#include <common.h>
#include <klib.h>

static void os_init() {
  pmm->init();
}

static void hello() {
  intptr_t locked=0;
  my_spin_lock(locked);
  for (const char *ptr = "Hello from CPU #"; *ptr; ptr++) {
    _putc(*ptr);
  }
  _putc("12345678"[_cpu()]); _putc('\n');
  my_spin_unlock(locked);
}

static void os_run() {
  intptr_t locked=0;
  my_spin_lock(locked);
  hello();
  my_spin_unlock(locked);
  _intr_write(1);
  while (1) {
    _yield();
  }
}

static _Context *os_trap(_Event ev, _Context *context) {
  return context;
}

static void os_on_irq(int seq, int event, handler_t handler) {
}

MODULE_DEF(os) {
  .init   = os_init,
  .run    = os_run,
  .trap   = os_trap,
  .on_irq = os_on_irq,
};
