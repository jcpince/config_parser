#ifndef __CONFIG_PARSER__
#define __CONFIG_PARSER__

#include <string.h>
#include <stdint.h>

#ifndef CONFIG_PARSER_COMMENT_SEPARATOR
//#define CONFIG_PARSER_COMMENT_SEPARATOR "#"
//#define CONFIG_PARSER_COMMENT_SEPARATOR "//"
#define CONFIG_PARSER_COMMENT_SEPARATOR "\"\"\""
#endif

#ifndef CONFIG_PARSER_ASSIGN_SEPARATOR
//#define CONFIG_PARSER_ASSIGN_SEPARATOR "="
//#define CONFIG_PARSER_ASSIGN_SEPARATOR ":"
#define CONFIG_PARSER_ASSIGN_SEPARATOR ":="
#endif

typedef struct
{
    void        *cache;
} config_parser_context_t;

int config_parser_init(config_parser_context_t *parser, const char *filename);
void config_parser_deinit(config_parser_context_t *parser);

void config_parser_dump_cache(config_parser_context_t *parser);

const char *config_parser_get_string(config_parser_context_t *parser, const char *name);

int config_parser_get_int8 (config_parser_context_t *parser, const char *name, int8_t  *value);
int config_parser_get_int16(config_parser_context_t *parser, const char *name, int16_t *value);
int config_parser_get_int32(config_parser_context_t *parser, const char *name, int32_t *value);
int config_parser_get_int64(config_parser_context_t *parser, const char *name, int64_t *value);

int config_parser_get_uint8 (config_parser_context_t *parser, const char *name, uint8_t  *value);
int config_parser_get_uint16(config_parser_context_t *parser, const char *name, uint16_t *value);
int config_parser_get_uint32(config_parser_context_t *parser, const char *name, uint32_t *value);
int config_parser_get_uint64(config_parser_context_t *parser, const char *name, uint64_t *value);

int config_parser_get_uint8x (config_parser_context_t *parser, const char *name, uint8_t  *value);
int config_parser_get_uint16x(config_parser_context_t *parser, const char *name, uint16_t *value);
int config_parser_get_uint32x(config_parser_context_t *parser, const char *name, uint32_t *value);
int config_parser_get_uint64x(config_parser_context_t *parser, const char *name, uint64_t *value);

/* Arrays */
int config_parser_get_string_array(config_parser_context_t *parser, const char *name, const char *delimiters, char ***array, size_t *len);

int config_parser_get_int8_array (config_parser_context_t *parser, const char *name, const char *delimiters, int8_t  **array, size_t *len);
int config_parser_get_int16_array(config_parser_context_t *parser, const char *name, const char *delimiters, int16_t **array, size_t *len);
int config_parser_get_int32_array(config_parser_context_t *parser, const char *name, const char *delimiters, int32_t **array, size_t *len);
int config_parser_get_int64_array(config_parser_context_t *parser, const char *name, const char *delimiters, int64_t **array, size_t *len);

int config_parser_get_uint8_array (config_parser_context_t *parser, const char *name, const char *delimiters, uint8_t  **array, size_t *len);
int config_parser_get_uint16_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint16_t **array, size_t *len);
int config_parser_get_uint32_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint32_t **array, size_t *len);
int config_parser_get_uint64_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint64_t **array, size_t *len);

int config_parser_get_uint8x_array (config_parser_context_t *parser, const char *name, const char *delimiters, uint8_t  **array, size_t *len);
int config_parser_get_uint16x_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint16_t **array, size_t *len);
int config_parser_get_uint32x_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint32_t **array, size_t *len);
int config_parser_get_uint64x_array(config_parser_context_t *parser, const char *name, const char *delimiters, uint64_t **array, size_t *len);


#endif /* __CONFIG_PARSER__ */
