#include "../../lib/nm.h"

static int compare_symbols(const Elf32_Sym *sym_a, const Elf32_Sym *sym_b, elf32_manager * org, active_flags flags) {
    const char *name_a;
    const char *name_b;

	if (ELF32_ST_TYPE(sym_a->st_info) == STT_SECTION)
		name_a = (const char *)&org->sh_strtab[org->shdr[sym_a->st_shndx].sh_name];
	else
		name_a = (const char *)&org->sym_strtab[sym_a->st_name];

	if (ELF32_ST_TYPE(sym_b->st_info) == STT_SECTION)
		name_b = (const char *)&org->sh_strtab[org->shdr[sym_b->st_shndx].sh_name];
	else
		name_b = (const char *)&org->sym_strtab[sym_b->st_name];

    
    if (flags.r)
        return ft_strncmp(name_b, name_a, ft_strlen(name_b));
    return ft_strncmp(name_a, name_b, ft_strlen(name_a));
}

void bubble_sort_sym32(elf32_manager * org, active_flags flags)
{
    Elf32_Sym aux;
    
    for (int i = 0; i < org->num_symbols - 1; i++) {
        if (compare_symbols(&org->symbols[i], &org->symbols[i + 1], org, flags) > 0)
        {
        	aux = org->symbols[i];
            org->symbols[i] = org->symbols[i+1];
            org->symbols[i + 1] = aux;
            i = -1;
        }
    }
    
}