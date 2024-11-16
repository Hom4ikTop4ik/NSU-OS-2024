#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

char* shortFileName(char* fileName)
{
    char* name = fileName;
    for (int ptr = 0; name[ptr] != '\0'; ptr++) {
        if (name[ptr] == '/')
        {
            name = name + ptr + 1;
            ptr = -1;
        }
    }
    return name;
}

void func(struct stat buf)
{
    char mask[] = "?---___---";
    mask[0] = S_ISDIR(buf.st_mode) ? 'd' : (S_ISREG(buf.st_mode) ? '-' : '?');

    mask[1] = S_IRUSR & buf.st_mode ? 'r' : '-';
    mask[2] = S_IWUSR & buf.st_mode ? 'w' : '-';
    mask[3] = S_IXUSR & buf.st_mode ? 'x' : '-';

    mask[4] = S_IRGRP & buf.st_mode ? 'r' : '-';
    mask[5] = S_IWGRP & buf.st_mode ? 'w' : '-';
    mask[6] = S_IXGRP & buf.st_mode ? 'x' : '-';

    mask[7] = S_IROTH & buf.st_mode ? 'r' : '-';
    mask[8] = S_IWOTH & buf.st_mode ? 'w' : '-';
    mask[9] = S_IXOTH & buf.st_mode ? 'x' : '-';


    uid_t uid = buf.st_uid;
    gid_t gid = buf.st_gid;

    char* empty = "-";
    struct passwd* aboutUser;
    char* userName;
    if (((aboutUser = getpwuid(uid)) == NULL) || ((userName = aboutUser->pw_name) == NULL))
    {
        perror("Can't get user name");
        userName = empty;
    }
    struct group* aboutGroup;
    char* groupName;
    if (((aboutGroup = getgrgid(gid)) == NULL) || ((groupName = aboutGroup->gr_name) == NULL))
    {
        perror("Can't get group name");
        groupName = empty;
    }


    // About off_t
    // cat /usr/include/sys/types.h
    // https://github.com/illumos/illumos-gate/blob/2317519637fb284865ddb3da32debe2fb93d8f39/usr/src/uts/common/sys/types.h#L145
    off_t fileSize = S_ISREG(buf.st_mode) ? buf.st_size : -1;

    // About time_t
    // cat /usr/include/sys/types.h (between SIZE_T and CLOCK_T)
    // https://github.com/illumos/illumos-gate/blob/2317519637fb284865ddb3da32debe2fb93d8f39/usr/src/uts/common/sys/types.h#L569
    time_t fileMTime = buf.st_mtime;
    char* date = ctime(&fileMTime);
    date[16] = '\0'; // cut seconds and year
    date += 4; // skip day of week

    char* fileName = shortFileName(argv[i]);

    if (fileSize == (off_t)-1)
    {
        printf("%s %u %s %s %7s %s %s\n", mask, buf.st_nlink, userName, groupName, empty, date, fileName);
    }
    else 
    {
#if defined(_LP64) || _FILE_OFFSET_BITS == 32
        printf("%s %u %s %s %7ld %s %s\n", mask, buf.st_nlink, userName, groupName, fileSize, date, fileName);
#elif _FILE_OFFSET_BITS == 64
        printf("%s %u %s %s %lld %s %s\n", mask, buf.st_nlink, userName, groupName, fileSize, date, fileName);
#endif
    }
}

int main(int argc, char** argv)
{
    struct stat buf;
    if (argc < 2)
    {
        if (stat(".", &buf) == -1)
        {
            perror("stat was broken");
            exit(EXIT_FAILURE);
        }

        func(buf);
    }
    else 
    {
        for (int i = 1; i < argc; i++)
        {
            if (stat(argv[i], &buf) == -1)
            {
                perror("stat was broken");
                continue;
            }

            func(buf);
        }
    }
}
