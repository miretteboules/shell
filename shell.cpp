#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

using namespace std;
void Write_in_log_file(){
 FILE*f ;
 f=fopen("logfile.log","a");
 fprintf(f,"child process Terminated\n"); // fprintf(f,"child process Terminated with id = %d\n",getpid());

 fclose(f);

 }
//Read Line and remove the new line
int read_line(char *x, char *args[])
{

    fgets(x, 256, stdin);
    x[strlen(x) - 1] = '\0';
    char *token;

    int i = 0;
    token = strtok(x, " ");

    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i++] = NULL;
    return i;
}

void handle_error(int error)
{
    printf("Error: %s\n", strerror(errno));
}
void child_signal_handler(int signum){
    printf("Child Process Terminated\n");
    Write_in_log_file();

}
int main(int argc, char const *argv[])
{

    char x[256];
    char *args[10];

    while (true)
    {
        printf("shell > ");
        int size = read_line(x, args);


        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
            break;

        pid_t pid = fork();

        if (pid == -1)
        {
            printf("Fork Failed\n");
        }
        else if (pid == 0)
        {
            int err = execvp(args[0], args);
            if (err == -1)
                handle_error(err);
        }
        else
        {
            signal(SIGCHLD,child_signal_handler);

            if ((size - 2) > 0 && strcmp(args[size - 2], "&") == 0)
                continue;
            wait(NULL);
        }
    }
    return 0;
}
