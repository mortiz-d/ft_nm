#include "../lib/nm.h"

int header_checker(void * _map, struct stat _file_data)
{
    unsigned char *e_ident = (unsigned char *)_map;

    if (e_ident[EI_MAG0] != ELFMAG0 || 
        e_ident[EI_MAG1] != ELFMAG1 || 
        e_ident[EI_MAG2] != ELFMAG2 || 
        e_ident[EI_MAG3] != ELFMAG3) {
        printf("El archivo no es un ELF válido\n");
        return -1;
    }
    
    return e_ident[EI_CLASS];
}