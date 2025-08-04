#include "aes.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

// Function to generate a random key
std::vector<uint8_t> generateRandomKey(size_t keySize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::vector<uint8_t> key(keySize);
    for (size_t i = 0; i < keySize; i++) {
        key[i] = static_cast<uint8_t>(dis(gen));
    }
    return key;
}

// Function to print bytes as hex
void printHex(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << label << ": ";
    for (uint8_t byte : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "=== AES Encryption/Decryption Demo ===" << std::endl << std::endl;
    
    // Test with AES-256 (32 bytes = 256 bits)
    std::cout << "Testing AES-256:" << std::endl;
    std::vector<uint8_t> key256 = generateRandomKey(32);
    printHex(key256, "Key (256-bit)");
    
    AES aes256(key256);
    
    std::string plaintext = "Hello, this is a test message for AES encryption!";
    std::cout << "Plaintext: " << plaintext << std::endl;
    
    // Encrypt
    std::string encrypted = aes256.encryptString(plaintext);
    std::cout << "Encrypted (hex): " << encrypted << std::endl;
    
    // Decrypt
    std::string decrypted = aes256.decryptString(encrypted);
    std::cout << "Decrypted: " << decrypted << std::endl;
    
    std::cout << "Encryption/Decryption successful: " 
              << (plaintext == decrypted ? "YES" : "NO") << std::endl << std::endl;
    
    // Test with AES-192 (24 bytes = 192 bits)
    std::cout << "Testing AES-192:" << std::endl;
    std::vector<uint8_t> key192 = generateRandomKey(24);
    printHex(key192, "Key (192-bit)");
    
    AES aes192(key192);
    
    std::string plaintext2 = "Another test message with AES-192";
    std::cout << "Plaintext: " << plaintext2 << std::endl;
    
    std::string encrypted2 = aes192.encryptString(plaintext2);
    std::cout << "Encrypted (hex): " << encrypted2 << std::endl;
    
    std::string decrypted2 = aes192.decryptString(encrypted2);
    std::cout << "Decrypted: " << decrypted2 << std::endl;
    
    std::cout << "Encryption/Decryption successful: " 
              << (plaintext2 == decrypted2 ? "YES" : "NO") << std::endl << std::endl;
    
    // Test with AES-128 (16 bytes = 128 bits)
    std::cout << "Testing AES-128:" << std::endl;
    std::vector<uint8_t> key128 = generateRandomKey(16);
    printHex(key128, "Key (128-bit)");
    
    AES aes128(key128);
    
    std::string plaintext3 = "Short message";
    std::cout << "Plaintext: " << plaintext3 << std::endl;
    
    std::string encrypted3 = aes128.encryptString(plaintext3);
    std::cout << "Encrypted (hex): " << encrypted3 << std::endl;
    
    std::string decrypted3 = aes128.decryptString(encrypted3);
    std::cout << "Decrypted: " << decrypted3 << std::endl;
    
    std::cout << "Encryption/Decryption successful: " 
              << (plaintext3 == decrypted3 ? "YES" : "NO") << std::endl << std::endl;
    
    // Test with binary data
    std::cout << "Testing with binary data:" << std::endl;
    std::vector<uint8_t> binaryData = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    
    printHex(binaryData, "Binary data");
    
    std::vector<uint8_t> encryptedBinary = aes256.encrypt(binaryData);
    printHex(encryptedBinary, "Encrypted binary");
    
    std::vector<uint8_t> decryptedBinary = aes256.decrypt(encryptedBinary);
    printHex(decryptedBinary, "Decrypted binary");
    
    bool binarySuccess = (binaryData == decryptedBinary);
    std::cout << "Binary encryption/decryption successful: " 
              << (binarySuccess ? "YES" : "NO") << std::endl << std::endl;
    
    // Test error handling
    std::cout << "Testing error handling:" << std::endl;
    try {
        std::vector<uint8_t> invalidKey = {1, 2, 3, 4, 5}; // Invalid key size
        AES invalidAES(invalidKey);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }
    
    try {
        std::vector<uint8_t> invalidCiphertext = {1, 2, 3, 4, 5}; // Not multiple of 16
        aes256.decrypt(invalidCiphertext);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }
    
    std::cout << std::endl << "All tests completed!" << std::endl;
    
    return 0;
}