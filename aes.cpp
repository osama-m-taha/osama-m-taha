#include "aes.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// AES S-box
static const uint8_t SBOX[256] = {
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

// Inverse S-box
static const uint8_t INV_SBOX[256] = {
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
static const uint8_t RCON[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

AES::AES(const std::vector<uint8_t>& key) : key_(key) {
    if (key.size() != AES_128 && key.size() != AES_192 && key.size() != AES_256) {
        throw std::invalid_argument("Key size must be 16, 24, or 32 bytes");
    }
    
    if (key.size() == AES_128) numRounds_ = NUM_ROUNDS_128;
    else if (key.size() == AES_192) numRounds_ = NUM_ROUNDS_192;
    else numRounds_ = NUM_ROUNDS_256;
    
    keyExpansion();
}

AES::~AES() = default;

void AES::keyExpansion() {
    size_t keyWords = key_.size() / 4;
    size_t totalWords = (numRounds_ + 1) * 4;
    
    roundKeys_.resize(totalWords);
    
    // Copy initial key
    for (size_t i = 0; i < keyWords; i++) {
        roundKeys_[i][0] = key_[4*i];
        roundKeys_[i][1] = key_[4*i + 1];
        roundKeys_[i][2] = key_[4*i + 2];
        roundKeys_[i][3] = key_[4*i + 3];
    }
    
    // Generate remaining round keys
    for (size_t i = keyWords; i < totalWords; i++) {
        std::array<uint8_t, 4> temp;
        temp[0] = roundKeys_[i-1][0];
        temp[1] = roundKeys_[i-1][1];
        temp[2] = roundKeys_[i-1][2];
        temp[3] = roundKeys_[i-1][3];
        
        if (i % keyWords == 0) {
            // RotWord
            uint8_t t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            
            // SubWord
            temp[0] = SBOX[temp[0]];
            temp[1] = SBOX[temp[1]];
            temp[2] = SBOX[temp[2]];
            temp[3] = SBOX[temp[3]];
            
            // XOR with round constant
            temp[0] ^= RCON[i / keyWords];
        } else if (keyWords > 6 && i % keyWords == 4) {
            // SubWord for AES-256
            temp[0] = SBOX[temp[0]];
            temp[1] = SBOX[temp[1]];
            temp[2] = SBOX[temp[2]];
            temp[3] = SBOX[temp[3]];
        }
        
        // XOR with previous word
        for (size_t j = 0; j < 4; j++) {
            roundKeys_[i][j] = roundKeys_[i - keyWords][j] ^ temp[j];
        }
    }
}

void AES::addRoundKey(std::array<uint8_t, 16>& state, size_t round) {
    for (size_t i = 0; i < 16; i++) {
        state[i] ^= roundKeys_[round * 4 + i / 4][i % 4];
    }
}

void AES::subBytes(std::array<uint8_t, 16>& state) {
    for (size_t i = 0; i < 16; i++) {
        state[i] = SBOX[state[i]];
    }
}

void AES::invSubBytes(std::array<uint8_t, 16>& state) {
    for (size_t i = 0; i < 16; i++) {
        state[i] = INV_SBOX[state[i]];
    }
}

void AES::shiftRows(std::array<uint8_t, 16>& state) {
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

void AES::invShiftRows(std::array<uint8_t, 16>& state) {
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

void AES::mixColumns(std::array<uint8_t, 16>& state) {
    for (size_t i = 0; i < 4; i++) {
        uint8_t s0 = state[4*i];
        uint8_t s1 = state[4*i + 1];
        uint8_t s2 = state[4*i + 2];
        uint8_t s3 = state[4*i + 3];
        
        state[4*i] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3;
        state[4*i + 1] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3;
        state[4*i + 2] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3);
        state[4*i + 3] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3);
    }
}

void AES::invMixColumns(std::array<uint8_t, 16>& state) {
    for (size_t i = 0; i < 4; i++) {
        uint8_t s0 = state[4*i];
        uint8_t s1 = state[4*i + 1];
        uint8_t s2 = state[4*i + 2];
        uint8_t s3 = state[4*i + 3];
        
        state[4*i] = gmul(0x0e, s0) ^ gmul(0x0b, s1) ^ gmul(0x0d, s2) ^ gmul(0x09, s3);
        state[4*i + 1] = gmul(0x09, s0) ^ gmul(0x0e, s1) ^ gmul(0x0b, s2) ^ gmul(0x0d, s3);
        state[4*i + 2] = gmul(0x0d, s0) ^ gmul(0x09, s1) ^ gmul(0x0e, s2) ^ gmul(0x0b, s3);
        state[4*i + 3] = gmul(0x0b, s0) ^ gmul(0x0d, s1) ^ gmul(0x09, s2) ^ gmul(0x0e, s3);
    }
}

std::vector<uint8_t> AES::encrypt(const std::vector<uint8_t>& plaintext) {
    std::vector<uint8_t> padded = padPKCS7(plaintext);
    std::vector<uint8_t> ciphertext;
    
    for (size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
        std::array<uint8_t, 16> block;
        std::copy(padded.begin() + i, padded.begin() + i + BLOCK_SIZE, block.begin());
        
        // Initial round
        addRoundKey(block, 0);
        
        // Main rounds
        for (size_t round = 1; round < numRounds_; round++) {
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

std::vector<uint8_t> AES::decrypt(const std::vector<uint8_t>& ciphertext) {
    if (ciphertext.size() % BLOCK_SIZE != 0) {
        throw std::invalid_argument("Ciphertext length must be multiple of 16");
    }
    
    std::vector<uint8_t> plaintext;
    
    for (size_t i = 0; i < ciphertext.size(); i += BLOCK_SIZE) {
        std::array<uint8_t, 16> block;
        std::copy(ciphertext.begin() + i, ciphertext.begin() + i + BLOCK_SIZE, block.begin());
        
        // Initial round
        addRoundKey(block, numRounds_);
        
        // Main rounds
        for (size_t round = numRounds_ - 1; round > 0; round--) {
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
    
    return unpadPKCS7(plaintext);
}

std::vector<uint8_t> AES::padPKCS7(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> padded = data;
    uint8_t padValue = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    padded.resize(data.size() + padValue, padValue);
    return padded;
}

std::vector<uint8_t> AES::unpadPKCS7(const std::vector<uint8_t>& data) {
    if (data.empty()) return data;
    
    uint8_t padValue = data.back();
    if (padValue > BLOCK_SIZE || padValue == 0) {
        throw std::runtime_error("Invalid PKCS7 padding");
    }
    
    // Verify padding
    for (size_t i = data.size() - padValue; i < data.size(); i++) {
        if (data[i] != padValue) {
            throw std::runtime_error("Invalid PKCS7 padding");
        }
    }
    
    return std::vector<uint8_t>(data.begin(), data.end() - padValue);
}

std::string AES::encryptString(const std::string& plaintext) {
    std::vector<uint8_t> plainBytes(plaintext.begin(), plaintext.end());
    std::vector<uint8_t> cipherBytes = encrypt(plainBytes);
    return bytesToHex(cipherBytes);
}

std::string AES::decryptString(const std::string& ciphertext) {
    std::vector<uint8_t> cipherBytes = hexToBytes(ciphertext);
    std::vector<uint8_t> plainBytes = decrypt(cipherBytes);
    return std::string(plainBytes.begin(), plainBytes.end());
}

std::string AES::bytesToHex(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (uint8_t byte : bytes) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

std::vector<uint8_t> AES::hexToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        bytes.push_back(static_cast<uint8_t>(std::stoi(byteString, nullptr, 16)));
    }
    return bytes;
}