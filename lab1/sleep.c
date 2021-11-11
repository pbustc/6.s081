#include "../kernel/types.h"
#include "../user/user.h"

int
main(int argc , char * argv[])
{
    if(argc != 2){
        fprintf(2 , "argument error.\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    fprintf(1 , "sleep a while.\n");
    exit(0);
}

