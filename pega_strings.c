#include <stdio.h>
#include <stdbool.h>

#define MIN_SEQ_LENGTH 4

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Uso: %s <nome_do_arquivo>\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    perror("Erro ao abrir o arquivo");
    return 1;
  }

  unsigned char byte;
  int count = 0;
  bool in_sequence = false;
  long position = 0;
  long start_position = 0;
  char buffer[256];
  int buffer_index = 0;

  printf("Strings encontradas:\n");

  while (fread(&byte, sizeof(byte), 1, fp)) {
    if (byte >= 0x20 && byte <= 0x7e) {
      if (!in_sequence) {
        in_sequence = true;
        count = 0;
        start_position = position;
        buffer_index = 0;  // Reset buffer index
      }
      buffer[buffer_index++] = byte;
      count++;
    } else {
      if (in_sequence) {
        if (count >= MIN_SEQ_LENGTH) {
          buffer[buffer_index] = '\0';  // Null-terminate the string
          printf("[%lx] - %s\n", start_position, buffer);
        }
        in_sequence = false;
      }
    }
    position++;
  }

  if (in_sequence && count >= MIN_SEQ_LENGTH) {
    buffer[buffer_index] = '\0';  // Null-terminate the string
    printf("[%lx] - %s\n", start_position, buffer);
  }

  fclose(fp);

  return 0;
}
