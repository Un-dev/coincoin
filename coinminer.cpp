#include <iostream>
#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

inline int RandInt(int low, int high) {return low + std::rand() % (high - low + 1);}

inline void append(char* s, char c) {
  int len = strlen(s);
  s[len] = c; s[len+1] = '\0';
}


class Miner{

  public:
  int tmstp=0;
  unsigned char* digest;
  char* nonce;
  char endToken[29];

  void mine(int minToPrint);
  void benchmark();
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

  private:
  char* generateNonce();
  unsigned char* generateHash(char* nonce, unsigned char* digest, int tmstp);
  void printBenchmark(char* nonce, int seconds);
  void printDigest(unsigned char* digest);
};

class CoinMiner: public Miner{
  
  public:
  CoinMiner(){
    tmstp = std::chrono::milliseconds(std::time(NULL)).count();
    unsigned char d[SHA_DIGEST_LENGTH];
    digest = d;
    endToken[0]= '\0';
    strcat(endToken, "-FLY-CC1.0-");
    char* p = &(endToken[11]);
    sprintf(p, "%d", tmstp);
    strcat(endToken, "-0f0f0f");
  }
  CoinMiner(char* tri){
    tmstp = std::chrono::milliseconds(std::time(NULL)).count();
    unsigned char d[SHA_DIGEST_LENGTH];
    digest = d;
    endToken[0]= '\0';
    strcat(endToken, "-");
    strcat(endToken, tri);
    strcat(endToken, "-CC1.0-");
    char* p = &(endToken[11]);
    sprintf(p, "%d", tmstp);
    strcat(endToken, "-0f0f0f");    
  }
  void mine(int minToPrint){
    printf("%d", minToPrint);
    while(true){
      nonce = this->generateNonce();
      generateHash(nonce, digest, tmstp);
      int occur = countCOccurences(digest);
      if (occur >= minToPrint){
        printf("%s\n",nonce);
        printDigest(digest);
      }
      free(nonce);
    }
  }
  void benchmark(){
    auto start = std::chrono::system_clock::now();
    auto end = start;
    bool found = false;
    int diff = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    while (diff <= 60 && found == false){
      nonce = generateNonce();
      generateHash(nonce, digest, tmstp);
      int occur = countCOccurences(digest);
      end = std::chrono::system_clock::now();
      diff = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      if (occur == 6){
        printBenchmark(nonce, diff);
        found = true;
      }
      free(nonce);
    }
    if (found == false){
      throw 59;
    }
  }

  private:
  inline void generateHash(char* nonce, unsigned char* digest, int tmstp){
    strcat(nonce, endToken);
    SHA1((const unsigned char*)nonce, strlen((const char*)nonce), digest);
  }

  char* generateNonce(){
    char* nonce =(char*) malloc(61);
    nonce[0] = '\0';
    for (int i = 0; i<32 ; i++){
      append(nonce, RandInt('!', '~'));
    }
    return nonce;
  }

  void printBenchmark(char* nonce, int seconds){
    vector<string> coinName = {"Subcoin", "Coin", "Hexcoin", "Arkenstone", "Blackstar", "Grand Cross"};
    printf("6c (subcoin) mined in %d s : %s\n", seconds, nonce);
    printf(" *** Mining projection *** \n" );
    for (int i = 0; i<6; i++){
      cout << i+6 << "c (" << coinName[i] << ") " << seconds << "s\n";
      seconds*= 16;
    }
  }

  void printDigest(unsigned char* digest){
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
      printf("%02x", digest[i]);
    }
    printf("\n");
  }
};

int main(int argc, char* argv[]) {

  if (argc == 5 && strcmp(argv[1], "-t") == 0 && strcmp(argv[3], "-m") == 0 ){
    CoinMiner cm(argv[2]);
    cm.mine(atoi(argv[4]));
  } else if (argc == 2 && strcmp(argv[1], "-z") == 0) {
    std::srand(std::time(0));
    try {
    CoinMiner cm;
      cm.benchmark();
    } catch(int notfound){
      cout << "Unfortunately no Subcoin were found during this benchmark, please try again\n" ;
    }

  }else{
    printf("only available uses are ./coinminer -z or ./coinminer -t <triown> -m <minimum c to print a token>\n");
  }

  return 0;
}

