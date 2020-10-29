#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>

void print_file(char* path) {
    FILE* fp;
    char c;

    fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("test   >> could not open file %s\n", path);
        return;
    }

    printf("test   >> ");
    while ( (c = fgetc(fp)) != EOF ) {
        printf("%c", c);
    }

    fclose(fp);
}

void print_tracer_pid() {
    FILE* fp;
    char line[50];

    fp = fopen("/proc/self/status", "rb");
    if (fp == NULL) {
        printf("test   >> could not open /proc/self/status\n");
        return;
    }

    while ( fgets(line, sizeof(line), fp) != NULL ) {
        if (strstr(line, "TracerPid:") != NULL) {
            printf("%s", line);
            break;
        }
    }

    fclose(fp);
}

void print_resuid() {
    uid_t ruid, euid,  suid;

    getresuid(&ruid, &euid, &suid);
    printf("test   >> ruid %d\n", ruid);
    printf("test   >> euid %d\n", euid);
    printf("test   >> suid %d\n", suid);
}

int main(int argc, char** argv) {
    print_resuid();

    /* if we create a file, the owner of the file is the one with the effective user ID
     * uncomment the line below to create a new file owned by root
     */
    //fopen("file_owner_test", "w");

    /* uncomment the following line to test the effective user id
     * by opening and reading the secret.txt file
     */
    //print_file("secret.txt");

    /* PTRACE_TRACEME only works if the parent process has the
     * real user ID equal to the effective user ID of the setuid binary
     */
    //ptrace(PTRACE_TRACEME, 0, 0, 0);
    //print_tracer_pid();

    /* when exec-ing into a new process, the real user id becomes the effective user id
     * uncomment the setresuid line below to set the real id to the effective id
     * this way, you get a root shell
     */
    //setresuid( geteuid(), geteuid(), geteuid() );
    //print_resuid();

    /* uncomment the following line to spawn a shell and test the final real and effective user ids
     * in bash, you can echo $UID for the real user id and $EUID for the effective user id
     */
    //execl("/bin/bash", "/bin/bash", NULL);

    return 0;
}


