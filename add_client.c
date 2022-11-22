#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#define SERVER_FIFO "/tmp/addition_fifo_server"
char my_fifo_name [128];
char buf1 [512], buf2 [1024];
int main (int argc, char **argv)
{
 int fd, fd_server, bytes_read;
 // make client fifo
 sprintf (my_fifo_name, "/tmp/add_client_fifo%ld", (long) getpid ());
 if (mkfifo (my_fifo_name, 0664) == -1)
 perror ("mkfifo");
 while (1) {
 // get user input
 printf ("Type numbers to be added: ");
 if (fgets(buf1, sizeof (buf1), stdin) == NULL)
 break;
 strcpy (buf2, my_fifo_name);
 strcat (buf2, " ");
 strcat (buf2, buf1);
 // send message to server
 if ((fd_server = open (SERVER_FIFO, O_WRONLY)) == -1) {
 perror ("open: server fifo");
 break;
 }
 if (write (fd_server, buf2, strlen (buf2)) != strlen (buf2)) {
 perror ("write");
 break;
 }
 if (close (fd_server) == -1) {
 perror ("close");
 break;
 }
 // read the answer
 if ((fd = open (my_fifo_name, O_RDONLY)) == -1)
 perror ("open");
 memset (buf2, '\0', sizeof (buf2));
 if ((bytes_read = read (fd, buf2, sizeof (buf2))) == -1)
 perror ("read");
 if (bytes_read > 0) {
 printf ("Answer: %s\n", buf2);
 }
 if (close (fd) == -1) {
 perror ("close");
 break;
 }
 }
}


// Using waitpid() to wait for a specific child process
// #include<stdio.h>
// #include<unistd.h>
// #include<sys/types.h>
// #include<sys/wait.h>
// int main() {
// int pid;
// int pids[3];
// int status;
// int numprocesses = 0;
// int total_processes = 3;
// while (numprocesses < total_processes) {
// pid = fork();
// // Child process
// if (pid == 0) {
// printf("In child process: process id is %d\n", getpid());
// sleep(5);
// return 4;
// } else {
// pids[numprocesses] = pid;
// numprocesses++;
// printf("In parent process: created process number: %d\n", pid);
// }
// }
// // Waiting for 3rd child process
// waitpid(pids[total_processes - 1], &status, 0);
// if (WIFEXITED(status) != 0) {
// printf("process %d exited normally\n", pids[total_processes - 1]);
// printf("exit status from child is %d\n", WEXITSTATUS(status));
// } else {
// printf("process %d not exited normally\n", pids[total_processes - 1]);
// }
// return 0;
// }
