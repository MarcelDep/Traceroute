/******************************************************************************

Ten plik pokazuje jak struktury z C reprezentowane są w pamięci komputera.

*******************************************************************************/

#include <stdio.h>

typedef struct  {
    int a;  // 4 bajty
    short b;  // 2 bajty
    char c;  // 1 bajty
    // uwaga! 1 bajt PADDING'U
} Foo; // łącznie 8 bajtów

typedef struct  {
    char c;  // 1 bajty
    // uwaga! 1 bajt PADDING'U
    short b;  // 2 bajty
    int a;  // 4 bajty
} Bar; // łącznie 7 bajtów

int main()
{
    char bytes[7] = {
        1, 0, 0, 0, 
        2, 0,
        3
    };
    Foo foo = {
        .a = 1, .b = 2, .c = 3
    };
    
    Foo* pointer_to_foo = &foo;
    
    printf("sizeof Foo %lu\n", sizeof(Foo));
    printf("sizeof Bar %lu\n", sizeof(Bar));
    
    printf("adres .a %p\n", &foo.a);
    printf("adres .b %p\n", &foo.b);
    printf("adres .c %p\n", &foo.c);
    
    printf("1. { ");
    for(int i = 0; i < 7; i++) {
        printf("%d, ", bytes[i]);
    };
    printf("}\n");
    printf("2. { ");
    for(int i = 0; i < 7; i++) {
        printf("%d, ", ((char*)&foo)[i]);
    };
    printf("}\n");
    
    printf("foo { .a = %d, .b = %d, .c = %d }\n", (*pointer_to_foo).a, (*pointer_to_foo).b, (*pointer_to_foo).c);
    
    pointer_to_foo = (Foo*) bytes;
    
    printf("foo { .a = %d, .b = %d, .c = %d }\n", (*pointer_to_foo).a, (*pointer_to_foo).b, (*pointer_to_foo).c);
    
    return 0;
}