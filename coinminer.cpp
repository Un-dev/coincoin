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

inline void append(char* s, char c, int& len) {
  s[len] = c; s[len+1] = '\0';
}


class Miner{

  public:
  int tmstp=0;
  unsigned char digest[SHA_DIGEST_LENGTH];
  char* nonce;
  char endToken[29];
  vector<string> coinName;

  void mine(int minToPrint);
  void benchmark();
  int countCOccurences(){
    char* hexDigest =(char *)malloc(SHA_DIGEST_LENGTH*2+1);
    hexDigest[0] = '\0';
    char curr[41];
    curr[0] = '\0';

    int i;
    int len;
    for (i=0 ; i < SHA_DIGEST_LENGTH; i++){
      sprintf(curr, "%02x", this->digest[i]);
      len = i*2;
      // len = strlen(hexDigest);
      // printf("%d %d %d\n", len, i, i*2);
      append(hexDigest, curr[0], len);
      len++;
      append(hexDigest, curr[1], len);
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
  unsigned char* generateHash();
  void printBenchmark(char* nonce, int seconds);
  void printDigest();
};

class CoinMiner: public Miner{
  
  public:
  CoinMiner(){
    tmstp = std::chrono::milliseconds(std::time(NULL)).count();
    endToken[0]= '\0';
    strcat(endToken, "-FLY-CC1.0-");
    char* p = &(endToken[11]);
    sprintf(p, "%d", tmstp);
    strcat(endToken, "-0f0f0f");
    coinName = {"Subcoin", "Coin", "Hexcoin", "Arkenstone", "Blackstar", "Grand Cross"};
  }
  CoinMiner(char* tri){
    tmstp = std::chrono::milliseconds(std::time(NULL)).count();
    endToken[0]= '\0';
    strcat(endToken, "-");
    strcat(endToken, tri);
    strcat(endToken, "-CC1.0-");
    char* p = &(endToken[11]);
    sprintf(p, "%d", tmstp);
    strcat(endToken, "-0f0f0f");    
    coinName = {"Subcoin", "Coin", "Hexcoin", "Arkenstone", "Blackstar", "Grand Cross"};
  }
  void mine(int minToPrint){
    while(true){
      this->nonce = this->generateNonce();
      this->generateHash();
      int occur = this->countCOccurences();
      if (occur >= minToPrint){
        if (occur>5){
          cout << occur << "c (" << coinName[occur-6]<< ") : " << this->nonce <<"\n";
        } else {
          cout << occur << "c (Miette): " << this->nonce <<"\n";
        }
      }
      free(this->nonce);
    }
  }
  void benchmark(){
    auto start = std::chrono::system_clock::now();
    auto end = start;
    bool found = false;
    int diff = 0;
    int i = 0;
    while (diff <= 60 && found == false){
      this->nonce = this->generateNonce();
      this->generateHash();
      int occur = this->countCOccurences();
      end = std::chrono::system_clock::now();
      diff = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      i++;
      if (occur == 6){
        this->printDigest();
        this->printBenchmark(nonce, diff);
        // found = true;
      }
      free(nonce);
    }
    printf("hashed %d time \n", i);
    if (found == false){
      throw 59;
    }
  }

  private:
  inline void generateHash(){
    strcat(this->nonce, this->endToken);
    SHA1((const unsigned char*)this->nonce, strlen((const char*)this->nonce), this->digest);
  }

  char* generateNonce(){
    char* nonce =(char*) malloc(61);
    nonce[0] = '\0';
    for (int i = 0; i<32 ; i++){
      append(nonce, RandInt('!', '~'), i);
    }
    return nonce;
  }

  void printBenchmark(char* nonce, int seconds){
    printf("6c (subcoin) mined in %d s : %s\n", seconds, nonce);
    printf(" *** Mining projection *** \n" );
    for (int i = 0; i<6; i++){
      cout << i+6 << "c (" << coinName[i] << ") " << seconds << "s\n";
      seconds*= 16;
    }
  }

  void printDigest(){
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
      printf("%02x", digest[i]);
    }
    printf("\n");
  }
};

int main(int argc, char* argv[]) {

  std::srand(std::time(0));
  if (argc == 5 && strcmp(argv[1], "-t") == 0 && strcmp(argv[3], "-m") == 0 ){
    CoinMiner cm(argv[2]);
    cm.mine(atoi(argv[4]));
  } else if (argc == 2 && strcmp(argv[1], "-z") == 0) {
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

