#include "../../lib/nm.h"

int debug_type_file (Elf64_Half type )
{
    if (DEBUG)
    {
        if ( type == ET_NONE)
            printf("(Undefined): ELF Format unknown or not specified.\n");
        else if (type == ET_EXEC)
            printf("(Executable file): An ELF executable.\n");
        else if (type == ET_DYN)
            printf("(Shared object): A library or a dynamically-linked executable.\n");
        else if (type == ET_REL)
            printf("(Relocatable file): Relocatable files (.o object files).\n");
        else if (type == ET_CORE)
            printf("(Core dump): A core dump file.\n");
    }
    
    // ET_NONE (Undefined): ELF Format unknown or not specified.
    // ET_EXEC: (Executable file): An ELF executable.
    // ET_DYN: (Shared object): A library or a dynamically-linked executable.
    // ET_REL (Relocatable file): Relocatable files (.o object files).
    // ET_CORE (Core dump): A core dump file.

    return 1;
}