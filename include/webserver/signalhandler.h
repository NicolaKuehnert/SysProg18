
#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#define SIGHUP 7
#define SIGUSR1 8
#define SIGUSR2 9

#endif

void signalHandler(int signum);
void signal_save_db_exit(int signum);
void signal_show_led(int signum);
void signal_update_game(int signum);
