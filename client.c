#include<stdio.h> //pritnf
#include<string.h> //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h> //inet_addr
 #include<time.h>
 #define PORT 1919
	
int main( int argc , char *argv[]) {
	int sock; 
	struct sockaddr_in server ; 
	char message[1000], server_reply[2000]; 
	char ch1[1000]; 
	char ch10[2000];
	char c,c1; 
/* */int x; 
/* */ int y; 
	int n ; 
	int Z; 
	int maxx;
	int o;
	//Declaration de temps
	time_t currentTime; 
	time(&currentTime);
	struct tm *myTime=localtime(&currentTime); 
	printf("**Le temps d'entrée :\n-DATE:%i/%i/%i\n-TIME:%i:%i:%i\n",myTime->tm_mon +1,myTime->tm_mday,myTime->tm_year + 1900,myTime->tm_hour,myTime->tm_min,myTime->tm_sec);
	
	//create socket 
			 sock=socket(AF_INET,SOCK_STREAM,0); 
			if (sock==-1) {
				printf("Could not create socket"); 
			}
			puts("socket created"); 
			
			
			
				//prepare the sockaddr_in structure
				server.sin_addr.s_addr=inet_addr("127.0.0.1");// il faut changer cette adresse pour connecter au serveur  a distance ca veut dire dans le reseaux locale   
				     server.sin_family=AF_INET; 
					server.sin_port=htons(PORT); 
				
				
					 printf("\nVous-voulez quitter si oui taper 'o' si non taper 'n' \n");
					 scanf("%c",&c); 
					 if  (c=='o'){// Il faut ajouter majus pour tester 
					 close(sock); 
				  } else {
						
					//Connect to remote Server 
					if (connect(sock , (struct sockaddr *)&server,sizeof(server))<0) {
						perror("connect failed.Error"); 
						return 1;
					} 
					puts ("connected\n");  
						
					//keep communicating with server 
					
					if ( read(sock ,&n, sizeof(int) ) < 0) {
							puts("recv failed"); 
							
						}
/* temps de fin d'enchere en Minute*/read(sock , &y, sizeof(int) );


/* temps de fin d'enchere en seconde  */read(sock , &x, sizeof(int) );
printf("l'enchere se termine:\n");
                						printf("minute:%d\n",y);
				                 		printf("seconde:%d \n",x); 
						
						
                        read(sock,&maxx,sizeof(int));
						printf("\nLe mximum pour le momment est %d\n",maxx);
						
						
						// pour le time
						read(sock , ch10, 2000 );
						puts(ch10);
						
						
						
						
						if (((y==myTime->tm_min)&&(x==myTime->tm_sec))||((y==myTime->tm_min)&&(x<myTime->tm_sec))||(y<myTime->tm_min)) {
							
							printf("l'encher est terminé"); 
							close(sock);
							} else {
			
			
			
			
						printf("\nVous etes le client numero %d \n Merci d'ecrire votre prix:\n NB:Pour sortir taper exit \n",n); 
						scanf("%s",message); 
						if((strcmp(message,"exit"))==0){
							close(sock); 
						}else {
						
			
				
					
				
						//send some Data 
						if (send(sock,message , strlen(message),0)<0) {
							puts ("send failed"); 
							return 1 ; 
						}
			
					
						
						
						
						
					if ( read(sock ,&Z, sizeof(int) ) < 0) {
							puts("recv failed"); 
							
						} 
						printf("\t \tLE CLIENT %d est le gaggnant\n\n",Z);
						
						 
						
						
						
					
					close(sock); 
				}
			}
		}
		
					return 0;
}
