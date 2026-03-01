int x, y;
char a;

void foo()
{
    x = y >> 9;
    a = y >> 1;
    a = x >> 9;
}