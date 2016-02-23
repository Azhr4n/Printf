
#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define LLINT long long int

enum
{
	FORMAT_HASHTAG,
	FORMAT_ZERO,
	FORMAT_MINUS,
	FORMAT_PLUS,
	FORMAT_SPACE,
	NB_FORMAT_FLAGS
};

enum
{
	TYPE_H,
	TYPE_L,
	TYPE_J,
	TYPE_Z,
	NB_TYPE_FLAGS
};

typedef struct	s_fhandler
{
	int			format_flags[NB_FORMAT_FLAGS];
	int			field;
	int			precision;
	int			type_flags[NB_TYPE_FLAGS];
}				t_fhandler;

#endif