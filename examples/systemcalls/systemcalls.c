#include "systemcalls.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    if(cmd == NULL){
        return false;
    }

    int ret = system(cmd);

    if(ret == -1){
        return false;
    }
    else if(ret == 127){
        return false;
    }
    else if( WIFEXITED(ret) && WEXITSTATUS(ret) == 0){
        return true;
    }
    else{
        return false;
    }

}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    pid_t cpid = fork();

    if(cpid == -1){
        va_end(args);
        return false;
    }
    else if(cpid == 0){
        execv(command[0], command);
        _exit(1);
    }
    else{
        pid_t w;
        int status;

        w = waitpid(cpid, &status, 0);

        va_end(args);  

        if(w == -1){
            return false;
        }
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
            return true;
        }
        else{
            return false;
        }
    }
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    pid_t cpid = fork();

    if(cpid == -1){
        return false;
    }
    else if(cpid == 0){
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC );

        if(fd == -1){
            return false;
        }

        if(dup2(fd, STDOUT_FILENO) == -1){
            close(fd);
            return false;
        }

        close(fd);

        execv(command[0], command);
        _exit(1);
    }
    else{
        pid_t w;
        int status;

        w = waitpid(cpid, &status, 0);

        va_end(args);  

        if(w == -1){
            return false;
        }
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
            return true;
        }
        else{
            return false;
        }
    }

    return true;
}
