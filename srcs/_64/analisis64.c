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




char get_type_sym64(Elf64_Sym *sym, elf64_manager * org) {
    int shdr_tipo;
    int shdr_flag;
    int sym_bind;
    int sym_type;
    
    (void)shdr_flag;
    (void)shdr_tipo;
    (void)org;
    sym_bind = ELF64_ST_BIND(sym->st_info);
    sym_type = ELF64_ST_TYPE(sym->st_info);

	// A/a The symbol's value is absolute
    if ( sym->st_shndx == SHN_ABS)
    {
        if (sym_bind == STB_GLOBAL)
            return 'A';
        return 'a';
    }

	shdr_tipo = org->shdr[sym->st_shndx].sh_type;
    shdr_flag = org->shdr[sym->st_shndx].sh_flags;

    // V/v The symbol is a weak object 
    // W/w The symbol is a weak symbol
    if (sym_bind == STB_WEAK)
    {
    // printf("Encontramos cosas\n");
        
        if (sym_type == STT_OBJECT)
        {
            if (shdr_tipo == SHT_NULL)
                return 'v';
            return 'V';
        }  
        if (shdr_tipo == SHT_NULL)
            return 'w';
        return 'W';
    }
    
    // //i STT_GNU_IFUNC  ///* Symbol is indirect code object */
    if ( sym_type == STT_GNU_IFUNC) 
        return 'I';
    
    // // u The symbol is a unique global symbol.
    if (sym_bind == STB_GNU_UNIQUE)
        return 'u';
    
    // U The symbol is undefined.
    if ( sym->st_shndx == SHN_UNDEF)
        return 'U';

    // // 'B/b' The symbol is in the BSS data section
    // // 'S/s' The symbol is in an uninitialized or zero-initialized data section for small objects
    if (shdr_tipo == SHT_NOBITS && (shdr_flag == (SHF_WRITE | SHF_ALLOC)))
    {
        if (sym_type == STT_OBJECT)
        {
            if (sym_bind == STB_GLOBAL)
                return 'S';
            return 's';
        }
        if (sym_bind == STB_GLOBAL)
            return 'B';
        return 'b';
    }

    // // 'C' SHN_COMMON - Symbols defined relative to this section are common symbols
    if (shdr_tipo == SHN_COMMON)
    {
        if (sym_bind == STB_GLOBAL)
            return 'C';
        return 'c';
    }

    
    if ((shdr_tipo == SHT_DYNAMIC && shdr_flag == (SHF_ALLOC | SHF_WRITE)))
    {
        if (sym_bind == STB_GLOBAL)
            return 'D';
        return 'd';
    }

    // // D/d    The symbol is in the initialized data section .data && .data1
    // // G/g    The symbol is in the initialized data section for small objects.  
    if ((shdr_tipo == SHT_PROGBITS && shdr_flag == (SHF_ALLOC | SHF_WRITE)))
    {
        if (sym_type == STT_OBJECT)
        {
            if (sym_bind == STB_GLOBAL)
                return 'G';
            return 'g';
        }
        if (sym_bind == STB_GLOBAL)
            return 'D';
        return 'd';
    }

    // // R   - The symbol is in a read only data section.
    if ( (shdr_tipo == SHT_PROGBITS || shdr_tipo == SHT_NOTE ) && shdr_flag == (SHF_ALLOC) )
    {
        if (sym_bind == STB_GLOBAL)
            return 'R';
        return 'r';
    }

    // // 
    if ((shdr_tipo == SHT_PROGBITS && shdr_flag & ( SHF_EXECINSTR | SHF_ALLOC)) != 0 )
    {
        if (sym_bind == STB_GLOBAL)
            return 'T';
        return 't';
    }

    
    return '?';  // Otros casos (símbolo no reconocido o no inicializado)
}

char get_type_sym_special_sections_64(Elf64_Sym *sym, elf64_manager * org) {
    char const * name ;
    char type;
    char *sections [9] = {".bss",".dynamic",".got",".data",".data1",".init_array",".preinit_array",".fini_array", ".text"};
    char char_type[9] = {'b','d','d','d','d','d','d','d', 't'};
    type = '?';
    if ( sym->st_shndx == SHN_ABS)
        return (type);
    name = (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name];
    for (int i = 0; i < 9; i++)
    {
    // printf("Vuelta %d\n", 0);
        // printf("Vuelta %d - %s vs %s\n", i, name , sections[i]);
        if(strcmp(sections[i],name) == 0)
        {
            type = (char_type[i]);
            break;
        }
    }
    if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
        type = ft_toupper(type);

    return (type);

}

void	show_Sym64(Elf64_Sym *sym, elf64_manager *org, active_flags flags)
{
	char			type;
    const char *sym_name;
	unsigned char	info;
    unsigned char	bind;

	info = ELF64_ST_TYPE(sym->st_info);
    bind = ELF64_ST_BIND(sym->st_info);
    type = get_type_sym_special_sections_64(sym,org);
    if (type == '?')
	    type = get_type_sym64(sym, org);
    (void)sym_name;
    (void)type;
	if (info == STT_SECTION)
		sym_name = (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name];
	else
        sym_name = (const char *)&org->sym_strtab[sym->st_name];

	// if  (type == 'B' || type == 'b' || type == 'S' || type == 's')
		// debug_sym64(sym, org);

	if (info == STT_NOTYPE || info == STT_OBJECT || info == STT_FUNC || info == STT_COMMON || flags.a)
	{
		// debug_sym64(sym, org);
		if(flags.g && bind != STB_GLOBAL)
			return;

		if (type == 'U' || type == 'w')
			printf("                 %c %-20s\n",type, sym_name);
		else
			if(!flags.u)
				printf("%016lx %c %-20s\n",sym->st_value,type, sym_name);
	}
	return;
    

}

static void	 extract_Sym64(elf64_manager * org,void * _map)
{
	Elf64_Sym	*symbols_temp;
	const char	*strtab_aux;
	int			c_aux;

	strtab_aux = NULL;
	org->sym_strtab = NULL;
    org->sh_strtab = NULL;
	//Miramos en cada entrada de la seccion header
	for (int i = 0; i < org->elf_header->e_shnum; i++) {
		//Comprobamos en los tipos de shoulder la tabla de simbolos
		if (org->shdr[i].sh_type == SHT_SYMTAB) {  // debug_shdr64(&org->shdr[i]);
			symbols_temp = (Elf64_Sym *)(_map + org->shdr[i].sh_offset);
			org->num_symbols = org->shdr[i].sh_size / sizeof(Elf64_Sym);
            
			org->symbols = calloc (org->num_symbols + 1 , sizeof(Elf64_Sym));
			c_aux = 0;
            //Extraemos cada simbolo y quitamos la cabecera vacia (1er sym) de nuestros simbolos
			for (int i = 0; i < org->num_symbols; i++) {
				if ( symbols_temp[i].st_name == 0 && symbols_temp[i].st_info == 0 && symbols_temp[i].st_other == 0 && \
					symbols_temp[i].st_shndx == 0 && symbols_temp[i].st_size == 0 && symbols_temp[i].st_value == 0)
				{
					c_aux++;
					org->num_symbols--;
				}
				org->symbols[i] = symbols_temp[c_aux++];                
			}
        }
        if (org->shdr[i].sh_type == SHT_STRTAB ) {

            strtab_aux = (const char *)(_map + org->shdr[i].sh_offset);
            if ( i != org->elf_header->e_shstrndx)
                org->sym_strtab = strtab_aux;
            else
                org->sh_strtab = strtab_aux;

        }
    }
    
    if (!DEBUG)
    {
        for (int i = 0; i < org->elf_header->e_shnum; i++) 
        {
           debug_shdr64(&org->shdr[i], org);
        }
    }


	if (org->sym_strtab == NULL)
		org->sym_strtab = strtab_aux;

	return;
}


static	int	process_sym64(elf64_manager *org, active_flags flags)
{
	if (org->symbols && org->sym_strtab) {
		if (!flags.p)
		{
			bubble_sort_sym64(org,flags);		//Organizamos los simbolos alfabeticamente
		}
		for (int i = 0; i < org->num_symbols; i++) {
			show_Sym64(&org->symbols[i], org, flags); //Mostramos los simbolos
            // if (i == 3)
            //     break;
		}
	}
	return (0);
}

int	analisis_ELF64(void * _map, active_flags flags)
{
	elf64_manager org;

	org.elf_header =  (Elf64_Ehdr *)_map;                               //Sacamos el header del archivo elf
	org.shdr = (Elf64_Shdr *)((char *)_map + org.elf_header->e_shoff);  //Sacamos la lista de los shoulders
	extract_Sym64(&org,_map);     //Extraigo los simbolos
	process_sym64(&org, flags);          //Se procesan para mostrarse
	free(org.symbols);              //Los liberamos de la memoria
    return (0);
}