void func(int);

void foo(int x,
         int (*f)(int))
{
    func(x);
    f(x);
}