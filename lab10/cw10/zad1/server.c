#include "common.h"
#include <pthread.h>

#define MAX_CONN 16
#define PING_INTERVAL 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int epoll_fd;

void write_to_file(char *mess){

    FILE* file = fopen("history.txt", "a");

    if(!file){
        perror("Can't open history.txt\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Message %s\n", mess);
    time_t curr_time = time(NULL);
    struct tm* local_time = localtime(&curr_time);

    fprintf(file, "sent at: %02d:%02d:%02d\n\n\n",
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec);

    fclose(file);
}

struct client {
    int fd;
    enum client_state { empty = 0, init, ready } state;
    char nickname[16];
    char symbol;
    struct game_state* game_state;
    bool responding;
} clients[MAX_CONN];
typedef struct client client;

typedef struct event_data {
    enum event_type { socket_event, client_event } type;
    union payload { client* client; int socket; } payload;
} event_data;

void delete_client(client* client) {
    char information[MSG_LEN];
    sprintf(information, "Deleting %s\n", client->nickname);
    write_to_file(information);
    printf("%s", information);
    client->state = empty;
    client->nickname[0] = 0;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL);
    close(client->fd);
}

void send_msg(client* client, message_type type, char *text) {
    message msg;
    msg.type = type;
    memcpy(&msg.text, text, MSG_LEN*sizeof(char));
    write_to_file(text);
    write(client->fd, &msg, sizeof(msg));
}

void on_client_message(client* client) {
    if (client->state == init) {
        int nick_size = read(client->fd, client->nickname, sizeof client->nickname - 1);
        int j = client - clients;
        pthread_mutex_lock(&mutex);
        if (find(int i = 0; i < MAX_CONN; i++, i != j && strncmp(client->nickname, clients[i].nickname, sizeof clients->nickname) == 0) == -1) {
            client->nickname[nick_size] = '\0';
            client->state = ready;
            char information[MSG_LEN];
            sprintf(information, "New client %s\n", client->nickname);
            write_to_file(information);
            printf("%s", information);

        }
        else {
            message msg = { .type = USERNAME };
            char information[MSG_LEN];
            sprintf(information, "Nickname %s already taken\n", client->nickname);
            write_to_file(information);
            printf("%s", information);
            write(client->fd, &msg, sizeof msg);
            strcpy(client->nickname, "new client");
            delete_client(client); // username taken
        }
        pthread_mutex_unlock(&mutex);
    }
    else {
        message msg;
        read(client->fd, &msg, sizeof msg);
        char information[MSG_LEN*2];
        sprintf(information, "Received a msg %i %s\n", (int)msg.type, msg.text);
        write_to_file(information);
        printf("%s", information);


        if (msg.type == PING) {
            pthread_mutex_lock(&mutex);
            char information[MSG_LEN];
            sprintf(information, "Ping %s\n", client->nickname);
            write_to_file(information);
            printf("%s", information);
            pthread_mutex_unlock(&mutex);
        } else if (msg.type == DISCONNECT || msg.type == STOP) {
            pthread_mutex_lock(&mutex);
            delete_client(client);
            pthread_mutex_unlock(&mutex);
        } else if (msg.type == TO_ALL) {
            char out[MSG_LEN] = "";
            strcat(out, client->nickname);
            strcat(out, ": ");
            strcat(out, msg.text);

            for (int i = 0; i < MAX_CONN; i++) {
                if (clients[i].state != empty)
                    send_msg(clients+i, GET, out);
            }
        } else if (msg.type == LIST) {
            for (int i = 0; i < MAX_CONN; i++) {
                if (clients[i].state != empty)
                    send_msg(client, GET, clients[i].nickname);
            }
        } else if (msg.type == TO_ONE) {
            char out[MSG_LEN] = "";
            strcat(out, client->nickname);
            strcat(out, ": ");
            strcat(out, msg.text);

            for (int i = 0; i < MAX_CONN; i++) {
                if (clients[i].state != empty) {
                    if (strcmp(clients[i].nickname, msg.other_nickname) == 0) {
                        send_msg(clients+i, GET, out);
                    }
                }
            }
        }
    }
}

void init_socket(int socket, void* addr, int addr_size) {
    safe (bind(socket, (struct sockaddr*) addr, addr_size));
    safe (listen(socket, MAX_CONN));
    struct epoll_event event = { .events = EPOLLIN | EPOLLPRI };
    event_data* event_data = event.data.ptr = malloc(sizeof *event_data);
    event_data->type = socket_event;
    event_data->payload.socket = socket;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &event);
}

client* new_client(int client_fd) {
    pthread_mutex_lock(&mutex);
    int i = find(int i = 0; i < MAX_CONN; i++, clients[i].state == empty);
    if (i == -1) return NULL;
    client* client = &clients[i];

    client->fd = client_fd;
    client->state = init;
    client->responding = true;
    pthread_mutex_unlock(&mutex);
    return client;
}

void* ping(void* _) {
    static message msg = { .type = PING};
    while(1){
        sleep(PING_INTERVAL);
        pthread_mutex_lock(&mutex);
        char information[MSG_LEN];
        sprintf(information, "Pinging clients\n");
        write_to_file(information);
        printf("%s", information);
        for (int i = 0; i < MAX_CONN; i++) {
            if (clients[i].state != empty) {
                if (clients[i].responding) {
                    clients[i].responding = false;
                    write(clients[i].fd, &msg, sizeof msg);
                }
                else delete_client(&clients[i]);
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage [port] [path]\n");
        exit(0);
    }
    int port = atoi(argv[1]);
    char* socket_path = argv[2];

    epoll_fd = safe (epoll_create1(0));

    struct sockaddr_un local_addr = { .sun_family = AF_UNIX };
    strncpy(local_addr.sun_path, socket_path, sizeof local_addr.sun_path);

    struct sockaddr_in web_addr = {
            .sin_family = AF_INET, .sin_port = htons(port),
            .sin_addr = { .s_addr = htonl(INADDR_ANY) },
    };

    unlink(socket_path);
    int local_sock = safe (socket(AF_UNIX, SOCK_STREAM, 0));
    init_socket(local_sock, &local_addr, sizeof local_addr);

    int web_sock = safe (socket(AF_INET, SOCK_STREAM, 0));
    init_socket(web_sock, &web_addr, sizeof web_addr);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping, NULL);

    char information[MSG_LEN];
    sprintf(information, "Server listening on *:%d and '%s'\n", port, socket_path);
    write_to_file(information);
    printf("%s", information);

    struct epoll_event events[10];
    while(1) {
        int nread = safe (epoll_wait(epoll_fd, events, 10, -1));
        for(int i = 0; i < nread; i++) {
            event_data* data = events[i].data.ptr;
            if (data->type == socket_event) { // new conenction
                int client_fd = accept(data->payload.socket, NULL, NULL);
                client* client = new_client(client_fd);
                if (client == NULL) {
                    char information[MSG_LEN];
                    sprintf(information, "Server is full\n");
                    write_to_file(information);
                    printf("%s", information);
                    message msg = { .type = SERVER_FULL};
                    write(client_fd, &msg, sizeof msg);
                    close(client_fd);
                    continue;
                }

                event_data* event_data = malloc(sizeof(event_data));
                event_data->type = client_event;
                event_data->payload.client = client;
                struct epoll_event event = { .events = EPOLLIN | EPOLLET | EPOLLHUP, .data = { event_data } };

                safe (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event));
            } else if (data->type == client_event) {
                if (events[i].events & EPOLLHUP) {
                    pthread_mutex_lock(&mutex);
                    delete_client(data->payload.client);
                    pthread_mutex_unlock(&mutex);
                }
                else on_client_message(data->payload.client);
            }
        }
    }
}