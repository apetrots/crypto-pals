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

byte_array* xor_byte_arrays(byte_array *lhs, byte_array *rhs)
{
    size_t len = lhs->length < rhs->length ? lhs->length : rhs->length;  
    byte_array *arr = new byte_array(len);
    for( int i = 0; i < len; i++ )
    {
        arr->bytes[i] = lhs->bytes[i] ^ rhs->bytes[i];
    }

    return arr;
}

int main(int argc, char* argv[])
{
    assert (argc > 2);

    std::string hex_1(argv[1]);
    std::string hex_2(argv[2]);
    printf("%s XOR %s\n", hex_1.c_str(), hex_2.c_str());

    byte_array* hex_1_arr = hex_str_to_byte_array(hex_1.c_str());
    byte_array* hex_2_arr = hex_str_to_byte_array(hex_2.c_str());

    for( int i = 0; i < hex_1_arr->length; i++ )
    {
        printf("%x", hex_1_arr->bytes[i]);
    }

    printf (" XOR ");

    for( int i = 0; i < hex_2_arr->length; i++ )
    {
        printf("%x", hex_2_arr->bytes[i]);
    }

    printf(" = ");

    byte_array *xored_arr = xor_byte_arrays(hex_1_arr, hex_2_arr);

    for( int i = 0; i < xored_arr->length; i++ )
    {
        printf("%x", xored_arr->bytes[i]);
    }

    delete hex_1_arr;
    delete hex_2_arr;
    delete xored_arr;

    return 0;
}
