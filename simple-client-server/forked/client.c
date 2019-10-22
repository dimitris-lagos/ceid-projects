#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>                        
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>

void establish_socket(); //sunartisi gia tin sundesi tou socket
int check_connection(); //sunartisi gia na elegxoume an einai anoixtos o server kai dexetai dedomena
int sockjg; //i metavliti pou tha periexei to socket mas
struct sockaddr sock_add_kjg;
void catch_alarm(int sig_num); //dilwsi tis sunartisis pou an o pelatis mpei se anamoni, kathe 10sec tupwnei to antistoixo mnm
int zz = 0; //edw tha apothukeuetai to megethos tou onomatos tou xristi

void establish_socket(){ //Arxikopoihsi tou socket mas

	sockjg = socket (AF_UNIX, SOCK_STREAM, 0);
	if(sockjg < 0){
		printf("Client socket failure %d \n", errno);
		perror("\nSocket error: ");
		exit(0);
	           }

}  

int check_connection(){ //Sundesi ston server meso tou socket mas

	sock_add_kjg.sa_family=AF_UNIX;
	strcpy(sock_add_kjg.sa_data, "/tmp/unix.str");
	int conlength= (strlen(sock_add_kjg.sa_data) + sizeof(sock_add_kjg.sa_family));
	if (connect(sockjg, &sock_add_kjg, conlength) < 0) {
		printf("Client Connect Failure %d\n", errno);
		perror("Connect Error: "); 
		return 0;   }
	else{  
		return 1;                                              
		}
}
  
int main(int argc, char *argv[]) {
	if (argc < 1) //elgexos gia ta inputs
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

	int compare_id;
	int zonh = 0;
	char c[4];
	int i,x=0;
	compare_id = atoi(argv[1]);	
	int arith_thesewn=0, balance=0, cr_ok=0; //oi metavlites pou tha apothikeuetai i paraggelia tou pelati
	char zwni;
	
	int count=2;
    char id[5]="";
	int incoming2=0;
	int incoming=0; 
	int tmp=0;
	int kostos=0;
	signal(SIGALRM, catch_alarm);
	establish_socket();
	if(check_connection()){
		read(sockjg, &incoming, sizeof(incoming));
		if(incoming==0){//an einai 0 mpaineis anamoni, alliws
			alarm(10);
			read(sockjg, &incoming, sizeof(incoming));
			alarm(0);
			}
		else {
			read(sockjg, &incoming2, sizeof(incoming2));//2nd check
			}
		read(sockjg, &incoming, sizeof(incoming));//eite den exei theseis eite proxwrame stin paraggelia
		if(incoming==0){
			printf("Oles oi theseis einai kleismenes.");
			exit(1);
			}
}

	if(compare_id== 0) // an o client klithike me orisma 0, h eisodos tha ginei apo to pliktrologio
		{
				 //Eiswdos apo to pliktrologio           
				 while(1)
					   {
						printf("Poses thseseis thelete? \n" );
						scanf("%d",&arith_thesewn);
						if(arith_thesewn<=0 || arith_thesewn>4){ 
							printf("lathos eisodos.Parakalw prospatheiste ksana: \n");
							}
						else{ 
							break; //an i eisodos einai ok vges apo tin while
							}
					}
			   
			  
				 while(1){
					printf("Se poia zwni thelete ta eisitiria?Epiloges a,b,c,d :\n" ); 
					while (getchar() != '\n');//clear \n wait for char
					zwni=getchar();
					if(zwni == 'a' || zwni== 'b' || zwni =='c' || zwni =='d'){	
						break;
						}
				else {
					printf("Lathos eisodos.Parakalw prospatheiste ksana \n");
					}

						 }
				switch (zwni){
					case 'a':
						zonh = 1;
						tmp=50;
						break;		
					case 'b':
						zonh = 2;
						tmp=40;
						break;
					case 'c':
						zonh = 3;
						tmp=35;
						break;
					case 'd':
						zonh = 4;
						tmp=30;
						break;
				}
				printf("Dwste onoma pelati, max 4 characters:\n");
				scanf("%s",id);
				for(;;){
				if (strlen(id)>4){
					printf("Parakalw eisagete ksana:\n");
					scanf(" %[^\n]",id);
					}else {break;}	
				}
				srand (time(NULL));
				cr_ok=rand() % 100 + 1;
				if(cr_ok <90){
					cr_ok=1;
				}
				else{
					cr_ok=0;
				}
				zz=strlen(id);
		}	
	else {//an to orisma tis klisis tis client dn einai 0, to orisma ginetai arithmitiko tou onomatos tou pelati
			//kai i eisodos ginetai automata me random apo to sustima
			srand (time(NULL));
			arith_thesewn=rand() % 4+1;
			srand (time(NULL));
			cr_ok=rand() % 100 + 1;
			if(cr_ok <90){
				cr_ok=1;
				}
			else{
				cr_ok=0;
				}
				srand (time(NULL));	
				   zonh=rand() % 100 + 1;
				   srand (time(NULL));
					  zonh= rand() % 101;
					  if(zonh<=10){
						zonh=1;
						zwni='a';
						tmp=50;
					  }else if(zonh<=30){
						zonh=2;
						zwni='b';
						tmp=40;
					  }else if(zonh<=60){
						zonh=3;
						zwni='c';
						tmp=35;
					  }else{
						zonh=4;
						zwni='d';
						tmp=30;
					  }
/* Ston kwdika pou akolouthei, ginetai h prosthiki tou orismatos tis klisis tis client (./client xxx, x!=0) , san arithmitiko
xaraktiristiko tou onomatos tou pelati. O pelatis exei onoma Pxxx, opou x opoiosdipote tripsifios arithmos ektos apo to 0 */
			sprintf(c, "%d", compare_id); //metatrepetai arithmos/orisma se char
			x = strlen(c);	//metraw ta byte
			for(i=0;i<3;i++) //kanw shift ola ta byte deksia kata 1
			{	
				id[i+1]=c[i];
			
			}
			id[0] ='P';//stin prwti thesi vasw to P
			zz=strlen(id); //apothukeyw to megethos tou string, wste me write na to steilw ston server kai etsi
							//na apofugoume ta trash

		}
		write(sockjg, &zonh, sizeof(zonh));
		write(sockjg, &arith_thesewn, sizeof(arith_thesewn));
		write(sockjg, &cr_ok, sizeof(cr_ok));	
		write(sockjg, &zz, sizeof(zz));
		write(sockjg, &id, zz);
		read(sockjg, &incoming, sizeof(int));
		if(incoming==0){
			printf("Den uparxoun theseis stin zwni pou epile3ate.\n");
			exit(1);
		}
		else if(incoming==1){
			printf("Yparxei provlima me tin pistwtiki sas karta.\n");
			exit(1);
		}
		else if(incoming==2){
			kostos=arith_thesewn*tmp;
			printf("H kratisi pragmatopoihthike!\n");
			printf("Ta stoixeia einai:\n");
			printf("\t Onoma: %s \n",id);
			printf("\t Arith.Eisitiriwn: %d \n",arith_thesewn);
			printf("\t Zwni: %c \n",zwni);
			printf("\t Kostos: %d euro \n",kostos);
			exit(1);
		}
	
	close(sockjg);//kleinoume to socket
	
}
/* Sunartisi pou kathei 10sec an o client einai se anamoni, emfanizei to antistoixo minuma */
		void catch_alarm(int sig) {
		//signal(SIGALRM, SIG_IGN);
		printf("Call waiting has been activated. Please don't hung up unless you hear the busy tone!\n");
		fflush(stdout);
		signal(SIGALRM, catch_alarm);
		alarm(10);
        }
