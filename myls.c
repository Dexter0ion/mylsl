/*
    2018/10/21
    ShaoBochao
*/

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

void enter_dir(char *dir, int depth)
{
    DIR *dp;              /*  See README struct __dirstream */
    struct dirent *entry; /*  See README struct dirent */
    struct stat statbuf;  /*  See README struct stat */

    /* Open a directory stream on NAME.
   Return a DIR stream on the directory, or NULL if it could not be opened.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */

    /*
    根据名称打开目录流
    返回 DIR 即 struct __dirstream 当无法打开目标目录返回NULL*/

    //dp = opendir(dir);
    //if (dp == NULL)
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        //printf("[Failed] Can't open direcory: %s\n", dir);
        return;
    }

    /* Change the process's working directory to PATH.  */
    /* 打开 dir名称目录不失败 切换进程当前工作路径到PATH即dir   */

    chdir(dir);

    /* Read a directory entry from DIRP.  Return a pointer to a `struct
    dirent' describing the entry, or NULL for EOF or error.  The
    storage returned may be overwritten by a later readdir call on the
    same DIR stream. */

    /* 读取目标DIR stream进入点
       返回一个'struct dirent'指针来描述这个entry */

    //entry = readdir(dp);
    //while (entry != NULL)
    while ((entry = readdir(dp)) != NULL)
    {
        /* Get file attributes about FILE and put them in BUF.
            If FILE is a symbolic link, do not follow it.  */

        /*  将文件属性放置到stat buf缓冲区
            如果是符号链接则省略    */

        lstat(entry->d_name, &statbuf); /* d_name 文件名*/

        /*  如果文件类型是目录 */
        if (S_ISDIR(statbuf.st_mode))
        {
            /*  忽略. 以及 ..*/
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            /*  显示缩进以及目录名 */
            printf("%*s%s/\n", depth, "", entry->d_name);
            /* Recurse at a new indent level */
            enter_dir(entry->d_name, depth + 4);
        }
        else
        {
            printf("%*s%s\n", depth, "", entry->d_name);
        }
        /*  进入下一级目录  */
        chdir("..");
        /* Close the directory stream DIRP. Return 0 if successful, -1 if not.*/
        closedir(dp);
        //entry = readdir(dp);
    }
}

int main(int argc, char *argv[])
{
    char *topdir, pwd[2] = ".";
    if (argc != 2)
        topdir = pwd;
    else
        topdir = argv[1];

    printf("Directory scan of %s\n", topdir);
    enter_dir(topdir, 0);
    printf("done.\n");

    exit(0);
}