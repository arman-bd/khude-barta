#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

#include <pthread.h>
#include <fcntl.h>

#include<windows.h>
#include<winsock2.h>
#include<ws2tcpip.h>

int port, user_id, last_message;
char buffer, input_msg[256], username[33], password[33];
char token[33], host[128], ip[16];
char new_line[2] = "\r\n";
pthread_t checker_thread;

#include "lib/sds/sds.c"

void user_login();
void *check_message();
void send_message(int ignore_input);
int urlencode(char *dest, const char *src);
void WSA_Check();
void hostname_to_ip(char *hostname, char *ip);
int connect_to_server(char *c_host, int c_port);
int send_request(char *url, char *r_response);

int main(){

    printf("::: Welcome to Khude Barta :::\n\n");

    // Chat Server
    //strcpy(host, "vps105442.vps.ovh.ca");
    // Default Port
    strcpy(host, "144.217.84.192");


    port = 80;
    // Get Host IP
    hostname_to_ip(host, ip);

    // Start Login
    user_login();

    // Multi Thread
    if(pthread_create(&checker_thread, NULL, check_message, NULL)){
        printf("\nError: Creating Thread");
        return 1;
    }

    // Send Message
    while(1){
        send_message(0);
    }

    return 0;
}

void user_login(){
    int i, j = 0;
    char url_path[256] = "", enc_txt[33], response[10240];

    retry_login:
    printf("\nUser Name: ");
    scanf("%s", username);

    printf("Password: ");

    i = 0;
    while((buffer = getch())){
        if(buffer == 13 || i > 31){break;}
        password[i] = buffer;
        printf("*");
        i++;
    }

    // Process URL
    sprintf(url_path, "/chat/service/auth.php?auth=true");

    urlencode(enc_txt, username);
    sprintf(url_path, "%s&user=%s", url_path, enc_txt);

    urlencode(enc_txt, password);
    sprintf(url_path, "%s&pass=%s", url_path, enc_txt);

    send_request(url_path, response);

    int param_count;
    sds param_sds, *param_token;

    param_sds = sdsnew(response);
    param_token = sdssplitlen(param_sds, sdslen(param_sds), "<:>", 3, &param_count);

    if(strcmp(param_token[0], "CONTINUE") == 0){
        printf("\n\n>> Hello, %s\n", param_token[4]);
        printf(">> You Logged In Successfully!\n");

        sprintf(token, "%s", param_token[1]);
        token[32] = '\0';
        user_id = atoi(param_token[2]);
        last_message = atoi(param_token[5]);
    }else{
        printf("\n\n%s\n", param_token[1]);
        j++;if(j < 3){goto retry_login;}
    }
}

void *check_message(){
    while(1){
        Sleep(2000);
        if(strcmp(token, "") != 0){
            send_message(1);
        }
    }
    return NULL;
}

void send_message(int ignore_input){
    int i = 0;
    char buffer, url_path[256] = "", enc_txt[256] = "", response[10240];

    if(ignore_input != 1){
        printf("\n> ");
        while((buffer = getch())){
            if(buffer == 127 || buffer == 8){
                if(i > 0){
                    printf("\b \b");
                    i--;
                    input_msg[i+1] = '\0';
                }else{
                    printf(" \b");
                    input_msg[0] = '\0';
                }

                fflush(stdout);
                continue;
            }
            if((buffer == 13 && i > 0) || i > 254){
                break;
            }else if(buffer == 13 && i == 0){
                continue;
            }

            input_msg[i] = buffer;
            input_msg[i+1] = '\0';
            printf("%c", buffer);
            fflush(stdout);
            i++;
        }
    }

    // Process URL
    if(i > 0){
        urlencode(enc_txt, input_msg);
        sprintf(url_path, "/chat/service/transceiver.php?message=%s&token=%s&last=%d", enc_txt, token, last_message);
        memset(input_msg, 0, strlen(input_msg));
    }else{
        sprintf(url_path, "/chat/service/transceiver.php?token=%s&last=%d", token, last_message);
    }

    send_request(url_path, response);

    if(strcmp(response, "NOTHING") != 0){
        int param_count;
        sds param_sds, *param_token;

        param_sds = sdsnew(response);
        param_token = sdssplitlen(param_sds, sdslen(param_sds), "<:>", 3, &param_count);

        if(strcmp(param_token[1], "FAILED") == 0){
            printf("\n\n%s", param_token[1]);

            strcpy(token, "");
            user_id = -1;
            last_message = -1;

            user_login();
        }else{
            last_message = atoi(param_token[1]);
            printf("\r");
            for(i = 2; i < param_count; i++){
                if(strcmp(param_token[i], "") != 0){
                    if(i > 2){printf("\n");}
                    printf("%s", param_token[i]);
                }
            }
            if(ignore_input == 1){
                printf("\n> %s", input_msg);
            }
        }
    }
}

void WSA_Check(){
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
        printf("\n> Failed. Error Code : %d", WSAGetLastError());
    }
}

void hostname_to_ip(char *hostname, char *ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    // Check Socket Library
	WSA_Check();

	if((he = gethostbyname(hostname)) == NULL){
        printf("\n> Error: Failed to Get IP Address");
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++){
		sprintf(ip, "%s", inet_ntoa(*addr_list[i]) );
    }

    WSACleanup();
}

int connect_to_server(char *c_host, int c_port){

    int MySocket;
    struct sockaddr_in server;


    // Check Socket Library
	WSA_Check();

    //Create a socket
    if((MySocket = socket(AF_INET , SOCK_STREAM , 0)) == INVALID_SOCKET){
        printf("\n> Could Not Create Socket : %d" , WSAGetLastError());
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(c_host);
    server.sin_family = AF_INET;
    server.sin_port = htons(c_port);

    //Connect to remote server
    if(connect(MySocket, (struct sockaddr *)&server , sizeof(server)) < 0){
        printf("\n>Error: Connection Failed\n");
        printf("        Please Check Internet Connection!\n");
        return -1;
    }

    return MySocket;
}

int urlencode(char *dest, const char *src){
    char *d;
    int i;
    for(i=0, d=dest; src[i]; i++) {
        if(isalnum(src[i])) *(d++) = src[i];
        else {
            sprintf(d, "%%%02X", src[i]);
            d += 3;
        }
    }
    *d = 0;
    return d-dest;
}

int send_request(char *url, char *r_response){
    int i, Sock, BufferSize;
    char header[4096], response[10240];

    Sock = connect_to_server(ip, port);

    sprintf(header, "GET %s HTTP/1.1%s", url, new_line);
    sprintf(header, "%sHost: %s%s", header, host, new_line);
    sprintf(header, "%sUser-Agent: KhudroClient/0.0.1%s", header, new_line);
    sprintf(header, "%sConnection: close%s", header, new_line);
    sprintf(header, "%sAccept: text/html%s", header, new_line);
    sprintf(header, "%s%s", header, new_line);

    //printf("Request:\n%s\n", header);
    if(send(Sock, header, strlen(header), 0) < 0){
        printf("\n> Send Failed");
        return -1;
    }

    //Receive a reply from the server
    while((BufferSize = recv(Sock, response, 10240 , 0)) != '\0'){
        response[BufferSize] = '\0';
    }

    //printf("Response:\n%s\n", response);

    // SDS Library
    int param_count;
    char new_line2[4];
    sds param_sds, *param_token;

    sprintf(new_line2, "%s%s", new_line, new_line);
    param_sds = sdsnew(response);
    param_token = sdssplitlen(param_sds, sdslen(param_sds), new_line2, sizeof(new_line2), &param_count);

    strcpy(r_response, "");
    for(i = 1; i < param_count; i++){
        sprintf(r_response, "%s%s", r_response, param_token[i]);
    }

    //printf("\n>>\n%s\n<<\n", r_response);

    return 0;
}
