/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pivanovi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/24 17:24:32 by pivanovi          #+#    #+#             */
/*   Updated: 2016/02/24 17:24:34 by pivanovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>

#include "libft.h"
#include "ft_printf.h"

void		rstr(char *str)
{
	char	tmp;
	int		len;
	int		end;
	int		i;

	len = ft_strlen(str);
	end = len - 1;
	i = 0;
	while (i < len / 2)
	{
		tmp = str[i];
		str[i] = str[end];
		str[end] = tmp;
		i++;
		end--;
	}
}

char		*charjoin(const char *s1, const char c)
{
	char	*ret;
	int		i;

	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (ret == NULL)
		return (ret);
	i = 0;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	ret[i] = c;
	ret[i + 1] = 0;
	return (ret);
}

size_t		ft_wcslen(wchar_t *str)
{
	int		i;

	i = 0;
	while (*str)
	{
		i += 1 * sizeof(*str);
		str++;
	}
	return (i);
}

int			btoi(char *binary)
{
	int		ret;
	int		last;
	int		factor;

	last = ft_strlen(binary) - 1;
	ret = 0;
	if (binary[last] == '1')
		ret = 1;
	factor = 2;
	while (--last >= 0)
	{
		if (binary[last] == '1')
			ret += factor;
		factor *= 2;
	}
	return (ret);
}

char		*itob(intmax_t value, const char *base, int len)
{
	char	*ret;
	char	*tmp;

	tmp = (char *)malloc(sizeof(char));
	tmp[0] = 0;
	if (!value)
		ret = charjoin(tmp, '0');
	while (value > 0)
	{
		ret = charjoin(tmp, base[value % len]);
		free(tmp);
		tmp = ret;
		value /= len;
	}
	rstr(ret);
	return (ret);
}

char		*uitob(uintmax_t value, const char *base, unsigned int len)
{
	char	*ret;
	char	*tmp;

	tmp = (char *)malloc(sizeof(char));
	tmp[0] = 0;
	if (!value)
		ret = charjoin(tmp, '0');
	while (value > 0)
	{
		ret = charjoin(tmp, base[value % len]);
		free(tmp);
		tmp = ret;
		value /= len;
	}
	rstr(ret);
	return (ret);
}

void		fill_and_print(char *mask, int len)
{
	int		print[len / 8];
	char	part[9];
	int		max;
	int		i;

	i = 0;
	while (i < len)
	{
		if (mask[i] == 'x')
			mask[i] = '0';
		i++;
	}
	max = len / 8;
	i = 0;
	while (i < max)
	{
		ft_strncpy(part, mask + 8 * i, 8);
		part[8] = 0;
		print[i] = btoi(part);
		i++;
	}
	i = 0;
	while (i < max)
	{
		write(1, &print[i], 1);
		i++;
	}
}

void		unicode8(char *binary)
{
	char	mask[9];
	int		len;
	int		i;

	ft_strcpy(mask, "0xxxxxxx");
	mask[8] = 0;
	i = ft_strlen(binary) - 1;
	len = 7;
	while (i >= 0)
	{
		if (mask[len] == 'x')
		{
			mask[len] = binary[i];
			i--;
		}
		len--;
	}
	fill_and_print(mask, 8);
}

void		unicode16(char *binary)
{
	char	mask[17];
	int		len;
	int		i;

	ft_strcpy(mask, "110xxxxx10xxxxxx");
	mask[16] = 0;
	i = ft_strlen(binary) - 1;
	len = 15;
	while (i >= 0)
	{
		if (mask[len] == 'x')
		{
			mask[len] = binary[i];
			i--;
		}
		len--;
	}
	fill_and_print(mask, 16);
}

void		unicode24(char *binary)
{
	char	mask[25];
	int		len;
	int		i;

	ft_strcpy(mask, "110xxxxx10xxxxxx");
	mask[24] = 0;
	i = ft_strlen(binary) - 1;
	len = 23;
	while (i >= 0)
	{
		if (mask[len] == 'x')
		{
			mask[len] = binary[i];
			i--;
		}
		len--;
	}
	fill_and_print(mask, 24);
}

void		unicode32(char *binary)
{
	char	mask[33];
	int		len;
	int		i;

	ft_strcpy(mask, "11110xxx10xxxxxx10xxxxxx10xxxxxx");
	mask[32] = 0;
	i = ft_strlen(binary) - 1;
	len = 31;
	while (i >= 0)
	{
		if (mask[len] == 'x')
		{
			mask[len] = binary[i];
			i--;
		}
		len--;
	}
	fill_and_print(mask, 32);
}

void		unicode_magic(char *binary)
{
	int		len;

	len = ft_strlen(binary);
	if (len < 8)
		unicode8(binary);
	else if (len < 12)
		unicode16(binary);
	else if (len < 17)
		unicode24(binary);
	else
		unicode32(binary);
}

void		ft_putwchar(wchar_t c)
{
	unsigned int	val;
	char			*binary;

	val = (unsigned int)c;
	binary = itob(val, "01", 2);
	unicode_magic(binary);
	free(binary);
}

void		ft_putwcstr(wchar_t *str)
{
	while (*str)
	{
		ft_putwchar(*str);
		str++;
	}
}

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

int			others_flags(t_handler *handler)
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

int			others_type_flags(t_handler *handler, int flag)
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

void		handle_format_flags(char **format, t_handler *handler)
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
		(*format)++;
	}
}

void		handle_type_flags(char **format, t_handler *handler)
{
	int		inc;

	inc = 0;
	while (**format != 's' && **format != 'S' && **format != 'p'
		&& **format != 'd' && **format != 'i' && **format != 'D'
		&& **format != 'u' && **format != 'U' && **format != 'o'
		&& **format != 'O' && **format != 'x' && **format != 'X'
		&& **format != 'c' && **format != 'C' && **format != '%')
	{
		if (**format == 'h' && !others_type_flags(handler, 0) && inc < 2)
			handler->type_flags[TYPE_H] += 1;
		else if (**format == 'l' && !others_type_flags(handler, 1) && inc < 2)
			handler->type_flags[TYPE_L] += 1;
		else if (**format == 'j' && !inc)
			handler->type_flags[TYPE_J] += 1;
		else if (**format == 'z' && !inc)
			handler->type_flags[TYPE_Z] += 1;
		(*format)++;
		inc++;
	}
}

void		handle_field(char **format, t_handler *handler)
{
	if (**format == '0')
		exit(-1);
	handler->field = atoi(*format);
	while ((**format >= '0' && **format <= '9'))
		(*format)++;
}

void		handle_precision(char **format, t_handler *handler)
{
	int		nb;

	if (**format == '.')
	{
		handler->ppoint = 1;
		(*format)++;
	}
	nb = ft_atoi(*format);
	if (nb < 0)
		nb = 0;
	while (**format == '-' || (**format >= '0' && **format <= '9'))
		(*format)++;
	handler->precision = nb;
}

void		handle_conversion(char **format, t_handler *handler, va_list *ap)
{
	(void)handler;
	(void)ap;

	if (**format == 's')
	{

	}
	else if (**format == 'S')
	{

	}
	else if (**format == 'p')
	{

	}
	else if (**format == 'd' || **format == 'i')
	{

	}
	else if (**format == 'D')
	{

	}
	else if (**format == 'o')
	{

	}		
	else if (**format == 'O')
	{

	}
	else if (**format == 'u')
	{

	}
	else if (**format == 'U')
	{

	}
	else if (**format == 'x')
	{

	}
	else if (**format == 'X')
	{

	}
	else if (**format == 'c')
	{

	}
	else if (**format == 'C')
	{

	}
	else if (**format == '%')
	{

	}
	(*format)++;
}

void		set_handler(t_handler *handler)
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
	handler->ppoint = 0;
}

void		print_flags(t_handler *handler)
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
	t_handler	handler;

	va_start(ap, format);
	handler.count = 0;
	while ((ptr = ft_strchr(format, '%')) != NULL)
	{
		handler.count += ptr - format;
		ft_putnstr(format, ptr - format);
		format = ptr + 1;
		set_handler(&handler);
		handle_format_flags(&format, &handler);
		handle_field(&format, &handler);
		handle_precision(&format, &handler);
		handle_type_flags(&format, &handler);
		handle_conversion(&format, &handler, &ap);
	}
	va_end(ap);
	handler.count += ft_strlen(format);
	ft_putstr(format);
	return (handler.count);
}

//#include <locale.h>
//
//int			main(void)
//{
//	ft_printf("%x\n", -42);
//	return (0);
//}
