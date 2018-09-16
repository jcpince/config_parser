#include <config_parser.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//#define debug_printf printf
#define debug_printf(...)   do{} while (0)

int main(int argc, const char **argv)
{
    size_t len;
    config_parser_context_t _parser, *parser = &_parser;
    int rc = config_parser_init(parser, argv[1]);
    debug_printf("config_parser_init returned %d\n", rc);
    assert(!rc);
    //config_parser_dump_cache(parser);

    debug_printf("value2 is '%s'\n", config_parser_get_string(parser, "value2"));
    debug_printf("value2.1 is '%s'\n", config_parser_get_string(parser, "value2.1"));

    uint8_t u8;
    rc = config_parser_get_uint8(parser, "value8", &u8);
    assert(!rc);
    if (!rc)
        debug_printf("value8 is %d\n", u8);
    else
        debug_printf("config_parser_get_uint8(value8) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint8(parser, "value8.1", &u8);
    assert(rc == ERANGE);
    if (!rc)
        debug_printf("value8.1 is %d\n", u8);
    else
        debug_printf("config_parser_get_uint8(value8.1) error --  %s\n", strerror(rc));

    uint16_t u16;
    rc = config_parser_get_uint16(parser, "value16", &u16);
    assert(!rc);
    if (!rc)
        debug_printf("value16 is %d\n", u16);
    else
        debug_printf("config_parser_get_uint16(value16) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint16(parser, "value16.1", &u16);
    assert(rc == ERANGE);
    if (!rc)
        debug_printf("value16.1 is %d\n", u16);
    else
        debug_printf("config_parser_get_uint16(value16.1) error --  %s\n", strerror(rc));

    uint32_t u32;
    rc = config_parser_get_uint32x(parser, "value32x", &u32);
    assert(!rc);
    if (!rc)
        debug_printf("value32x is %d -- 0x%x\n", u32, u32);
    else
        debug_printf("config_parser_get_uint32x(value32x) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint32x(parser, "value32x.1", &u32);
    assert(!rc);
    assert(u32 == 0x16);
    if (!rc)
        debug_printf("value32x.1 is %d -- 0x%x\n", u32, u32);
    else
        debug_printf("config_parser_get_uint32x(value32x.1) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint32x(parser, "value32x.2", &u32);
    assert(!rc);
    assert(u32 == 0xa13);
    if (!rc)
        debug_printf("value32x.2 is %d -- 0x%x\n", u32, u32);
    else
        debug_printf("config_parser_get_uint32x(value32x.2) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint32x(parser, "value32x.3", &u32);
    assert(rc == ERANGE);
    if (!rc)
        debug_printf("value32x.3 is %d\n", u16);
    else
        debug_printf("config_parser_get_uint32x(value32x.3) error --  %s\n", strerror(rc));

    uint64_t u64;
    rc = config_parser_get_uint64x(parser, "value64x", &u64);
    assert(!rc);
    if (!rc)
        debug_printf("value64x is %ld -- 0x%lx\n", u64, u64);
    else
        debug_printf("config_parser_get_uint64x(value64x) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint64x(parser, "value64x.1", &u64);
    assert(!rc);
    if (!rc)
        debug_printf("value64x.1 is %ld -- 0x%lx\n", u64, u64);
    else
        debug_printf("config_parser_get_uint64x(value64x.1) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint64x(parser, "value64x.2", &u64);
    assert(rc);
    if (!rc)
        debug_printf("value64x.2 is %ld -- 0x%lx\n", u64, u64);
    else
        debug_printf("config_parser_get_uint64x(value64x.2) error --  %s\n", strerror(rc));

    int8_t s8;
    rc = config_parser_get_int8(parser, "signed8", &s8);
    assert(!rc);
    if (!rc)
        debug_printf("signed8 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8) error --  %s\n", strerror(rc));

    rc = config_parser_get_int8(parser, "signed8.1", &s8);
    assert(!rc);
    if (!rc)
        debug_printf("signed8.1 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8.1) error --  %s\n", strerror(rc));

    rc = config_parser_get_int8(parser, "signed8.2", &s8);
    assert(!rc);
    if (!rc)
        debug_printf("signed8.2 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8.2) error --  %s\n", strerror(rc));

    rc = config_parser_get_int8(parser, "signed8.3", &s8);
    assert(rc == ERANGE);
    if (!rc)
        debug_printf("signed8.3 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8.3) error --  %s\n", strerror(rc));

    rc = config_parser_get_int8(parser, "signed8.4", &s8);
    assert(!rc);
    if (!rc)
        debug_printf("signed8.4 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8.4) error --  %s\n", strerror(rc));

    rc = config_parser_get_int8(parser, "signed8.5", &s8);
    assert(rc == ERANGE);
    if (!rc)
        debug_printf("signed8.5 is %d -- 0x%x\n", s8, s8);
    else
        debug_printf("config_parser_get_int8(signed8.5) error --  %s\n", strerror(rc));

    char **string_array;
    config_parser_get_string_array(parser, "string_array", ",", &string_array, &len);
    assert(len = 3);
    debug_printf("config_parser_get_string_array(string_array, ',') result:\n");
    for (int idx = 0 ; idx < len ; idx++)
    {
        debug_printf("\tstring_array[%d] = '%s'\n", idx, string_array[idx]);
        free(string_array[idx]);
    }
    if (string_array)
        free(string_array);
    config_parser_get_string_array(parser, "string_array", ", ", &string_array, &len);
    assert(len = 4);
    debug_printf("config_parser_get_string_array(string_array, ', ') result:\n");
    for (int idx = 0 ; idx < len ; idx++)
    {
        debug_printf("\tstring_array[%d] = '%s'\n", idx, string_array[idx]);
        free(string_array[idx]);
    }
    if (string_array)
        free(string_array);
    config_parser_get_string_array(parser, "string_array2", ", ", &string_array, &len);
    assert(string_array == NULL);

    uint8_t *uin8_array;
    rc = config_parser_get_uint8_array(parser, "uint8_array", ",", &uin8_array, &len);
    assert(uin8_array);
    assert(len == 5);
    assert(uin8_array[0] == 12);
    assert(uin8_array[len-1] == 255);
    free(uin8_array);

    rc = config_parser_get_uint8_array(parser, "uint8_array.1", ",", &uin8_array, &len);
    assert(rc == ERANGE);
    assert(!uin8_array);
    assert(!len);
    free(uin8_array);

    uint32_t *uin32_array;
    config_parser_get_uint32_array(parser, "uint32_array", ",", &uin32_array, &len);
    assert(uin32_array);
    assert(len == 5);
    assert(uin32_array[0] == 12);
    assert(uin32_array[len-1] == 188569);
    free(uin32_array);

    rc = config_parser_get_uint32_array(parser, "uint32_array.1", ",", &uin32_array, &len);
    assert(rc == ERANGE);
    assert(!uin32_array);
    assert(!len);
    free(uin32_array);

    config_parser_get_uint32x_array(parser, "uint32_arrayx", ",", &uin32_array, &len);
    assert(uin32_array);
    assert(len == 5);
    assert(uin32_array[0] == 0x12);
    assert(uin32_array[len-1] == 0x188569);
    free(uin32_array);

    rc = config_parser_get_uint32x_array(parser, "uint32_arrayx.1", ",", &uin32_array, &len);
    assert(rc == ERANGE);
    assert(!uin32_array);
    assert(!len);
    free(uin32_array);

    uint64_t *uin64_array;
    rc = config_parser_get_uint64x_array(parser, "uint64_arrayx", ",", &uin64_array, &len);
    assert(uin64_array);
    assert(len == 5);
    assert(uin64_array[0] == 0x12);
    assert(uin64_array[len-1] == 0x188569);
    free(uin64_array);

    rc = config_parser_get_uint64x_array(parser, "uint64_arrayx.1", ",", &uin64_array, &len);
    assert(rc == ERANGE);
    assert(!uin64_array);
    assert(!len);
    free(uin64_array);

    int8_t *signed8_array;
    rc = config_parser_get_int8_array(parser, "signed8_array", ",", &signed8_array, &len);
    debug_printf("config_parser_get_int8_array(parser, \"signed8_array\", \",\", &signed8_array, &len) returned %s\n", strerror(rc));
    assert(signed8_array);
    assert(len == 5);
    assert(signed8_array[0] == 12);
    assert(signed8_array[len-1] == -128);
    free(signed8_array);

    rc = config_parser_get_int8_array(parser, "signed8_array.1", ",", &signed8_array, &len);
    assert(rc == ERANGE);
    assert(!signed8_array);
    assert(!len);
    free(signed8_array);

    int32_t *signed32_array;
    rc = config_parser_get_int32_array(parser, "signed32_array", ",", &signed32_array, &len);
    int32_t t = 2415919103;
    assert(signed32_array);
    assert(len == 5);
    assert(signed32_array[0] == 12);
    assert(signed32_array[len-1] == -2147483648);
    free(signed32_array);

    rc = config_parser_get_int32_array(parser, "signed32_array.1", ",", &signed32_array, &len);
    assert(rc == ERANGE);
    assert(!signed32_array);
    assert(!len);

    rc = config_parser_get_int32_array(parser, "signed32_array.2", ",", &signed32_array, &len);
    assert(rc == ERANGE);
    assert(!signed32_array);
    assert(!len);

    config_parser_deinit(parser);
    fprintf(stdout, "\n\nAll tests are successful!\n");
    return 0;
}
