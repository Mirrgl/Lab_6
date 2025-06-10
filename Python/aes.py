import os
import secrets
import sys

# S-box and inverse S-box
sbox = [
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
]

inv_sbox = [
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
]

# Rcon table
Rcon = [0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36]

# Helper functions
def printState(state):
    for i in range(4):
        for j in range(4):
            print(f"{state[j][i]:02x}", end=" ")
        print()
    print()

def printKey(key):
    for i, byte in enumerate(key):
        print(f"{byte:02x}", end=" ")
        if (i + 1) % 16 == 0:
            print()

def bytesToState(bytes_list):
    state = [[0] * 4 for _ in range(4)]
    for i in range(4):
        for j in range(4):
            state[j][i] = bytes_list[i * 4 + j]
    return state

def stateToBytes(state):
    bytes_list = [0] * 16
    for i in range(4):
        for j in range(4):
            bytes_list[i * 4 + j] = state[j][i]
    return bytes_list

# AES transformations
def subBytes(state):
    for i in range(4):
        for j in range(4):
            state[i][j] = sbox[state[i][j]]

def invSubBytes(state):
    for i in range(4):
        for j in range(4):
            state[i][j] = inv_sbox[state[i][j]]

def shiftRows(state):
    # Row 1: no shift
    # Row 2: shift left by 1
    temp = state[1][0]
    state[1][0] = state[1][1]
    state[1][1] = state[1][2]
    state[1][2] = state[1][3]
    state[1][3] = temp
    
    # Row 3: shift left by 2
    state[2][0], state[2][2] = state[2][2], state[2][0]
    state[2][1], state[2][3] = state[2][3], state[2][1]
    
    # Row 4: shift left by 3 (or right by 1)
    temp = state[3][3]
    state[3][3] = state[3][2]
    state[3][2] = state[3][1]
    state[3][1] = state[3][0]
    state[3][0] = temp

def invShiftRows(state):
    # Row 1: no shift
    # Row 2: shift right by 1
    temp = state[1][3]
    state[1][3] = state[1][2]
    state[1][2] = state[1][1]
    state[1][1] = state[1][0]
    state[1][0] = temp
    
    # Row 3: shift left by 2 (same as shift right by 2)
    state[2][0], state[2][2] = state[2][2], state[2][0]
    state[2][1], state[2][3] = state[2][3], state[2][1]
    
    # Row 4: shift right by 3 (or left by 1)
    temp = state[3][0]
    state[3][0] = state[3][1]
    state[3][1] = state[3][2]
    state[3][2] = state[3][3]
    state[3][3] = temp

def gmul(a, b):
    p = 0
    for _ in range(8):
        if b & 1:
            p ^= a
        hi_bit_set = a & 0x80
        a <<= 1
        if hi_bit_set:
            a ^= 0x1b  # x^8 + x^4 + x^3 + x + 1
        b >>= 1
    return p & 0xFF  # Keep within byte

def mixColumns(state):
    for i in range(4):
        s0 = state[i][0]
        s1 = state[i][1]
        s2 = state[i][2]
        s3 = state[i][3]
        
        state[i][0] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3
        state[i][1] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3
        state[i][2] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3)
        state[i][3] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3)

def invMixColumns(state):
    for i in range(4):
        s0 = state[i][0]
        s1 = state[i][1]
        s2 = state[i][2]
        s3 = state[i][3]
        
        state[i][0] = gmul(0x0e, s0) ^ gmul(0x0b, s1) ^ gmul(0x0d, s2) ^ gmul(0x09, s3)
        state[i][1] = gmul(0x09, s0) ^ gmul(0x0e, s1) ^ gmul(0x0b, s2) ^ gmul(0x0d, s3)
        state[i][2] = gmul(0x0d, s0) ^ gmul(0x09, s1) ^ gmul(0x0e, s2) ^ gmul(0x0b, s3)
        state[i][3] = gmul(0x0b, s0) ^ gmul(0x0d, s1) ^ gmul(0x09, s2) ^ gmul(0x0e, s3)

def addRoundKey(state, roundKey):
    for i in range(4):
        for j in range(4):
            state[i][j] ^= roundKey[i][j]

# Key expansion
def rotWord(word):
    return [word[1], word[2], word[3], word[0]]

def subWord(word):
    return [sbox[b] for b in word]

def expandKey(key):
    roundKeys = [[0] * 16 for _ in range(11)]
    
    # First round key is the original key
    roundKeys[0] = key.copy()
    
    for i in range(1, 11):
        # Get last 4 bytes of previous key
        temp = roundKeys[i-1][12:16]
        
        # RotWord and SubWord
        temp = subWord(rotWord(temp))
        
        # XOR with Rcon
        temp[0] ^= Rcon[i]
        
        # First 4 bytes of new key
        for j in range(4):
            roundKeys[i][j] = roundKeys[i-1][j] ^ temp[j]
        
        # Remaining 12 bytes
        for j in range(4, 16):
            roundKeys[i][j] = roundKeys[i-1][j] ^ roundKeys[i][j-4]
    
    return roundKeys

# AES encryption and decryption
def aesEncryptBlock(block, roundKeys):
    state = bytesToState(block)
    roundKey = bytesToState(roundKeys[0])
    
    print("Initial state:")
    printState(state)
    print("Initial round key:")
    printState(roundKey)
    
    # Initial round
    addRoundKey(state, roundKey)
    print("After initial AddRoundKey:")
    printState(state)
    
    # 9 main rounds
    for rnd in range(1, 10):
        print(f"Round {rnd}:")
        
        subBytes(state)
        print("After SubBytes:")
        printState(state)
        
        shiftRows(state)
        print("After ShiftRows:")
        printState(state)
        
        mixColumns(state)
        print("After MixColumns:")
        printState(state)
        
        roundKey_state = bytesToState(roundKeys[rnd])
        print("Round key:")
        printState(roundKey_state)
        
        addRoundKey(state, roundKey_state)
        print("After AddRoundKey:")
        printState(state)
    
    # Final round
    print("Round 10:")
    subBytes(state)
    print("After SubBytes:")
    printState(state)
    
    shiftRows(state)
    print("After ShiftRows:")
    printState(state)
    
    roundKey_state = bytesToState(roundKeys[10])
    print("Round key:")
    printState(roundKey_state)
    
    addRoundKey(state, roundKey_state)
    print("After AddRoundKey:")
    printState(state)
    
    return stateToBytes(state)

def aesDecryptBlock(block, roundKeys):
    state = bytesToState(block)
    roundKey = bytesToState(roundKeys[10])
    
    # Initial round
    addRoundKey(state, roundKey)
    invShiftRows(state)
    invSubBytes(state)
    
    # 9 main rounds
    for rnd in range(9, 0, -1):
        roundKey_state = bytesToState(roundKeys[rnd])
        addRoundKey(state, roundKey_state)
        invMixColumns(state)
        invShiftRows(state)
        invSubBytes(state)
    
    # Final round
    roundKey_state = bytesToState(roundKeys[0])
    addRoundKey(state, roundKey_state)
    
    return stateToBytes(state)

# OFB mode
def generateIV():
    return list(secrets.token_bytes(16))

def generateKey():
    return list(secrets.token_bytes(16))

def saveKeyToFile(key, filename):
    try:
        with open(filename, 'wb') as file:
            file.write(bytes(key))
    except IOError:
        print(f"Unable to open file for writing key: {filename}")

def readKeyFromFile(filename):
    try:
        with open(filename, 'rb') as file:
            key = file.read(16)
            return list(key)
    except IOError:
        print(f"Unable to open file for reading key: {filename}")
        return []

def padData(data):
    pad_len = 16 - (len(data) % 16)
    padded = data.copy()
    padded.extend([pad_len] * pad_len)
    return padded

def unpadData(data):
    if not data:
        return data
    pad_len = data[-1]
    if pad_len > 16:
        return data  # Invalid padding
    for i in range(len(data) - pad_len, len(data)):
        if data[i] != pad_len:
            return data  # Invalid padding
    return data[:-pad_len]

def aesOFBEncrypt(plaintext, key, iv):
    roundKeys = expandKey(key)
    
    print("Expanded keys:")
    for i, rk in enumerate(roundKeys):
        print(f"Round {i} key:")
        printKey(rk)
    
    padded = padData(plaintext)
    blocks = len(padded) // 16
    ciphertext = []
    
    print("Initialization Vector (IV):")
    printKey(iv)
    
    for i in range(blocks):
        print(f"Block {i+1} processing:")
        block_start = i * 16
        block_end = block_start + 16
        
        # Encrypt IV to get keystream
        keystream = aesEncryptBlock(iv, roundKeys)
        
        # XOR plaintext block with keystream
        block = []
        for j in range(16):
            block.append(padded[block_start + j] ^ keystream[j])
        ciphertext.extend(block)
        
        # Update IV for next block
        iv[:] = keystream
    
    return ciphertext

def aesOFBDecrypt(ciphertext, key, iv):
    roundKeys = expandKey(key)
    plaintext = []
    blocks = len(ciphertext) // 16
    
    for i in range(blocks):
        block_start = i * 16
        block_end = block_start + 16
        
        # Encrypt IV to get keystream
        keystream = aesEncryptBlock(iv, roundKeys)
        
        # XOR ciphertext block with keystream
        block = []
        for j in range(16):
            block.append(ciphertext[block_start + j] ^ keystream[j])
        plaintext.extend(block)
        
        # Update IV for next block
        iv[:] = keystream
    
    return unpadData(plaintext)




choice = input("AES-128 OFB Mode Implementation\n1. Encrypt\n2. Decrypt\n3. Generate new random key\nEnter choice: ")
key_file = "aes_key.bin"
iv_file = "aes_iv.bin"
key = []
iv = []

if choice == '3':
    key = generateKey()
    saveKeyToFile(key, key_file)
    print(f"New random key generated and saved to {key_file}")
    print("Key: ", end="")
    printKey(key)
    
    iv = generateIV()
    saveKeyToFile(iv, iv_file)
    print(f"New random IV generated and saved to {iv_file}")
    print("IV: ", end="")
    printKey(iv)
    exit()

input_text = input("Enter text: ")

if choice == '1':
    # Encryption
    key = generateKey()
    saveKeyToFile(key, key_file)
    iv = generateIV()
    saveKeyToFile(iv, iv_file)
    
    print("Generated key: ", end="")
    printKey(key)
    print(f"Key saved to {key_file}\n")
    print("Generated IV: ", end="")
    printKey(iv)
    print(f"IV saved to {iv_file}\n")
    
    plaintext = [ord(c) for c in input_text]
    ciphertext = aesOFBEncrypt(plaintext, key, iv)
    
    print("Ciphertext (hex): ", end="")
    for byte in ciphertext:
        print(f"{byte:02x}", end="")
    print()

elif choice == '2':
    # Decryption
    key = readKeyFromFile(key_file)
    if not key:
        print("Error: Could not read key from file. Please encrypt first to generate a key.")
        exit()
    
    iv_choice = input("Choose:\n1. Input IV key manually\n2. Read IV key from file\nEnter choice: ")
    if iv_choice == '1':
        iv_hex = input("Enter IV (32 hex chars): ")
        if len(iv_hex) != 32:
            print("Error: IV must be 32 hexadecimal characters (16 bytes).")
            exit()
        iv = [int(iv_hex[i:i+2], 16) for i in range(0, 32, 2)]
    elif iv_choice == '2':
        iv = readKeyFromFile(iv_file)
        if not iv:
            print("Error: Could not read IV from file. Please encrypt first to generate an IV.")
            exit()
    else:
        print("Invalid choice")
        exit()
    
    # Convert hex string to bytes
    ciphertext = [int(input_text[i:i+2], 16) for i in range(0, len(input_text), 2)]
    plaintext_bytes = aesOFBDecrypt(ciphertext, key, iv)
    plaintext = ''.join(chr(b) for b in plaintext_bytes)
    print("Decrypted text:", plaintext)