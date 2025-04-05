#include "types.h"
#include "user.h"

int main(void) {
    while(1) {
        yield();  // give up CPU voluntarily
    }
    exit();
}
