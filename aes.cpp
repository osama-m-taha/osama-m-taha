#include "aes.h"
#include <cstring>
#include <stdexcept>
#include <iomanip>
#include <sstream>

// S-box table for SubBytes transformation
const uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Inverse S-box table for InvSubBytes transformation
const uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Round constants for key expansion
const uint8_t RCON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

AES::AES(KeySize keySize) : keySize_(keySize) {
    switch (keySize) {
        case AES_128: numRounds_ = NUM_ROUNDS_128; break;
        case AES_192: numRounds_ = NUM_ROUNDS_192; break;
        case AES_256: numRounds_ = NUM_ROUNDS_256; break;
        default: throw std::invalid_argument("Invalid key size");
    }
    roundKeys_.resize(4 * (numRounds_ + 1));
}

AES::~AES() {}

std::vector<uint8_t> AES::encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key) {
    if (key.size() != keySize_) {
        throw std::invalid_argument("Key size mismatch");
    }
    
    // Pad the plaintext
    std::vector<uint8_t> paddedData = padPKCS7(plaintext);
    std::vector<uint8_t> ciphertext;
    
    // Expand the key
    keyExpansion(key);
    
    // Process each block
    for (size_t i = 0; i < paddedData.size(); i += BLOCK_SIZE) {
        std::vector<uint8_t> block(paddedData.begin() + i, paddedData.begin() + i + BLOCK_SIZE);
        
        // Initial round
        addRoundKey(block, 0);
        
        // Main rounds
        for (int round = 1; round < numRounds_; round++) {
            subBytes(block);
            shiftRows(block);
            mixColumns(block);
            addRoundKey(block, round);
        }
        
        // Final round
        subBytes(block);
        shiftRows(block);
        addRoundKey(block, numRounds_);
        
        ciphertext.insert(ciphertext.end(), block.begin(), block.end());
    }
    
    return ciphertext;
}

std::vector<uint8_t> AES::decrypt(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key) {
    if (key.size() != keySize_) {
        throw std::invalid_argument("Key size mismatch");
    }
    
    if (ciphertext.size() % BLOCK_SIZE != 0) {
        throw std::invalid_argument("Ciphertext length must be multiple of block size");
    }
    
    std::vector<uint8_t> plaintext;
    
    // Expand the key
    keyExpansion(key);
    
    // Process each block
    for (size_t i = 0; i < ciphertext.size(); i += BLOCK_SIZE) {
        std::vector<uint8_t> block(ciphertext.begin() + i, ciphertext.begin() + i + BLOCK_SIZE);
        
        // Initial round
        addRoundKey(block, numRounds_);
        
        // Main rounds
        for (int round = numRounds_ - 1; round > 0; round--) {
            invShiftRows(block);
            invSubBytes(block);
            addRoundKey(block, round);
            invMixColumns(block);
        }
        
        // Final round
        invShiftRows(block);
        invSubBytes(block);
        addRoundKey(block, 0);
        
        plaintext.insert(plaintext.end(), block.begin(), block.end());
    }
    
    // Remove padding
    return unpadPKCS7(plaintext);
}

std::string AES::encryptString(const std::string& plaintext, const std::string& key) {
    std::vector<uint8_t> keyBytes(key.begin(), key.end());
    std::vector<uint8_t> plaintextBytes(plaintext.begin(), plaintext.end());
    
    // Derive a proper key from the password
    std::vector<uint8_t> salt = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::vector<uint8_t> derivedKey = deriveKey(key, salt);
    
    std::vector<uint8_t> ciphertext = encrypt(plaintextBytes, derivedKey);
    
    // Convert to hex string
    std::stringstream ss;
    for (uint8_t byte : ciphertext) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

std::string AES::decryptString(const std::string& ciphertext, const std::string& key) {
    std::vector<uint8_t> keyBytes(key.begin(), key.end());
    
    // Derive the same key
    std::vector<uint8_t> salt = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::vector<uint8_t> derivedKey = deriveKey(key, salt);
    
    // Convert hex string to bytes
    std::vector<uint8_t> ciphertextBytes;
    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        std::string byteString = ciphertext.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        ciphertextBytes.push_back(byte);
    }
    
    std::vector<uint8_t> plaintext = decrypt(ciphertextBytes, derivedKey);
    return std::string(plaintext.begin(), plaintext.end());
}

void AES::keyExpansion(const std::vector<uint8_t>& key) {
    std::vector<uint32_t> words;
    
    // Convert key bytes to words
    for (int i = 0; i < keySize_ / 4; i++) {
        uint32_t word = (key[4*i] << 24) | (key[4*i+1] << 16) | (key[4*i+2] << 8) | key[4*i+3];
        words.push_back(word);
    }
    
    // Generate additional words
    for (int i = keySize_ / 4; i < 4 * (numRounds_ + 1); i++) {
        uint32_t temp = words[i-1];
        
        if (i % (keySize_ / 4) == 0) {
            temp = subWord(rotWord(temp)) ^ (RCON[i / (keySize_ / 4)] << 24);
        } else if (keySize_ == AES_256 && i % (keySize_ / 4) == 4) {
            temp = subWord(temp);
        }
        
        words.push_back(words[i - keySize_ / 4] ^ temp);
    }
    
    roundKeys_ = words;
}

void AES::addRoundKey(std::vector<uint8_t>& state, int round) {
    for (int i = 0; i < 16; i++) {
        int row = i % 4;
        int col = i / 4;
        uint8_t keyByte = (roundKeys_[round * 4 + col] >> (24 - 8 * row)) & 0xFF;
        state[i] ^= keyByte;
    }
}

void AES::subBytes(std::vector<uint8_t>& state) {
    for (int i = 0; i < 16; i++) {
        state[i] = SBOX[state[i]];
    }
}

void AES::invSubBytes(std::vector<uint8_t>& state) {
    for (int i = 0; i < 16; i++) {
        state[i] = INV_SBOX[state[i]];
    }
}

void AES::shiftRows(std::vector<uint8_t>& state) {
    // Row 0: no shift
    // Row 1: shift left by 1
    uint8_t temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2: shift left by 2
    std::swap(state[2], state[10]);
    std::swap(state[6], state[14]);
    
    // Row 3: shift left by 3
    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

void AES::invShiftRows(std::vector<uint8_t>& state) {
    // Row 0: no shift
    // Row 1: shift right by 1
    uint8_t temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;
    
    // Row 2: shift right by 2
    std::swap(state[2], state[10]);
    std::swap(state[6], state[14]);
    
    // Row 3: shift right by 3
    temp = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = state[3];
    state[3] = temp;
}

void AES::mixColumns(std::vector<uint8_t>& state) {
    for (int col = 0; col < 4; col++) {
        uint8_t s0 = state[col];
        uint8_t s1 = state[col + 4];
        uint8_t s2 = state[col + 8];
        uint8_t s3 = state[col + 12];
        
        state[col] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3;
        state[col + 4] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3;
        state[col + 8] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3);
        state[col + 12] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3);
    }
}

void AES::invMixColumns(std::vector<uint8_t>& state) {
    for (int col = 0; col < 4; col++) {
        uint8_t s0 = state[col];
        uint8_t s1 = state[col + 4];
        uint8_t s2 = state[col + 8];
        uint8_t s3 = state[col + 12];
        
        state[col] = gmul(0x0e, s0) ^ gmul(0x0b, s1) ^ gmul(0x0d, s2) ^ gmul(0x09, s3);
        state[col + 4] = gmul(0x09, s0) ^ gmul(0x0e, s1) ^ gmul(0x0b, s2) ^ gmul(0x0d, s3);
        state[col + 8] = gmul(0x0d, s0) ^ gmul(0x09, s1) ^ gmul(0x0e, s2) ^ gmul(0x0b, s3);
        state[col + 12] = gmul(0x0b, s0) ^ gmul(0x0d, s1) ^ gmul(0x09, s2) ^ gmul(0x0e, s3);
    }
}

uint8_t AES::gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t hi_bit_set;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) {
            a ^= 0x1b; // AES irreducible polynomial
        }
        b >>= 1;
    }
    return p;
}

uint32_t AES::rotWord(uint32_t word) {
    return ((word << 8) | (word >> 24));
}

uint32_t AES::subWord(uint32_t word) {
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t byte = (word >> (24 - 8 * i)) & 0xFF;
        result |= (SBOX[byte] << (24 - 8 * i));
    }
    return result;
}

std::vector<uint8_t> AES::padPKCS7(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> padded = data;
    int padding = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    for (int i = 0; i < padding; i++) {
        padded.push_back(static_cast<uint8_t>(padding));
    }
    return padded;
}

std::vector<uint8_t> AES::unpadPKCS7(const std::vector<uint8_t>& data) {
    if (data.empty()) return data;
    
    uint8_t padding = data.back();
    if (padding > BLOCK_SIZE || padding == 0) {
        throw std::runtime_error("Invalid PKCS7 padding");
    }
    
    // Verify padding
    for (size_t i = data.size() - padding; i < data.size(); i++) {
        if (data[i] != padding) {
            throw std::runtime_error("Invalid PKCS7 padding");
        }
    }
    
    return std::vector<uint8_t>(data.begin(), data.end() - padding);
}

std::vector<uint8_t> AES::deriveKey(const std::string& password, const std::vector<uint8_t>& salt) {
    // Simple key derivation - in production, use PBKDF2 or similar
    std::vector<uint8_t> key(keySize_, 0);
    
    for (size_t i = 0; i < password.length() && i < keySize_; i++) {
        key[i] = password[i];
    }
    
    // XOR with salt for additional entropy
    for (size_t i = 0; i < salt.size() && i < keySize_; i++) {
        key[i] ^= salt[i];
    }
    
    return key;
}