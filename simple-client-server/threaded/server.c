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
#include <pthread.h>
#define NUM_THREADS 10

//dilwsi metavlitwn gia tin dimiourgia tou socket kai tis sundesis
int error;
struct sockaddr sock_add_kjg, sec_sock_add;
int conlenght;
int sockjg;  /* Metablites gia kataxwrisi tou socket pou tha xrisimopoihthei  */
struct my_struct *pointer; //o pointer pou tha deixnei tin thesi mnimis stin opoia vrisketai to struct mas
int *ptr_mnimi;
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
int pid;
int  zwni, arith_thesewn, cr_ok, balance,bnk;
//dilwsi thread kai mutex
pthread_t thread;
pthread_mutex_t mutex_thl, mutex_bank, mutex_tickets, mutex_money;
pthread_cond_t cond_thl, cond_bank;
void *tickets_thread();
void *cr_check();
int *wtf;

 

struct my_struct // dilwsi tou struct me tous pinakes pou tha exoun tin diathesimotita psistwn, dianomewn, sunolikwn thesewn pou apomenoun kai zwnes thesewn
{
int thlefwnites;//arithmos diathesimwn thlefwnitwn
int bank;//
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
int secsock[1000];
};


int main(void){ /*kentriki sunartisi main*/
	
	pthread_mutex_init( &mutex_thl, NULL );//arxikopoiisi mutex gia tous thlefwnites
	pthread_mutex_init( &mutex_bank, NULL );//arxikopoiisi mutex gia ton bank server
	pthread_mutex_init( &mutex_tickets, NULL );//arxikopoiisi mutex gia tis diathesimes theseis kai tin kratisi
	pthread_mutex_init( &mutex_money, NULL );//arxikopoiisi mutex gia tin xrewsi tis pistwtikis
	pthread_cond_init (&cond_thl, &mutex_thl);
	pthread_cond_init (&cond_bank, &mutex_bank);
	
	ptr_mnimi=malloc(8000*sizeof(struct my_struct));//desmeusi mnimis
	*ptr_mnimi=0;//midenismos pointer
	pointer=(struct my_struct *)ptr_mnimi +sizeof(*ptr_mnimi);
	int rc;
	int t=0;
	signal(SIGINT, kill_server); 
	signal(SIGALRM, catch_alarm);
	time_t rawtime;
	struct tm * timeinfo;
	/*Ektupwse tin wra ekkinisis tou server */
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Server started at: %s", asctime (timeinfo) );
	printf ( "Kleiste ton server pantwntas CNTRL+C, gia na ektupwsete ta apotelesmata\n" );
	sockjg = socket(AF_UNIX, SOCK_STREAM, 0); /*dimiourgoume to socket epikoinonias*/
	unlink("/tmp/unix.str"); /*diagrafoume gia asfalia to proigoume socket(an iparxei)*/
	bzero( &sock_add_kjg, sizeof(sock_add_kjg));
	sock_add_kjg.sa_family = AF_UNIX; /*dilonoume ton tupo tou socket pou xrisimopoioume*/
	strcpy(sock_add_kjg.sa_data, "/tmp/unix.str"); /*dilonoume to path pou dialeksame gia tin epikoinonia*/
	conlenght = strlen(sock_add_kjg.sa_data) + sizeof(sock_add_kjg.sa_family);
	
		/*Arxikopoihsi tou my_struct */
	(*pointer).a_thesewn=640;
	(*pointer).thlefwnites=10;
	(*pointer).bank=4;
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
		if (bind(sockjg, &sock_add_kjg, conlenght) < 0) {/*bind tou server sto socket*/
	  printf("Server Bind Failure %d\n", errno); /*elegxos la8ous*/
	  perror("\nBind Error: ");
	  exit(1);
		}
		
	if (listen(sockjg, 10) < 0) { /*o server "akouei" sundeseis se auto to socket*/
	  printf("Server Listen Failure %d\n", errno);/*elegxos la8ous*/
	  perror("\nListen Error: ");
	  exit(1);
		}
		

	alarm(30); //kaleitai i sunartisi pou kathe 30sec metaferei ta xrimata ston logariasmo
	int pro=0;	
	while(1){
		(*pointer).secsock[pro] = accept(sockjg, ( struct sockaddr * )&sec_sock_add, &conlenght);/*o server dexetai sundeseis se auto to socket*/
		if ((*pointer).secsock[pro] < 0) { /*elegxos la8ous*/
		  printf("Server Accept Failure %d\n", errno);
		  perror("\nAccept Error: ");
		  exit(1);
			}
		
		if(rc=pthread_create(&thread,NULL,tickets_thread,(void *) &(*pointer).secsock[pro])!=0)//dimiourgia nimatwn
        {
            perror("PTHREAD ERROR !:");
        }
		pro++;
		pthread_detach(thread);//apodesmeusi nimatwn
	}//while(1)
	}//main
	
	
	void *tickets_thread(void *secsock){ /*server instance*/
			int clnt_sock = *((int*) secsock) ;
			int timediff1, timediff2; //metavliti pou apothikeuetai i diafora tou xronou apo tin lipsi mexri to peras tis kratisis
			void *check=0;
			char id[5]=""; 
			int bnk=0;
			time_t tv1, tv2; //metavlites gia tin apothukeusi tou xronou anamonis
			time_t  tm3, tm4; //metavlites gia tin apothukeusi tou xronou e3upiretisis
			int tmp=0;
			int tmp2=0;
			int tmp3=0;
			pid_t pid2;
			int rt=0;
			int size=0;
			int f=0;
			int temp4=0;
			//close(sockjg);  // kleinoume to socket 
			pthread_t thread2; //dilwsi tou deuterou thread pou tha kanei tautoxrona ton elegxo tis pistwtikis

			/* EDW KSEKINAEI O KWDIKAS */ 
			pthread_mutex_lock(&mutex_thl); //anoigw ton mutex gia na elen3w an uparxei diathesimos thlefwnitis
			(*pointer).counter1++; //au3anw tous sunolikous pelates kata 1
			if((*pointer).thlefwnites>0){//an vrethei thlefwnitis
				(*pointer).thlefwnites--; //meiwse kata 1 tous diathesimous thlefwnites
				(*pointer).counter4++; //au3anw tous pelates pou kataferan na sundethoun me thlefwniti kata 1
				write(clnt_sock, &operand, sizeof(int)); //steile ston client mnm oti sundeetai me tilefwniti
				}
			else{ //an den uparxei diathesimos thlefwnitis
					(*pointer).counter2++; //au3anw ton counter pou metraei posoi pelates mpainoun stin anamoni
					write(clnt_sock, &no_operand, sizeof(int));  //enimerwse ton client oti mpainei se anamoni
					tv1=time(NULL); //ksekinaw to xronometro gia ton sunoliko xrono anamonis
					pthread_cond_wait(&cond_thl, &mutex_thl); //kanoume wait to thread an eksipiretountai idi 10 pelates apo tilefwnites
					tv2=time(NULL);//stamataw to xronometro
					timediff1=tv2-tv1; //upologizw se deuterolepta ton xrono
					(*pointer).counter4++; //au3anw tous pelates pou kataferan na sundethoun me thlefwniti kata 1
					(*pointer).t_anamonis+= timediff1; //apothikeuw tin timi stin mnimi tou server
					(*pointer).thlefwnites--; //meiwse kata 1 tous diathesimous thlefwnites
					f=1;
					}
			pthread_mutex_unlock(&mutex_thl); //kleinw ton mutex			
			tm3=time(NULL); //ksekinaw to xronometro gia ton xrono e3upiretisis
			write(clnt_sock, &operand, sizeof(int)); //steile ston client mnm oti sundeetai me tilefwniti
			
			
			/*Elegxos an exei gemisei to theatro */
			pthread_mutex_lock(&mutex_tickets);	//anoigw mutex gia elegxo thesewn
			if((*pointer).a_thesewn>0){temp4=1;}
			pthread_mutex_unlock(&mutex_tickets);	//kleinw mutex gia elegxo thesewn
			
			if(temp4==0){ //an oles oi theseis einai kleismenes
				pthread_mutex_lock(&mutex_thl);
				(*pointer).thlefwnites++;//kanw pali diathesimo ton thlefwniti
				(*pointer).counter3++; //au3anw ton sunoliko arithmo twn pelatwn pou apotuxan na kanoun kratisi
				write(clnt_sock, &no_operand, sizeof(int));/*steile mnm ston client oti den uparxoun theseis */	
				pthread_cond_signal(&cond_thl);//eidopoihse ta blocked threads
				pthread_mutex_unlock(&mutex_thl); //apeleutherwsi mutex
				close(clnt_sock);
				pthread_exit(NULL);
				}
			else{//eite proxwraei stin paraggelia
				write(clnt_sock, &operand, sizeof(int)); //enimerwse ton client oti mporei na dextei paraggelia
				  //diavazoume to input pou esteile o client
				  read(clnt_sock, &zwni, sizeof(int));
				  read(clnt_sock, &arith_thesewn, sizeof(int));
				  read(clnt_sock, &cr_ok, sizeof(int));
				  read(clnt_sock, &size, sizeof(int));
				  read(clnt_sock, &id, size);
				  if(arith_thesewn<=4){
					tmp2=arith_thesewn;}
			  }
			
			
			pthread_mutex_lock(&mutex_bank);//anoigw ton mutex gia tous bank server
			if((*pointer).bank==0)//an dn uparxoun eleutheroi bank servers
			{
				pthread_cond_wait(&cond_bank, &mutex_bank);//block to thread mexri kapoio allo thread na apeleutherwsei
				}
			(*pointer).bank--;	
			pthread_mutex_unlock(&mutex_bank);//kleinw ton mutex

			if(rt=pthread_create(&thread2,NULL, cr_check, (void *)&cr_ok)!=0)//dimiourgia thread gia elegxo pistwtikis
				{perror("PTHREAD 2 ERROR");}
			pthread_mutex_lock(&mutex_tickets);	//anoigw mutex gia elegxo thesewn
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
				pthread_mutex_unlock(&mutex_tickets); //kleinw mutex apo elegxo thesewn

					
				int pl=0;
				usleep(6000000); //perimene 6sec
				pthread_join( thread2, &check); //to parent thread perimenei na oloklirwthei o elegxos tis pistwtikis
				 //apo to thread 2
				pthread_detach(thread2);
				if(tmp!=1){ //an den uparxoun theseis stin antistoixi zwni enimerwse analoga ton client
					write(clnt_sock, &no_operand, sizeof(int)); //send 0
					tm4=time(NULL); //stamataw to xronometro gia ton xrono e3upiretisis
					pthread_mutex_lock(&mutex_thl);
					timediff2=tm4-tm3; //upologizw se deuterolepta ton xrono
					(*pointer).t_eksupiretisis+= timediff2; //apothikeuw tin timi stin mnimi tou server
					(*pointer).counter3++;//au3anw twn arithmo twn apotuxiwn
					(*pointer).thlefwnites++;//epistrefw ton thlefwniti
					pthread_cond_signal(&cond_thl);
					pthread_mutex_unlock(&mutex_thl);
					
					pthread_mutex_lock(&mutex_bank);//anoigw ton mutex gia tous bank server
					(*pointer).bank++;//apodesmeuw ton bank server
					pthread_cond_signal(&cond_bank);//eidopoiw ta thread pou perimenoun na eleutherwthei bank server
					pthread_mutex_unlock(&mutex_bank);//kleinw ton mutex gia tous bank server
					
					tmp2=0;
					tmp3=0;
					close(clnt_sock);
					pthread_exit(NULL);
					}
				if(tmp==1 && *((int*) check)==0){ //an i pistwtiki exei provlima enimerwse analoga ton client
					write(clnt_sock, &operand, sizeof(int)); //send 1
					tm4=time(NULL); //stamataw to xronometro gia ton xrono e3upiretisis
					pthread_mutex_lock(&mutex_thl);
					timediff2=tm4-tm3; //upologizw se deuterolepta ton xrono
					(*pointer).t_eksupiretisis+= timediff2; //apothikeuw tin timi stin mnimi tou server
					(*pointer).counter3++; //au3anw ton arithmo twn apotuxiwn
					(*pointer).thlefwnites++;//epestrepse ton thlefwniti
					pthread_cond_signal(&cond_thl);//enimerwse ta waiting threads, wste ena na ginei unblock
					pthread_mutex_unlock(&mutex_thl);
					
					pthread_mutex_lock(&mutex_bank);//anoigw ton mutex gia tous bank server
					(*pointer).bank++;//apodesmeuw ton bank server
					pthread_cond_signal(&cond_bank);//eidopoiw ta thread pou perimenoun na eleutherwthei bank server
					pthread_mutex_unlock(&mutex_bank);//kleinw ton mutex gia tous bank server
					tmp2=0;
					tmp3=0;
					close(clnt_sock);
					pthread_exit(NULL);
					}
				else if( *((int*) check)== 1 && tmp == 1 && (*pointer).a_thesewn >0){//an ola einai ok proxwrame stin kratisi
					write(clnt_sock, &all_ok, sizeof(int));//send 2
					pthread_mutex_lock(&mutex_tickets);
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
					pthread_mutex_unlock(&mutex_tickets);
					
					pthread_mutex_lock(&mutex_thl);
					(*pointer).thlefwnites++;//epestrepse ton thlefwniti
					pthread_cond_signal(&cond_thl);
					pthread_mutex_unlock(&mutex_thl);
					
					pthread_mutex_lock(&mutex_money);
					(*pointer).theatro_money += (tmp2 * tmp3); //vale ston logariasmo tou theatrou ta lefta
					pthread_mutex_unlock(&mutex_money);
					pthread_mutex_lock(&mutex_bank);//anoigw ton mutex gia tous bank server
					(*pointer).bank++;//apodesmeuw ton bank server
					pthread_cond_signal(&cond_bank);//eidopoiw ta thread pou perimenoun na eleutherwthei bank server
					pthread_mutex_unlock(&mutex_bank);//kleinw ton mutex gia tous bank server
				}
				else {write(clnt_sock, &operand, sizeof(int));//send 1
						}
						
				close(clnt_sock);
			    pthread_exit(NULL);
		}//thread server instance
		   	 
	void *cr_check(void* status) {//thread gia na gia elegxei tautoxrona o thlefwnitis ta stoixeia tis pistwtikis			
					*((int*) status)=cr_ok;
						usleep(2000000); //sleep for 2sec
						
						pthread_exit((void *)status); //teleiwse i douleia tou thread, termatise to
					}//cr_thread
			 

	void kill_server(){ /* Sunartisi pou kaleite me cntrl + c kai klinei swsta ton server kai tha ektupwnei ta apotelesmata*/
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
		close(sockjg); /* Kleinei to listen socket */
		for(i=0;i++;i<1000){
		close((*pointer).secsock[i]);/*Kleinei kai to client-server socket*/}
		alarm(0); 
		pthread_cond_destroy(&cond_thl);  //katastrefoume ta condition variables
		pthread_cond_destroy(&cond_bank);
		pthread_mutex_destroy(&mutex_thl);
		pthread_mutex_destroy(&mutex_money);
		pthread_mutex_destroy(&mutex_tickets);/*Diagrafh ths metablhths mutex apo to systhma*/
		pthread_mutex_destroy(&mutex_bank);
		pthread_exit(NULL);
		free(ptr_mnimi);/*Apeleutherosi ton kommatiwn mnimis pou eixan desmeutei sthn arxi tou programmatos*/

		printf("\nO server ekleise epituxws!\n"); //den ftanei pote edw mias kai i parapanw kill ta kleinei ola
		exit(1); //
	}

	/* Sunartisi pou kathei 30 sec kanei tin metafora xrimatwn ston trapeziko logarismo */
	void catch_alarm(int sig) {
		//signal(SIGALRM, SIG_IGN);//apenergopoihse ta singals
		pthread_mutex_lock(&mutex_bank);
		(*pointer).moneymoneymoney[(*pointer).moneycounter]=(*pointer).theatro_money; //apothykeusi tou posou gia na ektupwthei sto telos
		(*pointer).moneycounter++;
		(*pointer).bank_money += (*pointer).theatro_money; //metafora xrimatwn ston trapeziko logariasmo
		(*pointer).theatro_money=0; //midenismos tou logariasmou tou theatrou
		pthread_mutex_unlock(&mutex_bank);
		signal(SIGALRM, catch_alarm);//energopoihse pali to signal wste na kanei catch kai tin epomeni fora
		alarm(30); //epanekinisi
    }