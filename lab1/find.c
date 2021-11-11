#include "../kernel/types.h"
#include "../kernel/fs.h"
#include "../kernel/stat.h"
#include "../user/user.h"

void
find(char *dir , char *file){
    char buf[512] , *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(dir , 0)) < 0){
        fprintf(2 , "find: cannot open %s\n" , dir);
        exit(1);
    }
    if(fstat(fd , &st) < 0){
        fprintf(2 , "find: cannot stat %s\n" , dir);
        close(fd);
        exit(1);
    }
    
    if(st.type != T_DIR){
        fprintf(2 , "usage: find <directory> <filename>n");
        exit(1);
    }

    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long");
    }
    strcpy(buf , dir);
    p = buf + strlen(buf);
    *p ++ = '/';
    //目录文件中存放中各个目录项 dirent 
    while(read(fd , &de , sizeof(de)) == sizeof(de)){
        //此文件夹无文件
        if(de.inum == 0) continue;
        if(!strcmp(de.name , ".") || !strcmp(de.name , "..")){
            continue;
        }
        memmove(p , de.name , DIRSIZ);
        p[DIRSIZ] = 0;
        //stat(const char * , &stat) 获取某个文件的stat状态
        if(stat(buf , &st) < 0){
            fprintf(2 , "find: cannot stat %s\n" , buf);
            continue;
        }
        if(st.type == T_FILE && !strcmp(de.name , file)){
            printf("%s\n" , buf);
        }
        else if(st.type == T_DIR){
            find(buf , file);
        }
    }
    close(fd);
}
int 
main(int argc , char *argv[])
{
    if(argc != 3){
        fprintf(2 , "usage: find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1] , argv[2]);
    exit(0);
}

