#include "../lib/nm.h"

// static int elf_32(char *filename);
// static int elf_64(char *filename);


int ft_nm(char *filename)
{
    int result;
    
    result = header_checker(filename);
    if (result == ELFCLASS64)
    {
        analisis_ELF64(filename);
    }
    else if(result == ELFCLASS32)
    {
        printf("Resultado archivo 32");
    }
    else if (result == ELFCLASSNONE)
    {
        printf("No es un archivo ELF");
    }
    return 0;
}

