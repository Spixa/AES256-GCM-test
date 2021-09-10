#include <botan/rng.h>
#include <botan/auto_rng.h>
#include <botan/cipher_mode.h>
#include <botan/hex.h>
#include <iostream>
#include <memory.h>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#define ERROR_MSG "an internal error occured whilst executing the program"

using namespace Botan;
class Cryptography {
public:
    Cryptography();
    std::string encrypt(const std::string & str, const char keyhex[]);
    std::unordered_map<std::string, std::string> decrypt(const std::string& str, const char keyhex[]);
private:
    std::unique_ptr<Cipher_Mode> enc;
    std::unique_ptr<Cipher_Mode> dec;
    AutoSeeded_RNG rng;
};