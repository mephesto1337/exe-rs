#ifndef __EXE_C_API__
#define __EXE_C_API__

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef void* rs_handle_t;

typedef struct {
    const char *name;
    uint32_t flags; 
    size_t paddr;
    size_t vaddr;
    size_t size;
} rs_section_t;

typedef struct {
    const char *os;
    const char *arch;
    size_t bits;
} rs_info_t;

typedef struct {
    rs_info_t* (*get_info)(rs_handle_t g);
    void (*free_info)(rs_info_t *h);
    size_t (*get_number_of_sections)(rs_handle_t h);
    rs_section_t* (*get_section_at)(rs_handle_t h, size_t idx);
    const uint8_t* (*get_data)(rs_handle_t h, size_t start, size_t len);
    void (*free_section)(rs_section_t *h);
    void (*free_exe)(rs_handle_t h);
} rs_ops_t;

typedef struct {
    rs_handle_t handle;
    const rs_ops_t *ops;
} rs_object_t;

typedef bool (*rs_parse_t)(rs_object_t *ptr, const uint8_t *data, size_t len);

#define GENERATE_BINDINGS(prefix) \
    extern rs_handle_t prefix##_parse(const uint8_t *data, size_t len); \
    extern void prefix##_free_info(rs_info_t *h); \
    extern rs_info_t* prefix##_get_info(rs_handle_t h); \
    extern size_t prefix##_get_number_of_sections(rs_handle_t h); \
    extern rs_section_t* prefix##_get_section_at(rs_handle_t h, size_t idx); \
    extern const uint8_t* prefix##_get_data(rs_handle_t h, size_t start, size_t len); \
    extern void prefix##_free_section(rs_section_t *h); \
    extern void prefix##_free_exe(rs_handle_t h); \
    \
    const rs_ops_t prefix##_ops = { \
        .get_info = prefix##_get_info, \
        .free_info = prefix##_free_info, \
        .get_number_of_sections = prefix##_get_number_of_sections, \
        .get_section_at = prefix##_get_section_at, \
        .get_data = prefix##_get_data, \
        .free_section = prefix##_free_section, \
        .free_exe = prefix##_free_exe \
    }; \
    \
    static inline bool prefix##_parse_helper(rs_object_t *ptr, const uint8_t *data, size_t len) { \
        ptr->handle = prefix##_parse(data, len); \
        ptr->ops = & prefix##_ops; \
        return ptr->handle == NULL ? false : true; \
    }

#endif // __EXE_C_API__
