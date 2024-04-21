#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unicode/ucnv.h>


int main()
{

    UErrorCode error = U_ZERO_ERROR;


    unsigned char source_string[] = { 0xD0, 'e', 0xF0, 'i', 'o', 's', ' ',
                                      0xFE, 0xE0, 's', 'y', 's', ' ', 's', 'u', ' ',
                                      0xF0, 'e', 0xF0, 'i', 'a', 'i', 's', ' ',
                                      0xFE, 0xE0, 's', 'y', 0xE8, 'i', 'a', 'i', 's', '.', '\0' };


    {

        const char *source_encoding = "windows-1257";   //cp1257
        const char *terget_encoding = "UTF-8";

        const char *source_ptr = (const char*) source_string;

        int32_t source_lenght = strlen(source_ptr);

        // Adjust size as needed based on expected output length
        char dest_buffer[1024];

        char *dest_ptr = dest_buffer;

        int32_t dest_capacity = sizeof( dest_buffer );

        ucnv_convert( terget_encoding,
                      source_encoding,
                      dest_ptr,
                      dest_capacity,
                      source_ptr,
                      source_lenght,
                      &error );

        if ( U_FAILURE(error) )
        {
            fprintf(stderr, "Error converting string: %s\n", u_errorName(error));
            u_cleanup( &error );
            return 1;
        }

        printf( "Converted string: %s\n", dest_buffer );

        u_cleanup( &error );

    }

//************************************************************

    {
        const char *source_encoding = "windows-1257";   //cp1257

        UConverter *converter;

        converter = ucnv_open( source_encoding, &error );

        if ( U_FAILURE( error ) )
        {
            fprintf( stderr, "Error opening converter: %s\n", u_errorName(error) );
            u_cleanup( &error );
            return 1;
        }

        const char *source_ptr = (const char*) source_string;

        const char *source_limit_ptr = source_ptr + strlen(source_ptr);

        // Adjust size as needed based on expected output length
        UChar target_buffer[1024];

        UChar *target_ptr = target_buffer;

        UChar *target_limit_ptr = target_ptr + sizeof(target_buffer);

        ucnv_toUnicode( converter,
                        &target_ptr,
                        target_limit_ptr,
                        &source_ptr,
                        source_limit_ptr,
                        NULL,
                        true,
                        &error );


        if ( U_FAILURE(error) )
        {
            fprintf(stderr, "Error converting string: %s\n", u_errorName(error));
            ucnv_close(converter);
            u_cleanup(&error);
            return 1;
        }

        // Null-terminate the converted string
        *target_ptr = '\0';

        ucnv_close(converter);


        // from  UChar * to char *

        converter = ucnv_open( "UTF-8", &error );

        if ( U_FAILURE(error) )
        {
            fprintf(stderr, "Error opening converter: %s\n", u_errorName(error));
            u_cleanup(&error);
            return 1;
        }

        UChar *src_ptr = target_buffer;
        int32_t src_length = u_strlen(target_buffer);

        // Adjust size as needed based on expected output length
        char dest_buffer[1024];
        int32_t dest_capacity = sizeof(dest_buffer);

        ucnv_fromUChars( converter,
                         dest_buffer,
                         dest_capacity,
                         src_ptr,
                         src_length,
                         &error );


        ucnv_close( converter );

        printf("Converted string: %s\n", dest_buffer);

        u_cleanup( &error );
    }

    return 0;
}
