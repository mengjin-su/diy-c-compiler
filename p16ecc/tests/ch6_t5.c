struct Data {
    int  x, y;
    char a[10];
};

void foo(struct Data *dp)
{
    dp->x = 0;
    ((struct Data *)0x100)->y = 0;
}