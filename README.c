/*
    struct __dirstream
*/

struct __dirstream
{
    int fd;                    /* File descriptor.  */
    __libc_lock_define(, lock) /* Mutex lock for this structure.  */

    size_t allocation;          /* Space allocated for the block.  */
    size_t size;           /* Total valid data in the block.  */
    size_t offset;         /* Current offset into the block.  */

    off_t filepos; /* Position of next entry to read.  */

    /* Directory block.  */
    char data[0] __attribute__((aligned(__alignof__(void *))));
};
/*
    struct dirent 
*/

/*
    d_ino       索引节点号
    d_off       目录文件中的偏移
    d_reclen    文件名长度
    d_type      文件类型
    d_name      文件名
*/

struct dirent
{
#ifndef __USE_FILE_OFFSET64
    __ino_t d_ino;
    __off_t d_off;
#else
    __ino64_t d_ino;
    __off64_t d_off;
#endif
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256]; /* We must not include limits.h! */
};

/*
    stat struct
*/

/*
    st_dev      文件使用设备号
    st_ino      索引节点号
    st_nlink    文件硬连接数
    st_mode     文件访问权限
    st_uid      所有者用户ID
    st_gid      用户组识别号
    st_size     文件大小 in bytes
    struct timespec st_atim;		 Time of last access.  
    struct timespec st_mtim;		 Time of last modification.  
    struct timespec st_ctim;		 Time of last status change.  
*/

struct stat
{
    __dev_t st_dev; /* Device.  */
#ifndef __x86_64__
    unsigned short int __pad1;
#endif
#if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    __ino_t st_ino; /* File serial number.	*/
#else
    __ino_t __st_ino;               /* 32bit file serial number.	*/
#endif
#ifndef __x86_64__
    __mode_t st_mode;   /* File mode.  */
    __nlink_t st_nlink; /* Link count.  */
#else
    __nlink_t st_nlink;             /* Link count.  */
    __mode_t st_mode;               /* File mode.  */
#endif
    __uid_t st_uid; /* User ID of the file's owner.	*/
    __gid_t st_gid; /* Group ID of the file's group.*/
#ifdef __x86_64__
    int __pad0;
#endif
    __dev_t st_rdev; /* Device number, if device.  */
#ifndef __x86_64__
    unsigned short int __pad2;
#endif
#if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    __off_t st_size; /* Size of file, in bytes.  */
#else
    __off64_t st_size;              /* Size of file, in bytes.  */
#endif
    __blksize_t st_blksize; /* Optimal block size for I/O.  */
#if defined __x86_64__ || !defined __USE_FILE_OFFSET64
    __blkcnt_t st_blocks; /* Number 512-byte blocks allocated. */
#else
    __blkcnt64_t st_blocks;         /* Number 512-byte blocks allocated. */
#endif
#ifdef __USE_XOPEN2K8
    /* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  */
    struct timespec st_atim;    /* Time of last access.  */
    struct timespec st_mtim;    /* Time of last modification.  */
    struct timespec st_ctim;    /* Time of last status change.  */
#define st_atime st_atim.tv_sec /* Backward compatibility.  */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
#else
    __time_t st_atime;              /* Time of last access.  */
    __syscall_ulong_t st_atimensec; /* Nscecs of last access.  */
    __time_t st_mtime;              /* Time of last modification.  */
    __syscall_ulong_t st_mtimensec; /* Nsecs of last modification.  */
    __time_t st_ctime;              /* Time of last status change.  */
    __syscall_ulong_t st_ctimensec; /* Nsecs of last status change.  */
#endif
#ifdef __x86_64__
    __syscall_slong_t __glibc_reserved[3];
#else
#ifndef __USE_FILE_OFFSET64
    unsigned long int __glibc_reserved4;
    unsigned long int __glibc_reserved5;
#else
    __ino64_t st_ino; /* File serial number.	*/
#endif
#endif
};