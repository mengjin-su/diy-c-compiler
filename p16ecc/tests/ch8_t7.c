typedef struct {
    int x[4];
    int y[4];
} Data;

Data a, b;

void foo()
{
    a = b;
}