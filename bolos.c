#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

//          PROTOTIPOS              //
void sig_handler();
void imprimirError(char* sennialError,int cod );
void redefinirSenial(char *argv0,char * argv1,char* argv2);
int  crearArbol();
int  calcularTiempo();
int check_alive();
void dibujarArbol();
//                                  //


//Var globales
char * nombreProceso;
int boloIzquierda,boloDerecha,flagPr,stat;
struct sigaction ss;
int statusB,statusC,statusE,statusI,statusD,statusF,statusG,statusH,statusI,statusJ;
int vidaG=1,vidaD=1,vidaB=1,vidaJ=1,vidaF=1,vidaH=1,vidaC=1,vidaE=1,vidaI=1;

int main(int argc,char* argv[]){

    if(strcmp(argv[0],"A") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
        sleep(4);
        if(flagPr == 1){  //A se ha desviado a la derecha matando a C.
            waitpid(atoi(argv[2]),&statusC,0);
            statusC = WEXITSTATUS(statusC);
            if(statusC == 1){
                vidaC = 0;
                vidaF = 0;
            }
            else if(statusC == 2){
                vidaC = 0;
            }
            else{
                vidaC = 0;
                vidaF = 0;
                vidaJ = 0;
            }
        }
        if(flagPr == 2){ //A se ha desviado a la izquierda matando a B.
            waitpid(atoi(argv[1]),&statusB,0);
            statusB = WEXITSTATUS(statusB);
            if(statusB == 1){
                vidaB = 0;
                vidaD = 0;
            }
            else if(statusB == 2){
                vidaB = 0;
            }
            else{
                vidaB = 0;
                vidaD = 0;
                vidaG = 0;
            }
        }
        if(flagPr == 3){  //A mata a B y C
            vidaB = 0;
            vidaC = 0;
            waitpid(atoi(argv[2]),&statusC,0);
            statusC = WEXITSTATUS(statusC);
            if(statusC == 1){
                vidaF = 0;
            }
            else if(statusC == 2){
                vidaC = 0;
            }
            else{
                vidaC = 0;
                vidaF = 0;
                vidaJ = 0;
            }
            waitpid(atoi(argv[1]),&statusB,0);
            statusB = WEXITSTATUS(statusB);
            if(statusB == 1){
                vidaB = 0;
                vidaD = 0;
            }
            else if(statusB == 2){
                vidaB = 0;
            }
            else{
                vidaB = 0;
                vidaD = 0;
                vidaG = 0;
            }
        }
        // Comprueba estado H no bloqueante.
        if(waitpid(atoi(argv[3]),&statusH,WNOHANG)!= atoi(argv[3])){
            vidaH = 1;
        }
        else{
            vidaH = 0;
        }
        // Comprueba estado I no bloqueante.
        if(waitpid(atoi(argv[4]),&statusI,WNOHANG)!= atoi(argv[4])){
            vidaI = 1;
        }
        else{
            vidaI = 0;
        }
        // Comprueba estado E no bloqueante.
        if(waitpid(atoi(argv[5]),&statusE,WNOHANG) != atoi(argv[5])){
            vidaE = 1;
        }
        else{
            vidaE = 0;
        }
        //Ejecuta ps -fu
        dibujarArbol();
        
        switch(fork()){
            case -1:
                exit(-1);
            case 0:
                execlp("/bin/ps", "ps", "-fu", NULL);
                break;
            default:
                wait(NULL);
                break;
        }
        kill(0,SIGINT);
        return 0;
    }
    if(strcmp(argv[0],"B") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
        if(flagPr==2 || flagPr == 3){  // 2 Izquierda
            waitpid(atoi(argv[1]),&statusD,0);
            statusD = WEXITSTATUS(statusD);
            if(statusD == 1){       
                exit(1);
            }
            else{
                exit(0);
            }  
        }
        else{
            exit(2);
        }
    }
    if(strcmp(argv[0],"D") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
        if(flagPr==2 || flagPr == 3){  // 2 Izquierda
            exit(0);
        }
        else{
            exit(1);
        }
    }
    
    if(strcmp(argv[0],"C") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
        if(flagPr==1 || flagPr == 3){  //1 derecha
            waitpid(atoi(argv[2]),&statusF,0);
            statusF = WEXITSTATUS(statusF);  
            if(statusF == 1){       
                exit(1);
            }
            else{
                exit(0);
            }  
        }
        else{
            exit(2);
        }
    }
    if(strcmp(argv[0],"F") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
        if(flagPr==1 || flagPr == 3){  //1 derecha
            exit(0);
        }
        else{
            exit(1);
        }
    }
    if(strcmp(argv[0],"E") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
    }
    if(strcmp(argv[0],"G") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
    }
    if(strcmp(argv[0],"H") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
    }
    if(strcmp(argv[0],"I") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
    }
    if(strcmp(argv[0],"J") == 0){
        redefinirSenial(argv[0],argv[1],argv[2]);
    }

    if(strcmp(argv[0],"./bolos") == 0){
        int pidA;

        //Flujo de la ejecución del main (./bolos).
        pidA = crearArbol();
        printf("\nUtilize kill -15 %d para lanzar la bola.\n",pidA);
        return 0;
    }
}


int crearArbol(){
    static int forkA,forkB,forkC,forkD,forkE,forkF,forkG,forkH,forkI,forkJ;
    char aux [12];
    char aux2 [12];
    char aux3 [12];
    char aux4 [12];
    char aux5 [12];
    switch(forkA = fork()){
            case -1:
                imprimirError("forkA",-1);
                exit(-1);
            case 0:
                //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                switch(forkH = fork()){ // A --> Creas H
                    case -1:
                        imprimirError("forkH",-1);
                        exit(-1);
                    case 0: 
                        //fprintf(stderr,"PID %d | PPID: %d H: %d\n",getpid(),getppid(),getpid());
                        execlp("./bolos","H","0","0",NULL); //No se pasa ningun pid porque es bolo "hoja"
                    break;
                    default: // A --> Creas I   
                        switch(forkI = fork()){
                            case -1:
                                imprimirError("forkI",-1);
                                exit(-1);
                            case 0:    
                                //fprintf(stderr,"Vida I: %d",*vidaI);
                                //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                execlp("./bolos","I","0","0",NULL); //No se pasa ningun pid porque es bolo "hoja"
                            break;
                            default: // A --> Creas E
                                switch(forkE = fork()){
                                    case -1:
                                        imprimirError("forkE",-1);
                                        exit(-1);
                                    case 0:
                                        //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                        sprintf(aux,"%d",forkH); //Convertimos a string el pid de H y de I
                                        sprintf(aux2,"%d",forkI);
                                        execlp("./bolos","E",aux,aux2,NULL);
                                    break;
                                    default: //A --> Creas B      
                                        switch(forkB = fork()){
                                            case -1:
                                                imprimirError("forkB",-1);
                                                exit(-1);
                                            case 0:
                                                //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());     
                                                switch(forkD = fork()){
                                                    case -1:
                                                        imprimirError("forkD",-1);
                                                        exit(-1);
                                                    case 0:
                                                        //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                                        switch(forkG = fork()){
                                                            case -1:
                                                                imprimirError("forkG",-1);
                                                                exit(-1);
                                                            case 0:
                                                                //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                                                execlp("./bolos","G","0","0",NULL);
                                                            break;
                                                            default: //D
                                                                
                                                                sprintf(aux,"%d",forkG); //Convertimos a string el pid de G y de H
                                                                sprintf(aux2,"%d",forkH);
                                                                execlp("./bolos","D",aux,aux2,NULL);
                                                            break;
                                                        }
                                                    break;
                                                    default:  //B
                                                        sprintf(aux,"%d",forkD); //Convertimos a string el pid de D y de E
                                                        sprintf(aux2,"%d",forkE);
                                                        execlp("./bolos","B",aux,aux2,NULL);
                                                    break;
                                                }
                                            break;
                                            default: //A --> Creas C                  
                                                switch(forkC = fork()){
                                                    case -1:
                                                        imprimirError("forkC",-1);
                                                        exit(-1);
                                                    case 0:
                                                        //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());                                              
                                                        switch(forkF = fork()){
                                                            case -1:
                                                                imprimirError("forkF",-1);
                                                                exit(-1);
                                                            case 0:
                                                                //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                                                switch(forkJ = fork()){
                                                                    case -1:
                                                                        imprimirError("forkJ",-1);
                                                                        exit(-1);
                                                                    case 0:
                                                                        //fprintf(stderr,"PID %d | PPID: %d\n",getpid(),getppid());
                                                                        execlp("./bolos","J","0","0",NULL);
                                                                    break;
                                                                    default: //F
                                                                        sprintf(aux,"%d",forkI); //Convertimos a string el pid de I y de J
                                                                        sprintf(aux2,"%d",forkJ);
                                                                        execlp("./bolos","F",aux,aux2,NULL);
                                                                    break;
                                                                }
                                                            break;
                                                            default: //C
                                                                sprintf(aux,"%d",forkE); //Convertimos a string el pid de E y de F
                                                                sprintf(aux2,"%d",forkF);
                                                                execlp("./bolos","C",aux,aux2,NULL);
                                                            break;
                                                        }
                                                    break;
                                                    default: //A
                                                        sprintf(aux,"%d",forkB); //Convertimos a string el pid de B y de C
                                                        sprintf(aux2,"%d",forkC);
                                                        sprintf(aux3,"%d",forkH);
                                                        sprintf(aux4,"%d",forkI);
                                                        sprintf(aux5,"%d",forkE);
                                                        execlp("./bolos","A",aux,aux2,aux3,aux4,aux5,NULL);
                                                    break; // break default A
                                                }                                                
                                            break; // break default C
                                        }
                                    break; // break default E
                                }
                            break; // break default I
                        }
                    break; //break default H
                }
            break; // break case 0 A
            default: 
               return getpid()+1;
                // fprintf(stderr,"\nPadre PID: %d",getpid());                      
            break;
        } //End of switch
}

void dibujarArbol(){
    char A='x',B='B',C='C',D='D',E='E',F='F',G='G',H='H',I='I',J='J';
    if(vidaB != 1){
        B = 'x';
    }
    if(vidaC != 1){
        C = 'x';
    }
    if(vidaD != 1){
        D = 'x';
    }
    if(vidaE != 1){
        E = 'x';
    }
    if(vidaF != 1){
        F = 'x';
    }
    if(vidaG != 1){
        G = 'x';
    }
    if(vidaH != 1){
        H = 'x';
    }
    if(vidaI != 1){
        I = 'x';
    }
    if(vidaJ != 1){
        J = 'x';
    }
    fprintf(stderr,"\n\t\t\t%c\n\t\t%c\t\t%c\n\t%c\t\t%c\t\t%c\n%c\t\t%c\t\t%c\t\t%c\n",A,B,C,D,E,F,G,H,I,J);
}


void sig_handler(){
    if(boloIzquierda != 0){  //NO entran ni G n i H ni I ni J
        int resto = calcularTiempo();   
        if(resto == 0){
                flagPr = 0;
                 //No hace nada, no envia la señal a ningún proceso.
        }
        if(resto == 1){
                flagPr = 1;  
                //fprintf(stderr,"Soy %s con pid %d y voy a matar a %d\n",nombreProceso,getpid(),boloDerecha);
                kill(boloDerecha,SIGTERM);     //Matar derecha
                
        }
        if(resto == 2){
                flagPr = 2;  

                //fprintf(stderr,"Soy %s con pid %d y voy a matar a %d\n",nombreProceso,getpid(),boloIzquierda);
                kill(boloIzquierda,SIGTERM);  //Matar izqda.
                
                
        }
        if(resto == 3){ 
                flagPr = 3;   
         
                //fprintf(stderr,"Soy %s con pid %d y voy a matar a %d y a %d\n",nombreProceso,getpid(),boloDerecha,boloIzquierda);
                kill(boloIzquierda,SIGTERM);
                kill(boloDerecha,SIGTERM);   //Matar ambos.
                
        } 
        //printf("Bolo derecha %s: %d Bolo izquierda %s: %d\n ",nombreProceso,WEXITSTATUS(retornoD), nombreProceso,WEXITSTATUS(retornoI));  ///////////////////////////////////////////////////DEBUG
    }  
}



void imprimirError(char* sennialError,int cod ){
    fprintf(stderr,"SennialError: %s | Cod_Error: %d\n",sennialError,cod);
}

void redefinirSenial(char *pid_Name,char *boloI,char* boloD){

    //Guardamos en variables globales los datos del proceso en ejecución.
    nombreProceso = pid_Name;
    boloIzquierda = atoi(boloI);
    boloDerecha = atoi(boloD);

    ss.sa_handler = &sig_handler; //Puntero a la función manejadora.
    ss.sa_flags=0; //Flags a 0.
    if(sigaction(SIGTERM,&ss,NULL) == -1){ // n es la señal (SIGTERM), &sigaction es el puntero a la estructura sigaction ss y NULL accion vieja.
        imprimirError("sigAction",-1);
    }   
    pause();
}
      

int calcularTiempo(){
    struct timeval startTimer;
    gettimeofday(&startTimer,NULL);
    int time = startTimer.tv_usec; //Tiempo usado en microsegundos.
    int resto = time % 4;
    return resto;
}