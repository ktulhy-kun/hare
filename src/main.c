#include "logging.h"
#include "addfiles2arch.h"
#include <stdio.h>
#include <time.h>

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  INIT_LOGGING;
  FILE *f = fopen("test", "wb");
  FileInfo info;
  info.name = "info.txt";
  info.sizeName = 8;
  info.timeLastModification = 10007463880;
  info.size = 1025;
  writeFileHeader(f, &info, 1025, 0, 0, 2, "12", 0, 1);

  fclose(f);
  DEINIT_LOGGING;
  printf("Programm exit!\x43\n");
  return 0;
}

