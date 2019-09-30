#include <stdio.h>
#include "csapp.h"


/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";


/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
void doit(int fd);



int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    

    listenfd = Open_listenfd("8080");
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);
        
    }
    return 0;
}

int main1(int argc,char **argv){
    int clientfd;
    char *hostname, *port,buf[10000];
    if(argc !=3 ){
        fprintf(stderr,"usage: %s host port\n",argv[0]);
        exit(0);
    }
    hostname="localhost";
    port="8080";

    clientfd=Open_clientfd(hostname,port);
    Rio_writen(clientfd,"GET / HTTP/1.1\r\nHost: www.taobao.com\r\n\r\n",100);
    Rio_readn(clientfd,buf,10000);
    printf("****\n%s\n ****",buf);
    return 0;
}


void doit(int fd){
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char filename[MAXLINE],cgiargs[MAXLINE];
    

    /* read request line and header */
    if(!Rio_readn(fd,buf,200)){
        return; }
    printf("receive:%s",buf);  //print header
    sscanf(buf,"%s %s %s",method,uri,version);
    if (strcasecmp(method,"GET")){
        
        return;
    }
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    sprintf(buf,"%sServer: proxy\r\n",buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, 10);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, "text/html");
    Rio_writen(fd, buf, strlen(buf)); 
    printf("Response headers:\n");
    printf("%s", buf);

    Rio_writen(fd,"hello",10);
    return;
}