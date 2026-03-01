typedef struct {
    int  x;
    char y;
} Data;

const Data data = {1000, 100};
const Data *const dptr = &data;

void foo()
{
    int  n;
    char m;
    n = dptr->x;
    m = dptr->y;
}