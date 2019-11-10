#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "b64.h"

#define MAX_STR_LEN 1024

void help_menu(){
  char menu[] =

    "/-------------------------------------------------------\\\n"
    "|              KPot v2.0 Decryptor                      |\n"
    "|-------------------------------------------------------|\n"
    "|  -h, --help             Print this Help Menu          |\n"
    "|  -a, --all     [sample] Dump All Strings              |\n"
    "|  -k, --key     [sample] Dump C2 Server Decryption Key |\n"
    "|  -d, --domain  [sample] Dump C2 Domain                |\n"
    "|-------------------------------------------------------|\n"
    "| Company: GoSecure TITAN                               |\n"
    "| Author : Lilly Chalupowski                            |\n"
    "\\-------------------------------------------------------/\n";
  printf("%s", menu);
}

int fsize(FILE *fp){
  int prev=ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int sz=ftell(fp);
  fseek(fp,prev,SEEK_SET);
  return sz;
}

void hex_dump(char *desc, void *addr, int len) {
  int i;
  unsigned char buff[17];
  unsigned char *pc = (unsigned char*)addr;
  if (desc != NULL){
    printf("%s:\n", desc);
  }
  if (len == 0) {
    printf("  ZERO LENGTH\n");
    return;
  }
  if (len < 0) {
    printf("  NEGATIVE LENGTH: %i\n",len);
    return;
  }
  for (i = 0; i < len; i++) {
    if ((i % 16) == 0) {
      if (i != 0){
        printf("  %s\n", buff);
      }
      printf("  %04x ", i);
    }
    printf(" %02x", pc[i]);
    if ((pc[i] < 0x20) || (pc[i] > 0x7e)){
      buff[i % 16] = '.';
    } else {
      buff[i % 16] = pc[i];
    }
    buff[(i % 16) + 1] = '\0';
  }
  while ((i % 16) != 0) {
    printf("   ");
    i++;
  }
  printf("  %s\n", buff);
}

int32_t *read_sample(char* fn){
  int size;
  FILE *fp = fopen(fn, "rb");
  size = fsize(fp);
  fseek(fp, 0, SEEK_SET);
  void *ed0 = malloc(size);
  fread(ed0, 1, size, fp);
  fclose(fp);
  return ed0;
}

// Function Definitions
int32_t *sd0(int32_t *sb, int32_t sid, int32_t *bin);

int main(int argc, char **argv){
  if (argc <= 1){
    help_menu();
    return 0;
  }
  for (int a = 0; a < argc; a++){
    if (strcmp(argv[a], "-h") == 0 ||
        strcmp(argv[a], "--help") == 0){
      help_menu();
      return 0;
    }
    if (strcmp(argv[a], "-a") == 0 ||
        strcmp(argv[a], "--all") == 0){
      void *bin = read_sample(argv[a+1]);
      void *sb = malloc(MAX_STR_LEN);
      for (int i = 0; i < 183; i++){
        memset(sb, 0, MAX_STR_LEN);
        sd0(sb, i, bin);
        printf("sid:0x%x", i);
        hex_dump("", sb, strlen(sb) + 1);
      }
      free(sb);
      free(bin);
      return 0;
    }
    if (strcmp(argv[a], "-d") == 0 ||
        strcmp(argv[a], "--domain") == 0){
      void *bin = read_sample(argv[a+1]);
      void *sb = malloc(MAX_STR_LEN);
      memset(sb, 0, MAX_STR_LEN);
      sd0(sb, 0, bin);
      printf("KPot v2.0 C2 Domain: %s\n", (char *)sb);
      free(sb);
      free(bin);
      return 0;
    }
    if (strcmp(argv[a], "-k") == 0 ||
        strcmp(argv[a], "--key") == 0){
      void *bin = read_sample(argv[a+1]);
      void *sb = malloc(MAX_STR_LEN);
      memset(sb, 0, MAX_STR_LEN);
      sd0(sb, 2, bin);
      printf("KPot v2.0 C2 Server Decryption Key: %s\n", (char *)sb);
      free(sb);
      free(bin);
    }
  }
  return 0;
}
