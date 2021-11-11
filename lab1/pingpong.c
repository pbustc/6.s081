#include "../kernel/types.h"
#include "../user/user.h"

int main()
{
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0){//创建管道,fd[0]为read——fd , fd[1]为write-fd 
        fprintf(2 , "pipe open failed.");
        exit(1);
    }
    int pid = fork();
    char buf;
    if(pid < 0){
        fprintf(2 , "fork error.");
        exit(1);
    }
    else if(pid == 0){
        read(pipe_fd[0] , &buf , sizeof(buf));
        fprintf(1 , "%d: received ping\n" , getpid());
        write(pipe_fd[1] , "y" , 1);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
    else{
        write(pipe_fd[1] , "x" , 1);
        int status;
        wait(&status);
        read(pipe_fd[0] ,&buf , sizeof(buf));
        fprintf(1 , "%d: received pong\n" , getpid());    
        close(pipe_fd[1]);
        close(pipe_fd[0]);
    }
    exit(0);
}
    
