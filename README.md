# AES Encryption/Decryption Implementation in C++

This is a complete implementation of the Advanced Encryption Standard (AES) algorithm in C++. The implementation supports AES-128, AES-192, and AES-256 encryption and decryption.

## Features

- **Full AES Implementation**: Complete implementation of the AES algorithm with all standard operations
- **Multiple Key Sizes**: Support for AES-128, AES-192, and AES-256
- **PKCS7 Padding**: Proper padding implementation for variable-length data
- **String Interface**: Easy-to-use string-based encryption/decryption methods
- **Error Handling**: Comprehensive error checking and exception handling
- **Standards Compliant**: Follows the official AES specification (FIPS 197)

## Files

- `aes.h` - Header file with class declaration and constants
- `aes.cpp` - Implementation of all AES operations
- `main.cpp` - Example usage and demonstration
- `Makefile` - Build configuration

## AES Algorithm Overview

AES is a symmetric block cipher that operates on 128-bit blocks with key sizes of 128, 192, or 256 bits. The algorithm consists of:

1. **Key Expansion**: Generates round keys from the original key
2. **Initial Round**: AddRoundKey operation
3. **Main Rounds**: SubBytes, ShiftRows, MixColumns, AddRoundKey (except last round)
4. **Final Round**: SubBytes, ShiftRows, AddRoundKey

### Core Operations

- **SubBytes**: Non-linear substitution using S-box tables
- **ShiftRows**: Cyclically shifts rows of the state array
- **MixColumns**: Linear transformation mixing columns using GF(2^8) arithmetic
- **AddRoundKey**: XOR operation with round keys

## Usage Example

```cpp
#include "aes.h"
#include <iostream>

int main() {
    // Create AES instance with 256-bit key
    AES aes(AES::AES_256);
    
    std::string message = "Hello, this is a secret message!";
    std::string password = "MySecretPassword123";
    
    // Encrypt
    std::string encrypted = aes.encryptString(message, password);
    std::cout << "Encrypted: " << encrypted << std::endl;
    
    // Decrypt
    std::string decrypted = aes.decryptString(encrypted, password);
    std::cout << "Decrypted: " << decrypted << std::endl;
    
    return 0;
}
```

## Building and Running

```bash
# Compile the project
make

# Run the demo
./aes_demo

# Clean build files
make clean
```

## Security Notes

- This implementation is for educational purposes
- For production use, consider using established cryptographic libraries like OpenSSL
- The key derivation function is simplified; use PBKDF2 or similar for production
- Always use cryptographically secure random number generators for keys and IVs

## Technical Details

### Key Sizes and Rounds
- AES-128: 128-bit key, 10 rounds
- AES-192: 192-bit key, 12 rounds  
- AES-256: 256-bit key, 14 rounds

### Block Size
- Fixed 128-bit (16 bytes) block size for all AES variants

### Padding
- PKCS7 padding is used to handle variable-length data
- Padding is automatically added during encryption and removed during decryption

## Dependencies

- C++11 or later
- Standard C++ library (no external dependencies)

## License

This implementation is provided as-is for educational purposes.
