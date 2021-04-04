#include<stdio.h>
#define a (x+1)
int x = 2; 
void b() { int x = 1; printf("b: %d\n", a);}
void c() { printf("c: %d\n", a);}
int main() {
    b();
    c();
    return 0;
}
