typedef struct {
    char a: 3;
    char b: 2;
    char c: 3;
} Data;

Data data;

void foo()
{
    if ( data.b++ )
        data.c++;
}