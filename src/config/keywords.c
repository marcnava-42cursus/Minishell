/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keywords.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:08:54 by marcnava          #+#    #+#             */
/*   Updated: 2025/08/03 14:12:11 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

char	*get_hostname(void)
{
	static char	hostname[256];

	if (gethostname(hostname, sizeof(hostname)) == 0)
		return (hostname);
	return ("unknown");
}

char	*get_whoami(void)
{
	struct passwd	*pw;

	pw = getpwuid(getuid());
	if (pw)
		return (pw->pw_name);
	return ("unknown");
}

char	*get_pwd(void)
{
	static char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		return (cwd);
	return ("unknown");
}

char	*get_gitbranch(void)
{
	int		fd;
	char	buffer[256];
	char	*ref;

	fd = open(".git/HEAD", O_RDONLY);
	if (fd < 0)
		return ("");
	read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	ref = ft_strnstr(buffer, "refs/heads/", ft_strlen(buffer));
	if (ref)
		return (ref + 11);
	return ("");
}

// char *get_time(void)
// {
// 	static char timestr[6];
// 	time_t now = time(NULL);
// 	struct tm *tm_info = localtime(&now);
// 	snprintf(timestr, sizeof(timestr), "%02d:%02d", tm_info->tm_hour,
// 		tm_info->tm_min);
// 	return (timestr);
// }

// char *get_datetime(void)
// {
// 	static char datetimestr[20];
// 	time_t now = time(NULL);
// 	struct tm *tm_info = localtime(&now);
// 	snprintf(datetimestr, sizeof(datetimestr), "%04d-%02d-%02d %02d:%02d",
// 		tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
// 		tm_info->tm_hour, tm_info->tm_min);
// 	return (datetimestr);
// }

// char *get_exitcode(int exitcode)
// {
// 	static char code[12];
// 	snprintf(code, sizeof(code), "%d", exitcode);
// 	return (code);
// }

char	*get_jobs(void)
{
	return ("0");
}

char	*get_battery(void)
{
	return ("100%");
}
