
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include <string.h>
#include <stdio.h>

#include "libft.h"
#include "ft_printf.h"

void		ft_putnstr(char *str, int n)
{
	write(1, str, n);
}

void		ft_putnchar(char c, int nb)
{
	int		i;

	i = 0;
	while (i < nb)
	{
		write(1, &c, 1);
		i++;
	}
}

int			others_flags(t_fhandler *handler)
{
	int		i;

	i = 0;
	while (i < NB_FORMAT_FLAGS)
	{
		if (handler->format_flags[i])
			return (1);
		i++;
	}
	i = 0;
	while (i < NB_TYPE_FLAGS)
	{
		if (handler->type_flags[i])
			return (1);
		i++;
	}
	return (0);
}

int			others_type_flags(t_fhandler *handler, int flag)
{
	int		i;

	i = 0;
	while (i < NB_TYPE_FLAGS)
	{
		if (handler->type_flags[i] && i != flag)
			return (1);
		i++;
	}
	return (0);
}

void		handle_format_flags(char **format, t_fhandler *handler)
{
	while (**format == '#' || **format == '0' || **format == '-'
		|| **format == '+' || **format == ' ')
	{
		if (**format == '#' && !handler->format_flags[FORMAT_HASHTAG])
			handler->format_flags[FORMAT_HASHTAG] += 1;
		else if (**format == '0' && !handler->format_flags[FORMAT_ZERO] &&
			!handler->format_flags[FORMAT_MINUS])
			handler->format_flags[FORMAT_ZERO] += 1;
		else if (**format == '-' && !handler->format_flags[FORMAT_MINUS] && 
			!handler->format_flags[FORMAT_ZERO])
			handler->format_flags[FORMAT_MINUS] += 1;
		else if (**format == '+' && !handler->format_flags[FORMAT_PLUS]
			&& !handler->format_flags[FORMAT_SPACE])
			handler->format_flags[FORMAT_PLUS] += 1;
		else if (**format == ' ' && !handler->format_flags[FORMAT_SPACE]
			&& !handler->format_flags[FORMAT_PLUS])
			handler->format_flags[FORMAT_SPACE] += 1;
		else
			exit(-1);
		(*format)++;
	}
}

void		handle_type_flags(char **format, t_fhandler *handler)
{
	int		inc;

	inc = 0;
	while (**format != 's' && **format != 'd')
	{
		if (**format == 'h' && !others_type_flags(handler, 0) && inc < 2)
			handler->type_flags[TYPE_H] += 1;
		else if (**format == 'l' && !others_type_flags(handler, 1) && inc < 2)
			handler->type_flags[TYPE_L] += 1;
		else if (**format == 'j' && !inc)
			handler->type_flags[TYPE_J] += 1;
		else if (**format == 'z' && !inc)
			handler->type_flags[TYPE_Z] += 1;
		else
			exit(-1);
		(*format)++;
		inc++;
	}
}

void		handle_field(char **format, t_fhandler *handler)
{
	if (**format == '0')
		exit(-1);
	handler->field = atoi(*format);
	while ((**format >= '0' && **format <= '9'))
		(*format)++;
}

void		handle_precision(char **format, t_fhandler *handler)
{
	int		nb;

	if (**format == '.')
		(*format)++;
	nb = ft_atoi(*format);
	if (nb < 0)
		nb = 0;
	while (**format == '-' || (**format >= '0' && **format <= '9'))
		(*format)++;
	handler->precision = nb;
}

void		handle_option(char **format, t_fhandler *handler, va_list *ap)
{
	char	*tmp;

	if (**format == 's')
		ft_putstr(va_arg(*ap, char *));
	else if (**format == 'd')
	{
		if (handler->type_flags[TYPE_L] == 1)
			tmp = ft_ltoa(va_arg(*ap, long int));
		else if (handler->type_flags[TYPE_L] > 1)
			tmp = ft_lltoa(va_arg(*ap, long long int));
		else
			tmp = ft_itoa(va_arg(*ap, int));

		handler->precision -= ft_strlen(tmp);
		if (handler->precision > 0)
			handler->field -= handler->precision + ft_strlen(tmp);
		else
			handler->field -= ft_strlen(tmp);
		if (!handler->format_flags[FORMAT_MINUS])
		{
			if (handler->field > 0)
				ft_putnchar(' ', handler->field);
		}

		
		if (handler->precision > 0)
			ft_putnchar('0', handler->precision);

		ft_putstr(tmp);

		if (handler->format_flags[FORMAT_MINUS])
		{
			if (handler->field > 0)
				ft_putnchar(' ', handler->field);
		}

		free(tmp);
	}
	else if (**format == '%' && !others_flags(handler))
		ft_putchar('%');
	else
		exit(-1);
	(*format)++;
}

void		set_handler(t_fhandler *handler)
{
	int		i;

	i = 0;
	while (i < NB_FORMAT_FLAGS)
	{
		handler->format_flags[i] = 0;
		i++;
	}
	i = 0;
	while (i < NB_TYPE_FLAGS)
	{
		handler->type_flags[i] = 0;
		i++;
	}
	handler->field = 0;
	handler->precision = 0;
}

void		print_flags(t_fhandler *handler)
{
	int		i;

	i = 0;
	while (i < NB_FORMAT_FLAGS)
	{
		printf("Format flag[%d] : %d\n", i, handler->format_flags[i]);
		i++;
	}
	i = 0;
	while (i < NB_TYPE_FLAGS)
	{
		printf("Type flag[%d] : %d\n", i, handler->type_flags[i]);
		i++;
	}
}

int			ft_printf(char *format, ...)
{
	va_list		ap;
	char		*ptr;
	t_fhandler	handler;

	va_start(ap, format);
	while ((ptr = ft_strchr(format, '%')) != NULL)
	{
		ft_putnstr(format, ptr - format);
		format = ptr + 1;
		set_handler(&handler);
		handle_format_flags(&format, &handler);
		handle_field(&format, &handler);
		handle_precision(&format, &handler);
		handle_type_flags(&format, &handler);
		handle_option(&format, &handler, &ap);
	}
	va_end(ap);
	ft_putstr(format);
	return (0);
}

#include <locale.h>

int			main(void)
{
	char	test[] = "(blabla)";
	long long int		bonjour = 127;

	ft_printf("bonjour ceci est %s (%50.5lld) un test.\n", test, bonjour);
	printf("bonjour ceci est %s (%50.5lld) un test.\n", test, bonjour);
	return (0);
}
