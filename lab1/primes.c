#include "../kernel/types.h"
#include "../user/user.h"

void 
primes(int *fdr){
    int fdw[2];

    close(fdr[1]);//xv6文件描述符上限
    int curPrime;
    int status;
    if((status = read(fdr[0] , &curPrime , sizeof(int))) != sizeof(int)){
        if(status != 0) fprintf(2 , "read error\n");
        exit(1);
    }
    fprintf(1 , "prime %d\n" , curPrime);

    close(fdw[0]);

    if(pipe(fdw) < 0){
        fprintf(2 , "pipe open failed.\n");
        exit(1);
    }
    int pid = fork();
    if(pid < 0){
        fprintf(2 , "fork failed.\n");
        exit(1);
    }
    else if(pid > 0){
        int rec;
        while(read(fdr[0] , &rec , sizeof(int)) > 0){
            if(rec % curPrime == 0) continue;
            write(fdw[1] , &rec , sizeof(int));
        }
        close(fdw[1]);
        close(fdr[0]);
        wait(0);
    }
    else if(pid == 0){
        primes(fdw);
    }
}

int 
main(int argc , char *argv[])
{
    //用Eratosthenes方法筛选出素数
    int fd[2];//fd[0]为读，fd[1]为写
    int pid;

    if(pipe(fd) < 0){
        fprintf(2 , "pipe open failed.");
        exit(1);
    }
    pid = fork();
    if(pid < 0){
        fprintf(2 , "fork failed.");
        exit(1);
    }
    else if(pid == 0){
        primes(fd);
    }
    else if(pid > 0){
        int limits = 35;
        if(argc == 2){
            limits = atoi(argv[1]);
        }
        int cur_write_bytes = 0;
        for(int i = 2;i <= limits;++i){
            if((cur_write_bytes = write(fd[1] , &i , sizeof(int))) != 4){
                fprintf(2 , "bytes write error.\n");    
            }
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}

