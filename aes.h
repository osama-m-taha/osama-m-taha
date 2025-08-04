#ifndef AES_H
#define AES_H

#include <vector>
#include <string>
#include <cstdint>
#include <array>

class AES {
public:
    enum KeySize {
        AES_128 = 16,
        AES_192 = 24,
        AES_256 = 32
    };

    AES(const std::vector<uint8_t>& key);
    ~AES();

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    
    // Helper methods for string input/output
    std::string encryptString(const std::string& plaintext);
    std::string decryptString(const std::string& ciphertext);

private:
    static constexpr size_t BLOCK_SIZE = 16;
    static constexpr size_t NUM_ROUNDS_128 = 10;
    static constexpr size_t NUM_ROUNDS_192 = 12;
    static constexpr size_t NUM_ROUNDS_256 = 14;

    std::vector<uint8_t> key_;
    size_t numRounds_;
    std::vector<std::array<uint8_t, 16>> roundKeys_;

    // Core AES operations
    void keyExpansion();
    void addRoundKey(std::array<uint8_t, 16>& state, size_t round);
    void subBytes(std::array<uint8_t, 16>& state);
    void invSubBytes(std::array<uint8_t, 16>& state);
    void shiftRows(std::array<uint8_t, 16>& state);
    void invShiftRows(std::array<uint8_t, 16>& state);
    void mixColumns(std::array<uint8_t, 16>& state);
    void invMixColumns(std::array<uint8_t, 16>& state);
    
    // Utility functions
    uint8_t gmul(uint8_t a, uint8_t b);
    std::vector<uint8_t> padPKCS7(const std::vector<uint8_t>& data);
    std::vector<uint8_t> unpadPKCS7(const std::vector<uint8_t>& data);
    std::string bytesToHex(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> hexToBytes(const std::string& hex);
};

#endif // AES_H