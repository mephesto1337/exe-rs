#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "check.h"
#include "exe_c_api.h"
GENERATE_BINDINGS(rs_pe);
GENERATE_BINDINGS(rs_elf32);
GENERATE_BINDINGS(rs_elf64);

const rs_parse_t parsers[] = { rs_pe_parse_helper, rs_elf32_parse_helper, rs_elf64_parse_helper, NULL };

#define safe_rs_free(h)     SAFE_FREE(h.handle, NULL, h.ops->free_exe)

char* show_flags(uint32_t flags) {
    static char str_flags[4] = "---";
    if ( flags & 4U ) { str_flags[0] = 'r'; } else { str_flags[0] = '-'; }
    if ( flags & 2U ) { str_flags[1] = 'w'; } else { str_flags[1] = '-'; }
    if ( flags & 1U ) { str_flags[2] = 'x'; } else { str_flags[2] = '-'; }
    return str_flags;
}

int main(int argc, char *const argv[]) {
    int fd = -1;
    void *ptr = MAP_FAILED;
    const uint8_t *data;
    struct stat sb;
    rs_object_t obj = { NULL, NULL };
    rs_section_t *section = NULL;
    size_t nsections;
    int ret = EXIT_FAILURE;

    if ( argc != 2 ) {
        fprintf(stderr, "Usage : %s file\n", argv[0]);
        return EXIT_FAILURE;
    }

    CHK_NEG(fd = open(argv[1], O_RDONLY));
    CHK_NEG(fstat(fd, &sb));
    CHK_MMAP(ptr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));
    data = (const uint8_t *)ptr;

    for ( size_t idx = 0; parsers[idx] != NULL; idx++ ) {
        if ( parsers[idx](&obj, data, sb.st_size) ) {
            break;
        }
    }
    if ( obj.handle == NULL ) {
        error("No parser match");
        goto fail;
    }

    nsections = obj.ops->get_number_of_sections(obj.handle);
    for ( size_t idx = 0; idx < nsections; idx++ ) {
        CHK_NULL(section = obj.ops->get_section_at(obj.handle, idx));

        printf(
            "Section %02lu / %-20s : flags=%s, offset=0x%08lx, size=%lu\n",
            idx, section->name, show_flags(section->flags), section->paddr, section->size
        );

        obj.ops->free_section(section);
    }

    ret = EXIT_SUCCESS;

    fail:
    safe_rs_free(obj);
    safe_munmap(ptr, sb.st_size);
    safe_close(fd);
    return ret;
}
