#include "../lib/nm.h"

int main (int argc , char** argv)
{
    int i;
    int fd;
    (void) argc;
    (void) argv;
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_ELF>\n", argv[0]);
        // ft_nm("a.out");
        return 0;
    }
    
    i = 1;
    while (i < argc)
    {
        // printf("Leemos %s \n",argv[i]);
        fd = open(argv[i], O_RDONLY);
        ft_nm(argv[i],fd);
        if (fd != -1)
            close(fd);
        // printf("FIN de '%s'\n",argv[i]);
        i++;
    }
    
    // 
    // ft_nm("file_test/ft_strlen.o");
    return 0 ;
}