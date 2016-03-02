/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pivanovi <pivanovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/11 16:23:19 by pivanovi          #+#    #+#             */
/*   Updated: 2016/01/11 16:23:20 by pivanovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>

int			ft_atoi(const char *str);
void		ft_bzero(void *s, size_t n);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
char		*ft_itoa(int n);
char		*ft_ltoa(long int n);
char		*ft_lltoa(long long int n);
char		*ft_uimtoa(uintmax_t n);
char		*ft_imtoa(intmax_t n);
void		*ft_memalloc(size_t size);
void		*ft_memccpy(void *dst, const void *src, int c, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		ft_memdel(void **ap);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memset(void *b, int c, size_t len);
void		ft_putchar(char c);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl(const char *s);
void		ft_putendl_fd(const char *s, int fd);
void		ft_putnbr(int n);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr(const char *s);
void		ft_putstr_fd(const char *s, int fd);
char		*ft_strcat(char *dest, const char *src);
char		*ft_strchr(const char *s, int c);
void		ft_strclr(char *s);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strcpy(char *dest, const char *src);
void		ft_strdel(char **as);
char		*ft_strdup(const char *s);
int			ft_strequ(char const *s1, char const *s2);
void		ft_striter(char *s, void (*f)(char *));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *s);
char		*ft_strmap(char const *s, char (*f)(char));
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		*ft_strncat(char *dest, const char *src, size_t n);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strncpy(char *dest, const char *str, size_t n);
int			ft_strnequ(char const *s1, char const *s2, size_t n);
char		*ft_strnew(size_t size);
char		*ft_strnstr(const char *big, const char *little, size_t len);
char		*ft_strrchr(const char *s, int c);
char		**ft_strsplit(char const *s, char c);
char		*ft_strstr(const char *big, const char *little);
char		*ft_strsub(char const *s, unsigned int start, size_t len);
char		*ft_strtrim(char const *s);
int			ft_tolower(int c);
int			ft_toupper(int c);
size_t		ft_strlcat(char *dst, const char *src, size_t size);

#endif
