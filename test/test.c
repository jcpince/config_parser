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
    if (!rc)
        debug_printf("value32x.1 is %d -- 0x%x\n", u32, u32);
    else
        debug_printf("config_parser_get_uint32x(value32x.1) error --  %s\n", strerror(rc));

    rc = config_parser_get_uint32x(parser, "value32x.2", &u32);
    assert(!rc);
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
    size_t len;
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
    debug_printf("config_parser_get_string_array(string_array2, ', ') result: %zd\n", len);

    config_parser_deinit(parser);
    fprintf(stdout, "\n\nAll tests are successful!\n");
    return 0;
}
