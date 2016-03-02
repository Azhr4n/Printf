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
		if (**format == '#')
			handler->format_flags[FORMAT_HASHTAG] = 1;
		else if (**format == '0')
			handler->format_flags[FORMAT_ZERO] = 1;
		else if (**format == '-')
			handler->format_flags[FORMAT_MINUS] = 1;
		else if (**format == '+')
			handler->format_flags[FORMAT_PLUS] = 1;
		else if (**format == ' ' && !handler->format_flags[FORMAT_SPACE])
			handler->format_flags[FORMAT_SPACE] = 1;
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

void		hashtag(t_handler *handler, char conv)
{
	if (conv == 'x')
	{
		ft_putstr("0x");
		handler->count += 2;
	}
	else if (conv == 'X')
	{
		ft_putstr("0X");
		handler->count += 2;
	}
	else if (conv == 'o')
	{
		ft_putchar('0');
		handler->count++;
	}
	handler->order_flag[O_HASHTAG] = 0;
}

void		zero(t_handler *handler)
{
	ft_putnchar('0', handler->field);
	handler->count += handler->field;
	handler->order_flag[O_ZERO] = 0;
}

void		space(t_handler *handler, int n, int flag)
{
	if (!flag || (flag && handler->field > 0))
	{
		ft_putnchar(' ', n);
		handler->count += n;
	}
	if (!flag)
		handler->order_flag[O_SPACE] = 0;
	else
		handler->order_flag[O_FIELD] = 0;
}

void		plus(t_handler *handler, char first_c)
{
	if (first_c != '-')
	{
		ft_putchar('+');
		handler->count++;
	}
	handler->order_flag[O_PLUS] = 0;
}

void		precision(t_handler *handler, char conv)
{
	if ((conv == 'd' || conv == 'D' || conv == 'i' 
		|| conv == 'o' || conv == 'O' || conv == 'u'
		|| conv == 'U' || conv == 'x' || conv == 'X')
		&& handler->precision > 0)
	{
		ft_putnchar('0', handler->precision);
		handler->count += handler->precision;
	}
	handler->order_flag[O_PRECISION] = 0;
}

void		print(t_handler *handler, char *str, char conv)
{
	if (*str != 0)
	{
		if ((conv == 's' || conv == 'S') && handler->ppoint)
		{
			ft_putnstr(str, handler->precision);
			handler->count += handler->precision;
		}
		else
		{
			ft_putstr(str);
			handler->count += handler->str_len;
		}	
	}
	handler->order_flag[O_STRING] = 0;
}

int			get_next_index(int *tab, int len)
{
	int			i;
	int			index;
	static int	next = 0;

	next++;
	index = -1;
	i = 0;
	while (i < len)
	{
		if (tab[i] <= next && tab[i] > 0)
		{
			index = i;
			next = tab[i];
		}
		i++;
	}
	return (index);
}

void		get_field(t_handler *handler, char conv)
{
	

	if ((conv == 'x' || conv == 'X') && handler->order_flag[O_HASHTAG])
		handler->field -= 2;
	else if ((conv == 'o' || conv == 'O') && handler->order_flag[O_HASHTAG])
		handler->field--;
	if (handler->order_flag[O_PLUS])
		handler->field--;
	else if (handler->order_flag[O_SPACE])
		handler->field--;
	if ((conv == 'd' || conv == 'D' || conv == 'i' || conv == 'o' || conv == 'O' 
		|| conv == 'u' || conv == 'U' || conv == 'x' || conv == 'X')
		&& handler->ppoint && handler->precision > 0)
	{
		if (handler->str_len < handler->precision)
			handler->field -= handler->precision + (handler->precision - handler->str_len);
		else
			handler->field -= handler->str_len;
		handler->precision -= handler->str_len;
	}
	if ((conv == 's' || conv == 'S') && handler->ppoint 
		&& handler->precision >= 0 && handler->str_len > 0)
		handler->field -= handler->precision;
	else if ((handler->ppoint && handler->precision > 0) || !handler->ppoint)
		handler->field -= handler->str_len;
}

void		upgrade_priority(t_handler *handler, int flag)
{
	int		i;

	i = 0;
	while (i < NB_O_FLAGS)
	{
		if (handler->order_flag[i] != 0 || i == flag)
			handler->order_flag[i]++;
		i++;
	}
}

void		set_priority(t_handler *handler, char conv, char first_c)
{
	if (handler->field > 0 && handler->format_flags[FORMAT_MINUS])
		upgrade_priority(handler, O_FIELD);
	if ((conv != 's' || conv != 'S') && first_c == '0' && handler->ppoint
		&& handler->precision <= 0)
		handler->order_flag[O_STRING] = 0;
	else
		handler->order_flag[O_STRING]++;
	if (handler->ppoint && handler->precision > 0)
		upgrade_priority(handler, O_PRECISION);
	if (handler->format_flags[FORMAT_PLUS] && (conv == 'd' || conv == 'D') 
		&& first_c != '-')
		upgrade_priority(handler, O_PLUS);
	if (handler->format_flags[FORMAT_SPACE] && !handler->order_flag[O_PLUS] 
		&& (conv == 'd' || conv == 'D'))
		upgrade_priority(handler, O_SPACE);
	if (handler->format_flags[FORMAT_ZERO] && !handler->ppoint 
		&& !handler->format_flags[FORMAT_MINUS])
		upgrade_priority(handler, O_ZERO);
	if (handler->format_flags[FORMAT_HASHTAG] && (conv == 'x' || conv == 'X'
		|| conv == 'o' || conv == 'O') && first_c != '0')
		upgrade_priority(handler, O_HASHTAG);
	if (handler->field > 0 && !handler->format_flags[FORMAT_MINUS] 
		&& !handler->format_flags[FORMAT_ZERO])
		upgrade_priority(handler, O_FIELD);
}

void		print_in_order(t_handler *handler, char *conv, char *str)
{
	int		val;

	while ((val = get_next_index(handler->order_flag, NB_O_FLAGS)) != -1)
	{
		if (val == O_HASHTAG)
			hashtag(handler, *conv);
		else if (val == O_ZERO)
			zero(handler);
		else if (val == O_SPACE)
			space(handler, 1, 0);
		else if (val == O_PLUS)
			plus(handler, '+');
		else if (val == O_PRECISION)
			precision(handler, *conv);
		else if (val == O_FIELD)
			space(handler, handler->field, 1);
		else if (val == O_STRING)
			print(handler, str, *conv);
	}
}

intmax_t	get_signed_value(t_handler *handler, va_list *ap)
{
	intmax_t	ret;

	if (handler->type_flags[TYPE_H] == 1)
		ret = (short int)va_arg(*ap, int);
	else if (handler->type_flags[TYPE_H] == 2)
		ret = (char)va_arg(*ap, int);
	else if (handler->type_flags[TYPE_L] == 1)
		ret = (long int)va_arg(*ap, long int);
	else if (handler->type_flags[TYPE_L] == 2)
		ret = (long long int)va_arg(*ap, long long int);
	else if (handler->type_flags[TYPE_Z] == 1)
		ret = (ssize_t)va_arg(*ap, ssize_t);
	else if (handler->type_flags[TYPE_J] == 1)
		ret = va_arg(*ap, intmax_t);
	else
		ret = va_arg(*ap, int);
	return (ret);
}

uintmax_t	get_unsigned_value(t_handler *handler, va_list *ap)
{
	uintmax_t	ret;

	if (handler->type_flags[TYPE_H] == 1)
		ret = (short unsigned int)va_arg(*ap, unsigned int);
	else if (handler->type_flags[TYPE_H] == 2)
		ret = (unsigned char)va_arg(*ap, unsigned int);
	else if (handler->type_flags[TYPE_L] == 1)
		ret = (long unsigned int)va_arg(*ap, long unsigned int);
	else if (handler->type_flags[TYPE_L] == 2)
		ret = (long long unsigned int)va_arg(*ap, long long unsigned int);
	else if (handler->type_flags[TYPE_Z] == 1)
		ret = (size_t)va_arg(*ap, size_t);
	else if (handler->type_flags[TYPE_J] == 1)
		ret = va_arg(*ap, uintmax_t);
	else
		ret = va_arg(*ap, unsigned int);
	return (ret);
}

void		print_order(t_handler *handler)
{
	int			i;

	i = 0;
	while (i < NB_O_FLAGS)
	{
		printf("flag[%d] : %d\n", i, handler->order_flag[i]);
		i++;
	}
}

void		percent(t_handler *handler, char *conv)
{
	if (*conv == '%')
	{
		handler->str_len = 1;
		set_priority(handler, *conv, '%');
		get_field(handler, *conv);
		print_in_order(handler, conv, "%");
	}
}

void		last_step(t_handler *handler, char *conv, char *str)
{
	set_priority(handler, *conv, *str);
	get_field(handler, *conv);
	print_in_order(handler, conv, str);
}

void		x_minus(t_handler *handler, char *conv, va_list *ap)
{
	char		*str;

	if (*conv == 'x')
	{
		str = uitob(get_unsigned_value(handler, ap),
			"0123456789abcdef", 16);
		handler->str_len = ft_strlen(str);
		last_step(handler, conv, str);
	}
}

void		x_majus(t_handler *handler, char *conv, va_list *ap)
{
	char		*str;

	if (*conv == 'X')
	{
		str = uitob(get_unsigned_value(handler, ap),
			"0123456789ABCDEF", 16);
		handler->str_len = ft_strlen(str);
		last_step(handler, conv, str);
	}
}

void		s_minus(t_handler *handler, char *conv, va_list *ap)
{
	char	*str;

	if (*conv == 's')
	{
		str = va_arg(*ap, char *);

		if (str != NULL)
		{
			handler->str_len = ft_strlen(str);
			last_step(handler, conv, str);	
		}
		else
			ft_putstr("(null)");
	}
}

void		handle_conversion(char **format, t_handler *handler, va_list *ap)
{
	percent(handler, *format);
	x_minus(handler, *format, ap);
	x_majus(handler, *format, ap);
	s_minus(handler, *format, ap);
	(*format)++;
}

void		reset_handler(t_handler *handler)
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
	i = 0;
	while (i < NB_O_FLAGS)
	{
		handler->order_flag[i] = 0;
		i++;
	}
	handler->field = 0;
	handler->precision = 0;
	handler->ppoint = 0;
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
		reset_handler(&handler);
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

// #include <locale.h>

// int			main(void)
// {
// 	//unsigned int	val = 255;
// 	int				val = 1;
// 	//char			*val = "";

// 	t_handler		hand;

// 	reset_handler(&hand);

// 	char	*str = "#+.5x";

// 	handle_format_flags(&str, &hand);
// 	handle_field(&str, &hand);
// 	handle_precision(&str, &hand);
// 	handle_type_flags(&str, &hand);

// 	hand.str_len = 1;
// 	hand.count = 0;

// 	set_priority(&hand, *str, '1');
// 	get_field(&hand, *str, '1');

// 	int i = 0;
// 	while (i < NB_O_FLAGS)
// 	{
// 		printf("val flag : %d\n", hand.order_flag[i]);
// 		i++;
// 	}

// 	ft_putstr("(");
// 	print_in_order(&hand, str, "1");
// 	ft_putstr(")\n");

// 		printf("count : %d\n", hand.count);

// 	printf("(%#+.5x)\n", val);

// 	return (0);
// }
