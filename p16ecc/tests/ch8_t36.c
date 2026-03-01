typedef struct {
    char a: 3;
    char b: 2;
    char c: 3;
} Data;

Data data;

void foo()
{
    data.a |= 5;
    data.b &= 1;
    data.c ^= 6;
}