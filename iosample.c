#include "types.h"
#include "user.h"

int main() {
  for (int i = 0; i < 5; i++) {
    printf(1, "I/O-bound process running (step %d)\n", i);
    sleep(10);  // Let it sleep a bit between prints
  }
  exit();
}
