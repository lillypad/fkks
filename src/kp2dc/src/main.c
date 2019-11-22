#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "b64/b64.h"

#define MAX_STR_LEN 1024

void help_menu(){
  char menu[] =

    "/-------------------------------------------------------\\\n"
    "|              KPot v2.0 Decrypt/Encrypt                |\n"
    "|-------------------------------------------------------|\n"
    "|  -h, --help             Print this Help Menu          |\n"
    "|  -a, --all     [sample] Dump All Strings              |\n"
    "|  -k, --key     [sample] Dump C2 Server Decryption Key |\n"
    "|  -d, --domain  [sample] Dump C2 Domain                |\n"
    "|  --set-key     [keystr] Set Decryption Key            |\n"
    "|  -c, --c2      [base64] Decrypt C2 base64             |\n"
    "|  -o, --output  [_file_] Output to File                |\n"
    "|  -i, --input   [_file_] Input File                    |\n"
    "|  --std-in               Use stdin for -c, --c2        |\n"
    "|-------------------------------------------------------|\n"
    "| Company: GoSecure TITAN                               |\n"
    "| Author : Lilly Chalupowski                            |\n"
    "\\-------------------------------------------------------/\n";
  printf("%s", menu);
}

int iFileSize(FILE *fp){
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

int32_t *pReadFile(char* fn){
  int size;
  FILE *pFile = fopen(fn, "rb");
  if (pFile == NULL){
    fprintf(stderr, "%s %s\n", fn, strerror(errno));
    return NULL;
  }
  size = iFileSize(pFile);
  fseek(pFile, 0, SEEK_SET);
  void *ed0 = malloc(size);
  fread(ed0, 1, size, pFile);
  fclose(pFile);
  return ed0;
}

int32_t iGetFileSize(char* fn){
  int32_t size;
  FILE *fp = fopen(fn, "rb");
  size = iFileSize(fp);
  fclose(fp);
  return size;
}

bool bWriteFile(char *fn, void *pData, int iDataSize){
  FILE *pFile = fopen(fn, "wb");
  if (pFile == NULL){
    fprintf(stderr, "%s\n", strerror(errno));
    return false;
  }
  fwrite(pData, iDataSize, 1, pFile);
  return true;
}

// Function Definitions
int32_t *sd0(int32_t *sb, int32_t sid, int32_t *bin);
int32_t *c2dec(int32_t *data, int32_t data_size, int32_t *key, int32_t key_length);

int main(int argc, char **argv){
  bool bStdIn = false;
  char *pOutputFile= NULL;
  char *pKey = NULL;
  bool bAllStrings = false;
  bool bDomain = false;
  bool bExtractKey = false;
  bool bDecryptFile = false;
  int32_t iSid = -1;
  int32_t iSidLow = 0;
  int32_t iSidHigh = 182;
  int32_t iInputFileSize = 0;
  void *pInputFile = NULL;
  if (argc <= 1){
    help_menu();
    fprintf(stderr, "Not enough arguments...\n");
    return 1;
  }
  for (int a = 0; a < argc; a++){
    if (strcmp(argv[a], "--crypt-stdin") == 0){
      bStdIn = true;
    }
    if (strcmp(argv[a], "--sid-low") == 0){
      iSidLow = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "--sid-high") == 0){
      iSidHigh = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "--decrypt-sid") == 0){
      iSid = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "-k") == 0 || strcmp(argv[a], "--key") == 0){
      pKey = argv[a+1];
    }
    if (strcmp(argv[a], "-i") == 0 || strcmp(argv[a], "--input") == 0){
      pInputFile = pReadFile(argv[a+1]);
      if (pInputFile == NULL){
        return 1;
      }
      iInputFileSize = iGetFileSize(argv[a+1]);
    }
    if (strcmp(argv[a], "-o") == 0 || strcmp(argv[a], "--output") == 0){
      pOutputFile = argv[a+1];
    }
    if (strcmp(argv[a], "-h") == 0 || strcmp(argv[a], "--help") == 0){
      help_menu();
      return 0;
    }
    if (strcmp(argv[a], "--decrypt-strings") == 0){
      bAllStrings = true;
    }
    if (strcmp(argv[a], "--extract-domain") == 0){
      bDomain = true;
    }
    if (strcmp(argv[a], "--extract-key") == 0){
      bExtractKey = true;
    }
    if (strcmp(argv[a], "--crypt-file") == 0){
      bDecryptFile = true;
    }
  }
  if (iSidLow > iSidHigh || iSid < -1){
    fprintf(stderr, "String ID values invalid.\n");
    return 1;
  }
  if (bAllStrings == true && pInputFile!= NULL){
    void *sb = malloc(MAX_STR_LEN);
    for (int i = iSidLow; i < iSidHigh; i++){
      memset(sb, 0, MAX_STR_LEN);
      sd0(sb, i, pInputFile);
      printf("sid:0x%x", i);
      hex_dump("", sb, strlen(sb) + 1);
    }
    free(sb);
    free(pInputFile);
    return 0;
  }
  if (bExtractKey == true && pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, 2, pInputFile);
    printf("%s\n", (char *)sb);
    free(sb);
    free(pInputFile);
    return 0;
  }
  if (bDomain == true && pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, 0, pInputFile);
    printf("%s\n", (char *)sb);
    free(sb);
    free(pInputFile);
    return 0;
  }
  if (bStdIn == true && pKey != NULL){
    char *txt = malloc(MAX_STR_LEN);
    memset(txt, 0, MAX_STR_LEN);
    fgets(txt, MAX_STR_LEN, stdin);
    printf("C2 Server Base64 String:\n");
    printf("%s\n", txt);
    void *dec = base64_decode(txt);
    hex_dump("C2 Base64 Decoded String", dec, base64_output_size(txt));
    c2dec(dec, base64_output_size(txt), (int32_t *)pKey, strlen(pKey));
    hex_dump("decrypted", dec, base64_output_size(txt));
    free(txt);
    free(dec);
    return 0;
  }
  if (iSid != -1 && iSid >= 0 && pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, iSid, pInputFile);
    printf("%s\n", (char *)sb);
    free(sb);
    free(pInputFile);
    return 0;
  }
  if (bDecryptFile == true && pInputFile != NULL && pKey != NULL){
    hex_dump("encrypted", pInputFile, iInputFileSize);
    c2dec(pInputFile, iInputFileSize, (int32_t *)pKey, strlen(pKey));
    hex_dump("decrypted", pInputFile, iInputFileSize);
    if (pOutputFile != NULL){
      if (bWriteFile(pOutputFile, pInputFile, iInputFileSize) == false){
        return 1;
      }
    }
    free(pInputFile);
    return 0;
  }
  help_menu();
  fprintf(stderr, "Not enough arguments...\n");
  return 1;
}
