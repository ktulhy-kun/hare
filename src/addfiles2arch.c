#include "addfiles2arch.h"
/* 2014, Korovin Viktor <sarvash.inc@gmail.com> Потоковая запись файла
 * Будьте осторожны с writeBytes! Не пытайтесь одновременно писать в два файла, поведение будет не определено
 */


int _writeBytes(FILE *f, char *buf, size_t k_bytes) {
  size_t wr_bytes = 0;
  IO("Write bytes on file, bs=%d", BUF_LEN)
  wr_bytes = fwrite(buf, k_bytes, 1, f);
  IO("Wrote %zdx%zd bytes", wr_bytes, k_bytes)
  if (k_bytes & (!wr_bytes)) {
    WARNING("Writing error")
    return IO_WRITE_ERROR;
  }
  return 0;
}


int writeNBytes(FILE *f, int64_t N, char *str, int drop) {
  static char buf[BUF_LEN];
  static uint64_t pos;
  int64_t ext_pos = 0;
  int wr_result = 0;

  IO("Add string to buffer")

  while (ext_pos < N) {
    /* TODO: сделать нормульную копирование строк через strcpy */
    buf[pos] = str[ext_pos];
    if (++pos == BUF_LEN) {
      IO("Drop buffer")
      pos = 0;
      wr_result = _writeBytes(f, buf, BUF_LEN);
      if (wr_result)
        return wr_result;
    }
    ext_pos++;
  }

  if (drop) {
    IO("Forsed drop buffer")
    wr_result = _writeBytes(f, buf, pos);
  }

  return wr_result;
}

int writeuInt64(FILE *f, int64_t num, int drop) {
  /* возможны проблемы с low big endian */
  char tmp[sizeof(int64_t)];
  size_t i = 0;
  for (i=0; i < sizeof(int64_t); i += 1) {
    tmp[i] = ((char *) &num)[i];
    printf("%zu:%d\n",i,(unsigned char) tmp[i]);
  }
  return writeNBytes(f, i, tmp, drop);
}

int writeChar(FILE *f, char ch, int drop) {
  return writeNBytes(f, 1, &ch, drop);
}

int writeFileHeader(FILE *f, \
                    FileInfo *file, \
                    int64_t archSize, \
                    char endUnusedBits, \
                    char flags, \
                    int64_t haffTreeSize, \
                    char *haffTree, \
                    int64_t HeaderCheckSum,\
                    int drop) {
  writeNBytes(f, SIGNATURE_LEN, SIGNATURE, 0);
  writeuInt64(f, file->sizeName, 0);
  writeNBytes(f, file->sizeName, file->name, 0);
  writeuInt64(f, file->timeLastModification, 0);
  writeuInt64(f, file->size, 0);
  writeuInt64(f, archSize, 0);
  writeChar(f, endUnusedBits, 0);
  writeChar(f, flags, 0);
  writeuInt64(f, haffTreeSize, 0);
  writeNBytes(f, haffTreeSize, haffTree, 0);
  writeuInt64(f, HeaderCheckSum, drop);

  return 0;
}
