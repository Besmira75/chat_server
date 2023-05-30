#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h> // Add this line
#define MAX_TEXT_SIZE 2048
#define MAX_CLIENTS 100

struct message {
    long mtype;
    char mtext[MAX_TEXT_SIZE];
};

struct client {
    pid_t pid;
    char name[MAX_TEXT_SIZE];
};

int msgqid;
struct client clients[MAX_CLIENTS];

void cleanup() {
    if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
        perror("msgctl: remove message queue failed");
        exit(EXIT_FAILURE);
    }
}

void handle_client(struct message msg) {
    struct message reply;
    reply.mtype = 2;

    // Save client name and process ID
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].pid == 0) {
            clients[i].pid = getpid();
            strcpy(clients[i].name, msg.mtext);
            break;
        }
    }

    while (1) {
        if (msgrcv(msgqid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
            perror("msgrcv: receive message failed");
            exit(EXIT_FAILURE);
        }

        // Broadcast the message to all clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].pid != 0 && clients[i].pid != getpid()) {
                strcpy(reply.mtext, clients[i].name);
                strcat(reply.mtext, ": ");
                strcat(reply.mtext, msg.mtext);

                if (msgsnd(msgqid, &reply, strlen(reply.mtext) + 1, 0) == -1) {
                    perror("msgsnd: send message failed");
                    exit(EXIT_FAILURE);
                }
            }
        }

        if (strcmp(msg.mtext, "exit") == 0)
            break;
    }

    // Remove client from the list
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].pid == getpid()) {
            clients[i].pid = 0;
            break;
        }
    }

    cleanup();
    exit(EXIT_SUCCESS);
}

