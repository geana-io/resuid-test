#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

void print_tracer_pid() {
    FILE* fp;
    char line[50];

    fp = fopen("/proc/self/status", "rb");
    if (fp == NULL) {
        printf("runner >> could not open /proc/self/status\n");
        return;
    }

    while ( fgets(line, sizeof(line), fp) != NULL ) {
        if (strstr(line, "TracerPid:") != NULL) {
            printf("runner >> %s", line);
            break;
        }
    }

    fclose(fp);
}

int main(int argc, char** argv)
{
    pid_t cpid;
    int status;

    cpid = fork();

    if(cpid == 0) {
        print_tracer_pid();
        /* when exec-ing a setuid binary, effective uid = uid owner
         * however, if PTRACE_TRACEME is used, then effective uid = real uid
         */
        //ptrace(PTRACE_TRACEME, 0, 0, 0);
        //print_tracer_pid();
        execl("./test", "./test", NULL);
    }

    else {
        printf("runner >> cpid id is %d\n", cpid);

        /* PTRACE_ATTACH fails in both situations:
         *   if PTRACE_TRACEME is called, then the child is already being traced by the parent
         *   if PTRACE_TRACEME is not set, then the process is loaded with root effective and saved uids
         */
        //printf("runner >> PTRACE_ATTACH result %d\n", ptrace(PTRACE_ATTACH, cpid, 0, 0));

        while(1) {
            wait(&status);

            if ( WIFSTOPPED(status) ) {
                printf("runner >> stopped %d\n", WSTOPSIG(status));
                ptrace(PTRACE_CONT, cpid, 0, 0);
            }

            if ( WIFSIGNALED(status) ) {
                printf("runner >> signaled\n");
            }

            if( WIFEXITED(status) ) {
                printf("runner >> exited\n");
                break;
            }
        }
    }

    return 0;
}

