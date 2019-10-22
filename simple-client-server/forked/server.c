#include <semaphore.h>
#include <sys/types.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/un.h> 
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sched.h>
#include <assert.h>

#define SHM_KEY  IPC_PRIVATE
#define SHM_SIZE 8192
#define SEM_THLEFWNITES "sem_thlefwnites"
#define SEM_BANK "sem_bank"
#define SEM_NAME "sem_gg"

int shm_id, error;

sem_t *sem_thlefwnites; //semaforos gia tous thlefwnites
sem_t *sem_bank;

struct sockaddr sock_add_kjg, sec_sock_add;
int conlenght;

int pid;
pid_t parent_pid;
int sockjg, secsock;  /* Metablites gia kataxwrisi tou socket pou tha xrisimopoihthei  */
struct my_struct *pointer; //o pointer pou tha deixnei tin thesi mnimis stin opoia vrisketai to struct mas
void sig_chld(int signo);  // synartisi gia na svini tis  zombie synartiseis
void kill_server(); //sunartisi pou kleinei ton server kai termatizei ta child processes
void catch_alarm(int sig_num);
int const no_operand=0;
int const operand=1;
int const all_ok=2;
int const A=50;
int const B=40;
int const C=35;
int const D=30;
void sigquit_handler (int sig);
int i=0,j=0;
int temp=0;
float temp2=0.0;

 

struct my_struct // dilwsi tou struct me tous pinakes pou tha exoun tin diathesimotita psistwn, dianomewn, sunolikwn thesewn pou apomenoun kai zwnes thesewn
{
int thlefwnites;//arithmos diathesimwn thlefwnitwn
int bank[4];//pinakas me tous 4 diathesimous bank server, 1 thesi gia kathe server. 0 free /1`bussy
int bank_ok[4][2];  //[i][0]=(1 egine elegxos/0 den exei ginei akoma elegxos) [i][1]=(1 karta ok/ 0 karta not ok) 
int thesis;//temp
int a_thesewn;// arithmos diathesimwn thesew, arxika 640
char zwni_A[100][5]; //to onoma tou pelati mporei na einai P1...P100, mazi me to end '\0' einai to poly 5byte
char zwni_B[130][5]; //se kathe mia apo tis thesis tou pinaka apothukeuetai to onoma tis kratisis
char zwni_C[180][5];
char zwni_D[230][5];
int theatro_money, bank_money;// xrimata logariasmou theatrou, trapezas
int cntA, cntB, cntC, cntD; //counter pou o kathenas deixnei tin epomeni eleutheri thesi tis zwnis
int t_anamonis, t_eksupiretisis;//sunolikos xronos anamonis, eksupiretisis
int counter1, counter2, counter3, counter4; // counter4 metraei posoi sundeontai me thlefwniti, counter3 gia na metraei posoi pelates apotugxanoun na klisoun eisitiria, counter2 gia na metraei posoi pelates mpainoun stin anamoni, counter1 gia ton sunoliko arithmo pelatwn
int moneymoneymoney[600];//pinakas apothikeusis twn trapezikwn metaforwn
int moneycounter; //deixnei tin epomeni thesi tou pinaka
};


int main(void){ /*kentriki sunartisi main*/
	
	parent_pid=getpid();
	signal(SIGINT, kill_server); 
	signal(SIGALRM, catch_alarm);
	signal(SIGQUIT, sigquit_handler);
	time_t rawtime;
	struct tm * timeinfo;
	/*Ektupwse tin wra ekkinisis tou server */
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Server started at: %s", asctime (timeinfo) );
	printf ( "Kleiste ton server pantwntas CNTRL+C, gia na ektupwsete ta apotelesmata\n" );
	printf("size of int= %d\n",sizeof(int));
	sockjg = socket(AF_UNIX, SOCK_STREAM, 0); /*dimiourgoume to socket epikoinonias*/
	unlink("/tmp/unix.str"); /*diagrafoume gia asfalia to proigoume socket(an iparxei)*/
	bzero( &sock_add_kjg, sizeof(sock_add_kjg));
	sock_add_kjg.sa_family = AF_UNIX; /*dilonoume ton tupo tou socket pou xrisimopoioume*/
	strcpy(sock_add_kjg.sa_data, "/tmp/unix.str"); /*dilonoume to path pou dialeksame gia tin epikoinonia*/
	conlenght = strlen(sock_add_kjg.sa_data) + sizeof(sock_add_kjg.sa_family);
	
	shm_id = shmget(SHM_KEY, sizeof(struct my_struct), 0600 | IPC_CREAT);   // dimiourgia koinis mnimis gia tis paragelies
	if (shm_id == -1) {
	   printf("Could not create shared memory 1!\n");
	   exit(1);
	}

	shmctl(shm_id, IPC_RMID, NULL);
	shm_id = shmget(SHM_KEY, sizeof(struct my_struct), 0600 | IPC_CREAT);  

	pointer = (struct my_struct *) shmat(shm_id, NULL, 0);  /* syndesi tin tinis mnimis  */
	if (pointer == (struct my_struct *)-1) {
		   printf("Could not attach to shared memory 3!\n");
		   exit(1);
	}


	sem_thlefwnites=sem_open(SEM_THLEFWNITES, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1); //edw dimiourgw ton semaforo gia tous psistes
	if (sem_thlefwnites == SEM_FAILED) {
		printf("Could not open semaphore!\n");
		exit(1);
	}


	sem_bank=sem_open(SEM_BANK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1); //edw dimiourgw ton semaforo gia tous dianomeis
	if (sem_bank == SEM_FAILED) {
	  printf("Could not open semaphore!\n");
	  exit(1);
	}
		if (bind(sockjg, &sock_add_kjg, conlenght) < 0) {/*bind tou server sto socket*/
	  printf("Server Bind Failure %d\n", errno); /*elegxos la8ous*/
	  perror("\nBind Error: ");
	  exit(1);
		}
		
	if (listen(sockjg, 20) < 0) { /*o server "akouei" sundeseis se auto to socket*/
	  printf("Server Listen Failure %d\n", errno);/*elegxos la8ous*/
	  perror("\nListen Error: ");
	  exit(1);
		}
		
		/*Arxikopoihsi tou my_struct */
	(*pointer).a_thesewn=640;
	(*pointer).thlefwnites=10;
	(*pointer).cntA=0;
	(*pointer).cntB=0;
	(*pointer).cntC=0;
	(*pointer).cntD=0;
	(*pointer).counter1=0; 
	(*pointer).counter2=0; 
	(*pointer).counter3=0;
	(*pointer).counter4=0;
	(*pointer).theatro_money=0;
	(*pointer).bank_money=0;
	(*pointer).t_anamonis=0;
	(*pointer).t_eksupiretisis=0;
	(*pointer).moneycounter=0;
	for (i=0;i<=3;i++){
		(*pointer).bank[i]=0;
		for (j=0;j<=1;j++){
			(*pointer).bank_ok[i][j]=0;
			}
	}
	for(i=0;i<100;i++){
		for(j=0;j<4;j++){
			(*pointer).zwni_A[i][j]='\0';
			}
		}
	for(i=0;i<130;i++){
		for(j=0;j<4;j++){
			(*pointer).zwni_B[i][j]='\0';
			}
		}
	for(i=0;i<180;i++){
		for(j=0;j<4;j++){
			(*pointer).zwni_B[i][j]='\0';
			}
		}
	for(i=0;i<230;i++){
		for(j=0;j<4;j++){
			(*pointer).zwni_B[i][j]='\0';
			}
		}
	alarm(10); //kaleitai i sunartisi pou kathe 30sec metaferei ta xrimata ston logariasmo
		
	while(1){
		secsock = accept(sockjg, &sec_sock_add, &conlenght);/*o server dexetai sundeseis se auto to socket*/
		if (secsock < 0) { /*elegxos la8ous*/
		  printf("Server Accept Failure %d\n", errno);
		  perror("\nAccept Error: ");
		  exit(1);
			}
		pid=fork();
		if (pid==0){ /*klisi tis forc gia dimiourgia tis diergasias paidiou, server instance*/

			int timediff1, timediff2; //metavliti pou apothikeuetai i diafora tou xronou apo tin lipsi mexri to peras tis kratisis
			int  zwni, arith_thesewn, cr_ok, balance;
			char id[5]=""; 
			int bnk=0;
			time_t tv1, tv2; //metavlites gia tin apothukeusi tou xronou anamonis
			time_t  tm3, tm4; //metavlites gia tin apothukeusi tou xronou e3upiretisis
			int tmp=0;
			int tmp2=0;
			int tmp3=0;
			pid_t pid2;
			int size=0;
				
				close(sockjg);  // kleinoume to socket 

			/* EDW KSEKINAEI O KWDIKAS */ 

			sem_wait(sem_thlefwnites); //anoigw ton semaforo gia na elen3w an uparxei diathesimos thlefwnitis
			(*pointer).counter1++; //au3anw tous sunolikous pelates kata 1
			if((*pointer).thlefwnites>0){//an vrethei thlefwnitis
				(*pointer).thlefwnites--; //meiwse kata 1 tous diathesimous thlefwnites
				(*pointer).counter4++; //au3anw tous pelates pou kataferan na sundethoun me thlefwniti kata 1
				sem_post(sem_thlefwnites); //kleinw ton semaforo
				write(secsock, &operand, sizeof(int)); //steile ston client mnm oti sundeetai me tilefwniti
				}
			else{ //an den uparxei diathesimos thlefwnitis
				(*pointer).counter2++; //au3anw ton counter pou metraei posoi pelates mpainoun stin anamoni
				sem_post(sem_thlefwnites); //kleinw ton semaforo
				write(secsock, &no_operand, sizeof(int));  //enimerwse ton client oti mpainei se anamoni
				 tv1=time(NULL); //ksekinaw to xronometro gia ton sunoliko xrono anamonis
				int f=0;
				while(f==0){ //oso den uparxoun diathesimoi thlefwnites
					sem_wait(sem_thlefwnites);//anoigw pali ton semaforo gia na 3anaelegthei i sunthiki tis while
					if((*pointer).thlefwnites>0){
						tv2=time(NULL);//stamataw to xronometro
						timediff1=tv2-tv1; //upologizw se deuterolepta ton xrono
						(*pointer).counter4++; //au3anw tous pelates pou kataferan na sundethoun me thlefwniti kata 1
						(*pointer).t_anamonis+= timediff1; //apothikeuw tin timi stin mnimi tou server
						(*pointer).thlefwnites--; //meiwse kata 1 tous diathesimous thlefwnites
						sem_post(sem_thlefwnites); //kleinw ton semaforo
						f=1;
						}
					sem_post(sem_thlefwnites); //kleinw ton semaforo
				}
			}	
			
			tm3=time(NULL); //ksekinaw to xronometro gia ton xrono e3upiretisis
			write(secsock, &operand, sizeof(int)); //steile ston client mnm oti sundeetai me tilefwniti
			sem_wait(sem_thlefwnites); //anoigw ton semaforo
			if((*pointer).a_thesewn<=0){ //an oles oi theseis einai kleismenes
				(*pointer).thlefwnites++;//kanw pali diathesimo ton thlefwniti
				(*pointer).counter3++; //au3anw ton sunoliko arithmo twn pelatwn pou apotuxan na kanoun kratisi
				sem_post(sem_thlefwnites); //kleinw ton semaforo
				write(secsock, &no_operand, sizeof(int));/*steile mnm ston client oti den uparxoun theseis */	
				exit(0);
				}
			else{//eite proxwraei stin paraggelia
				sem_post(sem_thlefwnites); //kleinw ton semaforo
				write(secsock, &operand, sizeof(int)); //enimerwse ton client oti mporei na dextei paraggelia

				  //diavazoume to input pou esteile o client
				  read(secsock, &zwni, sizeof(int));
				  read(secsock, &arith_thesewn, sizeof(int));
				  read(secsock, &cr_ok, sizeof(int));
				  read(secsock, &size, sizeof(int));
				  read(secsock, &id, size);
				  if(arith_thesewn<=4){
					tmp2=arith_thesewn;}
			  }
			  int k=0;
			while(k==0){//edw ginetai elegxos gia na vrethei diathesimo bank server
				sem_wait(sem_bank);//anoigw ton semaforo gia tous bank server
				for(bnk=0;bnk<=3;bnk++){
					if((*pointer).bank[bnk]==0){//an vrethei diathesimos bank server
						(*pointer).bank[bnk]=1;//kanton mh diathesimo
						k=1;//kane to k 1 gia na vgei apo tin while
						sem_post(sem_bank);//kleinw ton semaforo
						break;//vges apo tin for
						}
					sem_post(sem_bank);//kleinw ton semaforo
					}
				}

			pid=fork(); //fork gia na psaxnei tautoxrona o thlefwnitis gia a_thesewn kai na elegxei ta stoixeia tis pistwtikis
			if (pid==0){
					if(cr_ok==1 ){ //an einai ok i pistwtiki
						usleep(2000000); //sleep for 2sec
						sem_wait(sem_bank);
						(*pointer).bank_ok[bnk][1]=1; //i karta einai ok
						(*pointer).bank_ok[bnk][0]=1; //enimerwse oti egine elegxos
						sem_post(sem_bank);
						}
					else if(cr_ok==0){//an i pistwtiki den einaiok
						usleep(2000000); //sleep for 2sec
						sem_wait(sem_bank);
						(*pointer).bank_ok[bnk][1]=0; //i karta einai den einai ok
						(*pointer).bank_ok[bnk][0]=1;//enimerwse oti egine elegxos
						sem_post(sem_bank);
						}
						exit(0); //teleiwse i douleia tis fork, termatise to child
					}//fork2
			sem_wait(sem_thlefwnites);	//anoigw semaforo gia elegxo thesewn
			if(zwni ==1){ //A zwni
				if((*pointer).cntA + arith_thesewn > 100){ //an den uparxoun arketes a_thesewn stin zwni A
					tmp=0;
					}
				else{tmp=1;//alliws vale 1
					}
				}	
			if(zwni ==2){ //B zwni
				if((*pointer).cntB+ arith_thesewn >130){ //an den uparxoun arketes a_thesewn stin zwni B
					tmp=0;
					}
				else{tmp=1;
					}
				}	
			if(zwni ==3){ //C zwni
				if((*pointer).cntC + arith_thesewn > 180){ //an den uparxoun arketes a_thesewn stin zwni C
					tmp=0;
					}
				else{tmp=1;
					}  
				}
			 if(zwni ==4){ //D zwni
				if((*pointer).cntD + arith_thesewn > 230){ //an den uparxoun arketes a_thesewn stin zwni D
					tmp=0;
					}
				else{tmp=1;
					}
				}
				sem_post(sem_thlefwnites); //kleinw semaforo apo elegxo thesewn

					
				int pl=0;
				while(pl==0){//edw ginetai elegxos gia na vrethei diathesimo bank server
					sem_wait(sem_bank);//anoigw ton semaforo gia tous bank server
					if((*pointer).bank_ok[bnk][0]==1){ //oso den exei vrethei bank server perimene
						sem_post(sem_bank);
						pl=1;
						break;
						}
					sem_post(sem_bank);//kleinw ton semaforo
					}
				usleep(6000000); //perimene 6sec
				if(tmp!=1){ //an den uparxoun theseis stin antistoixi zwni enimerwse analoga ton client
					write(secsock, &no_operand, sizeof(int)); //send 0
					tm4=time(NULL); //stamataw to xronometro gia ton xrono e3upiretisis
					sem_wait(sem_thlefwnites);
					timediff2=tm4-tm3; //upologizw se deuterolepta ton xrono
					(*pointer).t_eksupiretisis+= timediff2; //apothikeuw tin timi stin mnimi tou server
					(*pointer).counter3++;//au3anw twn arithmo twn apotuxiwn
					(*pointer).thlefwnites++;//epistrefw ton thlefwniti
					sem_post(sem_thlefwnites);
					sem_wait(sem_bank);
					(*pointer).bank_ok[bnk][1]=0; //reset to ok sima gia tin pistwtiki
					(*pointer).bank_ok[bnk][0]=0; //reset to sima gia to an exei ginei trapezikos elegxos
					(*pointer).bank[bnk]=0; //kane pali diathesimo to bank server
					sem_post(sem_bank);
					tmp2=0;
					tmp3=0;
					exit(0);//vges apo to programma
					}
				else if((*pointer).bank_ok[bnk][1]==0){ //an i pistwtiki exei provlima enimerwse analoga ton client
					sem_post(sem_bank);
					write(secsock, &operand, sizeof(int)); //send 1
					tm4=time(NULL); //stamataw to xronometro gia ton xrono e3upiretisis
					sem_wait(sem_thlefwnites);
					timediff2=tm4-tm3; //upologizw se deuterolepta ton xrono
					(*pointer).t_eksupiretisis+= timediff2; //apothikeuw tin timi stin mnimi tou server
					(*pointer).counter3++; //au3anw ton arithmo twn apotuxiwn
					(*pointer).thlefwnites++;//epestrepse ton thlefwniti
					sem_post(sem_thlefwnites);
					sem_wait(sem_bank);
					(*pointer).bank_ok[bnk][1]=0; //reset to ok sima gia tin pistwtiki
					(*pointer).bank_ok[bnk][0]=0; //reset to sima gia to an exei ginei trapezikos elegxos
					(*pointer).bank[bnk]=0; //kane pali diathesimo to bank server
					sem_post(sem_bank);
					tmp2=0;
					tmp3=0;
					exit(0);//vges apo tin fork
					}
				else if((*pointer).bank_ok[bnk][1] == 1 && tmp == 1 && (*pointer).a_thesewn >0){//an ola einai ok proxwrame stin kratisi
					sem_post(sem_thlefwnites);
					sem_post(sem_bank);
					write(secsock, &all_ok, sizeof(int));//send 2
					sem_wait(sem_thlefwnites);
					if(zwni ==1){
						for(arith_thesewn;arith_thesewn>0;arith_thesewn--){
							strncpy((*pointer).zwni_A[(*pointer).cntA], id, size); //pragmatopoihse tin kratisi me to id tou pelati stis antistoixes theseis
							(*pointer).cntA++;
							(*pointer).a_thesewn--;//meiwnw ton sunoliko arithmo thesewn
							tmp3=50;
							}
					}
					else if(zwni ==2){
							for(arith_thesewn;arith_thesewn>0;arith_thesewn--){
								strncpy((*pointer).zwni_B[(*pointer).cntB], id, size); //pragmatopoihse tin kratisi me to id tou pelati stis antistoixes theseis
								(*pointer).cntB++;
								(*pointer).a_thesewn--;//meiwnw ton sunoliko arithmo thesewn
								tmp3=40;
								}
						}
					 else if(zwni ==3){
							for(arith_thesewn;arith_thesewn>0;arith_thesewn--){
								strncpy((*pointer).zwni_C[(*pointer).cntC], id, size); //pragmatopoihse tin kratisi me to id tou pelati stis antistoixes theseis
								(*pointer).cntC++;
								(*pointer).a_thesewn--;//meiwnw ton sunoliko arithmo thesewn
								tmp3=35;
								}
						}
					else if(zwni ==4){
							for(arith_thesewn;arith_thesewn>0;arith_thesewn--){
								strncpy((*pointer).zwni_D[(*pointer).cntD], id, size); //pragmatopoihse tin kratisi me to id tou pelati stis antistoixes theseis
								(*pointer).cntD++;
								(*pointer).a_thesewn--;//meiwnw ton sunoliko arithmo thesewn
								tmp3=30;
								}
						}
					
					tm4=time(NULL); //stamataw to xronometro gia ton xrono e3upiretisis
					timediff2=tm4-tm3; //upologizw se deuterolepta ton xrono
					(*pointer).t_eksupiretisis+= timediff2; //apothikeuw tin timi stin mnimi tou server
					(*pointer).thlefwnites++;//epestrepse ton thlefwniti
					sem_post(sem_thlefwnites);
					sem_wait(sem_bank);
					(*pointer).bank_ok[bnk][1]=0; //reset to ok sima gia tin pistwtiki
					(*pointer).bank_ok[bnk][0]=0; //reset to sima gia to an exei ginei trapezikos elegxos
					(*pointer).bank[bnk]=0; //kane pali diathesimo to bank server
					(*pointer).theatro_money += (tmp2 * tmp3); //vale ston logariasmo tou theatrou ta lefta
					sem_post(sem_bank);
				}
				else {write(secsock, &operand, sizeof(int));//send 1
						}
				//sem_post(sem_thlefwnites);
				//sem_post(sem_bank);
				
				
			    signal( SIGCHLD, sig_chld );  // apofigi ton zobie
		
			exit(1);
		}//fork server instance
	}//while(1)
	close(secsock); /*klinoume to socket tou child*/
	sem_close(sem_thlefwnites);
	sem_close(sem_bank);
	sem_unlink(SEM_BANK);
	sem_unlink(SEM_THLEFWNITES);
	if(shmdt(pointer)==-1){ 
		printf("H koini mnimi dn katafere na apodesmeutei.\n"); 
		exit(1);
			}
	kill(getppid(), SIGINT);
	}//main
		   	 
			 
			 /* Avoid "zombie" child-proccesses */
	void sig_chld(int signo){
			   signal( SIGCHLD, sig_chld );
			   pid_t pid;
			   int stat;
			   while( (pid=waitpid(-1,&stat,WNOHANG)) > 0){
			   }
}

	void kill_server(){ /* Sunartisi pou kaleite me cntrl + c kai klinei swsta ton server kai tha ektupwnei ta apotelesmata*/
		kill(-parent_pid, SIGQUIT); //kleise ola ta process ektos apo ton parent
		int i=0;
		int m=0;
		int n=0;
		int p=0;
		printf("\nZwni A: [");
		for(i=0;i<100;i++){
			printf("%.4s, ",(*pointer).zwni_A[i]);
			}
		printf("]\nZwni B: [");
		for(m=0;m<130;m++){
			printf("%.4s, ",(*pointer).zwni_B[m]);
			}
		printf("]\nZwni C: [");
		for(n=0;n<180;n++){
			printf("%.4s, ",(*pointer).zwni_C[n]);
			}
		printf("]\nZwni D: [");
		for(p=0;p<230;p++){
			printf("%.4s, ",(*pointer).zwni_D[p]);
			}
		printf("]\n");
		if((*pointer).counter1!=0 && (*pointer).counter3!=0 && (*pointer).counter2!=0){//an dn exoume diairesi me to 0
			temp2=(100.0f * (*pointer).counter3 / (*pointer).counter1);
			printf("To pososto apotuximenwn paraggeliwn se sxesi me tis sunolikes: %.1f ",temp2);
			puts("%");
			temp2=(1.0f * (*pointer).t_anamonis / (*pointer).counter2);
			printf("O mesos xronos anamonis: %2.2f sec\n",temp2);
			temp2=(1.0f *(*pointer).t_eksupiretisis / (*pointer).counter4);
			printf("O mesos xronos eksupiretisis: %.2f sec\n",temp2);
			printf("To sunolo xrimatwn: %d \n",(*pointer).bank_money);
			printf("Oi metafores apo ton logariasmo tou theatrou ston logariasmo tis trapezas einai:\n");
			for(i=0;i<(*pointer).moneycounter;i++){
				printf("%d \n",(*pointer).moneymoneymoney[i]);
				}
		}
		close(sockjg); /* Kleinei to arxiko socket kai epikoinonias */
		close(secsock);/*Kleinei kai to deutero socket*/
		if(shmdt(pointer)==-1){ 
			printf("H koini mnimi dn katafere na apodesmeutei.\n"); 
			exit(1);
		}
		sem_post(sem_thlefwnites);
		sem_post(sem_bank);
		shmctl(shm_id, IPC_RMID, NULL); /*Diagrafi koinis mnimis*/
		sem_close(sem_thlefwnites);
		sem_close(sem_bank);
		sem_unlink(SEM_THLEFWNITES);
		sem_unlink(SEM_BANK);
		kill(0, SIGKILL);//ok twra kleista ola

		printf("\nO server ekleise epituxws!\n"); //den ftanei pote edw mias kai i parapanw kill ta kleinei ola
		exit(0); //
	}
	void sigquit_handler (int sig) { //sunartisi gia na kleinoun mono ta paidia me to cntrl+c kai to parent na mporei na ektupwnei ta apotelesmata prin kleisei
		assert(sig == SIGQUIT);
		pid_t self = getpid();
		if (parent_pid != self) _exit(0);
	}
	/* Sunartisi pou kathei 30 sec kanei tin metafora xrimatwn ston trapeziko logarismo */
	void catch_alarm(int sig) {
		//signal(SIGALRM, SIG_IGN);//apenergopoihse ta singals
		sem_wait(sem_bank);
		(*pointer).moneymoneymoney[(*pointer).moneycounter]=(*pointer).theatro_money; //apothykeusi tou posou gia na ektupwthei sto telos
		(*pointer).moneycounter++;
		(*pointer).bank_money += (*pointer).theatro_money; //metafora xrimatwn ston trapeziko logariasmo
		(*pointer).theatro_money=0; //midenismos tou logariasmou tou theatrou
		sem_post(sem_bank);
		signal(SIGALRM, catch_alarm);//energopoihse pali to signal wste na kanei catch kai tin epomeni fora
		alarm(30); //epanekinisi
    }