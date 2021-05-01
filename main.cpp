#include <iostream>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

inline char RandInt(int low, int high) {
  char c = low + std::rand() % (high - low + 1);
  while (c == '"' || c == '\\' || c=='`') {
    c = low + std::rand() % (high - low + 1);
  }
  return c;
}

void append(char* s, char c) {
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}
char* generateNonce(){
  char c=RandInt('!', '~'); 
  char* nonce =(char*) malloc(61);
  nonce[0] = '\0';
  for (int i = 0; i<32 ; i++){
    c=RandInt('!', '~');
    append(nonce, c);
  }
  return nonce;
}

unsigned char* generateHash(char* nonce, unsigned char* digest, int tmstp){
  char tmpstptos[11];
  tmpstptos[0] = '\0';
  sprintf(tmpstptos, "%d", tmstp);
  const char* endToken= "-FLY-CC1.0-";
  strcat(nonce, endToken);
  strcat(nonce, tmpstptos);
  strcat(nonce, "-0f0f0f");
  SHA1((const unsigned char*)nonce, strlen((const char*)nonce), digest);

  return digest;
}

void printDigest(unsigned char* digest){
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    printf("%02x", digest[i]);
  }
  printf("\n");
}

int countCOccurences(unsigned char* digest){
  char* hexDigest =(char *)malloc(SHA_DIGEST_LENGTH*2+1);
  hexDigest[0] = '\0';
  char curr[41];
  curr[0] = '\0';
  int i;
  for (i = 0; i < SHA_DIGEST_LENGTH; i++){
    sprintf(curr, "%02x", digest[i]);
    append(hexDigest, curr[0]);
    append(hexDigest, curr[1]);
  }
  i=0;
  char hexC[2];
  sprintf(hexC, "%01x", 12);
  while (i < 40 && hexDigest[i] == hexC[0]){
    i++;
  };
  free(hexDigest);
  return i;
}

int main() {
  std::srand(std::time(0));
  auto unix_timestamp = std::chrono::milliseconds(std::time(NULL));
  unsigned char digest[SHA_DIGEST_LENGTH];
  char* nonce;

  // bool found = false;
  // while (found == false){
  int i =0;
  // while (i<20){
  while (true){
    nonce = generateNonce();
    generateHash(nonce, digest, unix_timestamp.count());


    int occur = countCOccurences(digest);
    if (occur >7){
      printf("%s", nonce);
      printf("\n");
      printDigest(digest);
      // found = true;
    }
    i++;
    free(nonce);

  }

  return 0;
}