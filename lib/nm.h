/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguelangelortizdelburgo <miguelangelor    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 12:51:39 by mortiz-d          #+#    #+#             */
/*   Updated: 2024/10/16 22:04:05 by miguelangel      ###   ########.fr       */
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
# define DEBUG 1
#endif

typedef struct elf64_manager{
    struct stat _raw_data;           //Estructura util para almacenar los datos del archivo que se lee
    void * _map;                     //El mapeo de los datos para poder extraer que necesitamos
    Elf64_Sym *symbols;
    Elf64_Ehdr *header;
    Elf64_Shdr *shdr;
} elf64_manager;

int	ft_nm(char *filename);
int header_checker(char *filename);
int	 analisis_ELF64(char *filename);

//DEBUG
int debug_type_file (Elf64_Half type );

