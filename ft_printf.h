
#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define LLINT long long int
# define LLUINT long long unsigned int

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

typedef struct	s_handler
{
	int			format_flags[NB_FORMAT_FLAGS];
	int			type_flags[NB_TYPE_FLAGS];
	int			field;
	int			precision;
	int			ppoint;
	int			count;
}				t_handler;

#endif