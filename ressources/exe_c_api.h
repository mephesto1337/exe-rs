#ifndef __EXE_C_API__
#define __EXE_C_API__

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef void* rs_handle_t;

typedef struct {
    size_t (*get_number_of_sections)(rs_handle_t *h);
    rs_handle_t (*get_section_at)(rs_handle_t *h, size_t idx);
    const char* (*get_section_name_at)(rs_handle_t *h, size_t idx);
    uint32_t (*get_flags)(rs_handle_t *h);
    size_t (*get_size)(rs_handle_t *h);
    size_t (*get_offset)(rs_handle_t *h);
    void (*free_exe)(rs_handle_t *h);
} rs_ops_t;

typedef struct {
    rs_handle_t handle;
    const rs_ops_t *ops;
} rs_object_t;

typedef bool (*rs_parse_t)(rs_object_t *ptr, const uint8_t *data, size_t len);

#define GENERATE_BINDINGS(prefix) \
    extern rs_handle_t prefix##_parse(const uint8_t *data, size_t len); \
    extern size_t prefix##_get_number_of_sections(rs_handle_t *h); \
    extern rs_handle_t prefix##_get_section_at(rs_handle_t *h, size_t idx); \
    extern const char* prefix##_get_section_name_at(rs_handle_t *h, size_t idx); \
    extern uint32_t prefix##_get_flags(rs_handle_t *h); \
    extern size_t prefix##_get_size(rs_handle_t *h); \
    extern size_t prefix##_get_offset(rs_handle_t *h); \
    extern void prefix##_free_exe(rs_handle_t *h); \
    \
    const rs_ops_t prefix##_ops = { \
        .get_number_of_sections = prefix##_get_number_of_sections, \
        .get_section_at = prefix##_get_section_at, \
        .get_section_name_at = prefix##_get_section_name_at, \
        .get_flags = prefix##_get_flags, \
        .get_size = prefix##_get_size, \
        .get_offset = prefix##_get_offset, \
        .free_exe = prefix##_free_exe \
    }; \
    \
    static inline bool prefix##_parse_helper(rs_object_t *ptr, const uint8_t *data, size_t len) { \
        ptr->handle = prefix##_parse(data, len); \
        ptr->ops = & prefix##_ops; \
        return ptr->handle == NULL ? false : true; \
    }

#endif // __EXE_C_API__
