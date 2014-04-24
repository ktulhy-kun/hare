#ifndef ADDFILE_H
#define ADDFILE_H

#include "archfiles.h"
#include "lowfile.h"
#include "hare.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>



#define OPEN_FILE_ERROR (1234)

/* если recurse == 1, то начинает рекурсивно обходить папки и добавлять файлы из них. иначе пнх! */
int addFiles2Arch(Context context, int recurse);
int addFile2Arch(ArchFileInfo archFileInfo, const char* nameArchive);

void coding(char* huffTree, char* bytesForCoding, int countBytesForCoding, char* codingBits, int* countCodingBits);

void recurseAddFiles2Arch(char * path);
#endif // ADDFILE_H
