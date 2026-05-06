#include <stdio.h>
#include <string.h>
#include <stdlib.h> // ADDED: needed for atoi()
#include <time.h>   // ADDED: needed for logging timestamps

#define NODEE_VERSION "0.1.0"
#define CONFIG_FILE "nodee.conf" // ADDED: config file name
#define LOG_FILE "nodee.log"     // ADDED: log file name

struct NodeInfo
{
    char hostname[50];
    char ip[20];
    int online;
};

// ADDED: removes the newline at the end of a string
void remove_newline(char text[])
{
    text[strcspn(text, "\n")] = '\0';
}

// ADDED: logs events to nodee.log
void log_event(const char *message)
{
    FILE *logfile = fopen(LOG_FILE, "a"); // ADDED: open log file for appending
    if (logfile != NULL)
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fprintf(logfile, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec, message);
        fclose(logfile);
    }
}

// ADDED: loads node data from nodee.conf
void load_config(struct NodeInfo *node)
{
    FILE *file;     // ADDED: file pointer
    char line[100]; // ADDED: stores one line from the file
    char key[50];   // ADDED: stores text before =
    char value[50]; // ADDED: stores text after =

    file = fopen(CONFIG_FILE, "r"); // ADDED: open config file for reading

    if (file == NULL)
    {
        printf("Could not open config file: %s\n", CONFIG_FILE);
        printf("Using default config.\n");
        log_event("Config file not found, using defaults");

        strcpy(node->hostname, "DEFAULT-NODE");
        strcpy(node->ip, "0.0.0.0");
        node->online = 0;
        return;
    }

    log_event("Loading configuration from nodee.conf");

    while (fgets(line, sizeof(line), file))
    { // ADDED: read file line by line
        remove_newline(line);

        if (sscanf(line, "%49[^=]=%49s", key, value) == 2)
        { // ADDED: split key=value
            if (strcmp(key, "hostname") == 0)
            {
                strcpy(node->hostname, value);
            }
            else if (strcmp(key, "ip") == 0)
            {
                strcpy(node->ip, value);
            }
            else if (strcmp(key, "online") == 0)
            {
                node->online = atoi(value);
            }
        }
    }

    fclose(file); // ADDED: close file after reading
    log_event("Configuration loaded successfully");
}

void show_help()
{
    printf("Node-E Agent\n");
    printf("Usage:\n");
    printf("  node-e --help\n");
    printf("  node-e --version\n");
    printf("  node-e --status\n");
}

void show_version()
{
    printf("Node-E version %s\n", NODEE_VERSION);
}

void show_status(struct NodeInfo node)
{
    printf("=== Node-E Status ===\n");
    printf("Hostname: %s\n", node.hostname);
    printf("IP Address: %s\n", node.ip);

    if (node.online == 1)
    {
        printf("Status: ONLINE\n");
    }
    else
    {
        printf("Status: OFFLINE\n");
    }

    char log_msg[100];
    sprintf(log_msg, "Status displayed: %s (%s) - %s",
            node.hostname, node.ip, node.online ? "ONLINE" : "OFFLINE");
    log_event(log_msg);
}

int main(int argc, char *argv[])
{

    struct NodeInfo currentNode;

    log_event("Node-E agent started");

    load_config(&currentNode); // CHANGED: load node data from nodee.conf

    if (argc < 2)
    {
        show_help();
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        show_help();
    }
    else if (strcmp(argv[1], "--version") == 0)
    {
        show_version();
    }
    else if (strcmp(argv[1], "--status") == 0)
    {
        show_status(currentNode);
    }
    else
    {
        printf("Unknown command\n");
    }

    return 0;
}