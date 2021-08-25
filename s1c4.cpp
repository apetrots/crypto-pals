#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <fstream>
#include <streambuf>

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

int main(int argc, char* argv[])
{
    assert (argc > 1);
    char *file_path = argv[1];

    std::ifstream t(file_path);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    int overall_best_score = 0;
    byte_array *overall_leading_arr;

    size_t start = 0;
    size_t end = str.find("\n");
    while (end != std::string::npos)
    {
        std::string hex = str.substr(start, end - start);
        printf("%s\n", hex.c_str());

        byte_array* hex_arr = hex_str_to_byte_array(hex.c_str());

        for( int i = 0; i < hex_arr->length; i++ )
        {
            printf("%02x", hex_arr->bytes[i]);
        }
        printf ("\n");

        int best_score = 0;
        byte_array *leading_arr;

        for ( int key = 0; key < 256; key++ )
        {
            // printf("%u - XOR'd with %c\n", key, key);

            byte_array *xored = byte_array_xor_char(hex_arr, (char)key);

            int score = 0;
            for( int i = 0; i < xored->length; i++ )
            {
                char checking = xored->bytes[i];
                if (checking == 'e' || checking == 'a' || checking == ' ') 
                {
                    score++;
                }
            }

            if (score > best_score)
            {
                leading_arr = xored;
                best_score = score;
            }
            else
            {
                delete xored;
            }

            // printf("%i - %s\n", key, xored[key]->bytes);
        }

        // printf("best guess - %s\n", xored[leading_arr]->bytes);
        
        if (best_score > overall_best_score)
        {
            overall_best_score = best_score;
            overall_leading_arr = leading_arr;
        }


        delete hex_arr;

        start = end + 1;
        end = str.find("\n", start);
    }

    printf("best overall guess - %s\n", overall_leading_arr->bytes);


    return 0;
}
