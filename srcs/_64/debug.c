#include "../../lib/nm.h"

int debug_type_file (Elf64_Half type )
{
    // ET_NONE (Undefined): ELF Format unknown or not specified.
    // ET_EXEC: (Executable file): An ELF executable.
    // ET_DYN: (Shared object): A library or a dynamically-linked executable.
    // ET_REL (Relocatable file): Relocatable files (.o object files).
    // ET_CORE (Core dump): A core dump file.
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
    return 1;
}

int debug_sym64 ( Elf64_Sym *sym ,elf64_manager * org)
{
    if (DEBUG)
    {
		printf("Symbol\n");
        printf("name->%s\n",(const char *)&org->strtab[sym->st_name]);
        printf("sh_name->%d\n",sym->st_name);
		printf("sh_type:%d\n",ELF64_ST_TYPE(sym->st_info));
		printf("sh_bind:%d\n",ELF64_ST_BIND(sym->st_info));
        printf("st_shndx:%d\n",sym->st_shndx);
		printf("st_size:%ld\n",sym->st_size);
        printf("st_value:%ld\n",sym->st_value);
		printf("-----------\n");
    }
    return (1);
}

int debug_shdr64(Elf64_Shdr *shdr,elf64_manager * org) {
    if (DEBUG) {
        printf("name->%s\n",(const char *)&org->sh_strtab[shdr->sh_name]);
        printf("sh_name: %u\n", shdr->sh_name);
        printf("Section Header:\n");
        printf("sh_type: %u\n", shdr->sh_type);
        printf("sh_flags: %lx\n", shdr->sh_flags);
        printf("sh_addr: %lx\n", shdr->sh_addr);
        printf("sh_offset: %lx\n", shdr->sh_offset);
        printf("sh_size: %lx\n", shdr->sh_size);
        printf("sh_link: %u\n", shdr->sh_link);
        printf("sh_addralign: %lx\n", shdr->sh_addralign);
        printf("sh_info: %u\n", shdr->sh_info);
        printf("sh_entsize: %lx\n", shdr->sh_entsize);
        printf("-----------\n");
    }
    return (1);
}