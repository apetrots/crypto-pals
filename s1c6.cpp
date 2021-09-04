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

struct byte_array_slice
{
    byte_t *bytes;
    size_t length;

    byte_array_slice(byte_array* other, size_t begin, size_t size)
    {
        length = size;
        bytes = &other->bytes[begin];
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
    
    // remove off any padding
    // for (int i = base64->length - 1; i >= 0; i--)
    // {
    //     if (base64->bytes[i] == '=')
    //         hex->bytes--;
    // }

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
    byte_array *arr = new byte_array(len);
    int j = 0;
    for( int i = 0; i < len; i++ )
    {
        arr->bytes[i] = lhs->bytes[i] ^ rhs->bytes[j];
        j = (j + 1) % rhs->length;
    }

    return arr;
}


size_t hamming_distance(byte_array *lhs, byte_array *rhs)
{
    size_t counter = 0;
    size_t size = lhs->length > rhs->length ? lhs->length : rhs->length;
    for ( size_t i = 0; i < size; i++ )
    {
        // result of xor is 1 if two bits are different
        byte_t different = lhs->bytes[i] ^ rhs->bytes[i];

        for ( size_t b = 0; b < 8; b++ )
        {
            if( (different >> b) & 0x1 != 0 )
            {
                counter++;
            }
        }
    }

    return counter;
}

size_t hamming_distance(byte_array_slice *lhs, byte_array_slice *rhs)
{
    size_t counter = 0;
    size_t size = lhs->length > rhs->length ? lhs->length : rhs->length;
    for ( size_t i = 0; i < size; i++ )
    {
        // result of xor is 1 if two bits are different
        byte_t different = lhs->bytes[i] ^ rhs->bytes[i];

        for ( size_t b = 0; b < 8; b++ )
        {
            if( (different >> b) & 0x1 != 0 )
            {
                counter++;
            }
        }
    }

    return counter;
}



// void single_char_xor(byte_array_slice& )
// {
//     int best_score = 0;
//     byte_array *leading_arr;

//     for ( int key = 0; key < 256; key++ )
//     {
//         // printf("%u - XOR'd with %c\n", key, key);

//         byte_array *xored = byte_array_xor_char(hex_arr, (char)key);

//         int score = 0;
//         for( int i = 0; i < xored->length; i++ )
//         {
//             char checking = xored->bytes[i];
//             if (checking == 'e' || checking == 'a' || checking == ' ') 
//             {
//                 score++;
//             }
//         }

//         if (score > best_score)
//         {
//             leading_arr = xored;
//             best_score = score;
//         }
//         else
//         {
//             delete xored;
//         }

//         // printf("%i - %s\n", key, xored[key]->bytes);
//     }

//     // printf("best guess - %s\n", xored[leading_arr]->bytes);
    
//     if (best_score > overall_best_score)
//     {
//         overall_best_score = best_score;
//         overall_leading_arr = leading_arr;
//     }
// }

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

    // key_size in bytes
    double lowest = INFINITY;
    size_t lowest_key_size = 0;
    for( size_t key_size = 2; key_size <= 40; key_size++ )
    {
        size_t pair_count = hex->length / (key_size *2);

        double sum = 0;
        for (int i = 0; i < pair_count; i++)
        {
            byte_array_slice temp_a(hex, i*2*key_size, key_size);
            
            byte_array_slice temp_b(hex, i*2*key_size+key_size, key_size);
            sum += double(hamming_distance(&temp_a, &temp_b)) / key_size;
        }
        auto result = sum / pair_count;

        if (result < lowest)
        {
            lowest = result;
            lowest_key_size = key_size;
            printf("\nYo: %f %u", lowest, lowest_key_size);
        }
        else
        {
            printf("\n%u - %f", key_size, result);
        }
    }

    size_t key_size = lowest_key_size;

    size_t key_count = hex->length / (key_size * 2);

    byte_array *key_arr = new byte_array(key_size);
    for (size_t byte_pos = 0; byte_pos < key_size; byte_pos++)
    {
        size_t best_score = 0;
        char leading_key;
        for ( int ch_key = 0; ch_key < 256; ch_key++ )
        {

            byte_array* xored_arr = new byte_array(key_count);
            for (size_t i = 0; i < key_count; i++)
            {
                auto temp = hex->bytes[i * key_size + byte_pos];

                xored_arr->bytes[i] = temp ^ ch_key;
            }

            int score = 0;
            for( int i = 0; i < xored_arr->length; i++ )
            {
                char checking = xored_arr->bytes[i];
                //  We need a better heuristic
                if (checking == 'e' || checking == 'E' || checking == 'a' || checking == 'A' || checking == 'O' || checking == 'o' || checking == 'i' || checking == 'I' || checking == ' ') 
                {
                    score++;
                }
            }

            if (score > best_score)
            {
                leading_key = ch_key;
                best_score = score;
            }
            else
            {
                delete xored_arr;
            }
        }

        key_arr->bytes[byte_pos] = leading_key;
    }

    byte_array *stuff = byte_array_xor(hex, key_arr);

    printf("\n");
    for (int i = 0; i < key_arr->length; i++)
    {
        printf("%c", key_arr->bytes[i]);
    }

    delete base64_arr;
    delete hex;

    return 0;
}