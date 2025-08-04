# Project Files Overview

This directory contains a complete C++ implementation of the AES (Advanced Encryption Standard) algorithm.

## Core Implementation Files

### `aes.h`
- **Purpose**: Header file containing the AES class declaration
- **Contents**: 
  - AES class definition with public and private members
  - Enum for key sizes (AES_128, AES_192, AES_256)
  - Method declarations for encryption, decryption, and utility functions
- **Key Features**: Modern C++ design with proper encapsulation

### `aes.cpp`
- **Purpose**: Complete implementation of the AES algorithm
- **Contents**:
  - S-box and inverse S-box lookup tables
  - Key expansion algorithm
  - All AES operations: SubBytes, ShiftRows, MixColumns, AddRoundKey
  - PKCS7 padding implementation
  - String and binary data handling
- **Key Features**: Full AES-128/192/256 support with proper error handling

## Demo and Example Files

### `main.cpp`
- **Purpose**: Comprehensive demo program showing AES usage
- **Contents**:
  - Tests with all three key sizes (128, 192, 256 bits)
  - String and binary data encryption/decryption
  - Error handling demonstrations
  - Verification of encryption/decryption correctness
- **Usage**: `./aes_demo`

### `example.cpp`
- **Purpose**: Practical example showing file encryption/decryption
- **Contents**:
  - File I/O operations
  - Secure key generation
  - Complete file encryption/decryption workflow
  - Unicode text handling
- **Usage**: `./aes_example`

## Build and Documentation Files

### `Makefile`
- **Purpose**: Build configuration and automation
- **Features**:
  - Compiles both demo and example programs
  - Debug and release build targets
  - Clean and test targets
  - Installation targets
- **Usage**: `make`, `make debug`, `make release`, `make clean`

### `README.md`
- **Purpose**: Comprehensive documentation
- **Contents**:
  - Algorithm explanation
  - Installation and usage instructions
  - API reference
  - Security considerations
  - Performance notes

### `FILES.md`
- **Purpose**: This file - overview of all project files

## Compilation and Usage

### Building the Project
```bash
# Build all programs
make

# Build with debug information
make debug

# Build optimized release version
make release

# Clean build files
make clean
```

### Running the Programs
```bash
# Run the comprehensive demo
./aes_demo

# Run the file encryption example
./aes_example
```

## Key Features

1. **Complete AES Implementation**: Full implementation of AES-128, AES-192, and AES-256
2. **Modern C++**: Uses C++17 features and best practices
3. **Comprehensive Testing**: Both programs include extensive testing
4. **Error Handling**: Proper exception handling and validation
5. **Documentation**: Well-documented code with examples
6. **Build System**: Automated build process with multiple targets

## Security Notes

- This implementation is for educational purposes
- Uses cryptographically secure random number generation
- Implements proper PKCS7 padding
- Includes comprehensive error checking
- For production use, consider established libraries like OpenSSL

## File Sizes and Structure

- **aes.h**: ~2KB - Header file with class definition
- **aes.cpp**: ~15KB - Complete AES implementation
- **main.cpp**: ~4KB - Demo program
- **example.cpp**: ~6KB - File encryption example
- **Makefile**: ~1KB - Build configuration
- **README.md**: ~8KB - Documentation
- **FILES.md**: ~2KB - This overview file

Total project size: ~38KB of source code and documentation