typedef struct {
    int  x, y;
    char a[10];
} Data;

Data d, *dp;

void foo()
{
    d.y = 0;
//  dp[0].x = 0;
    (*dp).y = 0;
}