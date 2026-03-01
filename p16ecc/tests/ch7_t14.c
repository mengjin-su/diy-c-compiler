typedef struct {
    int x;
    int a[20];
} Data;

int foo(Data *dp)
{
    int n, m;
    n = m + 0;
    n = m * 0;
    n *= 1;
    n += 1;
    return dp->x;
}