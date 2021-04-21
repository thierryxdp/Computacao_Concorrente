#include<stdio.h>

int main(void) {
    FILE *fd_foo, *fd_bar;

    fd_foo = fopen("foo", "w");
    if (!fd_foo) return 1;
    fprintf(fd_foo, "%s", "Ola ");
    fclose(fd_foo);
    
    
    fd_bar = fopen("bar", "a+");
    rename("foo", "bar");
    if (fd_bar){
        putchar('a');
        fprintf(fd_bar, "%s", "mundo!\n");
        
        fd_foo = fopen("foo", "r");
        
        if (!fd_foo){
            rename("foo", "bar");
            fprintf(fd_bar, "%s", "foo nao existe\n");
        } else fclose(fd_foo);
        
        fclose(fd_bar);
    }
    
    rename("foo", "bar");
    return 0;
}