#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "ak_extern.h"

int do_not_use(){
  sem_t* b;
  int a = sem_init(b,1,0);

}

int main(int argc, char** argv){
  printf("%s x y\n",argv[0]);
  do_not_use();
  printf("ok\n");
  ak_drv_ptz_open();

/*  printf("ok\n");
  ak_drv_ptz_set_angle_rate(0x40000000,0x40000000);

  printf("ok\n");
  ak_drv_ptz_set_degree(0x168,0xc0);
  */

  printf("ok\n");
  ak_drv_ptz_check_self(1);

  printf("ok\n");
  ak_drv_ptz_turn_to_pos(atoi(argv[1]),atoi(argv[2]));

  printf("ok\n");
  return 0;
}
