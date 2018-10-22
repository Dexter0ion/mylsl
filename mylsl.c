/*
    2018/10/21
    ShaoBochao
    Achieve Linux 'ls -l' command using Linux C

    update:2018/10/22
    Features:
            1.show detail informations 
            2.add color pattern
            3.enter sub dir and print indent
*/

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
    printf color pattern
*/

#define L_GREEN "\e[1;32m" /*  dir     */
#define L_BLUE "\e[1;34m"  /*  xfile   */
#define L_CYAN "\e[1;36m"  /*  link    */
#define NECOLOR "\033[0m"  /*  nocolor */

/*
    自定义输出信息结构体
*/

typedef struct file_detial
{
    unsigned char f_type; // 文件名
    __mode_t u_right;     // 用户权限
    __mode_t g_right;     // 用户组权限
    __mode_t oth_right;   // 其他用户组权限
    __nlink_t lnk_num;    // 连接数子目录数
    char *u_name;         // 用户名
    char *g_name;         // 组名
    __off_t f_size;       // 文件大小
    time_t *lst_mtime;    // 最近修改时间
    char *f_name;         // 文件名
} f_detial;

/* 将整形类型权限表示转化为 rwx 字符型 */
char *convert_to_rwx(__mode_t right)
{
    char *rmods[8] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    return rmods[right];
}
//coded by shaobochao
/* 通过uid查找用户名 */
char *convert_uid_to_uname(__uid_t uid)
{
    char *emptyuid = " ";
    struct passwd *p;
    if (p = getpwuid(uid))
    {
        return p->pw_name;
    }
    else
    {
        return emptyuid;
    }
}
/* 通过gid查找组名 */
char *convert_gid_to_uname(__gid_t gid)
{
    char *emptygid = " ";
    struct group *g;
    if (g = getgrgid(gid))
    {
        return g->gr_name;
    }
    else
    {
        return emptygid;
    }
}

//coded by shaobochao
void print_file_detial(f_detial fd, int INDENT)
{

    /*  处理文件类型输出 */
    char type_o = ' ';
    switch (fd.f_type)
    {
    case 8:
        type_o = '-';
        break;
    case 4:
        type_o = 'd';
        break;
    case 10:
        type_o = 'l';
        break;
    };

    printf("%c ", type_o);

    char *uright_s = convert_to_rwx(fd.u_right);
    char *gright_s = convert_to_rwx(fd.g_right);
    char *othright_s = convert_to_rwx(fd.oth_right);

    printf("%s", uright_s);
    printf("%s", gright_s);
    printf("%s ", othright_s);
    /*
    printf("%d ",fd.u_right);
    printf("%d ",fd.g_right);
    printf("%d ",fd.oth_right);
    */

    printf("%s ", fd.u_name);
    printf("%s ", fd.g_name);
    printf("%2ld ", fd.lnk_num);
    printf("%5ld ", fd.f_size);

    /* 处理UTC时间格式 */
    char *mtime = ctime(fd.lst_mtime);
    mtime[strlen(mtime) - 1] = 0;
    printf("%s ", mtime);

    char *COLORPAT = NECOLOR;
    switch (fd.f_type)
    {
    case 8:
    {
        if (fd.u_right % 2 || fd.g_right % 2 || fd.oth_right % 2)
        {
            COLORPAT = L_GREEN;
            break;
        }
        break;
    }
    case 4:
        COLORPAT = L_BLUE;
        break; //type_o = 'd';
    case 10:
        COLORPAT = L_CYAN;
        break; //type_o = 'l';
    };
    /*  处理缩进INDENT    */
    printf("%*s", INDENT, "");
    printf("%s%s " NECOLOR, COLORPAT, fd.f_name);
    printf("\n");
};

//coded by shaobochao
void enter_dir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || strcmp(".git", entry->d_name) == 0)
            continue;

        lstat(entry->d_name, &statbuf);

        /*
            对自定义输出结构体进行成员赋值
        */

        f_detial efd;
        efd.f_type = entry->d_type;
        efd.lnk_num = statbuf.st_nlink;
        efd.u_right = ((statbuf.st_mode & 00700) / 64);
        efd.g_right = ((statbuf.st_mode & 00070) / 8);
        efd.oth_right = ((statbuf.st_mode & 00007));
        efd.u_name = convert_uid_to_uname(statbuf.st_uid);
        efd.g_name = convert_uid_to_uname(statbuf.st_gid);
        efd.f_name = entry->d_name;
        efd.lst_mtime = &statbuf.st_mtime;
        efd.f_size = statbuf.st_size;

        /*
            进行屏幕输出
        */
        print_file_detial(efd, depth);

        if (S_ISDIR(statbuf.st_mode))
        {
            /*  显示缩进以及目录名 */
            //printf("%*s%s/\n", depth, "", entry->d_name);
            /* Recurse at a new indent level */
            enter_dir(entry->d_name, depth + 4);
        }
    }

    chdir("..");
    closedir(dp);
}


/*  main    
    默认为进入当前路径
    若有参数进入参数路径 */

int main(int argc, char *argv[])
{
    char *current_dir;
    char pwd[2] = ".";
    if (argc != 2)
        current_dir = pwd;
    else
        current_dir = argv[1];

    enter_dir(current_dir, 0);
    return 0;
}