#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

/*
    自定义输出信息结构体
*/

typedef struct file_detial{
    unsigned char   f_type;       // 文件名         
    __mode_t        u_right;      // 用户权限        
    __mode_t        g_right;      // 用户组权限      
    __mode_t        oth_right;    // 其他用户组权限     
    __nlink_t       lnk_num;      // 连接数子目录数 
    char*           u_name;       // 用户名 
    char*           g_name;       // 组名 
    __off_t         f_size;       // 文件大小 
    time_t*         lst_mtime;    // 最近修改时间 
    char*           f_name;       // 文件名 
}f_detial;

/* 将整形类型权限表示转化为 rwx 字符型 */
char* convert_to_rwx(__mode_t right){
    char *rmods[8] = {"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};
    return rmods[right];
}

/* 通过uid查找用户名 */
char* convert_uid_to_uname(__uid_t uid){
    char *emptyuid = " ";
    struct passwd *p;
    if(p=getpwuid(uid)){
        return p->pw_name;
    }
    else{
        return emptyuid;
    }
    
} 
/* 通过gid查找组名 */
char* convert_gid_to_uname(__gid_t gid){
    char *emptygid = " ";
    struct group *g;
    if(g=getgrgid(gid)){
        return g->gr_name;
    }
    else{
        return emptygid;
    }
} 
void print_file_detial(f_detial fd){
    /*  处理文件类型输出 */
    char type_o = ' ';
    switch(fd.f_type){
        case 8  :type_o = '-';break;
        case 4  :type_o = 'd';break;
        case 10 :type_o = 'l';break;
    };

    printf("%c ",type_o);
    
    char *uright_s = convert_to_rwx(fd.u_right);
    char *gright_s = convert_to_rwx(fd.g_right);
    char *othright_s = convert_to_rwx(fd.oth_right);

    printf("%s",uright_s);
    printf("%s",gright_s);
    printf("%s ",othright_s);
    /*
    printf("%d ",fd.u_right);
    printf("%d ",fd.g_right);
    printf("%d ",fd.oth_right);
    */

    printf("%s ",fd.u_name);
    printf("%s ",fd.g_name);
    printf("%2ld ",fd.lnk_num);
    printf("%5ld ",fd.f_size);


    /* 处理UTC时间格式 */
    char *mtime = ctime(fd.lst_mtime);
    mtime[strlen(mtime)-1]=0;
    printf("%s ",mtime);

    printf("%s ",fd.f_name);
    printf("\n");
};

void enter_dir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
            continue;

        lstat(entry->d_name,&statbuf);

        /*
            对自定义输出结构体进行成员赋值
        */

        f_detial efd;
        efd.f_type = entry->d_type;
        efd.lnk_num = statbuf.st_nlink;
        efd.u_right = ((statbuf.st_mode&00700)/64);
        efd.g_right = ((statbuf.st_mode&00070)/8);
        efd.oth_right = ((statbuf.st_mode&00007));
        efd.u_name = convert_uid_to_uname(statbuf.st_uid);
        efd.g_name = convert_uid_to_uname(statbuf.st_gid);
        efd.f_name = entry->d_name;
        efd.lst_mtime = &statbuf.st_mtime;
        efd.f_size = statbuf.st_size;

        /*
            进行屏幕输出
        */
        print_file_detial(efd);
    }
    chdir("..");
    closedir(dp);
}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
    char *current_dir; 
    char pwd[2]=".";
    if (argc != 2)
        current_dir=pwd;
    else
        current_dir=argv[1];

    enter_dir(current_dir,0);
    return 0;
}