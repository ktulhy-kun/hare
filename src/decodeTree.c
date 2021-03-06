#include "decodeTree.h"

unsigned char masks[8] = {128,64,32,16,8,4,2,1};


void _setbit(char *str, int pos, bool bit) {
  if (bit) {
    str[pos/8] = str[pos/8] | masks[pos%8];
  } else {
    str[pos/8] = str[pos/8] & (~masks[pos%8]);
  }
}


bool _getbit(char *str, int pos) {
  return !!(str[pos / 8] & masks[pos % 8]);
}

unsigned char _getchar(char *str, int pos) {
  char ch = 0;
  ch = ((unsigned char)str[pos / 8] << (pos % 8)) | ((unsigned char)(str[pos / 8 + 1]) >> (8 - (pos % 8)));
  return ch;
}

Tree *decodeTree(char *str, int len) {
  Tree *root = calloc(1,sizeof(Tree));
  Tree *elem = NULL;
  Tree **freeTwig = NULL;
  int pos = 1;

  elem = root;

  root->isTwig = true;

  while (pos < len) {
    while ((elem->left) && (elem->right)) {
      elem = elem->parent;
    }

    freeTwig = ((elem->left) ? &(elem->right) : &(elem->left));

    *freeTwig = calloc(1,sizeof(Tree));
    (*freeTwig)->parent = elem;

    if (_getbit(str,pos)) { // twig
      (*freeTwig)->isTwig = 1;
      elem = *freeTwig;
    } else {
      (*freeTwig)->isTwig = 0;
      (*freeTwig)->sym = _getchar(str,pos+1);
      pos+=8;
    }
    pos++;
  }

  return root;
}
