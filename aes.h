#ifndef AES_H
#define AES_H

#include <vector>
#include <string>
#include <cstdint>

class AES {
public:
    enum KeySize {
        AES_128 = 16,
        AES_192 = 24,
        AES_256 = 32
    };

    AES(KeySize keySize = AES_256);
    ~AES();

    // Main encryption/decryption methods
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key);
    
    // String-based convenience methods
    std::string encryptString(const std::string& plaintext, const std::string& key);
    std::string decryptString(const std::string& ciphertext, const std::string& key);

private:
    static const int BLOCK_SIZE = 16;
    static const int NUM_ROUNDS_128 = 10;
    static const int NUM_ROUNDS_192 = 12;
    static const int NUM_ROUNDS_256 = 14;
    
    int keySize_;
    int numRounds_;
    std::vector<uint32_t> roundKeys_;

    // Core AES operations
    void keyExpansion(const std::vector<uint8_t>& key);
    void addRoundKey(std::vector<uint8_t>& state, int round);
    void subBytes(std::vector<uint8_t>& state);
    void invSubBytes(std::vector<uint8_t>& state);
    void shiftRows(std::vector<uint8_t>& state);
    void invShiftRows(std::vector<uint8_t>& state);
    void mixColumns(std::vector<uint8_t>& state);
    void invMixColumns(std::vector<uint8_t>& state);
    
    // Helper functions
    uint8_t gmul(uint8_t a, uint8_t b);
    uint32_t rotWord(uint32_t word);
    uint32_t subWord(uint32_t word);
    std::vector<uint8_t> padPKCS7(const std::vector<uint8_t>& data);
    std::vector<uint8_t> unpadPKCS7(const std::vector<uint8_t>& data);
    std::vector<uint8_t> deriveKey(const std::string& password, const std::vector<uint8_t>& salt);
};

// S-box and inverse S-box tables
extern const uint8_t SBOX[256];
extern const uint8_t INV_SBOX[256];

// Round constants for key expansion
extern const uint8_t RCON[11];

#endif // AES_H