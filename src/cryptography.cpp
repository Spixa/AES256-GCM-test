#include "cryptography.h"

Cryptography::Cryptography() : rng() {
          enc = Cipher_Mode::create("AES-256/GCM", ENCRYPTION);
          dec = Cipher_Mode::create("AES-256/GCM", DECRYPTION); 

} 

std::string Cryptography::encrypt(const std::string & str, const char keyhex[]) {
             // defining variables
             secure_vector<uint8_t> input;
             secure_vector<uint8_t> key;
            try {
              key = hex_decode_locked(keyhex);
            } catch (std::exception e) {
                  std::cout << ERROR_MSG << '\n' << "\twhat(): "
                  << e.what() << '\n';
            }
            input = secure_vector<uint8_t>(str.data(), str.data() + str.length());
            secure_vector<uint8_t> iv = this->rng.random_vec(enc->default_nonce_length()); 

         // start to compile the string into a ciphertext
         enc->set_key(key);
         enc->start(iv);
         enc->finish(input);
         return std::string(hex_encode(iv) + hex_encode(input)); 
        
}
std::unordered_map<std::string, std::string> Cryptography::decrypt(const std::string& str, const char keyhex[]) {
          secure_vector<uint8_t> output;
          secure_vector<uint8_t> iv;
          secure_vector<uint8_t> key;
            try {
             key = hex_decode_locked(keyhex);
            } catch (std::exception e) {
                  std::cout << ERROR_MSG << '\n' << "\twhat(): "
                  << e.what() << '\n';
            }
            std::string ivstr;
            for(int i = 0; i < 24; i++) {
                 ivstr+= str.c_str()[i];
            }
            std::string encstr = str;
            encstr.erase(0, 24);
            iv = secure_vector<uint8_t>(hex_decode_locked(ivstr));
            output = secure_vector<uint8_t>(hex_decode_locked(encstr));


           // start to decompile 
           dec->set_key(key);
           dec->start(iv);

           dec->finish(output, 0); 

          std::unordered_map<std::string, std::string> map;
          map["iv"] = hex_encode(iv);
          map["msg"] = hex_encode(output); 

         return map;
}