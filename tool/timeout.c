#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-s signal] SECONDS COMMAND [ARGS...]\n", prog);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sig = SIGTERM; // default signal
    int argi = 1;

    // Parse -s signal
    if (argc > 1 && strcmp(argv[argi], "-s") == 0) {
        if (argc <= argi + 2) usage(argv[0]);
        argi++;
        char *sigstr = argv[argi++];
        if (strncmp(sigstr, "SIG", 3) == 0)
            sigstr += 3;
        if (!strcasecmp(sigstr, "TERM")) sig = SIGTERM;
        else if (!strcasecmp(sigstr, "KILL")) sig = SIGKILL;
        else if (!strcasecmp(sigstr, "INT")) sig = SIGINT;
        else if (!strcasecmp(sigstr, "HUP")) sig = SIGHUP;
        else {
            int nsig = atoi(sigstr);
            if (nsig <= 0 || nsig > 64) usage(argv[0]);
            sig = nsig;
        }
    }

    if (argc <= argi + 1) usage(argv[0]);
    int seconds = atoi(argv[argi++]);
    if (seconds <= 0) usage(argv[0]);

    char **cmd = &argv[argi];

    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        return 1;
    }
    if (child == 0) {
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(127);
    }

    int status = 0;
    int timed_out = 0;
    pid_t timer = fork();
    if (timer < 0) {
        perror("fork (timer)");
        kill(child, SIGKILL);
        return 1;
    }
    if (timer == 0) {
        sleep(seconds);
        exit(0);
    }

    pid_t done;
    while ((done = wait(&status)) > 0) {
        if (done == child) {
            kill(timer, SIGKILL); // Child finished, kill timer
            break;
        }
        if (done == timer) {
            kill(child, sig); // Timeout, send signal
            timed_out = 1;
        }
    }

    if (timed_out) {
        // Wait for child if killed
        waitpid(child, &status, 0);
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : 124;
}