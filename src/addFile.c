#include  "addFile.h"

#define WRITE_HEADER_

static char* concatenateStrings(const char * str1, const char * str2)
{
    int lengthStr1 = strlen(str1);
    int i;
    char * result = (char*)calloc((strlen(str1) + strlen(str2) + 1), sizeof(char));

    if (NULL == result)
    {
        return NULL;
    }

    for (i = 0; i < lengthStr1; i++)
    {
        result[i] = str1[i];
    }
    for (i = 0; i < strlen(str2); i++)
    {
        result[i + lengthStr1] = str2[i];
    }

    return result;
}


int addFiles2Arch(Context context, int recurse)
{
    int i;

    for (i = 0; i < context.argc - 3; i++)
    {
        struct stat fileInfo;
        stat(context.workFiles[i], &fileInfo);

        if (S_ISREG(fileInfo.st_mode))
        {
            ArchFileInfo archFileInfo;
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                return -1;

            getFileInfo(context.workFiles[i], archFileInfo.fileInfo);

            addFile2Arch(archFileInfo, context.archName);
        }
        else if (recurse && S_ISDIR(fileInfo.st_mode))
        {
            recurseAddFiles2Arch(context.workFiles[i], context);
        }
    }

    return 0;
}

int addFile2Arch(ArchFileInfo archFileInfo, const char* nameArchive)
{
    FILE* archive;
    FILE* file;

    struct Code codes[COUNT_SYMBOLS] = {0, 0};

    const int sizeBlock = 30000;
    int sizeReadBlock;
    unsigned char block[sizeBlock];
    unsigned char codingBlock[sizeBlock];

    int countCodingBits = 0;
    char byteForWrite;
    char countUsedBits = 0;
    char partialByte = 0;
    long positionHeaderInFile;


    char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};


    strcpy(block, "");


    file     = fopen(archFileInfo.fileInfo->name,  "rb");
    if (!file)
        return OPEN_FILE_ERROR;


    archive  = fopen(nameArchive, "rb+");
    if (!archive)
    {
        archive  = fopen(nameArchive, "wb");
        if (!archive)
            return OPEN_ARCHIVE_ERROR;

        fclose(archive);
        archive  = fopen(nameArchive, "rb+");

    }

    createCodes(codes, createTree(createList(createTableFrequencies(file))), "");
//    printCodes(codes);

    fseek(file, 0L, SEEK_SET);
    fseek(archive, 0L, SEEK_END);
    positionHeaderInFile = ftell(archive);


    archFileInfo.dataSize = 0;
    archFileInfo.endUnusedBits = 0;
    archFileInfo.haffTreeSize = 0;
    archFileInfo.haffTree = (char*)calloc(archFileInfo.haffTreeSize + 1, sizeof(char));
    strcpy(archFileInfo.haffTree, "");
    archFileInfo.HeaderCheckSum = 0;

#ifdef WRITE_HEADER
    writeFileHeader(archive, &archFileInfo);
    dropWrBytes(archive);
#endif

    while (!feof(file))
    {
        int i;

        sizeReadBlock = fread(block, sizeof(char), sizeBlock, file);

        coding(codes, block, sizeBlock, codingBlock, &countCodingBits);
        {
            int count = 0, j;
            for (i = 0; i < strlen(block); i++)
            {
                for (j = 0; j < codes[block[i]].size; j++)
                {
                    printf("%c", codes[block[i]].code[j]);
                    count++;

                    if (!(count % 8))
                        printf(" ");
                }
            }
        }
        printf("\n----------------------------------------\n");
//        getchar();

        for(i = 0; i < countCodingBits / 8; i++)
        {
            byteForWrite = partialByte;
            byteForWrite |= ((codingBlock[i] >> countUsedBits) & right1[8 - countUsedBits]);

            partialByte = ((codingBlock[i] << (8 - countUsedBits)) & left1[countUsedBits]);
            writeChar(archive, byteForWrite);
        }

        if (countCodingBits % 8)
        {
            if ((countUsedBits + countCodingBits % 8) >= 8)
            {
                byteForWrite = partialByte;
                byteForWrite |= ((codingBlock[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits]);

                partialByte = ((codingBlock[countCodingBits / 8] << (8 - countUsedBits)) & left1[countUsedBits]);

                writeChar(archive, byteForWrite);
            }
            else
            {
                partialByte |= ((codingBlock[countCodingBits / 8] >> countUsedBits) & right1[8 - countUsedBits] & left1[countUsedBits + countCodingBits % 8]);
                countUsedBits += countCodingBits % 8;
            }
        }


        strcpy(block, "");
    }

    if (countUsedBits)
        writeChar(archive, partialByte);
    dropWrBytes(archive);


    archFileInfo.endUnusedBits = 8 - countUsedBits;

#ifdef WRITE_HEADER
    fseek(archive, positionHeaderInFile, SEEK_SET);
    writeFileHeader(archive, &archFileInfo);
    dropWrBytes(archive);
#endif

    fclose(archive);
    fclose(file);

    return 0;
}


void coding_(char* huffTree, char* bytesForCoding, int countBytesForCoding, char* codingBits, int* countCodingBits)
{
    memcpy(codingBits, bytesForCoding, countBytesForCoding);
    *countCodingBits = countBytesForCoding * 8;
}

void recurseAddFiles2Arch(char * path, Context context)
{
    DIR * dir = NULL;
    struct dirent * dir_entry;
    ArchFileInfo archFileInfo;
    static int depth = 0;
    static char buffer[1000000] = "";

    if (NULL == path)
        return;

    dir = opendir(path);

    if (NULL == dir)
    {
        depth--;
        return;
    }

    strcpy(buffer, path);

    while ((dir_entry = readdir(dir)) != NULL)
    {
        if (dir_entry->d_type == DT_REG)
        {
            // add file to arch
            archFileInfo.fileInfo = (FileInfo*)malloc(sizeof(FileInfo));

            if (NULL == archFileInfo.fileInfo)
                return;

            getFileInfo(concatenateStrings(buffer, dir_entry->d_name), archFileInfo.fileInfo);
            addFile2Arch(archFileInfo, context.archName);
        }
        else if (dir_entry->d_type == DT_DIR)
        {
            if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                depth++;
                strcat(buffer, dir_entry->d_name);
                strcat(buffer, "/");
                recurseAddFiles2Arch(buffer, context);
            }
        }

    }

    if (depth > 0)
    {
        int i;
        buffer[strlen(buffer) - 1] = '\0';
        for (i = strlen(buffer) - 1; buffer[i] != '/'; i--)
            buffer[i] = '\0';
    }
    depth--;
}


