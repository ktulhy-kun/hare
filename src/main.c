#include "logging.h"
#include "hare.h"
#include "init.h"
#include "extract.h"
#include <stdio.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include "crc.h"

GLOBAL_LOGGING

int main(int argc, char *argv[], char *env[])
{
  Context *cnt = (Context *)malloc(sizeof(Context));
  FILE *f = NULL;
  int _error = 0;
  INIT_LOGGING;
  LOGGING_FUNC_START;
  INFO(L"Programm started");
  INFO(L"Тест русского языка");
  printf("Hello World!\n");
  INFO(L"Programm ended");
  cnt->argv = argv;
  cnt->argc = argc;
  cnt->env  = env;
  _error = parseArgs(&cnt);

  if (0 != _error) {
    printHelp();
  } else {
    switch(cnt->keys) {
    case 0x0:
      ;
      break;
    case 0x1:
      ;
      break;
    case 0x2:
      ;
      break;
    case 0x4:
      ;
      break;
    case 0x8:
      f = fopen(argv[2], "rb");
      extractFiles(f, cnt);
      fclose(f);
      break;
    case 0x10:
      ;
      break;
    default:
      printHelp();
      break;
    }
  }

//  crcInit();
//  printf("%X\n",crcFast("test",4));
//  printf("%X\n",crcFast("lol",3));
//  crcInit();
//  printf("%X\n",crcFast("lol",3));

  LOGGING_FUNC_STOP;
  DEINIT_LOGGING;
  printf("Programm exit!\n");
  return 0;
}

