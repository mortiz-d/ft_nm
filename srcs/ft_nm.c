#include "../lib/nm.h"

static int	header_checker(void *_map)
{
	unsigned char	*e_ident;

	e_ident = (unsigned char *)_map;
	if (e_ident[EI_MAG0] != ELFMAG0 || \
		e_ident[EI_MAG1] != ELFMAG1 || \
		e_ident[EI_MAG2] != ELFMAG2 || \
		e_ident[EI_MAG3] != ELFMAG3)
	{
		printf("El archivo no es un ELF válidgo\n");
		return (-1);
	}
	return (e_ident[EI_CLASS]);
}

static	int class_analisis(void *_map, int ei_class, active_flags flags)
{
	if (ei_class == ELFCLASSNONE)
	{
		printf("EL ELF NO TIENE VERSION\n");
	}
	else if (ei_class == ELFCLASS32)
	{
		analisis_ELF32(_map,flags);
	}
	else if (ei_class == ELFCLASS64)
	{
		analisis_ELF64(_map,flags);
	}
	else
		return (1);
	return (0);
}

int	ft_nm(char *filename, int fd, active_flags flags, int mc)
{
	struct stat	_file_data; 		//Estructura util para almacenar los datos del archivo que se lee
	void		*_map;            	//El mapeo de los datos para poder extraer que necesitamos en una direccion
	int			_ei_class;			//indent del archivo

	if (fstat(fd, &_file_data) == -1 || fd == -1 ) 
	    return NM_FILE_NOT_FOUND(filename);
	if (S_ISDIR(_file_data.st_mode))
	    return NM_IS_DIR(filename);

	_map = mmap(NULL,_file_data.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if (_map == MAP_FAILED)
	    return NM_CANT_MMAP(filename);
	_ei_class = header_checker(_map);
	if (mc)
		printf("\n%s:\n",filename);
	class_analisis(_map, _ei_class, flags);
	if (munmap(_map, _file_data.st_size) == -1)
	    return NM_CANT_MUNMAP(filename);
	return (1); // Retorno exitoso
}

