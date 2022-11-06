#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>


// Trouver si le nombre est 255, un 0 ou un autre nombre => IPV4
int findNumberIPV4(char* tabValeur, int compteur){
    if(*(tabValeur+compteur)==48){ return -1;
    }else if ( *(tabValeur+compteur)==50 && *(tabValeur+compteur+1)==53 && *(tabValeur+compteur+2)==53){  return 0;
    }else{ return 1;  }
}

// trouver la correspondance du nombre dans un tableau => IPV4 
int findCorrespondanceIPV4(char* tabValeur, int compteur){
    int i =0;
    int val =0;
    while(i<3 && val==0){
        if(*(tabValeur+compteur)==49){
            if(*(tabValeur+compteur+1)==50 && *(tabValeur+compteur+2)==56){
                val=0; //128;
            }else if(*(tabValeur+compteur+1)==57 && *(tabValeur+compteur+2)==50){
                val=1; //256;
            }else{
                val=-1;
            }
        }else if(*(tabValeur+compteur)==50){
            if(*(tabValeur+compteur+1)==50 && *(tabValeur+compteur+2)==52){
                val=2; //224;
            }else if(*(tabValeur+compteur+1)==52 && *(tabValeur+compteur+2)==48){
                val=3; //240;
            }else if(*(tabValeur+compteur+1)==52 && *(tabValeur+compteur+2)==56){
                val=4; //248;
            }else if(*(tabValeur+compteur+1)==53 && *(tabValeur+compteur+2)==50){
                val=5; //252;
            }else if(*(tabValeur+compteur+1)==53 && *(tabValeur+compteur+2)==52){
                val=6; //254;
            }else{
                val=-1;
            }
        }else{
            val=-1;
        }
        i++;
    }
    return val;
}

// Definir le prefixe => IPV4
int findPrefixeIPV4(char* masque){
    int tabChiffres[8]={1,2,3,4,5,6,7,8};
    int correspondance[8]={1,2,3,4,5,6,7,8};
    bool valeur = false;
    int val =0;
    int i =0;
    int compteurTour=0;
    int valueFindNumber =0 ;
    while(!valeur && masque[i]!=32 && i<15){
        valueFindNumber =findNumberIPV4(masque,i);
        for(int j = 0 ; j<8; j++){
            correspondance[j] = tabChiffres[j]+compteurTour*8;
        }
        if(valueFindNumber==0){
            val = correspondance[7];
            compteurTour++;
            i=i+4;
        }else if(valueFindNumber==-1){
            valeur= true;
        }else{
            int valueFindCorrespondance = findCorrespondanceIPV4(masque,i);
            if( valueFindCorrespondance==-1){
                perror("Probleme");
            }else{
                val = correspondance[valueFindCorrespondance];
                valeur = true;
            }
        }
    }
    return val;        
}

//Afficher les adresses ipv4
char* affichageIPV4(char* hostname, struct ifaddrs *addr){
    char caractere[11]    = {48,49,50,51,52,53,54,55,56,57,58};
    int s                 = getnameinfo(addr->ifa_addr,sizeof(struct sockaddr_in),hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    char hostname2[NI_MAXHOST];
    

    int w                 = getnameinfo(addr->ifa_netmask,sizeof(struct sockaddr_in),hostname2, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    int prefixe = findPrefixeIPV4(hostname2);
    char *message;
    message = malloc(sizeof(char)*200);
    sprintf(message, "\nInterface :  %s    Type : IPV4 \n Adresse : %s/%d  \n", addr->ifa_name, hostname, prefixe);
    return message;
}

// Trouver si le nombre est ffff, un 0, un : ou un autre nombre => IPV6
int findNumberIPV6(char* tabValeur, int compteur){
     if(*(tabValeur+compteur)==48 || *(tabValeur+compteur)==58 ){ return -1;
    }else if ( *(tabValeur+compteur)==102 && *(tabValeur+compteur+1)==102 && *(tabValeur+compteur+2)==102  && *(tabValeur+compteur+3)==102){  return 0;
    }else{ return 1;  }
}

// trouver la correspondance du nombre dans un tableau => IPV6
// 8000 c000 e000 f000 f800 fc00  fe000 ff00 ff80 ffc0 ffe0 fff0 fff8 fffc fffe ffff
int findCorrespondanceIPV6(char* tabValeur, int compteur){
     int i =0;
    int val =0;
    while(i<3 && val==0){
        if(*(tabValeur+compteur)==102){
            if(*(tabValeur+compteur+1)==102){
                if(*(tabValeur+compteur+2)==102){
                    if(*(tabValeur+compteur+3)==101){
                        val = 14; //fffe;
                    }else if (*(tabValeur+compteur+3)==99){
                        val = 13; //fffc;
                    }else if (*(tabValeur+compteur+3)==56){
                        val = 12; //fff8;
                    }else if (*(tabValeur+compteur+3)==48){
                        val = 11; //fff0;
                    }else{
                        val=-1;
                    }
                }else if (*(tabValeur+compteur+2)==101 && *(tabValeur+compteur+3)==48){
                    val=10; //ffe0;
                }else if (*(tabValeur+compteur+2)==99 && *(tabValeur+compteur+3)==48){
                    val=9; //ffc0;
                }else if (*(tabValeur+compteur+2)==101 && *(tabValeur+compteur+3)==48){
                    val=8; //ff80;
                }else{
                    val=-1;
                }
            }else if(*(tabValeur+compteur+1)==101 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
                val=6; //fe00;
            }else if(*(tabValeur+compteur+1)==99 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
                val=5; //fc00;
            }else if(*(tabValeur+compteur+1)==56 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
                val=4; //f800;
            }else if(*(tabValeur+compteur+1)==48 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
                val=3; //f00;
            }else{
                val=-1;
            }
        }else if(*(tabValeur+compteur)==101 && *(tabValeur+compteur+1)==48 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
            val=2; //e000;
        }else if(*(tabValeur+compteur)==99 && *(tabValeur+compteur+1)==48 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
            val=1; //c000;
        }else if(*(tabValeur+compteur)==56 && *(tabValeur+compteur+1)==48 && *(tabValeur+compteur+2)==48 && *(tabValeur+compteur+3)==48){
            val=0; //8000;
        }else {
            val=-1;
        }
        
        i++;
    }
    return val;
}

// Definir le prefixe => IPV6
int findPrefixeIPV6(char* masque){
    int correspondance[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    int tabChiffres[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    bool valeur = false;
    int val =0;
    int i =0;
    int compteurTour=0;
    int valueFindNumber =0 ;
    while(!valeur && masque[i]!=32 && i<39){
        for(int j = 0 ; j<16; j++){
            correspondance[j] = tabChiffres[j]+(compteurTour*16);
        }
        valueFindNumber =findNumberIPV6(masque,i);
        if(valueFindNumber==0){
            val = correspondance[15];
            compteurTour++;
            i=i+5;
        }else if(valueFindNumber==-1){
            valeur= true;
        }else{
            int valueFindCorrespondance = findCorrespondanceIPV6(masque,i);
            if( valueFindCorrespondance==-1){
                perror("Probleme");
            }else{
                val = correspondance[valueFindCorrespondance];
                valeur = true;
            }
        }
    }
    return val;           
}

//Afficher les adresses IPV6
char* affichageIPV6(char* hostname, struct ifaddrs *addr){
    int s                 = getnameinfo(addr->ifa_addr, sizeof(struct sockaddr_in6), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    char newHostname[NI_MAXHOST];
    int compteur =0 ;
    bool espace= false;
    while(compteur<68){
        if(hostname[compteur]==37){ espace=true; }

        if(espace){ newHostname[compteur] = 0;
        }else{ newHostname[compteur] = hostname[compteur];  }

        compteur++;
    }

    char hostname2[NI_MAXHOST];
    int w                 = getnameinfo(addr->ifa_netmask,sizeof(struct sockaddr_in6),hostname2, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    int prefixe = findPrefixeIPV6(hostname2);
    char *message;
    message = malloc(sizeof(char)*200);
    sprintf(message, "\nInterface :  %s    Type : IPV6 \n Adresse : %s/%d   \n\n", addr->ifa_name,newHostname, prefixe);
    return message;
}

int main(int argc, char *argv[])
{
    int sock=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in address_sock;
    address_sock.sin_family=AF_INET;
    address_sock.sin_port=htons(4242);
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    char* message;
    message = malloc(sizeof(char)*10000);
    int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
    if(r==0){
        r=listen(sock,0);
        while(1){
            struct sockaddr_in caller;
            socklen_t size=sizeof(caller);
            int sock2=accept(sock,(struct sockaddr *)&caller,&size);
            if(sock2>=0){
                char mess[38];
                int recu=read(sock2,mess,38*sizeof(char));

                if(mess[0]=='-' && mess[1]=='a'){
                    struct ifaddrs *addr, *intf;
                    char hostname[NI_MAXHOST];
                    int family, s;

                    if (getifaddrs(&intf) == -1) {
                        perror("getifaddrs");
                        exit(EXIT_FAILURE);
                    }
                    for (addr = intf; addr != NULL; addr = addr->ifa_next) {

                        family        = addr->ifa_addr->sa_family;
                        if (family == AF_INET ) {
                            strcat(message, affichageIPV4(hostname, addr));
                        }else if (family ==  AF_INET6) {
                            strcat(message, affichageIPV6(hostname, addr));
                        }
                    }
                }else{
                    struct ifaddrs *addr, *intf;
                    char hostname[NI_MAXHOST];
                    int family, s;

                    if (getifaddrs(&intf) == -1) {
                        perror("getifaddrs");
                        exit(EXIT_FAILURE);
                    }
                   for (addr = intf; addr != NULL; addr = addr->ifa_next) {
                        family        = addr->ifa_addr->sa_family;
                        if (family == AF_INET && strcmp(addr->ifa_name,mess)==0) {
                            strcat(message, affichageIPV4(hostname, addr));
                        }else if (family ==  AF_INET6 && strcmp(addr->ifa_name,mess)==0) {
                            strcat(message, affichageIPV6(hostname, addr));
                        }
                    }                   

                }  
                int send1 = send(sock2,message,strlen(message)*sizeof(char),0);
                char messa[3];
                int recu1=read(sock2,messa,3*sizeof(char));
                if(recu1==3 && messa[0]=='e' && messa[1]=='n' && messa[2]=='d'){
                    break;
                }         
            }
            close(sock2);
        }
    }
    return 0;
}
