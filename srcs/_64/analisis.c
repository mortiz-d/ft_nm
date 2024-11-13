#include "../../lib/nm.h"



void process_symbol ( Elf64_Sym *sym , const char *sym_name, const char *sec_name)
{
    char type;
    // printf("procesa aqsui\n");
    if (strlen(sym_name) == 0)
        return;
    // Check if the symbol is undefined
    if (sym->st_shndx == SHN_UNDEF) {
        type = 'U'; // Símbolo indefinido
    } else if (sym->st_shndx == SHN_ABS) {
        type = 'A'; // Símbolo absoluto
    }
    else {

        if (sec_name == NULL)
            sec_name = "Desconocido";

        switch (ELF64_ST_TYPE(sym->st_info)) {
            case STT_OBJECT: type = ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ? 'D' : 'd'; break;
            case STT_FUNC: type = ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ? 'T' : 't'; break;
            case STT_SECTION: type = 's'; break;
            case STT_FILE: type = 'F'; break;
            default: type = '?'; break;
        }
    }

    // Debug output
    // printf("PRUEBA ->  %d %d %ld %ld | %016lx %c %s \n", sym->st_info , sym->st_name , sym->st_size ,  sym->st_value, sym->st_value, type, sym_name);
    printf("%016lx %c %s -\n", sym->st_value, type, sym_name);
}


// int	 analisis_ELF64(char *filename)
// {
// 	// char *filename = argv[1];s
//     elf64_manager org;
//     int fd = open(filename, O_RDONLY);
//     struct stat ejemplo;
//     void * map;
//     int num_symbols;
//     Elf64_Sym *symbols;
//     Elf64_Ehdr *elf_header;
//     Elf64_Shdr *shdr;
//     const char *strtab;
//     fstat(fd,&ejemplo);

//     if (fd < 0) {
//         printf("Error al abrir el archivo ELF\n");
//         return 1;
//     }
//     map = mmap(NULL,ejemplo.st_size,PROT_READ,MAP_PRIVATE,fd,0);
//     if (map == MAP_FAILED)
//     {
//         close(fd);
//         printf("Error en el map\n");
//         return 1;

//     }

//     // Mostrar la "Palabra Mágica" (identificación ELF)
//     elf_header =  (Elf64_Ehdr *)map;
//     shdr = (Elf64_Shdr *)((char *)map + elf_header->e_shoff);
//     strtab = NULL;
//     printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
//     debug_type_file(elf_header->e_type);
//     printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");

    

//     for (int i = 0; i < elf_header->e_shnum; i++) {
//         if (shdr[i].sh_type == SHT_SYMTAB) {
//             // printf("Sección de símbolos encontrada: .symtab\n");
//             symbols = (Elf64_Sym *)(map + shdr[i].sh_offset);
//             // num_symbols = shdr[i].sh_size;
//             num_symbols = shdr[i].sh_size / sizeof(Elf64_Sym);
//             printf("Número de símbolos: %d - %ld\n", num_symbols, shdr[i].sh_size );
//         }

//         if (shdr[i].sh_type == SHT_STRTAB && i != elf_header->e_shstrndx) {
//             strtab = (const char *)(map + shdr[i].sh_offset);
//             printf("0.strtab->%s\n",strtab);
//         }
//     }

//     //  && strtab
//     if (symbols && strtab) {
//         const char *shstrtab = (const char *)((char *)map + shdr[elf_header->e_shstrndx].sh_offset);
//         Elf64_Sym *symbol_table[num_symbols];
//         //__
//         for (int i = 0; i < num_symbols; i++) {
//             symbol_table[i] = &symbols[i];
//         }

//         for (int i = 0; i < num_symbols; i++) {
//             // const char *sec_name = &shstrtab[shdr[symbol_table[i]->st_shndx].sh_name];
//             // printf("%d - %s\n",i,"sec_name");
//             // Elf64_Sym *sym = &symbols[i];
//             // const char *sym_name = &strtab[sym->st_name];
//             // process_symbol(symbol_table[i], &strtab[symbol_table[i]->st_name], "a" );
//             process_symbol(&symbols[i], &strtab[symbols[i].st_name], "a" );
//         }
//     }else
//     {
//         printf("No simbol\n");
//     }



//     // Limpiar y cerrar
//     munmap(map, ejemplo.st_size);
//     close(fd);


//     return 0;
// }

int read_ELF_data (char *filename, elf64_manager *org)
{
    int fd = open(filename, O_RDONLY);
    printf("lectura\n");
    fstat(fd,&org->_raw_data);
    // fstat(fd,&ejemplo);
    printf("post\n");
    if (fd < 0) {
        printf("Error al abrir el archivo ELF\n");
        return 0;
    }
    printf("map\n");
    org->_map = mmap(NULL,org->_raw_data.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    printf("postmap\n");
    if (org->_map == MAP_FAILED)
    {
        close(fd);
        printf("Error en el map\n");
        return 0;
    }
    printf("end?\n");
    close(fd);
    return 1;
}

int	 analisis_ELF64(char *filename)
{
	// char *filename = argv[1];s
    elf64_manager *org = malloc(sizeof(elf64_manager) * 1);
    printf("aaaa\n");
    if (read_ELF_data(filename, org) == 0)
    {
        return 0;
    }

    int num_symbols;
    Elf64_Sym *symbols;
    Elf64_Ehdr *elf_header;
    Elf64_Shdr *shdr;
    const char *strtab;
    

    // Mostrar la "Palabra Mágica" (identificación ELF)
    elf_header =  (Elf64_Ehdr *)org->_map;
    shdr = (Elf64_Shdr *)((char *)org->_map + elf_header->e_shoff);
    strtab = NULL;
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    debug_type_file(elf_header->e_type);
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");



    // Limpiar y cerrar
    munmap(org->_map, org->_raw_data.st_size);


    return 0;
}
