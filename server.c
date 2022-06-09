#include<stdio.h> 
#include<time.h>
#include<string.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include<pthread.h>
#include<stdlib.h>
#define PORT 1919
char ch10[2000];
int global=0;
int max=0;
int q=0;
int n=0; 
int x;
int y;
int s=0;

pthread_cond_t cond=PTHREAD_COND_INITIALIZER; //Initialisation de condition pour le mutex
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER; //Initialisation de condition pour le mutex
 pthread_mutex_t mutex_stdout = PTHREAD_MUTEX_INITIALIZER; 
void *connection_handler(void *); 
 void *test_mutex(void*);
 void *test_time(void*);
int main(int argc , char *argv[]) {
	char ch[1000]; 	
int nbreClient=0;

//Declaration du temps 
	time_t currentTime; 
	time(&currentTime);
	struct tm *myTime=localtime(&currentTime);
	
	
	// Affichage le temps de début et fin de l'enchére  
	printf("**l'enchere se commence le/à  :\n-DATE:%i/%i/%i\n-TIME:%i:%i:%i\n",myTime->tm_mon +1,myTime->tm_mday,myTime->tm_year + 1900,myTime->tm_hour,myTime->tm_min,myTime->tm_sec);
	sprintf(ch10, "l'enchere se termine le/à :\n-DATE:%i/%i/%i\n-TIME:%i:%i:%i\n",myTime->tm_mon +1,myTime->tm_mday,myTime->tm_year + 1900,myTime->tm_hour,myTime->tm_min+1,myTime->tm_sec);
	printf("**%s",ch10);
	
	
	
	//Initialiser ou presiser la fin de l'enchere x: c'est en minute , y c'est en seconde,dans notre cas on prend une seule minute    
	x=myTime->tm_min+1; 
	y=myTime->tm_sec; 
	
	//messagee pour le test !!a verifier printf("a message for the is only %d \n",x);
	
	
	
	int socket_desc , client_sock, c, read_size; 
	struct sockaddr_in  server,client; 
	int *new_sock; 
	
	
	
		//create socket 
			 socket_desc=socket(AF_INET,SOCK_STREAM,0); 
			if (socket_desc==-1) {
				printf("Could not create socket"); 
				
			}
			puts("socket created");
			
			 
			//prepare the sockaddr_in structure 
				     server.sin_family=AF_INET; 
					server.sin_addr.s_addr=htonl(INADDR_ANY); 
					server.sin_port=htons(PORT); 
					
					//Bind 
					if (bind(socket_desc,(struct sockaddr *)&server,sizeof (server))<0) { 
						perror("Bind failed Error"); 
						return 1; 
					}
					puts("Bind donne"); 
				
				
				
				
					//listen 
				   listen(socket_desc,5); 
				   
				   
				   //accept and incoming connection 
				   puts("waitting for incoming connection");
				   c=sizeof(struct sockaddr_in); 
				   
				
					   
					   
				   //accept connection forom an incoming client
				   
				   while ((client_sock=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
	//redeclaration du time a chaque fois que le client  connecte au serveur 				   
	time_t currentTime; 
	time(&currentTime);
	struct tm *myTime=localtime(&currentTime); 
	nbreClient++;// nombre de  client incremente 1 
	
	//si on depasse la fin de l'enchere la socket de serveur sera fermé  
	if (((x==myTime->tm_min)&&(y==myTime->tm_sec))||((x==myTime->tm_min)&&(y<myTime->tm_sec))||(x<myTime->tm_min)) {
		 printf("le temps est ecoulé :(( \n");
		close (socket_desc); 
		}else {
			// On affiche dans  notre serveur chaque fois le nombre actuel de client  + ladresse ip de client (machine) + SocketId
	printf("\n|||||||||||||||| Le nombre de client pour le moment: %d\n ||||||||||||||||",nbreClient);
	printf ("from %s %d SocketId =%d\n", inet_ntoa(client.sin_addr), client.sin_port,client_sock);
					puts("connection accepted");
					
					 
				   	pthread_t Thr1;//Thread numero1 pour chaque Client 
				   	pthread_t Thr2;//Thread  pour  eviter le probleme de concurence  entre les threads là on utilise les mutexs 
				   	pthread_t Thr3; //Thread pour calculer le temps acctuel , on a utilisé ce thread pour mettre fin a l'enchere
				   	new_sock=malloc(1); 
				   	*new_sock=client_sock;
if ((pthread_create(&Thr1,NULL,connection_handler,(void *)new_sock)<0)) 
 {
				   		perror("could not creat thread"); 
				   		return 1 ; 
					   }
					   
					   if (pthread_create(&Thr2,NULL,test_mutex,(void *)new_sock)<0 ){
						   perror("could not creat thread"); 
				   		return 1 ;
						   }
						    if (pthread_create(&Thr3,NULL,test_time,(void *)new_sock)<0 ){
						   perror("could not creat thread"); 
				   		return 1 ;
						   }
					  
					   
					   //affichage pour le test et mieux comprendre le sujet de probleme
					   printf("\n Thread1 numéro %d\n",global); 
					   printf(" Le Maximum pour le moment est %d\n",max); 
					   puts("handler assigned ");
					    
					  }
					  if (client_sock<0) {
					  	perror("accept failed"); 
					  	return 1 ; 
					  } 
					 
					 
				}     
				   					  return 0; 
 
				   
}				   
				   
				  /* */ void *connection_handler(void*socket_desc){
				   	int sock=*(int *)socket_desc; 
				   	int read_size; 
				   	char client_message[2000];
				    int Z=0;
				   	int n;
				   pthread_mutex_lock(& mutex_stdout);
				   	 // Affichage pour le test  
				   	 q++;
				   	  printf("\n Nombre de client est %d\n",q);
				   	  
				  
				 //L'envoi De nombre de client ca veut dire coté client on va l'utiliser pour specifier le numero de client 	  	 
				   	 write(sock,&q,sizeof(int));
				   	 
				  /* Time x=Minute de fin d'enchere*/write(sock,&x,sizeof(int));
				  
				   	  /*Time y=Secondes de fin d'ecnhere */write(sock,&y,sizeof(int));
				   	   
				   	 /* l'envoie de max actuel a chaque client connecté au serveur*/ write(sock,&max,sizeof(int));
				   	 
				   	  	 /* L'envoi de Date,TIME en chaine de caractere */  
				   	 send(sock, ch10,strlen(ch10),0);
				  
			
				// recevoire le message du client  
				   	
			
				   		//solution de concurence avec mutex
				   		
				   		
				   		
				   		
				   		
	 
				   		
				   		(read_size=recv(sock,client_message,2000,0));
				   		 n=atoi(client_message); 
				   		
							if (n>max) {
								max=n;
								s=q;
								}
				   	 global++;
				   	 
				   	 
				   	 pthread_mutex_unlock(& mutex_stdout);
				   	 
				   	 //on peut utiliser au lieux de mutex mais  cest une grosse faute ..sleep(1);
				   	 
				   	 
				   	 // 
				   	 pthread_mutex_lock(& mutex_stdout); 
						   pthread_cond_wait(& cond1,& mutex_stdout);
						 pthread_mutex_unlock(& mutex_stdout);
				   	  //if (Test==1) {
				   	  pthread_mutex_lock(& mutex_stdout); 
				   	  pthread_cond_broadcast(& cond); 
				  
					 pthread_mutex_unlock(& mutex_stdout);
						/*}*/ 
						
					 
					   if (read_size==0) {
					   	puts("Client Disconnected"); 
					   	fflush(stdout); 
					   }
					   else if (read_size==-1) {
					   	perror("recv failed"); 
					   }
					  
					  
				   }
			   
			   
				   void *test_mutex(void* socket_desc){
					   int sock=*(int *)socket_desc; 
					   printf ("\n|||||||||Le Thread 2 Avant l'autorisation |||||||||\n");  
						   
					       pthread_mutex_lock(& mutex_stdout); 
						   pthread_cond_wait(& cond,& mutex_stdout);
						 pthread_mutex_unlock(& mutex_stdout);
						 printf ("\n|||||||||Le Thread 2 apres l'autorisation |||||||||\n");  
					   write(sock, &s,sizeof(int));
					   
				   }
					  
					  void *test_time(void* socket_desc){
					   int sock=*(int *)socket_desc; 
					   time_t currentTime; 
	                   time(&currentTime);
	                   struct tm *myTime=localtime(&currentTime);
					   printf ("\n|||||||||Le Thread 3 Avant l'autorisation |||||||||\n");  
					   while (!((x==myTime->tm_min)&&(y==myTime->tm_sec))||((x==myTime->tm_min)&&(y<myTime->tm_sec))||(x<myTime->tm_min)) {
						   time_t currentTime; 
	                       time(&currentTime);
	                       struct tm *myTime=localtime(&currentTime);
						    } 
						    //Test=1;
						   
					       pthread_mutex_lock(& mutex_stdout); 
						   pthread_cond_broadcast(& cond1);
						 pthread_mutex_unlock(& mutex_stdout);
						 printf ("\n|||||||||Le Thread 2 apres l'autorisation |||||||||\n");  
					   
					   
				   } 
				   
				   
						

