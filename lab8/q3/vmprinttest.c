#include "types.h"
#include "user.h"

int main(void){
  vmprint();
  sbrk(8*4096);
  vmprint();
  sbrk(-4096);
  vmprint();
  exit();
}