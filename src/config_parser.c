#include <config_parser.h>

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

#define DECLARE_GET_UINTx(type_size, hex)                                       \
    int config_parser_get_uint##type_size##hex(config_parser_context_t *parser, \
            const char *name, uint##type_size##_t *value)                       \
    {                                                                           \
        uint64_t tmp;                                                           \
        int rc = config_parser_get_uint64##hex(parser, name, &tmp);             \
        if (rc)                                                                 \
            return rc;                                                          \
        if (check_urange(tmp, type_size))                                       \
            return ERANGE;                                                      \
        *value = tmp & (uint##type_size##_t)(~0);                               \
        return 0;                                                               \
    }

#define DECLARE_GET_INTx(type_size)                                             \
    int config_parser_get_int##type_size(config_parser_context_t *parser,       \
            const char *name, int##type_size##_t *value)                        \
    {                                                                           \
        int64_t tmp, limit = 2 << (type_size - 2);                              \
        int rc = config_parser_get_int64(parser, name, &tmp);                   \
        if (rc)                                                                 \
            return rc;                                                          \
        if (check_srange(tmp, type_size))                                       \
            return ERANGE;                                                      \
        *value = (int##type_size##_t)tmp;                                       \
        return 0;                                                               \
    }

typedef struct config_tuple_
{
    char *name;
    char *value;
    struct config_tuple_ *next;
} config_tuple_t;

static int check_urange(uint64_t value, uint8_t type_size)
{
    uint64_t limit = ((uint64_t)1 << type_size) - 1;
    if (value > limit)
        return ERANGE;
    return 0;
}

static int check_srange(int64_t value, uint8_t type_size)
{
    int64_t limit = ((int64_t)1 << (type_size - 1)) - 1;
    if (value > limit)
        return ERANGE;
    if (value < (-limit - 1))
        return ERANGE;
    return 0;
}

static char *ltrim(char *line)
{
    char *trimed_line = line;
    while (isspace(trimed_line[0]))
        trimed_line++;
    return trimed_line;
}

static char *rtrim(char *line)
{
    size_t len = strlen(line);
    if (!len) return line;
    char *end = &line[len-1];
    while (isspace(end[0]))
        end--;
    end[1] = '\0';
    return line;
}

static char *trim(char *line)
{
    return rtrim(ltrim(line));
}

config_tuple_t *find_tuple(config_parser_context_t *parser, const char *name)
{
    config_tuple_t *tuple = parser->cache;
    while(tuple)
    {
        if (!strcmp(tuple->name, name))
            return tuple;
        tuple = tuple->next;
    }
    return NULL;
}

int fill_cache(config_parser_context_t *parser, const char *filename)
{
    FILE *stream;
    char *line = NULL, *trimed_line, *comment, *name, *value;
    size_t len = 0;
    ssize_t read, assign_sep_len = strlen(CONFIG_PARSER_ASSIGN_SEPARATOR);
    int ret = 0;

    stream = fopen(filename, "r");
    if (stream == NULL)
        return ENOENT;

    int line_idx = 0;
    while ((read = getline(&line, &len, stream)) != -1)
    {
        line_idx++;
        trimed_line = ltrim(line);
        if (!strlen(trimed_line))
            continue;
        comment = strstr(trimed_line, CONFIG_PARSER_COMMENT_SEPARATOR);
        if (comment)
            comment[0] = '\0';
        trimed_line = rtrim(trimed_line);
        if (!strlen(trimed_line))
            continue;
        value = strstr(trimed_line, CONFIG_PARSER_ASSIGN_SEPARATOR);
        if (!value)
        {
            fprintf(stderr, "Separator '"CONFIG_PARSER_ASSIGN_SEPARATOR
                "' not found in line %d, skipped...\n", line_idx);
            continue;
        }
        value[0] = '\0';
        value = trim(value + assign_sep_len);
        name = trim(trimed_line);
        if (!strlen(name))
        {
            fprintf(stderr, "Empty name at line %d, skipped...\n", line_idx);
            continue;
        }
        if (!strlen(value))
        {
            fprintf(stderr, "Empty value at line %d, skipped...\n", line_idx);
            continue;
        }
        config_tuple_t *tuple = find_tuple(parser, name);
        if (tuple)
        {
            free(tuple->value);
            tuple->value = strdup(value);
            if (!tuple->value)
            {
                ret = ENOMEM;
                goto on_error;
            }
        }
        else
        {
            tuple = malloc(sizeof(config_tuple_t));
            tuple->name = strdup(name);
            tuple->value = strdup(value);
            if (!tuple->name || !tuple->value)
            {
                ret = ENOMEM;
                goto on_error;
            }
            tuple->next = parser->cache;
            parser->cache = tuple;
        }
    }

on_error:
    /* cleanup if error */
    if (ret)
        config_parser_deinit(parser);
    free(line);
    fclose(stream);
    return ret;
}

int config_parser_init(config_parser_context_t *parser, const char *filename)
{
    parser->cache = NULL;
    return fill_cache(parser, filename);
}

void config_parser_dump_cache(config_parser_context_t *parser)
{
        config_tuple_t *tuple = parser->cache;
        while(tuple)
        {
            fprintf(stdout, "Name '%s' -- value '%s'\n", tuple->name, tuple->value);
            tuple = tuple->next;
        }
}

void config_parser_deinit(config_parser_context_t *parser)
{
    config_tuple_t *tuple = parser->cache;
    while(tuple)
    {
        if (tuple->name)
            free(tuple->name);
        if (tuple->value)
            free(tuple->value);
        void *mem = tuple;
        tuple = tuple->next;
        free(mem);
    }
}

const char *config_parser_get_string(config_parser_context_t *parser,
        const char *name)
{
    config_tuple_t *tuple = find_tuple(parser, name);
    if (!tuple)
        return NULL;
    return tuple->value;
}

int config_parser_get_int64(config_parser_context_t *parser, const char *name,
        int64_t *value)
{
    config_tuple_t *tuple = find_tuple(parser, name);
    if (!tuple)
        return ENOENT;

    const char *fmt;
    errno = 0;
    if (sscanf(tuple->value, "%ld", value))
    {
        return errno;
    }

    /* sscanf failed... */
    return EINVAL;
}

DECLARE_GET_INTx(8)
DECLARE_GET_INTx(16)
DECLARE_GET_INTx(32)

#include <assert.h>

static int str2uint64(const char *chain, uint64_t *value, bool is_decimal)
{
    const char *fmt;
    if (is_decimal)
        fmt = "%lu";
    else
        fmt = "%lx";
    errno = 0;
    if (sscanf(chain, fmt, value) == 1)
    {
        //printf("sscanf('%s', '%s', 0x%lx) -- errno is %d\n", chain, fmt, *value, rc);
        //assert(*value < 0x615f32336e6975);
        return errno;
    }

    /* sscanf failed... */
    return EINVAL;
}

static int config_parser_get_uint64_priv(config_parser_context_t *parser,
        const char *name, uint64_t *value, bool is_decimal)
{
    config_tuple_t *tuple = find_tuple(parser, name);
    if (!tuple)
        return ENOENT;

    return str2uint64(tuple->value, value, is_decimal);
}

int config_parser_get_uint64(config_parser_context_t *parser, const char *name, uint64_t *value)
{
    return config_parser_get_uint64_priv(parser, name, value, true);
}

int config_parser_get_uint64x(config_parser_context_t *parser, const char *name, uint64_t *value)
{
    return config_parser_get_uint64_priv(parser, name, value, false);
}

DECLARE_GET_UINTx(8, )
DECLARE_GET_UINTx(16, )
DECLARE_GET_UINTx(32, )

DECLARE_GET_UINTx(8, x)
DECLARE_GET_UINTx(16, x)
DECLARE_GET_UINTx(32, x)

static int config_parser_get_array_len(config_tuple_t *tuple,
        const char *delimiters, size_t *len)
{
    char *value = strdup(tuple->value);
    if (!value) return ENOMEM;
    /* len can be set to 1 safely since tuples have no empty values */
    *len = 1;
    strtok(value, delimiters);
    while(strtok(NULL, delimiters)) (*len)++;
    free(value);
    return 0;
}

int config_parser_get_string_array(config_parser_context_t *parser,
    const char *name, const char *delimiters, char ***array_ref, size_t *len)
{
    *array_ref = NULL;
    *len = 0;
    config_tuple_t *tuple = find_tuple(parser, name);
    if (!tuple)
        return ENOENT;

    int rc = config_parser_get_array_len(tuple, delimiters, len);
    if (rc) return rc;

    char *value = strdup(tuple->value);
    if (!value)
    {
        *len = 0;
        return errno;
    }
    char **array = calloc(*len, sizeof(char*));
    if (!array)
    {
        free(value);
        *len = 0;
        return ENOMEM;
    }
    array[0] = strdup(strtok(value, delimiters));
    if (!array[0])
    {
        free(value);
        free(array);
        *len = 0;
        return ENOMEM;
    }
    for (int idx = 1 ; idx < *len ; idx++)
    {
        array[idx] = strdup(strtok(NULL, delimiters));
        if (!array[idx])
        {
            rc = EINVAL;
            goto on_error;
        }
    }
on_error:
    if (rc)
    {
        for (int idx = 0 ; idx < *len ; idx++)
        {
            if (array[idx])
                free(array[idx]);
        }
        free(array);
        array = NULL;
        *len = 0;
    }
    free(value);
    *array_ref = array;
    return rc;
}

#define DECLARE_GET_UINTx_ARRAY(type_size, suffix, is_decimal)                        \
    int config_parser_get_uint##type_size##suffix##_array(                     \
        config_parser_context_t *parser, const char *name, \
        const char *delimiters, uint##type_size##_t **array_ref, size_t *len)\
    {\
        *array_ref = NULL;\
        *len = 0;\
        config_tuple_t *tuple = find_tuple(parser, name);\
        if (!tuple)\
            return ENOENT;\
        int rc = config_parser_get_array_len(tuple, delimiters, len);\
        if (rc) return rc;\
        char *value = strdup(tuple->value);\
        if (!value)\
        {\
            *len = 0;\
            return errno;\
        }\
        uint##type_size##_t *array = calloc(*len, sizeof(uint##type_size##_t));\
        if (!array)\
        {\
            free(value);\
            *len = 0;\
            return ENOMEM;\
        }\
        uint64_t tmp;\
        rc = str2uint64(strtok(value, delimiters), &tmp, is_decimal);\
        if (rc)\
        {\
            *len = 0;\
            goto on_error;\
        }\
        if ((type_size != 64) && check_urange(tmp, type_size))\
        {\
            rc = ERANGE;\
            *len = 0;\
            goto on_error;\
        }\
        array[0] = tmp;\
        for (int idx = 1 ; idx < *len ; idx++)\
        {\
            rc = str2uint64(strtok(NULL, delimiters), &tmp, is_decimal);\
            if (rc)\
            {\
                *len = 0;\
                goto on_error;\
            }\
            if ((type_size != 64) && check_urange(tmp, type_size))\
            {\
                rc = ERANGE;\
                *len = 0;\
                goto on_error;\
            }\
            array[idx] = tmp;\
        }\
    on_error:\
        if (rc)\
        {\
            free(array);\
            array = NULL;\
            *len = 0;\
        }\
        free(value);\
        *array_ref = array;\
        return rc;\
    }

DECLARE_GET_UINTx_ARRAY(64, , true)
DECLARE_GET_UINTx_ARRAY(64, x, false)
DECLARE_GET_UINTx_ARRAY(32, , true)
DECLARE_GET_UINTx_ARRAY(32, x, false)
DECLARE_GET_UINTx_ARRAY(16, , true)
DECLARE_GET_UINTx_ARRAY(16, x, false)
DECLARE_GET_UINTx_ARRAY(8 , , true)
DECLARE_GET_UINTx_ARRAY(8 , x, false)
