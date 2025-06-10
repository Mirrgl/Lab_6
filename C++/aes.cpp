#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <random>
#include <algorithm>
#include <string>

using namespace std;

// S-box and inverse S-box
const unsigned char sbox[256] = {
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
};

const unsigned char inv_sbox[256] = {
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
};

// Rcon table
const unsigned char Rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

typedef vector<vector<unsigned char>> State;

// Helper functions
void printState(const State& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << hex << setw(2) << setfill('0') << (int)state[j][i] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void printKey(const vector<unsigned char>& key) {
    for (int i = 0; i < key.size(); ++i) {
        cout << hex << setw(2) << setfill('0') << (int)key[i] << " ";
        if ((i + 1) % 16 == 0) cout << endl;
    }
}

State bytesToState(const vector<unsigned char>& bytes) {
    State state(4, vector<unsigned char>(4));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = bytes[i * 4 + j];
        }
    }
    return state;
}

vector<unsigned char> stateToBytes(const State& state) {
    vector<unsigned char> bytes(16);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            bytes[i * 4 + j] = state[j][i];
        }
    }
    return bytes;
}

// AES transformations
void subBytes(State& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

void invSubBytes(State& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

void shiftRows(State& state) {
    // Row 1: no shift
    // Row 2: shift left by 1
    unsigned char temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
    
    // Row 3: shift left by 2
    swap(state[2][0], state[2][2]);
    swap(state[2][1], state[2][3]);
    
    // Row 4: shift left by 3 (or right by 1)
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void invShiftRows(State& state) {
    // Row 1: no shift
    // Row 2: shift right by 1
    unsigned char temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
    
    // Row 3: shift left by 2 (same as shift right by 2)
    swap(state[2][0], state[2][2]);
    swap(state[2][1], state[2][3]);
    
    // Row 4: shift right by 3 (or left by 1)
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
        b >>= 1;
    }
    return p;
}

void mixColumns(State& state) {
    for (int i = 0; i < 4; ++i) {
        unsigned char s0 = state[i][0];
        unsigned char s1 = state[i][1];
        unsigned char s2 = state[i][2];
        unsigned char s3 = state[i][3];
        
        state[i][0] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3;
        state[i][1] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3;
        state[i][2] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3);
        state[i][3] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3);
    }
}

void invMixColumns(State& state) {
    for (int i = 0; i < 4; ++i) {
        unsigned char s0 = state[i][0];
        unsigned char s1 = state[i][1];
        unsigned char s2 = state[i][2];
        unsigned char s3 = state[i][3];
        
        state[i][0] = gmul(0x0e, s0) ^ gmul(0x0b, s1) ^ gmul(0x0d, s2) ^ gmul(0x09, s3);
        state[i][1] = gmul(0x09, s0) ^ gmul(0x0e, s1) ^ gmul(0x0b, s2) ^ gmul(0x0d, s3);
        state[i][2] = gmul(0x0d, s0) ^ gmul(0x09, s1) ^ gmul(0x0e, s2) ^ gmul(0x0b, s3);
        state[i][3] = gmul(0x0b, s0) ^ gmul(0x0d, s1) ^ gmul(0x09, s2) ^ gmul(0x0e, s3);
    }
}

void addRoundKey(State& state, const State& roundKey) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

// Key expansion
vector<unsigned char> rotWord(const vector<unsigned char>& word) {
    vector<unsigned char> result(4);
    result[0] = word[1];
    result[1] = word[2];
    result[2] = word[3];
    result[3] = word[0];
    return result;
}

vector<unsigned char> subWord(const vector<unsigned char>& word) {
    vector<unsigned char> result(4);
    for (int i = 0; i < 4; ++i) {
        result[i] = sbox[word[i]];
    }
    return result;
}

vector<vector<unsigned char>> expandKey(const vector<unsigned char>& key) {
    vector<vector<unsigned char>> roundKeys(11, vector<unsigned char>(16));
    
    // First round key is the original key
    for (int i = 0; i < 16; ++i) {
        roundKeys[0][i] = key[i];
    }
    
    for (int i = 1; i < 11; ++i) {
        vector<unsigned char> temp(4);
        
        // Get the last 4 bytes of the previous key
        for (int j = 0; j < 4; ++j) {
            temp[j] = roundKeys[i-1][12 + j];
        }
        
        // RotWord and SubWord
        temp = subWord(rotWord(temp));
        
        // XOR with Rcon
        temp[0] ^= Rcon[i];
        
        // First 4 bytes of new key
        for (int j = 0; j < 4; ++j) {
            roundKeys[i][j] = roundKeys[i-1][j] ^ temp[j];
        }
        
        // Remaining 12 bytes
        for (int j = 4; j < 16; ++j) {
            roundKeys[i][j] = roundKeys[i-1][j] ^ roundKeys[i][j-4];
        }
    }
    
    return roundKeys;
}

// AES encryption and decryption
vector<unsigned char> aesEncryptBlock(const vector<unsigned char>& block, const vector<vector<unsigned char>>& roundKeys) {
    State state = bytesToState(block);
    State roundKey = bytesToState(roundKeys[0]);
    
    cout << "Initial state:" << endl;
    printState(state);
    cout << "Initial round key:" << endl;
    printState(roundKey);
    
    // Initial round
    addRoundKey(state, roundKey);
    cout << "After initial AddRoundKey:" << endl;
    printState(state);
    
    // 9 main rounds
    for (int round = 1; round < 10; ++round) {
        cout << "Round " << round << ":" << endl;
        
        subBytes(state);
        cout << "After SubBytes:" << endl;
        printState(state);
        
        shiftRows(state);
        cout << "After ShiftRows:" << endl;
        printState(state);
        
        mixColumns(state);
        cout << "After MixColumns:" << endl;
        printState(state);
        
        roundKey = bytesToState(roundKeys[round]);
        cout << "Round key:" << endl;
        printState(roundKey);
        
        addRoundKey(state, roundKey);
        cout << "After AddRoundKey:" << endl;
        printState(state);
    }
    
    // Final round
    cout << "Round 10:" << endl;
    subBytes(state);
    cout << "After SubBytes:" << endl;
    printState(state);
    
    shiftRows(state);
    cout << "After ShiftRows:" << endl;
    printState(state);
    
    roundKey = bytesToState(roundKeys[10]);
    cout << "Round key:" << endl;
    printState(roundKey);
    
    addRoundKey(state, roundKey);
    cout << "After AddRoundKey:" << endl;
    printState(state);
    
    return stateToBytes(state);
}

vector<unsigned char> aesDecryptBlock(const vector<unsigned char>& block, const vector<vector<unsigned char>>& roundKeys) {
    State state = bytesToState(block);
    State roundKey = bytesToState(roundKeys[10]);
    
    // Initial round
    addRoundKey(state, roundKey);
    invShiftRows(state);
    invSubBytes(state);
    
    // 9 main rounds
    for (int round = 9; round > 0; --round) {
        roundKey = bytesToState(roundKeys[round]);
        addRoundKey(state, roundKey);
        invMixColumns(state);
        invShiftRows(state);
        invSubBytes(state);
    }
    
    // Final round
    roundKey = bytesToState(roundKeys[0]);
    addRoundKey(state, roundKey);
    
    return stateToBytes(state);
}

// OFB mode
vector<unsigned char> generateIV() {
    vector<unsigned char> iv(16);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < 16; ++i) {
        iv[i] = static_cast<unsigned char>(dis(gen));
    }
    return iv;
}

vector<unsigned char> generateKey() {
    vector<unsigned char> key(16);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < 16; ++i) {
        key[i] = static_cast<unsigned char>(dis(gen));
    }
    return key;
}

void saveKeyToFile(const vector<unsigned char>& key, const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(key.data()), key.size());
        file.close();
    } else {
        cerr << "Unable to open file for writing key: " << filename << endl;
    }
}

vector<unsigned char> readKeyFromFile(const string& filename) {
    vector<unsigned char> key(16);
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(key.data()), key.size());
        file.close();
    } else {
        cerr << "Unable to open file for reading key: " << filename << endl;
    }
    return key;
}

vector<unsigned char> padData(const vector<unsigned char>& data) {
    size_t padLen = 16 - (data.size() % 16);
    vector<unsigned char> padded = data;
    for (size_t i = 0; i < padLen; ++i) {
        padded.push_back(static_cast<unsigned char>(padLen));
    }
    return padded;
}

vector<unsigned char> unpadData(const vector<unsigned char>& data) {
    if (data.empty()) return data;
    unsigned char padLen = data.back();
    if (padLen > 16) return data; // Invalid padding
    
    for (size_t i = data.size() - padLen; i < data.size(); ++i) {
        if (data[i] != padLen) return data; // Invalid padding
    }
    
    return vector<unsigned char>(data.begin(), data.end() - padLen);
}

vector<unsigned char> aesOFBEncrypt(const vector<unsigned char>& plaintext, 
                                   const vector<unsigned char>& key, 
                                   vector<unsigned char>& iv) {
    vector<vector<unsigned char>> roundKeys = expandKey(key);
    vector<unsigned char> ciphertext;
    
    cout << "Expanded keys:" << endl;
    for (int i = 0; i < roundKeys.size(); ++i) {
        cout << "Round " << i << " key:" << endl;
        printKey(roundKeys[i]);
    }
    
    vector<unsigned char> padded = padData(plaintext);
    size_t blocks = padded.size() / 16;
    
    cout << "Initialization Vector (IV):" << endl;
    printKey(iv);
    
    for (size_t i = 0; i < blocks; ++i) {
        cout << "Block " << i + 1 << " processing:" << endl;
        
        // Encrypt the IV to get the keystream
        vector<unsigned char> keystream = aesEncryptBlock(iv, roundKeys);
        
        // XOR the plaintext block with the keystream
        vector<unsigned char> block(16);
        for (int j = 0; j < 16; ++j) {
            block[j] = padded[i * 16 + j] ^ keystream[j];
        }
        
        // The next IV is the keystream (not the ciphertext)
        iv = keystream;
        
        // Append to ciphertext
        ciphertext.insert(ciphertext.end(), block.begin(), block.end());
    }
    
    return ciphertext;
}

vector<unsigned char> aesOFBDecrypt(const vector<unsigned char>& ciphertext, 
                                   const vector<unsigned char>& key, 
                                   vector<unsigned char>& iv) {
    vector<vector<unsigned char>> roundKeys = expandKey(key);
    vector<unsigned char> plaintext;
    
    size_t blocks = ciphertext.size() / 16;
    
    for (size_t i = 0; i < blocks; ++i) {
        // Encrypt the IV to get the keystream
        vector<unsigned char> keystream = aesEncryptBlock(iv, roundKeys);
        
        // XOR the ciphertext block with the keystream
        vector<unsigned char> block(16);
        for (int j = 0; j < 16; ++j) {
            block[j] = ciphertext[i * 16 + j] ^ keystream[j];
        }
        
        // The next IV is the keystream (not the plaintext)
        iv = keystream;
        
        // Append to plaintext
        plaintext.insert(plaintext.end(), block.begin(), block.end());
    }
    
    return unpadData(plaintext);
}

int main() {
    int choice;
    string inputText, keyFile = "aes_key.bin";
    string ivFile = "aes_iv.bin";
    vector<unsigned char> key, iv;
    
    cout << "AES-128 OFB Mode Implementation\n";
    cout << "1. Encrypt\n2. Decrypt\n3. Generate new random key\nEnter choice: ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 3) {
        key = generateKey();
        saveKeyToFile(key, keyFile);
        cout << "New random key generated and saved to " << keyFile << endl;
        cout << "Key: ";
        printKey(key);

        iv = generateIV();
        saveKeyToFile(iv, ivFile);
        cout << "New random IV key generated and saved to " << ivFile << endl;
        cout << "IV: ";
        printKey(iv);
        return 0;
    }
    
    cout << "Enter text: ";
    getline(cin, inputText);
    
    if (choice == 1) {
        // Encryption
        key = generateKey();
        saveKeyToFile(key, keyFile);
        iv = generateIV();
        saveKeyToFile(iv, ivFile);
        
        cout << "Generated key: ";
        printKey(key);
        cout << "Key saved to " << keyFile << endl << endl;
        cout << "Generated IV: ";
        printKey(iv);
        cout << "IV key saved to " << ivFile << endl << endl;

        
        vector<unsigned char> plaintext(inputText.begin(), inputText.end());
        vector<unsigned char> ciphertext = aesOFBEncrypt(plaintext, key, iv);
        
        cout << "Ciphertext (hex): ";
        for (unsigned char c : ciphertext) {
            cout << hex << setw(2) << setfill('0') << (int)c;
        }
        cout << endl;
    } else if (choice == 2) {
        // Decryption
        key = readKeyFromFile(keyFile);
        if (key.empty()) {
            cerr << "Error: Could not read key from file. Please encrypt first to generate a key." << endl;
            return 1;
        }
        
        string ivHex;

        cout << "Choose:" << endl;
        cout << "1. Input IV key manually\n2. Read IV key from file " << ivFile << endl;
        int chooseFlag = 0;
        cin >> chooseFlag;
        switch (chooseFlag) {
            case 1:
                cout << "Enter IV (32 hex chars): ";
                getline(cin, ivHex);
                
                if (ivHex.length() != 32) {
                    cerr << "Error: IV must be 32 hexadecimal characters (16 bytes)." << endl;
                    return 1;
                }

                iv.resize(16);
                for (int i = 0; i < 16; ++i) {
                    iv[i] = stoul(ivHex.substr(i*2, 2), nullptr, 16);
                }
                break;
            case 2:
                iv = readKeyFromFile(ivFile);
                if (key.empty()) {
                    cerr << "Error: Could not read IV key from file. Please encrypt first to generate a IV key." << endl;
                    return 1;
                }
                break;
            default:
                cerr << "Invalid input" << endl;
        }
        
        // Convert hex string to bytes
        vector<unsigned char> ciphertext;
        for (size_t i = 0; i < inputText.length(); i += 2) {
            string byteString = inputText.substr(i, 2);
            ciphertext.push_back(static_cast<unsigned char>(stoul(byteString, nullptr, 16)));
        }
        
        vector<unsigned char> plaintext = aesOFBDecrypt(ciphertext, key, iv);
        
        cout << "Decrypted text: ";
        for (unsigned char c : plaintext) {
            cout << c;
        }
        cout << endl;
    }
    
    return 0;
}