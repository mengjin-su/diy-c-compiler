int x, y;
char a, b;

void foo()
{
    a = b << b;
    x = y << 9;
    x = (x << 9) + 1;
    x = 100 << y;
}