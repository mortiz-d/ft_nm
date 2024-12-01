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

char get_type_sym32(Elf32_Sym *sym, elf32_manager * org) {
    int shdr_tipo;
    int shdr_flag;
    int sym_bind;
    int sym_type;
    
    sym_bind = ELF32_ST_BIND(sym->st_info);
    sym_type = ELF32_ST_TYPE(sym->st_info);
    if (sym_bind == STB_WEAK)
    {
        if (sym_type == STT_OBJECT)
        {
            if (sym_bind == STB_GLOBAL)
                return 'V';
            return 'v';
        }  
        if (sym_bind == STB_GLOBAL)
            return 'W';
        return 'w';
    }
    
    //i STT_GNU_IFUNC  ///* Symbol is indirect code object */
    if ( sym_type == STT_GNU_IFUNC) 
    {
        return 'I';
    }
    
    if (sym_bind == STB_GNU_UNIQUE)
        return 'u';
    
    if ( sym->st_shndx == SHN_ABS)
        return 'A';
    if ( sym->st_shndx == SHN_UNDEF)
        return 'U';

    shdr_tipo = org->shdr[sym->st_shndx].sh_info;
    shdr_flag = org->shdr[sym->st_shndx].sh_flags;

    // printf("%016lx %-20s -> Type: %d -Bind: %d -Shdr Tipo: %d -Shdr Flag: %d\n", sym->st_value, (const char *)&org->sym_strtab[sym->st_name],sym_type ,sym_bind,shdr_tipo, shdr_flag);
    // 'B/b' 'S/s'
    // .SUNW_bss .bss
    if (shdr_tipo == SHT_NOBITS && (shdr_flag == ( SHF_WRITE | SHF_ALLOC)))
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

    // 'C' SHN_COMMON - Symbols defined relative to this section are common symbols
    if (shdr_tipo == SHN_COMMON)
    {
        if (sym_bind == STB_GLOBAL)
            return 'C';
        return 'c';
    }

    

    // D    The symbol is in the initialized data section
    //      .data && .data1
    if (shdr_tipo == SHT_PROGBITS && shdr_flag == (SHF_ALLOC | SHF_WRITE))
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

    //&& shdr_tipo == SHT_PROGBITS && shdr_flag == SHF_EXECINSTR sym_type == STT_FUNC sym_type == STT_FUNC && 
    if ((shdr_flag & ( SHF_EXECINSTR | SHF_ALLOC)) != 0 )
    {
        if (sym_bind == STB_GLOBAL)
            return 'T';
        return 't';
    }

    // R   - The symbol is in a read only data section.
    // .rodata && .rodata1
    if (shdr_tipo == SHT_PROGBITS && shdr_flag == SHF_ALLOC )
    {
        if (sym_bind == STB_GLOBAL)
            return 'R';
        return 'r';
    }
    
    return '?';  // Otros casos (símbolo no reconocido o no inicializado)
}

void	show_Sym32(Elf32_Sym *sym, elf32_manager *org, active_flags flags)
{
	char			type;
    const char *sym_name;
	unsigned char	info;
    unsigned char	bind;

	info = ELF32_ST_TYPE(sym->st_info);
    bind = ELF32_ST_BIND(sym->st_info);
	type = get_type_sym32(sym, org);
    
	if (info == STT_SECTION)
		sym_name = (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name];
	else
        sym_name = (const char *)&org->sym_strtab[sym->st_name];

	if (info == STT_NOTYPE || info == STT_OBJECT || info == STT_FUNC || info == STT_COMMON || flags.a)
	{
		if(flags.g && bind != STB_GLOBAL)
			return;

		if (type != 'U')
		{
			if(!flags.u)
				printf("%016x %c %-20s\n",sym->st_value,type, sym_name);
		}
		else
			printf("                 %c %-20s\n",type, sym_name);
	}
	return;
}

static void	 extract_Sym32(elf32_manager * org,void * _map)
{
	Elf32_Sym	*symbols_temp;
	const char	*strtab_aux;
	int			c_aux;

	strtab_aux = NULL;
	org->sym_strtab = NULL;
    org->sh_strtab = NULL;
	//Miramos en cada entrada de la seccion header
	for (int i = 0; i < org->elf_header->e_shnum; i++) {
		//Comprobamos en los tipos de shoulder la tabla de simbolos
		if (org->shdr[i].sh_type == SHT_SYMTAB) {  // debug_shdr32(&org->shdr[i]);
			symbols_temp = (Elf32_Sym *)(_map + org->shdr[i].sh_offset);
			org->num_symbols = org->shdr[i].sh_size / sizeof(Elf32_Sym);
            
			org->symbols = calloc (org->num_symbols + 1 , sizeof(Elf32_Sym));
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
    
    if (DEBUG)
    {
        for (int i = 0; i < org->elf_header->e_shnum; i++) 
        {
          debug_shdr32(&org->shdr[i], org);
        }
    }


	if (org->sym_strtab == NULL)
		org->sym_strtab = strtab_aux;

	return;
}

static	int	process_sym32(elf32_manager *org, active_flags flags)
{
	if (org->symbols && org->sym_strtab) {
		if (!flags.p)
		{
			bubble_sort_sym32(org,flags);		//Organizamos los simbolos alfabeticamente
		}
		for (int i = 0; i < org->num_symbols; i++) {
			show_Sym32(&org->symbols[i], org, flags); //Mostramos los simbolos
		}
	}
	return (0);
}

int	analisis_ELF32(void * _map, active_flags flags)
{
	elf32_manager org;

	org.elf_header =  (Elf32_Ehdr *)_map;                               //Sacamos el header del archivo elf
	org.shdr = (Elf32_Shdr *)((char *)_map + org.elf_header->e_shoff);  //Sacamos la lista de los shoulders
	extract_Sym32(&org,_map);     //Extraigo los simbolos
	process_sym32(&org, flags);          //Se procesan para mostrarse
	free(org.symbols);              //Los liberamos de la memoria
    return (0);
}