#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

int findIPV(char* adresse){
    int compteur =0;
    int nb = 0 ;
    int nbMax = 0 ;
    int caractere=0;
    bool comparaison = true;

    while(comparaison && adresse[compteur]!=32 && adresse[compteur]!=3){
        if(adresse[compteur]>=48 && adresse[compteur]<=57){
            nb++;

        }else if(adresse[compteur]>=97 && adresse[compteur]<=102){
            nb++;
        }else if(adresse[compteur]==58){
            nb=0;
            caractere = 6;
            if(nbMax==0){
                nbMax = nb;
            }
        }else if (adresse[compteur]==46){
            nb=0;
            caractere = 4;

            if(nbMax==0){
                nbMax = nb;
            }
        }else{
            comparaison=false;
            nbMax=-1;
        }
        if(nb>nbMax){
            nbMax = nb;
        }
        compteur++;   
/*         printf("Compteur : %d \n",compteur);
        printf("nb : %d \n",nb);
        printf("nbMax : %d \n",nbMax);
        printf("caractere : %d \n",caractere); */
    }
    if(compteur>39 || nbMax>4 ){
        nbMax = -1;
    }else if(caractere==6 || compteur==39 || nbMax==4){
        if(caractere==4){
            nbMax=-1;
        }else{
            nbMax=6;
        }
    }else if(caractere=4 || compteur<16 || nbMax==3){
        if(caractere==6){
            nbMax=-1;
        }else{
            nbMax=4;
        }

    }
    return nbMax;
}

//159.31.64.185 192.168.54.1 192.168.150.1 

int main(int argc, char *argv[])
{
    char *adresse= argv[2];

    if(argc<4 || argc>5){
        perror("Manque d'argument");
        exit(EXIT_FAILURE);
    }
    int ipv =0;

    if(strcmp(argv[1],"-n")==0 && strcmp(argv[3],"-i")==0){
        ipv = findIPV(adresse);

        
        if(argc==4){
            perror("Manque d'argument");
            exit(EXIT_FAILURE);
        }
        else{
            struct sockaddr_in adress_sock;
            if(ipv==4){
                adress_sock.sin_family = AF_INET;
            }else if(ipv==6){
                adress_sock.sin_family = AF_INET6;
            }else{
                perror("Problème adresse");
                exit(EXIT_FAILURE);
            }
            adress_sock.sin_port = htons(4242);
            inet_aton(adresse,&adress_sock.sin_addr);

            int descr=socket(PF_INET,SOCK_STREAM,0);
            int r=connect(descr,(struct sockaddr *)&adress_sock,
            sizeof(struct sockaddr_in));
            if(r!=-1){
                int i = send(descr,argv[4],strlen(argv[4])*sizeof(char),0);
            }       
            char* message;   
            message= malloc(sizeof(char)*10000);
            int recu=read(descr,message,10000*sizeof(char));
            printf("Value : \n %s \n", message); 
            if(recu!=-1){
                char *send1="end";
                int i = send(descr,send1,strlen(send1)*sizeof(char),0);
            }               
        }
    }else if(strcmp(argv[1],"-n")==0 && strcmp(argv[3],"-a")==0){
        ipv = findIPV(adresse);

        if(argc==5){
            perror("Manque d'argument");
            exit(EXIT_FAILURE);
        }
        else{
            struct sockaddr_in adress_sock;
            if(ipv==4){
                adress_sock.sin_family = AF_INET;
            }else if(ipv==6){
                adress_sock.sin_family = AF_INET6;
            }else{
                perror("Problème adresse");
                exit(EXIT_FAILURE);
            }
            adress_sock.sin_port = htons(4242);
            inet_aton(adresse,&adress_sock.sin_addr);

            int descr=socket(PF_INET,SOCK_STREAM,0);
            int r=connect(descr,(struct sockaddr *)&adress_sock,
            sizeof(struct sockaddr_in));
            if(r!=-1){
                char *mess="-a";
                int i = send(descr,mess,strlen(mess)*sizeof(char),0);
            }       
            char* message;   
            message= malloc(sizeof(char)*10000);
            int recu=read(descr,message,10000*sizeof(char));
            printf("Value :\n %s \n", message); 
            if(recu!=-1){
                char *send1="end";
                int i = send(descr,send1,strlen(send1)*sizeof(char),0);
            }    
        }
    }else{
        perror("Les arguments n'existent pas");
        exit(EXIT_FAILURE);
    }
    return 0; 
}


