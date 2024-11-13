#include "../lib/nm.h"

int main (int argc , char** argv)
{

    (void) argc;
    (void) argv;
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_ELF>\n", argv[0]);
        ft_nm("a.out");
    }
    ft_nm(argv[1]);
    // ft_nm("file_test/ft_strlen.o");
    return 0 ;
}