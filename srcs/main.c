#include "../lib/nm.h"

int debug_flags (active_flags flags )
{
    if (DEBUG)
    {
		printf("Flag\n");
        printf("-a : %d\n", flags.a);
		printf("-g : %d\n", flags.g);
		printf("-u : %d\n", flags.u);
		printf("-r : %d\n", flags.r);
		printf("-p : %d\n", flags.p);
		printf("- - - - - - - - - - - - - -\n");
		
    }
    return 1;
}

struct active_flags check_flags(int argc, char **argv)
{
	active_flags flags = {0, 0, 0, 0, 0};
    int i = 1;

    while (i < argc && argv[i][0] == '-')
    {
        int j = 1;
        while (argv[i][j] != '\0')
        {
            if (argv[i][j] == 'a')
				flags.a = 1;
            else if (argv[i][j] == 'g')
                flags.g = 1;
            else if (argv[i][j] == 'u')
                flags.u = 1;
            else if (argv[i][j] == 'r')
                	flags.r = 1;
            else if (argv[i][j] == 'p')
				flags.p = 1; 
            else
            {
                fprintf(stderr, "Flag desconocido: %c\n", argv[i][j]);
                break;
            }
            j++;
        }
        i++;
    }

    return flags;
}

int exec_ft_nm()
{

    return (0);
}

int	main(int argc, char **argv)
{
	int		i, fd , commands_exec;
	struct active_flags flags;

	// if (argc < 2)
	// {
	// 	fprintf(stderr, "Uso: %s [-flags] <archivo_ELF>\n", argv[0]);
	// 	// ft_nm("a.out");
	// 	return (0);
	// }j́j
	commands_exec = 0;
	flags = check_flags(argc,argv);
	// debug_flags(flags);
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{

			fd = open(argv[i], O_RDONLY);
			ft_nm(argv[i],fd, flags);
			if (fd != -1)
				close(fd);
			commands_exec++;
			// printf("FIN de '%s'\n",argv[i]);
		}
		i++;
	}

	if (commands_exec == 0)
	{
		fd = open("a.out", O_RDONLY);
		ft_nm("a.out",fd, flags);
		if (fd != -1)
			close(fd);
	}
		
    // 
    // ft_nm("file_test/ft_strlen.o");
    return 0 ;
}