# Enchere-en-ligne-
Multithreaded Auction Server
Pour compiler il faut mettre : 
gcc server.c -o server -pthread 
gcc client.c -o client 
RQ: on peut placer la ligne 156  				   pthread_mutex_lock(& mutex_stdout); dans la ligne 184 juste avant cette partie 
(read_size=recv(sock,client_message,2000,0)); 
				   		 n=atoi(client_message); 
				   		
							if (n>max) {
								max=n;
								s=q;
								}
				   	 global++;
				   	 
				   	 
				   	 pthread_mutex_unlock(& mutex_stdout);

pour qu'un utlisateur ne bloque pas tous les utilisateur a ecrire le prix ..

