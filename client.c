#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define MAX_TEXT_SIZE 2048
#define MAX_NAME_SIZE 32

struct message {
    long mtype;
    char mtext[MAX_TEXT_SIZE];
};

int msgqid;
char client_name[MAX_NAME_SIZE];

void cleanup() {
    if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
        perror("msgctl: remove message queue failed");
        exit(EXIT_FAILURE);
    }
}

void send_msg_handler() {
    struct message msg;
    msg.mtype = 1;

    strncpy(msg.mtext, client_name, MAX_NAME_SIZE);

    if (msgsnd(msgqid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("msgsnd: send message failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("> ");
        fgets(msg.mtext, MAX_TEXT_SIZE, stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';

        if (msgsnd(msgqid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
            perror("msgsnd: send message failed");
            exit(EXIT_FAILURE);
        }

        if (strcmp(msg.mtext, "exit") == 0)
            break;
    }

    cleanup();
    exit(EXIT_SUCCESS);
}

void recv_msg_handler() {
    struct message msg;
    while (1) {
        if (msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
            perror("msgrcv: receive message failed");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", msg.mtext);

        if (strcmp(msg.mtext, "exit") == 0)
            break;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <key> <client_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    key_t key = atoi(argv[1]);
    if (key <= 0) {
        printf("Invalid key\n");
        return EXIT_FAILURE;
    }

    strncpy(client_name, argv[2], MAX_NAME_SIZE);

    // Connect to the message queue
    msgqid = msgget(key, 0666);
    if (msgqid == -1) {
        perror("msgget: create message queue failed");
        return EXIT_FAILURE;
    }

    signal(SIGINT, cleanup);

    printf("=== WELCOME TO THE CHATROOM ===\n");

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0) {
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0) {
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    pthread_join(send_msg_thread, NULL);
    pthread_join(recv_msg_thread, NULL);

    return EXIT_SUCCESS;
}

