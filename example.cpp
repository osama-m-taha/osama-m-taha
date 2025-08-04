#include "aes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

// Function to generate a secure random key
std::vector<uint8_t> generateSecureKey(size_t keySize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::vector<uint8_t> key(keySize);
    for (size_t i = 0; i < keySize; i++) {
        key[i] = static_cast<uint8_t>(dis(gen));
    }
    return key;
}

// Function to read file into vector
std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

// Function to write vector to file
void writeFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Function to encrypt a file
void encryptFile(const std::string& inputFile, const std::string& outputFile, 
                 const std::vector<uint8_t>& key) {
    try {
        // Read input file
        std::vector<uint8_t> plaintext = readFile(inputFile);
        
        // Initialize AES
        AES aes(key);
        
        // Encrypt data
        std::vector<uint8_t> ciphertext = aes.encrypt(plaintext);
        
        // Write encrypted data
        writeFile(outputFile, ciphertext);
        
        std::cout << "File encrypted successfully!" << std::endl;
        std::cout << "Input size: " << plaintext.size() << " bytes" << std::endl;
        std::cout << "Output size: " << ciphertext.size() << " bytes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Encryption error: " << e.what() << std::endl;
    }
}

// Function to decrypt a file
void decryptFile(const std::string& inputFile, const std::string& outputFile, 
                 const std::vector<uint8_t>& key) {
    try {
        // Read encrypted file
        std::vector<uint8_t> ciphertext = readFile(inputFile);
        
        // Initialize AES
        AES aes(key);
        
        // Decrypt data
        std::vector<uint8_t> plaintext = aes.decrypt(ciphertext);
        
        // Write decrypted data
        writeFile(outputFile, plaintext);
        
        std::cout << "File decrypted successfully!" << std::endl;
        std::cout << "Input size: " << ciphertext.size() << " bytes" << std::endl;
        std::cout << "Output size: " << plaintext.size() << " bytes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Decryption error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== AES File Encryption Example ===" << std::endl << std::endl;
    
    // Generate a 256-bit key
    std::vector<uint8_t> key = generateSecureKey(32);
    
    std::cout << "Generated 256-bit key: ";
    for (uint8_t byte : key) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl << std::endl;
    
    // Create a test file
    std::string testContent = "This is a test file that will be encrypted using AES-256.\n"
                              "The content includes multiple lines and special characters: !@#$%^&*()\n"
                              "Numbers: 1234567890\n"
                              "Unicode: Hello 世界!";
    
    std::ofstream testFile("test_input.txt");
    testFile << testContent;
    testFile.close();
    
    std::cout << "Created test file: test_input.txt" << std::endl;
    std::cout << "Content: " << std::endl << testContent << std::endl << std::endl;
    
    // Encrypt the file
    std::cout << "Encrypting file..." << std::endl;
    encryptFile("test_input.txt", "test_encrypted.bin", key);
    std::cout << std::endl;
    
    // Decrypt the file
    std::cout << "Decrypting file..." << std::endl;
    decryptFile("test_encrypted.bin", "test_decrypted.txt", key);
    std::cout << std::endl;
    
    // Verify the decryption
    std::ifstream decryptedFile("test_decrypted.txt");
    std::string decryptedContent((std::istreambuf_iterator<char>(decryptedFile)),
                                 std::istreambuf_iterator<char>());
    decryptedFile.close();
    
    std::cout << "Verification:" << std::endl;
    std::cout << "Original content matches decrypted content: " 
              << (testContent == decryptedContent ? "YES" : "NO") << std::endl;
    
    if (testContent == decryptedContent) {
        std::cout << "Decrypted content: " << std::endl << decryptedContent << std::endl;
    }
    
    // Clean up test files
    std::remove("test_input.txt");
    std::remove("test_encrypted.bin");
    std::remove("test_decrypted.txt");
    
    std::cout << std::endl << "Test files cleaned up." << std::endl;
    
    return 0;
}