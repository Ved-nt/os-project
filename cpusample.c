#include "types.h"
#include "user.h"

int main() {
  volatile int j = 0;
  for (int i = 0; i < 100000000; i++) {
    j += i;
    if (i % 1000000 == 0)
      sleep(1);  // Give other processes time
  }
  printf(1, "CPU-bound process finished computation\n");
  exit();
}
