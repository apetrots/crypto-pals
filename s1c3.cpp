#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>

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

    std::string hex(argv[1]);
    byte_array* hex_arr = hex_str_to_byte_array(hex.c_str());
    for( int i = 0; i < hex_arr->length; i++ )
    {
        printf("%x", hex_arr->bytes[i]);
    }
    printf ("\n");

    byte_array *xored[256];
    int best_score = 0;
    int leading_arr = -1;


    for ( int key = 0; key < 256; key++ )
    {
        // printf("%u - XOR'd with %c\n", key, key);

        xored[key] = byte_array_xor_char(hex_arr, (char)key);

        int score = 0;
        for( int i = 0; i < xored[key]->length; i++ )
        {
            char checking = xored[key]->bytes[i];
            if (checking == 'e' || checking == 'a' || checking == ' ') 
            {
                score++;
            }
        }

        if (score > best_score)
        {
            leading_arr = key;
            best_score = score;
        }

        printf("%i - %s\n", key, xored[key]->bytes);
    }

    printf("best guess - %s\n", xored[leading_arr]->bytes);


    delete [] xored;
    delete hex_arr;

    return 0;
}
