#include "../../lib/nm.h"

static int compare_symbols(const Elf64_Sym *sym_a, const Elf64_Sym *sym_b, elf64_manager * org, active_flags flags) {
    const char *name_a;
    const char *name_b;

	if (ELF64_ST_TYPE(sym_a->st_info) == STT_SECTION)
		name_a = (const char *)&org->sh_strtab[org->shdr[sym_a->st_shndx].sh_name];
	else
		name_a = (const char *)&org->strtab[sym_a->st_name];

	if (ELF64_ST_TYPE(sym_b->st_info) == STT_SECTION)
		name_b = (const char *)&org->sh_strtab[org->shdr[sym_b->st_shndx].sh_name];
	else
		name_b = (const char *)&org->strtab[sym_b->st_name];

    
    if (flags.r)
        return ft_strncmp(name_b, name_a, ft_strlen(name_b));
    return ft_strncmp(name_a, name_b, ft_strlen(name_a));
}

void bubble_sort_sym64(elf64_manager * org, active_flags flags)
{
    Elf64_Sym aux;
    
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