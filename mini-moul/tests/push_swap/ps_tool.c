/*
** Helper for the push_swap checks; it is not student code.
**
**   ps_tool gen <count> <seed> [extremes]
**       count distinct integers, space separated, from a fixed seed. With
**       "extremes", INT_MIN and INT_MAX are among them.
**   ps_tool perms <count>
**       every permutation of a fixed set of count integers, one per line.
**   ps_tool solve <integer...>
**       the shortest list of instructions (one per line) that sorts up to 6
**       integers, found by a breadth-first search.
**   ps_tool sim <integer...>
**       reads instructions from stdin and applies them to stack a (the
**       integers, first one on top) and an empty stack b. Prints one line:
**         OK <count>        a is sorted and b is empty
**         KO <count>        it is not
**         BAD <reason>      an instruction does not exist or is not
**                           followed by a newline
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long	g_state;

static unsigned int	next_random(void)
{
	g_state ^= g_state << 13;
	g_state ^= g_state >> 7;
	g_state ^= g_state << 17;
	return ((unsigned int)(g_state >> 16));
}

static int	contains(const int *values, int count, int value)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (values[i] == value)
			return (1);
		i++;
	}
	return (0);
}

static int	gen(int count, unsigned long long seed, int extremes)
{
	int	*values;
	int	n;
	int	value;

	values = malloc(sizeof(int) * (count + 2));
	if (values == NULL)
		return (1);
	g_state = seed * 2654435761ULL + 88172645463325252ULL;
	n = 0;
	if (extremes && count >= 2)
	{
		values[n++] = INT_MIN;
		values[n++] = INT_MAX;
	}
	while (n < count)
	{
		/* a mix of small values and the whole int range */
		if (next_random() % 2)
			value = (int)(next_random() % 2001) - 1000;
		else
			value = (int)next_random();
		if (!contains(values, n, value))
			values[n++] = value;
	}
	n = 0;
	while (n < count)
	{
		printf(n ? " %d" : "%d", values[n]);
		n++;
	}
	printf("\n");
	free(values);
	return (0);
}

static void	permute(int *values, int n, int k)
{
	int	i;
	int	tmp;

	if (k == n)
	{
		i = 0;
		while (i < n)
		{
			printf(i ? " %d" : "%d", values[i]);
			i++;
		}
		printf("\n");
		return ;
	}
	i = k;
	while (i < n)
	{
		tmp = values[k];
		values[k] = values[i];
		values[i] = tmp;
		permute(values, n, k + 1);
		tmp = values[k];
		values[k] = values[i];
		values[i] = tmp;
		i++;
	}
}

static int	perms(int count)
{
	static const int	pool[] = {-3, 17, INT_MIN, 0, INT_MAX, 42};
	int					values[6];
	int					i;

	if (count < 1 || count > 6)
		return (1);
	i = 0;
	while (i < count)
	{
		values[i] = pool[i];
		i++;
	}
	permute(values, count, 0);
	return (0);
}

typedef struct s_stack
{
	long	*v;
	int		size;
}	t_stack;

static void	swap_top(t_stack *s)
{
	long	tmp;

	if (s->size < 2)
		return ;
	tmp = s->v[0];
	s->v[0] = s->v[1];
	s->v[1] = tmp;
}

static void	rotate(t_stack *s)
{
	long	first;
	int		i;

	if (s->size < 2)
		return ;
	first = s->v[0];
	i = 0;
	while (++i < s->size)
		s->v[i - 1] = s->v[i];
	s->v[s->size - 1] = first;
}

static void	reverse_rotate(t_stack *s)
{
	long	last;
	int		i;

	if (s->size < 2)
		return ;
	last = s->v[s->size - 1];
	i = s->size;
	while (--i > 0)
		s->v[i] = s->v[i - 1];
	s->v[0] = last;
}

static void	push(t_stack *from, t_stack *to)
{
	int	i;

	if (from->size == 0)
		return ;
	i = to->size;
	while (i-- > 0)
		to->v[i + 1] = to->v[i];
	to->v[0] = from->v[0];
	to->size++;
	i = 0;
	while (++i < from->size)
		from->v[i - 1] = from->v[i];
	from->size--;
}

static int	apply(const char *op, t_stack *a, t_stack *b)
{
	if (!strcmp(op, "sa") || !strcmp(op, "ss"))
		swap_top(a);
	if (!strcmp(op, "sb") || !strcmp(op, "ss"))
		swap_top(b);
	if (!strcmp(op, "ra") || !strcmp(op, "rr"))
		rotate(a);
	if (!strcmp(op, "rb") || !strcmp(op, "rr"))
		rotate(b);
	if (!strcmp(op, "rra") || !strcmp(op, "rrr"))
		reverse_rotate(a);
	if (!strcmp(op, "rrb") || !strcmp(op, "rrr"))
		reverse_rotate(b);
	if (!strcmp(op, "pa"))
		push(b, a);
	if (!strcmp(op, "pb"))
		push(a, b);
	return (strcmp(op, "sa") && strcmp(op, "sb") && strcmp(op, "ss")
		&& strcmp(op, "pa") && strcmp(op, "pb") && strcmp(op, "ra")
		&& strcmp(op, "rb") && strcmp(op, "rr") && strcmp(op, "rra")
		&& strcmp(op, "rrb") && strcmp(op, "rrr"));
}

static int	is_sorted(const t_stack *s)
{
	int	i;

	i = 1;
	while (i < s->size)
	{
		if (s->v[i - 1] > s->v[i])
			return (0);
		i++;
	}
	return (1);
}

typedef struct s_node
{
	long	a[6];
	long	b[6];
	int		asize;
	int		bsize;
	int		parent;
	int		op;
}	t_node;

static const char	*g_ops[11] = {"sa", "sb", "ss", "pa", "pb", "ra", "rb",
	"rr", "rra", "rrb", "rrr"};

static int	same_state(const t_node *x, const t_node *y)
{
	return (x->asize == y->asize && x->bsize == y->bsize
		&& !memcmp(x->a, y->a, sizeof(x->a))
		&& !memcmp(x->b, y->b, sizeof(x->b)));
}

static int	node_sorted(const t_node *n)
{
	t_stack	a;

	a.v = (long *)n->a;
	a.size = n->asize;
	return (n->bsize == 0 && is_sorted(&a));
}

static int	solve(int argc, char **argv)
{
	t_node	*nodes;
	int		count;
	int		head;
	int		i;
	int		k;
	t_node	next;
	t_stack	a;
	t_stack	b;
	long	av[8];
	long	bv[8];

	if (argc < 1 || argc > 6)
		return (1);
	nodes = calloc(6000, sizeof(t_node));
	if (nodes == NULL)
		return (1);
	nodes[0].asize = argc;
	i = -1;
	while (++i < argc)
		nodes[0].a[i] = atol(argv[i]);
	nodes[0].parent = -1;
	count = 1;
	head = 0;
	while (head < count && !node_sorted(&nodes[head]))
	{
		i = -1;
		while (++i < 11)
		{
			memset(av, 0, sizeof(av));
			memset(bv, 0, sizeof(bv));
			memcpy(av, nodes[head].a, sizeof(nodes[head].a));
			memcpy(bv, nodes[head].b, sizeof(nodes[head].b));
			a.v = av;
			a.size = nodes[head].asize;
			b.v = bv;
			b.size = nodes[head].bsize;
			apply(g_ops[i], &a, &b);
			k = a.size;
			while (k < 8)
				av[k++] = 0;
			k = b.size;
			while (k < 8)
				bv[k++] = 0;
			memset(&next, 0, sizeof(next));
			memcpy(next.a, av, sizeof(next.a));
			memcpy(next.b, bv, sizeof(next.b));
			next.asize = a.size;
			next.bsize = b.size;
			next.parent = head;
			next.op = i;
			k = 0;
			while (k < count && !same_state(&nodes[k], &next))
				k++;
			if (k == count && count < 6000)
				nodes[count++] = next;
		}
		head++;
	}
	if (head == count)
		return (1);
	count = 0;
	k = head;
	while (nodes[k].parent >= 0)
	{
		av[count++] = nodes[k].op;
		k = nodes[k].parent;
	}
	while (count-- > 0)
		printf("%s\n", g_ops[av[count]]);
	free(nodes);
	return (0);
}

static int	sim(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;
	char	line[256];
	long	count;
	int		i;
	size_t	len;

	a.v = malloc(sizeof(long) * (argc + 1));
	b.v = malloc(sizeof(long) * (argc + 1));
	if (a.v == NULL || b.v == NULL)
		return (1);
	a.size = argc;
	b.size = 0;
	i = 0;
	while (i < argc)
	{
		a.v[i] = atol(argv[i]);
		i++;
	}
	count = 0;
	while (fgets(line, sizeof(line), stdin))
	{
		len = strlen(line);
		if (len == 0 || line[len - 1] != '\n')
		{
			printf("BAD instruction %ld is not followed by a newline\n", count + 1);
			return (0);
		}
		line[len - 1] = '\0';
		if (apply(line, &a, &b))
		{
			printf("BAD instruction %ld is \"%s\"\n", count + 1, line);
			return (0);
		}
		count++;
	}
	printf("%s %ld\n", (is_sorted(&a) && b.size == 0) ? "OK" : "KO", count);
	free(a.v);
	free(b.v);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc >= 4 && !strcmp(argv[1], "gen"))
		return (gen(atoi(argv[2]), strtoull(argv[3], NULL, 10),
				argc > 4 && !strcmp(argv[4], "extremes")));
	if (argc == 3 && !strcmp(argv[1], "perms"))
		return (perms(atoi(argv[2])));
	if (argc >= 2 && !strcmp(argv[1], "solve"))
		return (solve(argc - 2, argv + 2));
	if (argc >= 2 && !strcmp(argv[1], "sim"))
		return (sim(argc - 2, argv + 2));
	fprintf(stderr, "usage: ps_tool gen|perms|sim ...\n");
	return (2);
}
