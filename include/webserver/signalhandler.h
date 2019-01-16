
#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#define SIGHUP 7
#define SIGUSR1 8
#define SIGUSR2 9

#endif

int init_handler();
void signalHandler(int signum);
