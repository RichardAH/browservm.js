#define PORT 80
#define BACKLOG 10
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        return fprintf(stderr, "could not create listen socket\n");
	struct sockaddr_in server_addr = { 0 };

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
    int err = 0;
    err = bind(listen_fd,
	           (struct sockaddr*)&server_addr,
	           sizeof(server_addr));
	if (err == -1)
    {
		perror("bind");
		printf("Failed to bind socket to address\n");
		return err;
	}

    if (err = listen(listen_fd, BACKLOG) < 0)
    {
        perror("listen");
        printf("Failed to put socket in passive mode\n");
        return err;
    }

    // accept loop
    while (1)
    {
        int err = 0;
        int conn_fd;
        socklen_t client_len;
        struct sockaddr_in client_addr;

        client_len = sizeof(client_addr);

        err =
          (conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len));
        if (err == -1) {
            perror("accept");
            printf("failed accepting connection\n");
            return err;
        }

        printf("Client connected!\n");

        // parse input

        do
        {
            char buffer[1024];
            int bytes_read = read(conn_fd, buffer, 1023);
            if (bytes_read <= 0)
                break;

            buffer[bytes_read] = '\0';
            printf("input: `%s`\n");
        } while(0);


        err = close(conn_fd);
        if (err == -1) {
            perror("close");
            printf("failed to close connection\n");
            return err;
        }
        


    }
    return 0;
}
