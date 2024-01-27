#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "ak_extern.h"

#define FIFOPATH "/tmp/ptz.daemon"

int main(int argc, char **argv) {
  struct stat buf;
  if (stat(FIFOPATH, &buf) != 0) {
    printf("%s - creating FIFO in /tmp...\n", argv[0]);
    int ret_mkfifo = mkfifo(FIFOPATH, 0700);
    if (ret_mkfifo != 0) {
      printf("could not create fifo @ %s\n", FIFOPATH);
      return 1;
    }
  }
  printf("%s - init ptz driver (anyka)\n", argv[0]);
  // ak_drv_ptz_open();
  // ak_drv_ptz_check_self(1);
  printf("%s - running as daemon now. awaiting commands in FIFO\n", argv[0]);

  /*
   ak_drv_ptz_set_angle_rate(0x40000000,0x40000000);
   ak_drv_ptz_set_degree(0x168,0xc0);
   ak_drv_ptz_turn_to_pos(atoi(argv[1]),atoi(argv[2]));
    */

  FILE *fd = fopen(FIFOPATH, "r+");
  char buff[100];
  char *cmd;
  char* param1;
  char* param2;
  int shuttingDown = 0;
  while (shuttingDown == 0) {
    // Open FIFO for write only
    buff[0] = 0;
    fgets(buff, 100, fd);

    if (strlen(buff) > 0) {
      printf("%s - received something in the FIFO:\n%s\n", argv[0], buff);
    }
    if (buff[0] == '0') {
      printf("%s - init ptz driver (anyka)", argv[0]);
      ak_drv_ptz_open();
      ak_drv_ptz_check_self(0);
      ak_drv_ptz_set_degree(0x168,0xc0);
      ak_drv_ptz_set_angle_rate(0x40000000,0x40000000);
    } else {
      printf("more than 1 argument (cmd) -> assuming 3 (cmd, p1, p2)\n");
      //cmd = malloc(2);
      cmd = strtok(buff," ");
      param1 = strtok(NULL, " ");
      param2 = strtok(NULL, " ");
      
      //sscanf(buff, "%s %llu %llu", cmd, param1, param2);
      printf("%s - cmd %s - parameters: %s %s\n", argv[0],  cmd, param1, param2);

      if (strcmp(cmd,"setar") == 0) {
        printf("%s - set angle rate - parameters: %ll %ll\n", argv[0], atoll(param1), atoll(param2));
        ak_drv_ptz_set_angle_rate(atoll(param1), atoll(param2));
      }
      if (strcmp(cmd,"setdeg") == 0) {
        printf("%s - set degree - parameters: %ll %ll\n", argv[0], atoll(param1), atoll(param2));
        ak_drv_ptz_set_degree(atoll(param1), atoll(param2));
      }
      if (strcmp(cmd,"t2p") == 0){
        printf("%s - turn to position - parameters: %ll %ll\n", argv[0], atoll(param1), atoll(param2));
        ak_drv_ptz_turn_to_pos(atoll(param1), atoll(param2));
      }
      if (strcmp(cmd,"t2p") == 0){
        printf("%s - turn to position - parameters: %ll %ll\n", argv[0], atoll(param1), atoll(param2));
        ak_drv_ptz_turn(atoll(param1), atoll(param2), 0);
      }

      free(cmd);
    }
    sleep(1);
  }
  fclose(fd);
  return 0;
}
