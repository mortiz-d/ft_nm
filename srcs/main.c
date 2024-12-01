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

int count_commands(int argc, char **argv)
{
	int i, count;

	i = 1;
	count = 0;
	while (i < argc && argv[i][0] == '-')
    {
        if (argv[i][0] != '-')
		{
			count++;
			if (count > 1)
				return (1);
		}
		i++;
    }
    return (0);
}

int	main(int argc, char **argv)
{
	int		i, fd , commands_exec, multiple_commands;
	struct active_flags flags;

	commands_exec = 0;
	flags = check_flags(argc,argv);
	multiple_commands = count_commands(argc, argv);
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{

			fd = open(argv[i], O_RDONLY);
			ft_nm(argv[i],fd, flags, multiple_commands);
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
		if (ft_nm("a.out",fd, flags, 0))
		{
			printf ("\n");
		}

		if (fd != -1)
			close(fd);
	}
		
    // 
    // ft_nm("file_test/ft_strlen.o");
    return 0 ;
}