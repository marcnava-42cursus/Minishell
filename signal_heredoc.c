#include "libft.h"
#include <signal.h>

static void handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd(STDOUT_FILENO, '\n');
		exit(130);
	}
}

void set_heredoc_signal(void)
{
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler_heredoc;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
