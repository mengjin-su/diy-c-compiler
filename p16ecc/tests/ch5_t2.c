int foo(int n)
{
#define SEED 10
    if ( n ) {
        enum{V1=1,V2,V3};
        n = V3 + SEED; 
    }
    else  {
        enum{V1=101,V2,V3};
        n = V3 + SEED; 
    }
    return n;
}
