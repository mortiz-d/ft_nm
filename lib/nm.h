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
#include <strings.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../libft/libft.h"


#ifndef DEBUG
# define DEBUG 1
#endif

#define NM_FILE_CORE_DUMP (printf("nm: fichero es coredump\n"), 0)
#define NM_FILE_NOT_FOUND(filename) (printf("nm: '%s': No hay tal fichero\n", filename), 0)
#define NM_IS_DIR(filename) (printf("nm: Aviso: '%s' es un directorio\n", filename), 0)
#define NM_CANT_MMAP(filename) (printf("nm: Aviso: '%s' no se puede mapear\n", filename), 0)
#define NM_CANT_MUNMAP(filename) (printf("nm: Aviso: '%s' no se puede desmapear\n", filename), 0)

//MANAGERS 32/64 Bits
typedef struct elf64_manager{                   
    int num_symbols;
    Elf64_Sym *symbols;
    Elf64_Ehdr *elf_header;
    Elf64_Shdr *shdr;
    const char *sym_strtab;
    const char *sh_strtab;
} elf64_manager;

typedef struct elf32_manager{                   
    int num_symbols;
    Elf32_Sym *symbols;
    Elf32_Ehdr *elf_header;
    Elf32_Shdr *shdr;
    const char *sym_strtab;
    const char *sh_strtab;
} elf32_manager;

//FLAG STRUCT
typedef struct active_flags{                   
    int a;
    int g;
    int u;
    int r;
    int p;
} active_flags;


//BASE FUNCTIONS
int	ft_nm			(char *filename, int fd, active_flags flags, int mc);
int	analisis_ELF64	(void * _map, active_flags flags);
int	analisis_ELF32	(void * _map, active_flags flags);
char get_type_sym64(Elf64_Sym *sym, elf64_manager * org);
char get_type_sym_special_sections_64(Elf64_Sym *sym, elf64_manager * org);

//SORTING
void	bubble_sort_sym64(elf64_manager * org, active_flags flags);
void	bubble_sort_sym32(elf32_manager * org, active_flags flags);

//DEBUG
int	debug_type_file	(Elf64_Half type );
int	debug_sym64		(Elf64_Sym *sym ,elf64_manager * org);
int	debug_shdr64	(Elf64_Shdr *shdr,elf64_manager * org);
int	debug_sym32		(Elf32_Sym *sym ,elf32_manager * org);
int	debug_shdr32	(Elf32_Shdr *shdr,elf32_manager * org);
