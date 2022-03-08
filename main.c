#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char *cmd;

	while (1)
	{
		cmd = readline("Minishell");
		if (!cmd)
			exit(0);
		if (cmd != NULL)
		{
			add_history(cmd);
			//Gestion de la commande historique
		}
		free(cmd);
	}
}
