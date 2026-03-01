typedef struct {
    char a: 3;
    char b: 2;
    char c: 3;
} Data;

Data data;
char x;

void foo()
{
    if ( data.c == 7 )
        x++;
}