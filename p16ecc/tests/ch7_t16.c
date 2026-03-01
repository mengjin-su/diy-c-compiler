typedef struct {
    int x, y, z;
    int *p;
} Data;

Data data;

int foo(Data *dp)
{
    dp->y   = data.x;
    *(dp->p) = 10;
    *data.p = 0;
}