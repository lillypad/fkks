#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "b64/b64.h"

#define MAX_STR_LEN 1024
#define VERSION "v1.0"
#define MODULE_SUCCESS 0
#define MODULE_FAIL 1
#define MODULE_SKIP 2

void help_menu(){
  char menu[] =

    "/----------------------------------------------------------\\\n"
    "|              KPot v2.0 Decrypt/Encrypt                   |\n"
    "|----------------------------------------------------------|\n"
    "|  -h, --help                Print this Help Menu          |\n"
    "|  --decrypt-strings         Dump All Strings              |\n"
    "|  -q, --quiet               Silence Output                |\n"
    "|  --extract-key             Dump C2 Server Decryption Key |\n"
    "|  --extract-domain          Dump C2 Domain                |\n"
    "|  --decrypt-sid      [#]    Decrypt String by ID          |\n"
    "|  -k, --key        [keystr] Crypt Key String              |\n"
    "|  --crypt-file              Crypt File                    |\n"
    "|  -o, --output     [_file_] Output to File                |\n"
    "|  -i, --input      [_file_] Input File                    |\n"
    "|  --crypt-stdin             StdIn (base64 only)           |\n"
    "|  --examples                Show Examples                 |\n"
    "|  -v, --version             Show Version                  |\n"
    "|----------------------------------------------------------|\n"
    "| Company: GoSecure TITAN                                  |\n"
    "| Author : Lilly Chalupowski                               |\n"
    "\\----------------------------------------------------------/\n";
  printf("%s", menu);
}

void examples(){
  char examples[] =
    "kp2dc --extract-key -i sample.bin\n"
    "kp2dc --extract-domain -i sample.bin\n"
    "kp2dc --decrypt-strings -i sample.bin\n"
    "kp2dc --examples\n"
    "kp2dc --crypt-file -i data.bin --key [KEY] -o out.bin\n"
    "cat c2.b64 | kp2dc --crypt-stdin --key [KEY] -o out.bin\n"
    "kp2dc --decrypt-strings --sid-low 0 --sid-high 182 -i sample.bin\n"
    "kp2dc --decrypt-sid [SID#] -i sample.bin\n";
  printf("%s", examples);
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

struct args_t{
  bool bStdIn;
  char *pOutputFile;
  char *pKey;
  bool bAllStrings;
  bool bDomain;
  bool bExtractKey;
  bool bDecryptFile;
  int32_t iSid;
  int32_t iSidLow;
  int32_t iSidHigh;
  int32_t iInputFileSize;
  void *pInputFile;
  bool bQuiet;
};


struct args_t *pArgsInit(){
  struct args_t *args = malloc(sizeof(struct args_t));
  args->bStdIn = false;
  args->pOutputFile = NULL;
  args->pKey = NULL;
  args->bAllStrings = NULL;
  args->bDomain = false;
  args->bExtractKey = false;
  args->bDecryptFile = false;
  args->iSid = -1;
  args->iSidLow = 0;
  args->iSidHigh = 182;
  args->iInputFileSize = 0;
  args->pInputFile = NULL;
  args->bQuiet = false;
  return args;
}

void ArgsCleanup(struct args_t *pArgs){
  if (pArgs->pInputFile != NULL){
    free(pArgs->pInputFile);
  }
  free(pArgs);
}

int iModuleCheck(int iModuleStatus, struct args_t *pArgs){
  switch (iModuleStatus){
  case MODULE_SUCCESS:
    ArgsCleanup(pArgs);
    exit(EXIT_SUCCESS);
    break;
  case MODULE_FAIL:
    ArgsCleanup(pArgs);
    exit(EXIT_FAILURE);
    break;
  case MODULE_SKIP:
    break;
  }
  return MODULE_SKIP;
}

int iArgsParse(int argc, char **argv, struct args_t *pArgs){
  if (argc <= 1){
    help_menu();
    fprintf(stderr, "Not enough arguments...\n");
    return 1;
  }
  for (int a = 0; a < argc; a++){
    if (strcmp(argv[a], "--examples") == 0){
      examples();
    }
    if (strcmp(argv[a], "-v") == 0 || strcmp(argv[a], "--version") == 0){
      printf("%s\n", VERSION);
    }
    if (strcmp(argv[a], "--quit") == 0 ||
        strcmp(argv[a], "-q") == 0){
      pArgs->bQuiet = true;
    }
    if (strcmp(argv[a], "--crypt-stdin") == 0){
      pArgs->bStdIn = true;
    }
    if (strcmp(argv[a], "--sid-low") == 0){
      pArgs->iSidLow = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "--sid-high") == 0){
      pArgs->iSidHigh = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "--decrypt-sid") == 0){
      pArgs->iSid = atoi(argv[a+1]);
    }
    if (strcmp(argv[a], "-k") == 0 ||
        strcmp(argv[a], "--key") == 0){
      pArgs->pKey = argv[a+1];
    }
    if (strcmp(argv[a], "-i") == 0 ||
        strcmp(argv[a], "--input") == 0){
      pArgs->pInputFile = pReadFile(argv[a+1]);
      if (pArgs->pInputFile == NULL){
        return 1;
      }
      pArgs->iInputFileSize = iGetFileSize(argv[a+1]);
    }
    if (strcmp(argv[a], "-o") == 0 ||
        strcmp(argv[a], "--output") == 0){
      pArgs->pOutputFile = argv[a+1];
    }
    if (strcmp(argv[a], "-h") == 0 ||
        strcmp(argv[a], "--help") == 0){
      help_menu();
      return 0;
    }
    if (strcmp(argv[a], "--decrypt-strings") == 0){
      pArgs->bAllStrings = true;
    }
    if (strcmp(argv[a], "--extract-domain") == 0){
      pArgs->bDomain = true;
    }
    if (strcmp(argv[a], "--extract-key") == 0){
      pArgs->bExtractKey = true;
    }
    if (strcmp(argv[a], "--crypt-file") == 0){
      pArgs->bDecryptFile = true;
    }
  }
  if (pArgs->iSidLow > pArgs->iSidHigh || pArgs->iSid < -1){
    fprintf(stderr, "String ID values invalid.\n");
    return 1;
  }
  return 0;
}

int32_t *sd0(int32_t *sb, int32_t sid, int32_t *bin);
int32_t *c2dec(int32_t *data, int32_t data_size, int32_t *key, int32_t key_length);

void iModuleAllStrings(struct args_t *pArgs){
  if (pArgs->bAllStrings == true && pArgs->pInputFile!= NULL){
    void *sb = malloc(MAX_STR_LEN);
    for (int i = pArgs->iSidLow; i < pArgs->iSidHigh; i++){
      memset(sb, 0, MAX_STR_LEN);
      sd0(sb, i, pArgs->pInputFile);
      printf("sid:0x%x", i);
      hex_dump("", sb, strlen(sb) + 1);
    }
    free(sb);
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}

void iModuleExtractKey(struct args_t *pArgs){
  if (pArgs->bExtractKey == true && pArgs->pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, 2, pArgs->pInputFile);
    if (pArgs->bQuiet != true){
      printf("%s\n", (char *)sb);
    }
    if (pArgs->pOutputFile != NULL){
      if (bWriteFile(pArgs->pOutputFile, sb, strlen(sb)) == false){
        iModuleCheck(MODULE_FAIL, pArgs);
      }
    }
    free(sb);
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}

void iModuleExtractDomain(struct args_t *pArgs){
  if (pArgs->bDomain == true && pArgs->pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, 0, pArgs->pInputFile);
    if (pArgs->bQuiet != true){
      printf("%s\n", (char *)sb);
    }
    if (pArgs->pOutputFile != NULL){
      if (bWriteFile(pArgs->pOutputFile, sb, strlen(sb)) == false){
        iModuleCheck(MODULE_FAIL, pArgs);
      }
    }
    free(sb);
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}

void iModuleStdIn(struct args_t *pArgs){
  if (pArgs->bStdIn == true && pArgs->pKey != NULL){
    char *txt = malloc(MAX_STR_LEN);
    memset(txt, 0, MAX_STR_LEN);
    fgets(txt, MAX_STR_LEN, stdin);
    if (pArgs->bQuiet != true){
      printf("C2 Server Base64 String:\n");
      printf("%s\n", txt);
    }
    void *dec = base64_decode(txt);
    if (pArgs->bQuiet != true){
      hex_dump("C2 Base64 Decoded String", dec, base64_output_size(txt));
    }
    c2dec(dec, base64_output_size(txt), (int32_t *)pArgs->pKey, strlen(pArgs->pKey));
    if (pArgs->bQuiet != true){
      hex_dump("decrypted", dec, base64_output_size(txt));
    }
    if (pArgs->pOutputFile != NULL){
      if (bWriteFile(pArgs->pOutputFile, dec, base64_output_size(txt)) == false){
        iModuleCheck(MODULE_FAIL, pArgs);
      }
    }
    free(txt);
    free(dec);
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}

void iModuleSID(struct args_t *pArgs){
  if (pArgs->iSid != -1 && pArgs->iSid >= 0 && pArgs->pInputFile != NULL){
    void *sb = malloc(MAX_STR_LEN);
    memset(sb, 0, MAX_STR_LEN);
    sd0(sb, pArgs->iSid, pArgs->pInputFile);
    if (pArgs->bQuiet != true){
      printf("%s\n", (char *)sb);
    }
    free(sb);
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}

void iModuleCryptFile(struct args_t *pArgs){
  if (pArgs->bDecryptFile == true && pArgs->pInputFile != NULL && pArgs->pKey != NULL){
    if (pArgs->bQuiet != true){
      hex_dump("encrypted", pArgs->pInputFile, pArgs->iInputFileSize);
    }
    c2dec(pArgs->pInputFile, pArgs->iInputFileSize, (int32_t *)pArgs->pKey, strlen(pArgs->pKey));
    if (pArgs->bQuiet != true){
      hex_dump("decrypted", pArgs->pInputFile, pArgs->iInputFileSize);
    }
    if (pArgs->pOutputFile != NULL){
      if (bWriteFile(pArgs->pOutputFile, pArgs->pInputFile, pArgs->iInputFileSize) == false){
        iModuleCheck(MODULE_FAIL, pArgs);
      }
    }
    iModuleCheck(MODULE_SUCCESS, pArgs);
  }
  iModuleCheck(MODULE_SKIP, pArgs);
}


int main(int argc, char **argv){
  struct args_t *pArgs = pArgsInit();
  if (iArgsParse(argc, argv, pArgs) == 1){
    return 1;
  }
  iModuleAllStrings(pArgs);
  iModuleExtractKey(pArgs);
  iModuleExtractDomain(pArgs);
  iModuleStdIn(pArgs);
  iModuleSID(pArgs);
  iModuleCryptFile(pArgs);
  return 0;
}
