#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_file_type(struct stat sb);
void print_permissions(struct stat sb);

int main(int argc, char *argv[]) {
    struct stat sb;
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the file
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Get file status using fstat
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);  // Close file descriptor after getting status

    printf("File: %s\n", argv[1]);
    printf("---------------------------\n");
    
    // File type
    printf("File type:               ");
    print_file_type(sb);
    
    // Permissions (FAP - File Access Permissions)
    printf("Access permissions:      ");
    print_permissions(sb);
    
    // Other important statistics
    printf("Inode number:            %ld\n", (long)sb.st_ino);
    printf("File size:               %lld bytes\n", (long long)sb.st_size);
    printf("Number of hard links:    %ld\n", (long)sb.st_nlink);
    printf("Device ID:               %ld\n", (long)sb.st_dev);
    printf("Last access time:        %s", ctime(&sb.st_atime));
    printf("Last modification time:  %s", ctime(&sb.st_mtime));
    printf("Block size:              %ld bytes\n", (long)sb.st_blksize);
    printf("Blocks allocated:        %lld\n", (long long)sb.st_blocks);

    return EXIT_SUCCESS;
}

void print_file_type(struct stat sb) {
    switch (sb.st_mode & S_IFMT) {
        case S_IFREG:  printf("Regular file\n");        break;
        case S_IFDIR:  printf("Directory\n");           break;
        case S_IFLNK:  printf("Symbolic link\n");       break;
        case S_IFCHR:  printf("Character device\n");    break;
        case S_IFBLK:  printf("Block device\n");        break;
        case S_IFIFO:  printf("FIFO/pipe\n");           break;
        case S_IFSOCK: printf("Socket\n");              break;
        default:       printf("Unknown file type?\n");  break;
    }
}

void print_permissions(struct stat sb) {
    printf("%c%c%c%c%c%c%c%c%c\n",
        (sb.st_mode & S_IRUSR) ? 'r' : '-',
        (sb.st_mode & S_IWUSR) ? 'w' : '-',
        (sb.st_mode & S_IXUSR) ? 'x' : '-',
        (sb.st_mode & S_IRGRP) ? 'r' : '-',
        (sb.st_mode & S_IWGRP) ? 'w' : '-',
        (sb.st_mode & S_IXGRP) ? 'x' : '-',
        (sb.st_mode & S_IROTH) ? 'r' : '-',
        (sb.st_mode & S_IWOTH) ? 'w' : '-',
        (sb.st_mode & S_IXOTH) ? 'x' : '-');
}#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_file_type(struct stat sb);
void print_permissions(struct stat sb);

int main(int argc, char *argv[]) {
    struct stat sb;
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the file
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Get file status using fstat
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);  // Close file descriptor after getting status

    printf("File: %s\n", argv[1]);
    printf("---------------------------\n");
    
    // File type
    printf("File type:               ");
    print_file_type(sb);
    
    // Permissions (FAP - File Access Permissions)
    printf("Access permissions:      ");
    print_permissions(sb);
    
    // Other important statistics
    printf("Inode number:            %ld\n", (long)sb.st_ino);
    printf("File size:               %lld bytes\n", (long long)sb.st_size);
    printf("Number of hard links:    %ld\n", (long)sb.st_nlink);
    printf("Device ID:               %ld\n", (long)sb.st_dev);
    printf("Last access time:        %s", ctime(&sb.st_atime));
    printf("Last modification time:  %s", ctime(&sb.st_mtime));
    printf("Block size:              %ld bytes\n", (long)sb.st_blksize);
    printf("Blocks allocated:        %lld\n", (long long)sb.st_blocks);

    return EXIT_SUCCESS;
}

void print_file_type(struct stat sb) {
    switch (sb.st_mode & S_IFMT) {
        case S_IFREG:  printf("Regular file\n");        break;
        case S_IFDIR:  printf("Directory\n");           break;
        case S_IFLNK:  printf("Symbolic link\n");       break;
        case S_IFCHR:  printf("Character device\n");    break;
        case S_IFBLK:  printf("Block device\n");        break;
        case S_IFIFO:  printf("FIFO/pipe\n");           break;
        case S_IFSOCK: printf("Socket\n");              break;
        default:       printf("Unknown file type?\n");  break;
    }
}

void print_permissions(struct stat sb) {
    printf("%c%c%c%c%c%c%c%c%c\n",
        (sb.st_mode & S_IRUSR) ? 'r' : '-',
        (sb.st_mode & S_IWUSR) ? 'w' : '-',
        (sb.st_mode & S_IXUSR) ? 'x' : '-',
        (sb.st_mode & S_IRGRP) ? 'r' : '-',
        (sb.st_mode & S_IWGRP) ? 'w' : '-',
        (sb.st_mode & S_IXGRP) ? 'x' : '-',
        (sb.st_mode & S_IROTH) ? 'r' : '-',
        (sb.st_mode & S_IWOTH) ? 'w' : '-',
        (sb.st_mode & S_IXOTH) ? 'x' : '-');
}