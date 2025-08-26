#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_config	config;
	t_envp		*envp;
	t_ent		*tree;

	if (argc != 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!env || !*env)
		return (printf("Error: Can't load env variables\n"), 1);

	load_config(&config, env);
	envp = save_envp(env);
	if (!envp)
		return (printf("Error saving envp\n"), 1);

	while (1)
	{
		line = readline(config.prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			tree = NULL;
			if (parse_command(envp, &config, &tree, line) != 0)
				printf("Error parsing command\n");
			else if (tree)
			{
				printf("===================================================\n");
				print_tree(tree, 0);
				printf("===================================================\n");
				config.exit_code = exec_tree(tree, envp, &config);
				if (config.exit_code != 0)
					printf("Command exited with code: %d\n", config.exit_code);
				ent_free(tree);
			}
			if (config.prompt)
				free(config.prompt);
			config.prompt = build_prompt(config.prompt_raw, config.exit_code);
		}
		free(line);
	}
	return (0);
}
