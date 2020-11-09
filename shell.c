#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <readline/readline.h> 
#include <readline/history.h> 
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <setjmp.h>

#define clear() printf("\033[H\033[J") 

static sigjmp_buf env1;
static sigjmp_buf env2;

static int n_proc;

//static volatile sig_atomic_t jump_active = 0;

// /int stat_loc;

char *input, **args_input;

void sigint_handler(int signo) {
    /*if (!jump_active) {
        return;
    }*/
    siglongjmp(env1, 42);
}

void sigtstp_handler(int signo){
    siglongjmp(env2, 1);
}

/*void sigint_handler_filho(int signo){
    siglongjmp(env, 1);
}*/

void inicia_shell(){
    
    clear();
    //puts("Nossa Shell:");
}

void printa_dir_atual(){
    
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s ", cwd);
    } else {
        perror("Erro: ");
        return;
    }
}

char *get_input(){

    char *buf;

    buf = readline(">>> ");
    
    // Sai da shell quando Ctrl+D são pressionados.
    if(buf == NULL){
        puts(" ");
        exit(0);
    }

    else if(strlen(buf) != 0){
        // Adiciona o input ao histórico do usuário
        add_history(buf);
        return buf; 
    }

    return NULL;
}


void exec_input(char *input){

    // Se for verdadeiro o programa deve ser rodado em bg
    int bg = 0;

    /*Haverá pelo menos um argumento.
    Criaremos com 3 pq normalmente é o máximo de argumentos passados pelo usuário
    + NULL por causa da função que executa os inputs*/
    args_input = (char**) malloc(sizeof(char*)*3);
    args_input[2] = NULL;

    char *arg = strtok(input, " ");
    
    for (int i = 0; ; i++)
    {
        
        if(i > 2){
            args_input = (char**) realloc(args_input, sizeof(char*)*(i+1));
        }
        
        args_input[i] = arg;

        if(strcmp(args_input[i], "&") == 0){
            bg = 1;
            args_input[i] = NULL;
            break;
        }        

        arg = strtok(NULL, " "); 

        if (arg == NULL) {
            args_input[i+1] = NULL;
            break;            
        }

    }
    
    //Ignora "&" na primeira posição
    if(args_input[0] == NULL){
        /*free(input);
        free(args_input);*/

        return;
    }
    
    // Já com os argumentos divididos, testa se são built-in.
    if(strcmp(args_input[0], "cd") == 0){     
      
      //“cd” não funciona nativamente usando exec..(), por isso executamos com chdir ()
      if(chdir(args_input[1]) != 0){
            perror("Erro");
        }

        /*free(input);
        free(args_input);*/

        return;
    }

    // Caso não seja um input built-in executamos esse o bloco abaixo
    else{

        int status;
        
        pid_t pid = fork();
        
        if(pid == -1){
            puts("Falha ao executar um novo processo.");

            /*free(input);
            free(args_input);*/

            return;
        }
        //Processo filho sendo executado
        else if(pid == 0){

            /*struct sigaction s_child;

            s_child.sa_handler = sigint_handler;
            sigemptyset(&s_child.sa_mask);
            s_child.sa_flags = SA_RESTART;

            sigaction(SIGINT, &s_child, NULL);*/

            signal(SIGINT, sigint_handler);

            //setpgid(0, 0);

            if(execv(args_input[0], args_input) < 0){
                puts("Não foi possível executar o input.");

                /*free(input);
                free(args_input);*/
            }


        }
        else{
            
            // Se o programa for pra ser rodado em backgroud(bg) vai entrar nesse bloco
            if(!bg){
                //O processo pai espera o processo filho terminar
                
                

                if(waitpid(pid, &status, 0) < 0){
                    perror("Erro: ");
                }

                /*if(wait(NULL) < 0){
                    perror("Erro: ");
                }*/
            }else{
                printf("PID: [%d]", pid);
                if(WIFEXITED(status) == 1){
                    printf(" status [%d]\n", WEXITSTATUS(status));
                }

                pause();
            }
                    

        }

        /*free(input);
        free(args_input);*/
 


    }

}


int main(){    
    
    inicia_shell();

    /* Setup SIGINT 
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, NULL);
    */

   signal(SIGTSTP, sigtstp_handler);
   signal(SIGINT, sigint_handler);

    
    while(1){        
        
        if (sigsetjmp(env1, 1) == 42 || sigsetjmp(env2, 1) == 1) {

            printf("\n");
            continue;

        }/*else if(sigsetjmp(env2, 1) == 1){
            printf("\n");
            continue;
        }*/        
        
        //jump_active = 1;
        
        printa_dir_atual();
        input = get_input();
        
        
        if(input != NULL){
            exec_input(input); 
        }


        free(args_input);
        free(input);

        // Pula uma linha entre inputs
        //printf("\n");

        
        
    }  

    exit(0);
}
