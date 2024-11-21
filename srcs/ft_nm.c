#include "../lib/nm.h"

// static int elf_32(char *filename);
// static int elf_64(char *filename);

int class_analisis (void * _map, struct stat _file_data, int ei_class)
{
    if (ei_class == ELFCLASSNONE)
    {
        printf("EL ELF NO TIENE VERSION\n");
    }
    else if (ei_class == ELFCLASS32)
    {
        printf("EL ELF ES VERSION 32\n");
    }
    else if (ei_class == ELFCLASS64)
    {
        printf("EL ELF ES VERSION 64\n");
        analisis_ELF64(_map, _file_data);
    }
    else
     return 1;
    return 0;
}

int ft_nm(char *filename, int fd)
{
    struct stat _file_data; //Estructura util para almacenar los datos del archivo que se lee
    void * _map;            //El mapeo de los datos para poder extraer que necesitamos en una direccion
    int _ei_class;

    if (fstat(fd, &_file_data) == -1 || fd == -1 ) 
        return NM_FILE_NOT_FOUND(filename);
    if (S_ISDIR(_file_data.st_mode))
        return NM_IS_DIR(filename);

    _map = mmap(NULL,_file_data.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    if (_map == MAP_FAILED)
        return NM_CANT_MMAP(filename);
    
    _ei_class = header_checker(_map, _file_data);
    class_analisis(_map, _file_data, _ei_class);

    if (munmap(_map, _file_data.st_size) == -1)
        return NM_CANT_MUNMAP(filename);
    
    // printf("EL FILE ESTA GOOD\n");
    
    return 0;  // Retorno exitoso
}

