#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <fstream>
#include <streambuf>
#include <algorithm>

using byte_t = unsigned char;

struct byte_array
{
    byte_t *bytes;
    size_t length;

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
        printf("Padding!!!! %c\n", ch);
        return 0;
    }
    return BASE_64_CHARACTERS - found+1; 
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


byte_array* byte_array_xor_char(byte_array *lhs, char rhs)
{
    size_t len = lhs->length;  
    byte_array *arr = new byte_array(len + 1);
    arr->bytes[len] = '\0'; 
    for( int i = 0; i < len; i++ )
    {
        arr->bytes[i] = lhs->bytes[i] ^ rhs;
    }

    return arr;
}

byte_array* byte_array_xor(byte_array *lhs, byte_array *rhs)
{
    size_t len = lhs->length;  
    byte_array *arr = new byte_array(len );
    int j = 0;
    for( int i = 0; i < len; i++ )
    {
        arr->bytes[i] = lhs->bytes[i] ^ rhs->bytes[j];
        j = (j + 1) % rhs->length;
    }

    return arr;
}

int main(int argc, char* argv[])
{
    assert (argc == 2);

    char *file_path = argv[1];

    std::ifstream t(file_path);
    std::string base64((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    base64.erase(std::remove(base64.begin(), base64.end(), '\n'), base64.end());

    byte_array *base64_arr = str_to_byte_array(base64.c_str());

    byte_array *hex = base64_to_hex(base64_arr);
    for (int i = 0; i < hex->length; i++ )
    {
        printf("%02x", hex->bytes[i]);
    }
    printf("\n");
    // for (int i = 0; i < hex->length; i++ )
    // {
    //     printf("%02x", hex->bytes[i]);
    // }
    // printf("\n");

    delete base64_arr;
    delete hex;

    return 0;
}