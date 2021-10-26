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

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

byte_array* aes_ecb_decrypt(byte_array *ciphertext,
                const uint8_t *key)
{

    AES_KEY aes_key;
    AES_set_decrypt_key(key, 8 * AES_BLOCK_SIZE, &aes_key);

    byte_array *plaintext = new byte_array(ciphertext->length);
    for (unsigned i = 0; i < ciphertext->length; i += 16)
    {
        AES_ecb_encrypt(&ciphertext->bytes[i], &plaintext->bytes[i], &aes_key, AES_DECRYPT);
    }
    
    return plaintext;
//     EVP_CIPHER_CTX *ctx;
//     int ret;

//     /* Create and initialise the context */
//     if(!(ctx = EVP_CIPHER_CTX_new()))
//         handleErrors();

// // EVP_CIPHER_CTX_set_padding(ctx, false);

//     /* Initialise the decryption operation. */
//     if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))
//         handleErrors();

//     /* Set IV length. Not necessary if this is 12 bytes (96 bits) */
//     // if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
//         // handleErrors();

//     // /* Set IV length. Not necessary if this is 12 bytes (96 bits) */
//     // if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
//     //     handleErrors();

//     printf("yo before\n");

//     /* Initialise key and IV */
//     int plaintext_len = 0;
//     if(!EVP_DecryptUpdate(ctx, plaintext->bytes, &plaintext_len, ciphertext->bytes, ciphertext->length))
//         handleErrors();
    
//     plaintext->length = plaintext_len;

//     printf("plaintext ptr: %p\n", plaintext->bytes);
//     /*
//      * Finalise the decryption. A positive return value indicates success,
//      * anything else is a failure - the plaintext is not trustworthy.
//      */
//     int add_len = 0;
//     ret = EVP_DecryptFinal_ex(ctx, plaintext->bytes + plaintext_len, &add_len);
//     printf("plaintext ptr: %p\n", plaintext->bytes);

//     /* Clean up */
//     EVP_CIPHER_CTX_free(ctx);

//     if(ret > 0) {
//         /* Success */
//         printf("plaintext ptr: %p\n", plaintext->bytes);
//         plaintext->length += add_len;
//         return plaintext;
//     } else {
//         /* Verify failed */
//         handleErrors();
//         return NULL;
//     }
}

const uint8_t *key = (uint8_t*)"YELLOW SUBMARINE";

int main(int argc, char* argv[])
{
    assert(argc == 2);

    char *file_path = argv[1];

    std::ifstream t(file_path);
    std::string hex_str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    
    int max_repeating = 0;
    byte_array *best_hex = 0;

    size_t pos, prev = 0;
    while( (pos = hex_str.find('\n', prev)) != std::string::npos )
    {
        std::string str = hex_str.substr(pos+1, pos - prev);
        byte_array *hex = hex_str_to_byte_array(str.c_str());
        int repeating = 0;
        for (int i = 0; i < hex->length; i+=2)
        {
            for (int j = i+2; j < hex->length; j+=2)
            {
                uint16_t comp1 = (uint16_t)(hex->bytes[i] << 8) | hex->bytes[i + 1];
                uint16_t comp2 = (uint16_t)(hex->bytes[j] << 8) | hex->bytes[j + 1];

                if (comp1 == comp2)
                {
                    repeating++;
                }
            }
        }

        if (repeating > max_repeating)
        {
            best_hex = hex;
            max_repeating = repeating;
        }
        else
        {
            delete hex;
        }

        prev = pos + 1;
    }

    for( int i = 0; i < best_hex->length; i++ )
    {
        printf("%02x", best_hex->bytes[i]);
    }
    printf("\n");

    // byte_array *base64 = str_to_byte_array(base64_str.c_str());

    // byte_array *cipher_text = base64_to_hex(base64);

    // byte_array *plaintext = aes_ecb_decrypt(cipher_text, key);

    // printf("%s", plaintext->bytes);
    
    // byte_array *final_text = new byte_array(plaintext->length + 1);

    // memcpy(plaintext, final_text, plaintext->length * sizeof(byte_t));
    // final_text[plaintext->length] = '\0';

    // printf("%s", final_text);

    // delete base64;
    // delete cipher_text;
    // delete plaintext;
    // delete final_text;
}