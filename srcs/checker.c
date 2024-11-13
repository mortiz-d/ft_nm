#include "../lib/nm.h"

int header_checker(char *filename)
{
    //Creamos una variable donde guardaremos los valires identificativos del ELF
    unsigned char e_ident[EI_NIDENT];
    int fd = open(filename, O_RDONLY);
    ssize_t bytes_read; 

    //Comprobamos si puede leer el archivo
    if (fd < 0) {
        printf("Error al abrir el archivo ELF\n");
        return -1;
    }

    bytes_read = read(fd, e_ident, sizeof(e_ident));
    close(fd);
    //Comprobamos si lee una cabezera ELF
    if (bytes_read != sizeof(e_ident)) {
        printf("Error al leer la cabecera ELF\n");
        
        return -1;
    }

    if (e_ident[EI_MAG0] != ELFMAG0 || 
        e_ident[EI_MAG1] != ELFMAG1 || 
        e_ident[EI_MAG2] != ELFMAG2 || 
        e_ident[EI_MAG3] != ELFMAG3) {
        printf("El archivo no es un ELF válido\n");
        return -1;
    }

    if (e_ident[EI_CLASS] == ELFCLASS64) {
        printf("Clase ELF es de 64 bits\n");
        // elf_64(filename);
    } else if (e_ident[EI_CLASS] == ELFCLASS32) {
        printf("Clase ELF es de 32 bits\n");
        // elf_32(filename);
    } else if (e_ident[EI_CLASS] == ELFCLASSNONE){
        printf("Clase ELF no encontrada\n");
    } else if (e_ident[EI_CLASS] == ELFCLASSNUM)
    {
        printf("Clase multiple\n");
        // fprintf(stderr, "Clase ELF no encontrada\n")
    }

    return e_ident[EI_CLASS];
}