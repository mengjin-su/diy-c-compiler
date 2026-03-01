int callee(int x)
{
    return x + 1;
}

void caller()
{
    int n, r;
    r = callee(n);
}