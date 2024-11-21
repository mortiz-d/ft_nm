/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 12:51:39 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/11/15 00:14:03 by miguelangel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
#endif

# include <stdio.h>
# include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#ifndef DEBUG
# define DEBUG 0
#endif

#define NM_FILE_NOT_FOUND(filename) (printf("nm: '%s': No hay tal fichero\n", filename), 1)
#define NM_IS_DIR(filename) (printf("nm: Aviso: '%s' es un directorio\n", filename), 1)
#define NM_CANT_MMAP(filename) (printf("nm: Aviso: '%s' no se puede mapear\n", filename), 1)
#define NM_CANT_MUNMAP(filename) (printf("nm: Aviso: '%s' no se puede desmapear\n", filename), 1)
#define STR_GLOBAL "GLOBAL"
#define STR_LOCAL "LOCAL"

typedef struct elf64_manager{                   
    int num_symbols;
    Elf64_Sym *symbols;
    Elf64_Ehdr *elf_header;
    Elf64_Shdr *shdr;
    const char *strtab;
} elf64_manager;


typedef struct symbol_data{
    char result;
    int type;
    int bind;
}symbol_data;



int	ft_nm(char *filename, int fd);
int header_checker(void * _map, struct stat _file_data);
int	 analisis_ELF64(void * _map, struct stat _file_data);

//CHEKER
int file_checker(char *filename, char *data);

//DEBUG
int debug_type_file (Elf64_Half type );
int debug_symbols(elf64_manager * org, void * _map);

