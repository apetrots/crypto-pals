#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>

using byte_t = unsigned char;

const char *BASE_64_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789+/";
const char BASE_64_PADDING = '=';

byte_t* hex_str_to_byte_array(size_t *length, const char *str)
{
    // 2 hex digits per byte
    *length = strlen(str) / 2;
    byte_t *arr = new byte_t[*length];
    for (int i = 0; i < *length; i++)
    {
        char ch1 = str[2*i];
        byte_t val1 = ch1 <= '9' ? ch1 - '0' : 10 + ch1 - 'a'; 
        char ch2 = str[2*i + 1];
        byte_t val2 = ch2 <= '9' ? ch2 - '0' : 10 + ch2 - 'a'; 
        arr[i] = (val1 << 4) | val2;
    }

    return arr;
}

// Base64 in string format
char *hex_to_base64(size_t *base64_length, byte_t *hex, int hex_length)
{
    // because base64 is a 6-bit encoding, we convert at 24-bit intervals to appease the byte system 
    // therefore, there are 3 hex digits to every 4 base64 digits 
    // int triplet_count = (int)ceilf(hex_length / 3.0f);
    int triplet_count = (hex_length + 2) / 3;

    *base64_length = triplet_count * 4;
    char *base64 = new char[*base64_length + 1];
    base64[*base64_length] = '\0';

    for (int triplet_idx = 0; triplet_idx < triplet_count; triplet_idx++ )
    {
        int triplet = hex[triplet_idx * 3] << 16;
        triplet |= triplet_idx * 3 + 1 < hex_length ? (hex[triplet_idx * 3 + 1] << 8) : 0;
        triplet |= triplet_idx * 3 + 2 < hex_length ? hex[triplet_idx * 3 + 2] : 0;

        int i = triplet_idx * 4;
        base64[i] = BASE_64_CHARACTERS[(triplet >> 18) & 0x3F];
        base64[i + 1] = BASE_64_CHARACTERS[(triplet >> 12) & 0x3F];
        base64[i + 2] = BASE_64_CHARACTERS[(triplet >> 6) & 0x3F];
        base64[i + 3] = BASE_64_CHARACTERS[triplet & 0x3F];
        if (triplet_idx * 3 + 1 >= hex_length)
            base64[i+2] = BASE_64_PADDING;
        if (triplet_idx * 3 + 2 >= hex_length)
            base64[i+3] = BASE_64_PADDING;
    }

    return base64;
}

int main(int argc, char* argv[])
{
    assert (argc > 1);

    std::string hex(argv[1]);
    printf("%s\n", hex.c_str());

    size_t hex_len = 0; 
    byte_t* hex_arr = hex_str_to_byte_array(&hex_len, hex.c_str());

    for( int i = 0; i < hex_len; i++ )
    {
        printf("%x", hex_arr[i]);
    }

    size_t base64_len = 0; 
    char *base64 = hex_to_base64(&base64_len, hex_arr, hex_len);

    printf("\n%s", base64);
    printf("\n%i", base64_len);

    delete [] hex_arr;
    delete [] base64;

    return 0;
}
