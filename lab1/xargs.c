#include "../kernel/types.h"
#include "../kernel/param.h"
#include "../user/user.h"

#define MAXSTDIN_CHAR 512

int main(int argc , char *argv[])
{
    // echo hello too | xargs echo bye
    // xargs的作用: 将 | 前的输出作为 | 后的输入参数，|为一个管道
    // xargs为一个参数过滤器
    char *cmd; //the command to be excuted
    char buf[512]; //save the params from stdin
    char *params[MAXARG]; //all the params 
    int ret = 1;
    int pid;
    char ch;//save a char from stdin 
    if(argc < 2){//2 arguments at least 
        fprintf(2 , "usage: xargs <command>\n");
        exit(1);
    }
    if(argc + 1 > MAXARG){ //the last param should be 0
        fprintf(2 , "Argument list too long.\n");
    }
    cmd = argv[1];
    for(int i = 1;i < argc;++i){
        //get the params after xargs
        params[i - 1] = argv[i];//浅拷贝

    }
    params[argc] = 0;
    while(ret){
        int i = 0;
        for(;;){
            if((ret = read(0 , &ch , sizeof(char))) < 0){
                fprintf(2 , "pipe read error.\n");
                exit(1);
            }
            if(ret == 0) exit(0);//read to the end from stdin 
            /*n == 0 means it reaches the end of the argument from stdin
             *   buf[i] == '\n' means it reaches the end of current line 
             */
            if(ch == '\n' || ch == ' '){
                buf[i] = 0;
                if(ch == '\n'){
                    break;
                }
            }
            buf[i++] = ch;
        }
        params[argc - 1] = buf;
        if((pid = fork()) < 0){
            fprintf(2 , "fork() error.\n");
            exit(1);
        }
        else if(pid > 0){
            wait(0);
        }
        else{
            exec(cmd , params);
        }
    }
    exit(0);
}

