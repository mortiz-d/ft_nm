#include "../lib/nm.h"

// static int elf_32(char *filename);
// static int elf_64(char *filename);

static	int class_analisis(void *_map, int ei_class, active_flags flags)
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
		analisis_ELF64(_map,flags);
	}
	else
		return (1);
	return (0);
}

int	ft_nm(char *filename, int fd, active_flags flags)
{
	struct stat	_file_data; //Estructura util para almacenar los datos del archivo que se lee
	void		*_map;            //El mapeo de los datos para poder extraer que necesitamos en una direccion
	int			_ei_class;

	if (fstat(fd, &_file_data) == -1 || fd == -1 ) 
	    return NM_FILE_NOT_FOUND(filename);
	if (S_ISDIR(_file_data.st_mode))
	    return NM_IS_DIR(filename);

	_map = mmap(NULL,_file_data.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if (_map == MAP_FAILED)
	    return NM_CANT_MMAP(filename);
	_ei_class = header_checker(_map);
	class_analisis(_map, _ei_class, flags);
	if (munmap(_map, _file_data.st_size) == -1)
	    return NM_CANT_MUNMAP(filename);
	return (0); // Retorno exitoso
}

