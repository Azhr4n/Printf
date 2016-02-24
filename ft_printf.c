
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

char		*itob(LLINT value, const char *base, int len)
{
	char	*ret;
	char	*tmp;

	tmp = (char *)malloc(sizeof(char));
	tmp[0] = 0;
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
	while (**format != 's' && **format != 'S' && **format != 'p'
		&& **format != 'd' && **format != 'i' && **format != 'D'
		&& **format != 'u' && **format != 'U' && **format != 'o'
		&& **format != 'O' && **format != 'x' && **format != 'X'
		&& **format != 'c' && **format != 'C')
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

void		get_type(t_fhandler *handler, va_list *ap, char **tmp)
{
	if (handler->type_flags[TYPE_L] == 1)
		*tmp = ft_ltoa(va_arg(*ap, long int));
	else if (handler->type_flags[TYPE_L] > 1)
		*tmp = ft_lltoa(va_arg(*ap, LLINT));
	else
		*tmp = ft_itoa(va_arg(*ap, int));
}

void		put_format(t_fhandler *handler, char *tmp)
{
	if (*tmp != '-' && handler->format_flags[FORMAT_PLUS])
	{
		handler->field -= 1;
		ft_putchar('+');
	}
	else if (*tmp != '-' && handler->format_flags[FORMAT_SPACE])
	{
		handler->field -= 1;
		ft_putchar(' ');
	}

	handler->precision -= ft_strlen(tmp);
	if (handler->precision > 0)
		handler->field -= handler->precision + ft_strlen(tmp);
	else
		handler->field -= ft_strlen(tmp);
	if (!handler->format_flags[FORMAT_MINUS])
	{
		if (handler->field > 0)
		{
			if (handler->format_flags[FORMAT_ZERO])
				ft_putnchar('0', handler->field);
			else
				ft_putnchar(' ', handler->field);
		}
	}

	if (handler->precision > 0)
		ft_putnchar('0', handler->precision);

	ft_putstr(tmp);

	if (handler->format_flags[FORMAT_MINUS])
	{
		if (handler->field > 0)
		{
			if (handler->format_flags[FORMAT_ZERO])
				ft_putnchar('0', handler->field);
			else
				ft_putnchar(' ', handler->field);
		}
	}
	free(tmp);
}

void		handle_option(char **format, t_fhandler *handler, va_list *ap)
{
	char	*tmp;
	char	hbase[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
		'B', 'C', 'D', 'E', 'F'};
	char	obase[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

	if (**format == 's')
	{
		int		len;

		tmp = va_arg(*ap, char *);
		len = ft_strlen(tmp);
		if (!handler->format_flags[FORMAT_MINUS] && handler->field - len > 0)
			ft_putnchar(' ', handler->field - len);
		ft_putstr(tmp);
		if (handler->format_flags[FORMAT_MINUS] && handler->field - len > 0)
			ft_putnchar(' ', handler->field - len);
	}
	else if (**format == 'S')
	{
		int		len2;
		wchar_t	*tmp2;

		if (others_type_flags(handler, -1))
			exit(-1);
		tmp2 = va_arg(*ap, wchar_t *);
		len2 = ft_wcslen(tmp2);
		if (!handler->format_flags[FORMAT_MINUS] && handler->field - len2 > 0)
			ft_putnchar(' ', handler->field - len2);
		ft_putwcstr(tmp2);
		if (handler->format_flags[FORMAT_MINUS] && handler->field - len2 > 0)
			ft_putnchar(' ', handler->field - len2);
	}
	else if (**format == 'p')
	{
		ft_putstr("0x");
		tmp = itob((int)va_arg(*ap, int *), hbase, 16);
		ft_putstr(tmp);
		free(tmp);
	}
	else if (**format == 'd' || **format == 'i')
	{
		get_type(handler, ap, &tmp);
		put_format(handler, tmp);
	}
	else if (** format == 'D')
	{
		if (others_type_flags(handler, -1))
			exit(-1);

		tmp = ft_ltoa(va_arg(*ap, long int));
		put_format(handler, tmp);
	}
	else if (**format == 'o')
	{
		get_type(handler, ap, &tmp);
		tmp = itob((LLINT)va_arg(*ap, unsigned int), obase, 8);
		put_format(handler, tmp);
	}		
	else if (**format == 'O')
	{
		if (others_type_flags(handler, -1))
			exit(-1);
		tmp = itob((LLINT)va_arg(*ap, long unsigned int), obase, 8);
		put_format(handler, tmp);
	}
	else if (**format == 'u')
	{
		get_type(handler, ap, &tmp);
		tmp = ft_lltoa((LLINT)va_arg(*ap, unsigned int));
		put_format(handler, tmp);
	}
	else if (**format == 'U')
	{
		if (others_type_flags(handler, -1))
			exit(-1);
		tmp = ft_lltoa((LLINT)va_arg(*ap, long unsigned int));
		put_format(handler, tmp);
	}
	else if (**format == 'x')
	{
		char	min_hbase[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f'};

		get_type(handler, ap, &tmp);
		tmp = itob((LLINT)va_arg(*ap, unsigned int), min_hbase, 16);
		put_format(handler, tmp);
	}
	else if (**format == 'X')
	{
		get_type(handler, ap, &tmp);
		tmp = itob((LLINT)va_arg(*ap, unsigned int), hbase, 16);
		put_format(handler, tmp);
	}
	else if (**format == 'c')
	{
		if (!handler->format_flags[FORMAT_MINUS] && handler->field - 1 > 0)
			ft_putnchar(' ', handler->field - 1);
		ft_putchar(va_arg(*ap, int));
		if (handler->format_flags[FORMAT_MINUS] && handler->field - 1 > 0)
			ft_putnchar(' ', handler->field - 1);
	}
	else if (**format == 'C')
	{
		if (others_type_flags(handler, -1))
			exit(-1);
		if (!handler->format_flags[FORMAT_MINUS] && handler->field - 1 > 0)
			ft_putnchar(' ', handler->field - 1);
		ft_putwchar((wchar_t)va_arg(*ap, int));
		if (handler->format_flags[FORMAT_MINUS] && handler->field - 1 > 0)
			ft_putnchar(' ', handler->field - 1);
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
	//char	test[] = "(blabla)";
	int		bonjour = 175;
	wchar_t	*hey = L"αααӜӜӜӜ";

	//wchar_t	h = 945;

	setlocale(LC_ALL, "");

	ft_printf("bonjour ceci est *%15S* (%-5d) un test.\n", hey, bonjour);
	printf("bonjour ceci est *%15ls* (%-5d) un test.\n", hey, bonjour);
	return (0);
}
