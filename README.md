# AES Encryption/Decryption Implementation in C++

This is a complete implementation of the Advanced Encryption Standard (AES) algorithm in C++. The implementation supports all three standard AES key sizes: 128-bit, 192-bit, and 256-bit.

## Features

- **Complete AES Implementation**: Full implementation of the AES algorithm with all required operations
- **Multiple Key Sizes**: Support for AES-128, AES-192, and AES-256
- **PKCS7 Padding**: Proper padding implementation for variable-length data
- **String and Binary Support**: Easy-to-use interfaces for both string and binary data
- **Error Handling**: Comprehensive error checking and exception handling
- **Modern C++**: Uses C++17 features and modern programming practices

## Algorithm Details

AES is a symmetric block cipher that operates on 128-bit blocks. The implementation includes:

- **Key Expansion**: Generates round keys from the master key
- **SubBytes**: Non-linear substitution using S-boxes
- **ShiftRows**: Cyclic shifting of rows
- **MixColumns**: Linear transformation of columns
- **AddRoundKey**: XOR with round key

### Key Sizes and Rounds

- **AES-128**: 16-byte key, 10 rounds
- **AES-192**: 24-byte key, 12 rounds  
- **AES-256**: 32-byte key, 14 rounds

## Files

- `aes.h` - Header file with class declaration
- `aes.cpp` - Implementation of AES algorithm
- `main.cpp` - Demo program showing usage examples
- `Makefile` - Build configuration
- `README.md` - This documentation

## Building

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- Make (for using the Makefile)

### Compilation

```bash
# Build the demo program
make

# Build with debug information
make debug

# Build optimized release version
make release

# Clean build files
make clean
```

## Usage

### Basic Usage

```cpp
#include "aes.h"
#include <vector>

// Create a 256-bit key (32 bytes)
std::vector<uint8_t> key = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

// Initialize AES with the key
AES aes(key);

// Encrypt a string
std::string plaintext = "Hello, World!";
std::string encrypted = aes.encryptString(plaintext);

// Decrypt the string
std::string decrypted = aes.decryptString(encrypted);
```

### Working with Binary Data

```cpp
// Encrypt binary data
std::vector<uint8_t> binaryData = {0x00, 0x01, 0x02, 0x03, /* ... */};
std::vector<uint8_t> encrypted = aes.encrypt(binaryData);

// Decrypt binary data
std::vector<uint8_t> decrypted = aes.decrypt(encrypted);
```

### Different Key Sizes

```cpp
// AES-128 (16 bytes)
std::vector<uint8_t> key128(16);
AES aes128(key128);

// AES-192 (24 bytes)
std::vector<uint8_t> key192(24);
AES aes192(key192);

// AES-256 (32 bytes)
std::vector<uint8_t> key256(32);
AES aes256(key256);
```

## API Reference

### Constructor

```cpp
AES(const std::vector<uint8_t>& key);
```

Creates an AES instance with the specified key. The key size must be 16, 24, or 32 bytes.

### Encryption Methods

```cpp
std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
std::string encryptString(const std::string& plaintext);
```

Encrypts the input data. The string version returns a hexadecimal representation of the encrypted data.

### Decryption Methods

```cpp
std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
std::string decryptString(const std::string& ciphertext);
```

Decrypts the input data. The string version expects a hexadecimal representation of the encrypted data.

## Security Considerations

- **Key Management**: Always use cryptographically secure random number generators for key generation
- **Key Storage**: Store keys securely and never hardcode them in source code
- **Padding**: The implementation uses PKCS7 padding, which is secure for most applications
- **Mode of Operation**: This implementation uses ECB mode. For production use, consider implementing CBC, GCM, or other authenticated encryption modes

## Testing

Run the demo program to see the implementation in action:

```bash
make test
```

The demo includes:
- Tests with all three key sizes
- String and binary data encryption/decryption
- Error handling examples
- Verification that encryption/decryption works correctly

## Performance

The implementation is optimized for clarity and correctness rather than maximum performance. For production use requiring high performance, consider:

- Using hardware acceleration (AES-NI instructions)
- Implementing parallel processing for large data
- Using optimized cryptographic libraries like OpenSSL

## License

This implementation is provided for educational purposes. For production use, consider using established cryptographic libraries that have been thoroughly tested and audited.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve this implementation.

## Disclaimer

This implementation is for educational purposes. While it follows the AES specification, it has not been formally audited for security vulnerabilities. For production applications, use established cryptographic libraries.
