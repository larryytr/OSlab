#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#define MAX_LEN 512

int cnt=0;

void Exitcrepl();
void Somethingwrong(char *str)
{
    perror(str);
    Exitcrepl();
    exit(EXIT_FAILURE);
}

void Exitcrepl()
{
    if(system("rm -rf test")!=0)
    {
        perror("rm -rf test");
        assert(0);
    }
    if(system("rm -rf lib")!=0)
    {
        perror("rm -rf lib");
        assert(0);
    }

}

char* my_read(const char *previous,char * buf){
    printf("%s ",previous);
    char *result=NULL;
    result=fgets(buf,MAX_LEN,stdin);
    if(ferror(stdin)&&result==NULL)
    {
        Somethingwrong("my_read");
    }
    int len=strlen(buf);
    buf[len-1]='\0';
    return result;

}

void *add_func_to_file(char *func,char*name)
{
    char C_file[64],SO_file[64];
    char gcc_command[256];

    sprintf(C_file,"./lib/C_%s.c",name);
    sprintf(SO_file,"./lib/SO_%s.so",name);
    sprintf(gcc_command,"gcc -shared -fPIC -Wno-implicit-function-declaration -o %s %s",SO_file,C_file);

    //so 编码指令;
    FILE *fp=fopen(C_file,"w+");
    fprintf(fp,"%s\n",func);
    fclose(fp);

    //执行指令;
    if(system(gcc_command)!=0){
        Somethingwrong("gcc_command");
    }
    //动态加载到内存;
    return dlopen(SO_file,RTLD_GLOBAL|RTLD_NOW);//LAZT,全局;

}

void solve_func(char *buf)
{
    char temp_name[64];

    //首先预编译康康对不对;
    // check_func;
    //然后再加载到库里面;
    //add_func_to_file;
    sprintf(temp_name,"_expr_wrap_%04d",cnt++);
    void *flag=add_func_to_file(buf,temp_name);

    void*handle=flag;
    int (*temp)();
    if(handle==NULL)assert(0);
    temp=dlsym(handle,"func");
    int result=temp();
    printf("%d\n",result);

    // 对flag处理;
    if(flag!=NULL){
        printf("Add:%s\n",buf);
        return;
    }
    else{
        printf("flag NULL!\n");
    }
}

void solve_val(char *val)
{
    // to be continued
}



int main(int argc, char *argv[]) {
    //创建测试目录以及lib目录;
    printf("使用exit()退出;如果使用了Ctrl+C 退出,需要在经历一次报错之后重新跑;\n");
    if(system("mkdir test")!=0)
        Somethingwrong("mkdir test");
    if(system("mkdir lib")!=0)
        Somethingwrong("mkdir lib");
    //输入部分;
    char in[MAX_LEN];
    while(my_read(">>",in)!=NULL)
    {
        if(in==NULL)continue;
        if(strcmp(in,"exit()")==0)
            break;
        //判断是函数还是表达式;
        if(strncmp("int ",in,4)==0)
        {
            //是func
     //   printf("Func:%s\n",in);
            solve_func(in);

        }else
        {
            //是表达式;
       // printf("Val:%s\n",in);
        }
    }

    Exitcrepl();
    printf("Success");


    return 0;
}

//参考:
//分析dl系列:https://blog.csdn.net/zhengqijun_/article/details/72540878
