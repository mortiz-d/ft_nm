#include "../../lib/nm.h"
#include <elf.h>

/*
    symbol posibilities
    """" -i              For PE format files this indicates that the symbol is in a section specific to the implementation of DLLs.
    -N              The symbol is a debugging symbol.
    -p              The symbol is in a stack unwind section.
    -'-'            The symbol is a stabs symbol in an a.out object file. 
    - ?             The symbol type is unknown
    */

char get_type_symbol(Elf64_Sym *sym, const char* name, elf64_manager * org) {
    symbol_data simbol;
    int         shdr_tipo;
    int         shdr_flag;
    simbol.bind = ELF64_ST_BIND(sym->st_info);
    simbol.type = ELF64_ST_TYPE(sym->st_info);
    

    if ( sym->st_shndx == SHN_ABS)
        return 'A';
    if ( sym->st_shndx == SHN_UNDEF)
        return 'U';
    
    
    

    
    if (simbol.type != STT_OBJECT && simbol.bind == STB_WEAK)
    {
        if (simbol.bind == STB_GLOBAL)
            return 'W';
        return 'w';
    }
    if (simbol.type == STT_OBJECT && simbol.bind == STB_WEAK)
    {
        if (simbol.bind == STB_GLOBAL)
            return 'V';
        return 'v';
    }
    
    //i STT_GNU_IFUNC  ///* Symbol is indirect code object */
    if ( simbol.type == STT_GNU_IFUNC) 
    {
        return 'I';
    }
    
    if (simbol.bind == STB_GNU_UNIQUE)
        return 'u';
    
    shdr_tipo = org->shdr[sym->st_shndx].sh_info;
    shdr_flag = org->shdr[sym->st_shndx].sh_flags;

    // printf("%016lx %-20s -> Type: %d -Bind: %d -Shdr Tipo: %d -Shdr Flag: %d\n", sym->st_value, (const char *)&org->strtab[sym->st_name],simbol.type ,simbol.bind,shdr_tipo, shdr_flag);
    // 'B/b' 'S/s'
    // .SUNW_bss .bss
    if (shdr_tipo == SHT_NOBITS && (shdr_flag == ( SHF_WRITE | SHF_ALLOC)))
    {
        if (simbol.type == STT_OBJECT)
        {
            if (simbol.bind == STB_GLOBAL)
                return 'S';
            return 's';
        }
        if (simbol.bind == STB_GLOBAL)
            return 'B';
        return 'b';
    }

    // 'C' SHN_COMMON - Symbols defined relative to this section are common symbols
    if (shdr_tipo == SHN_COMMON)
    {
        if (simbol.bind == STB_GLOBAL)
            return 'C';
        return 'c';
    }

    

    // D    The symbol is in the initialized data section
    //      .data && .data1
    if (shdr_tipo == SHT_PROGBITS && shdr_flag == (SHF_ALLOC | SHF_WRITE))
    {
        if (simbol.type == STT_OBJECT)
        {
            if (simbol.bind == STB_GLOBAL)
                return 'G';
            return 'g';
        }   
        if (simbol.bind == STB_GLOBAL)
            return 'D';
        return 'd';
    }

    //&& shdr_tipo == SHT_PROGBITS && shdr_flag == SHF_EXECINSTR simbol.type == STT_FUNC simbol.type == STT_FUNC && 
    if ((shdr_flag & ( SHF_EXECINSTR | SHF_ALLOC)) != 0 )
    {
        if (simbol.bind == STB_GLOBAL)
            return 'T';
        return 't';
    }

     // R   - The symbol is in a read only data section.
    // .rodata && .rodata1
    if (shdr_tipo == SHT_PROGBITS && shdr_flag == SHF_ALLOC )
    {
        if (simbol.bind == STB_GLOBAL)
            return 'R';
        return 'r';
    }

    
    
    return '?';  // Otros casos (símbolo no reconocido o no inicializado)
}



void process_symbol ( Elf64_Sym *sym , elf64_manager * org)
{
    char type = 'U';
    
    type = get_type_symbol(sym, (const char *)&org->strtab[sym->st_name],org);

   

    // if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
    //PARA SACAR EL NOMBRE DE LA TABLA -> (const char *)&org->strtab[sym->st_name]
    printf("%016lx %c %-20s\n", sym->st_value, type, (const char *)&org->strtab[sym->st_name]);
    // printf("simbol %-20s Bind:%d - Type:%d - absoluto?:%d\n",(const char *)&org->strtab[sym->st_name] , simbol.bind, simbol.type,sym->st_value);
    // printf("PRUEBA \nNombre %d - %s\n",sym->st_name ,(const char *)&org->strtab[sym->st_name]);
    // printf("BIND %s\n",ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ? "Global" : "Local");
    // if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
    //         printf("%016lx %c %s -\n", sym->st_value, type, (const char *)&org->strtab[sym->st_name]);

    // if (sym->st_shndx == SHN_UNDEF) {
    //     type = 'U';
    // } else if (sym->st_shndx == SHN_ABS) {
    //     type = 'A';
    // }
    // else {
    //     switch (ELF64_ST_TYPE(sym->st_info)) {
    //         case STT_OBJECT: type = ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ? 'D' : 'd'; break;
    //         case STT_FUNC: type = ELF64_ST_BIND(sym->st_info) == STB_GLOBAL ? 'T' : 't'; break;
    //         case STT_SECTION: type = 's'; break;
    //         case STT_FILE: type = 'F'; break;
    //         default: type = '?'; break;
    //     }
    // }

    // Debug output
    // printf("PRUEBA ->  %d %d %ld %ld | %016lx %c %s \n", sym->st_info , sym->st_name , sym->st_size ,  sym->st_value, sym->st_value, type, sym_name);
    // printf("%016lx %c %s -\n", sym->st_value, type, (const char *)&org->strtab[sym->st_name]);
}

void	 getSymbols(elf64_manager * org,void * _map)
{
    //Miramos en cada entrada de la seccion header
    for (int i = 0; i < org->elf_header->e_shnum; i++) {
        //Buscamos el sh
        if (org->shdr[i].sh_type == SHT_SYMTAB) {
            org->symbols = (Elf64_Sym *)(_map + org->shdr[i].sh_offset);
            org->num_symbols = org->shdr[i].sh_size / sizeof(Elf64_Sym);
            // printf("Número de símbolos: %d - %ld\n", org->num_symbols, org->shdr[i].sh_size );
            // printf("Seccion SYMTAB[%d]:%d -> %ld \n",i,org->shdr[i].sh_type, org->shdr[i].sh_entsize);
        }
        else
            // printf("Seccion[%d]:%d -> %ld \n",i,org->shdr[i].sh_type, org->shdr[i].sh_entsize);

        if (org->shdr[i].sh_type == SHT_STRTAB && i != org->elf_header->e_shstrndx) {
            org->strtab = (const char *)(_map + org->shdr[i].sh_offset);
        }
    }
}

int debug_symbols(elf64_manager * org, void * _map)
{
    if (org->symbols && org->strtab) {
        //Para sacar el nombre de las secciones
        // const char *shstrtab = (const char *)((char *)_map + org->shdr[org->elf_header->e_shstrndx].sh_offset);
        

        for (int i = 0; i < org->num_symbols; i++) {
            // const char *section_name = shstrtab + org->shdr[i].sh_name;
            process_symbol(&org->symbols[i], org);
            // printf("Nombre de la sección %d: %s\n", i, section_name);
        }
    }

    return 0;
}

int	 analisis_ELF64(void * _map, struct stat _file_data)
{
    elf64_manager org;

    org.elf_header =  (Elf64_Ehdr *)_map;
    org.shdr = (Elf64_Shdr *)((char *)_map + org.elf_header->e_shoff);
    debug_type_file(org.elf_header->e_type);


    getSymbols(&org,_map);
    debug_symbols(&org,_map);
    


    return 0;
}