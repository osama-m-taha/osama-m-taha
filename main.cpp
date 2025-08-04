#include "aes.h"
#include <iostream>
#include <string>

int main() {
    try {
        // Create AES instance with 256-bit key (strongest standard AES)
        AES aes(AES::AES_256);
        
        std::string message = "Hello, this is a secret message that will be encrypted using AES-256!";
        std::string password = "MySecretPassword123";
        
        std::cout << "=== AES-256 Encryption/Decryption Demo ===" << std::endl;
        std::cout << "Original message: " << message << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << std::endl;
        
        // Encrypt the message
        std::string encrypted = aes.encryptString(message, password);
        std::cout << "Encrypted (hex): " << encrypted << std::endl;
        std::cout << std::endl;
        
        // Decrypt the message
        std::string decrypted = aes.decryptString(encrypted, password);
        std::cout << "Decrypted message: " << decrypted << std::endl;
        std::cout << std::endl;
        
        // Verify the decryption was successful
        if (message == decrypted) {
            std::cout << "✓ Encryption/Decryption successful!" << std::endl;
        } else {
            std::cout << "✗ Encryption/Decryption failed!" << std::endl;
        }
        
        // Test different key sizes
        std::cout << "\n=== Testing different key sizes ===" << std::endl;
        
        std::string testMsg = "Test message for different AES variants";
        std::string testKey = "TestKey123";
        
        // AES-128
        AES aes128(AES::AES_128);
        std::string encrypted128 = aes128.encryptString(testMsg, testKey);
        std::string decrypted128 = aes128.decryptString(encrypted128, testKey);
        std::cout << "AES-128 Test: " << (testMsg == decrypted128 ? "✓ Passed" : "✗ Failed") << std::endl;
        
        // AES-192
        AES aes192(AES::AES_192);
        std::string encrypted192 = aes192.encryptString(testMsg, testKey);
        std::string decrypted192 = aes192.decryptString(encrypted192, testKey);
        std::cout << "AES-192 Test: " << (testMsg == decrypted192 ? "✓ Passed" : "✗ Failed") << std::endl;
        
        // AES-256
        std::string encrypted256 = aes.encryptString(testMsg, testKey);
        std::string decrypted256 = aes.decryptString(encrypted256, testKey);
        std::cout << "AES-256 Test: " << (testMsg == decrypted256 ? "✓ Passed" : "✗ Failed") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}