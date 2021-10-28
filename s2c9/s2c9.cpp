#include <cstdint>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <fstream>
#include <streambuf>
#include <algorithm>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/evperr.h>

using byte_t = uint8_t;

struct byte_array
{
    byte_t *bytes;
    size_t length;

    byte_array()
    {
        length = 0;
        bytes = NULL;
    }

    byte_array(size_t _length)
    {
        length = _length;
        bytes = new byte_t[length];
    }

    byte_array(const byte_array &other) 
    {
        length = other.length; 
        bytes = (byte_t*)malloc(length * sizeof(byte_t)); 
        memcpy(bytes, other.bytes, other.length * sizeof(byte_t)); 
    }

    ~byte_array()
    {
        delete [] bytes;
    }
};

const char *BASE_64_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char BASE_64_PADDING = '=';

int base64_char_to_hex(char ch)
{
    char *found = strchr(BASE_64_CHARACTERS, ch);
    if( found == NULL )
    {
        return 0;
    }
    // printf("%02x ", found - BASE_64_CHARACTERS);

    return found - BASE_64_CHARACTERS; 
}

// Base64 in string format
byte_array *base64_to_hex(byte_array *base64)
{
    // because base64 is a 6-bit encoding, we convert at 24-bit intervals to appease the byte system 
    // therefore, there are 3 hex digits to every 4 base64 digits 
    // int triplet_count = (int)ceilf(hex_length / 3.0f);
    int triplet_count = base64->length / 4;

    byte_array *hex = new byte_array(triplet_count * 3);

    for (int triplet_idx = 0; triplet_idx < triplet_count; triplet_idx++ )
    {
        int triplet = 0;

        int i = triplet_idx * 4;
        triplet |= base64_char_to_hex(base64->bytes[i]) << 18;
        triplet |= base64_char_to_hex(base64->bytes[i + 1]) << 12;
        triplet |= base64_char_to_hex(base64->bytes[i + 2]) << 6;
        triplet |= base64_char_to_hex(base64->bytes[i + 3]);

        hex->bytes[triplet_idx * 3] = triplet >> 16;
        hex->bytes[triplet_idx * 3 + 1] = triplet >> 8;
        hex->bytes[triplet_idx * 3 + 2] = triplet;
    }
    
    //remove off any padding
    for (int i = base64->length - 1; i >= 0; i--)
    {
        if (base64->bytes[i] == '=')
            hex->bytes--;
    }

    return hex;
}

byte_array* hex_str_to_byte_array(const char *str)
{
    size_t length = strlen(str) / 2;
    byte_array *arr = new byte_array(length);
    
    // 2 hex digits per byte
    for (int i = 0; i < arr->length; i++)
    {
        char ch1 = str[2*i];
        byte_t val1 = ch1 <= '9' ? ch1 - '0' : 10 + ch1 - 'a'; 
        char ch2 = str[2*i + 1];
        byte_t val2 = ch2 <= '9' ? ch2 - '0' : 10 + ch2 - 'a'; 
        arr->bytes[i] = (val1 << 4) | val2;
    }

    return arr;
}

byte_array* str_to_byte_array(const char *str)
{
    size_t length = strlen(str);
    byte_array *arr = new byte_array(length);
    
    memcpy(arr->bytes, str, length);

    return arr;
}

const uint8_t *key = (uint8_t*)"YELLOW SUBMARINE";

int main(int argc, char* argv[])
{
    assert(argc == 3);

    char *file_path = argv[1];
    int block_length = strtol(argv[2], NULL, 10);

    std::ifstream t(file_path);
    std::string key((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    int padded_len = (int)ceilf((float)key.length() / (float)block_length) * block_length;

    byte_array *padded_key = new byte_array(padded_len + 1);
    memcpy(padded_key->bytes, key.c_str(), key.length());

    byte_t padding_byte = (padded_len - key.length());
    for ( int i = 0; i < (padded_len - key.length()); i++)
    {
        padded_key->bytes[key.length() + i] = padding_byte;
    }

    padded_key->bytes[padded_len] = '\0';

    printf("%s\n", padded_key->bytes);

    // byte_array *base64 = str_to_byte_array(base64_str.c_str());

    // byte_array *cipher_text = base64_to_hex(base64);

    // byte_array *plaintext = aes_ecb_decrypt(cipher_text, key);

    // printf("%s", plaintext->bytes);
    
    // byte_array *final_text = new byte_array(plaintext->length + 1);

    // memcpy(plaintext, final_text, plaintext->length * sizeof(byte_t));
    // final_text[plaintext->length] = '\0';

    // printf("%s", final_text);

    delete padded_key;

    // delete base64;
    // delete cipher_text;
    // delete plaintext;
    // delete final_text;
}