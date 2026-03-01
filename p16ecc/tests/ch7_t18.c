struct _Data {
	int a, b, c, d, e;
};

void foo(struct _Data *p)
{
    p->a = (p->b * p->c * p->d) + p->e;
}