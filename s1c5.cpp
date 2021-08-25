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
    assert (argc == 3);
    char *key = argv[2];

    byte_array* key_arr = str_to_byte_array(key);

    char *file_path = argv[1];

    std::ifstream t(file_path);
    std::string input((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

      for( int i = 0; i < key_arr->length; i++ )
    {
        printf("%02x", key_arr->bytes[i]);
    }
    printf ("\n");

    printf("%s\n", input.c_str());
    byte_array* hex_arr = str_to_byte_array(input.c_str());

    // for( int i = 0; i < hex_arr->length; i++ )
    // {
    //     printf("%02x", hex_arr->bytes[i]);
    // }
    // printf ("\n");

    byte_array *xored = byte_array_xor(hex_arr, key_arr);
    
    for( int i = 0; i < xored->length; i++ )
    {
        printf("%02x", xored->bytes[i]);
    }
    
    delete hex_arr;
    delete key_arr;
    delete xored;

    return 0;
}
