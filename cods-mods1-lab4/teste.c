#include<stdio.h>
#include<pthread.h>
void * tarefa(void *arg) {
    FILE *fd_bar, *fd_foo;
    fd_bar = fopen("bar", "a+");

    if(fd_bar) {
        fprintf (fd_bar, "%s", "mundo!\n");
        fd_foo = fopen("foo", "r");
        
        if(!fd_foo){
            fprintf(fd_bar, "%s", "foo nao existe\n");
        } else fclose(fd_foo);

        fclose (fd_bar);
    }
    pthread_exit(NULL);
}
int main(void) {
    FILE *fd;
    pthread_t tid;
    fd = fopen("foo", "w");
    if (!fd) return 1;
    fprintf(fd, "%s", "Ola ");
    fclose(fd);
    if (pthread_create(&tid, NULL, tarefa, NULL)) return 2;
    rename ("foo", "bar");
    pthread_join(tid, NULL);
    return 0;
}