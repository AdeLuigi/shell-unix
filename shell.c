#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <readline/readline.h> 
#include <readline/history.h> 

#define clear() printf("\033[H\033[J") 

#define NUM_COMANDOS 5

void inicia_shell(){
    
    clear();
    //puts("Nossa Shell:");
}

char *get_input(){

    
    char *buf;
    
    /*A biblioteca libreadline-gplv2-dev:i386 talvez seja necessária para compilar a função
    readline em 32bits*/
    buf = readline(">>> ");

    if(strlen(buf) != 0){
        // Adiciona o comando ao histórico do usuário (Ainda não testado)
        add_history(buf); 
        return buf; 
    }

    return NULL;
}


void exec_comando(char *comando){

    /*Haverá pelo menos um argumento.
    Criaremos com 3 pq normalmente é o máximo de argumentos passados pelo usuário
    + NULL por causa da função que executa os comandos*/
    char **args_comando = (char**) malloc(sizeof(char*)*3);
    args_comando[2] = NULL;

    //int num_args;

    char *arg = strtok(comando, " ");
    
    for (int i = 0; ; i++)
    {
        if(i > 2){
            args_comando = (char**) realloc(args_comando, sizeof(char*)*(i+1));
        }
        
        args_comando[i] = arg;        
        arg = strtok(NULL, " "); 

        if (arg == NULL) {
            args_comando[i+1] = NULL;
            break;            
        }
        

    }

    /*for (int i = 0; args_comando[i] != NULL; i++)
    {
        printf("%s\n", args_comando[i]);
    }
    
    exit(0);
    */
    

    /*
    //Lista dos nossos comandos built-in possíveis
    char *comandos[NUM_COMANDOS] = {"quit", "fg", "bg", "jobs", "cd"};
    
    // Será o index do argumento chamado da lista de comandos.
    int numero_agr;    
    
    for (int i = 0; i < NUM_COMANDOS; i++)
    {
        if(strcmp(comando, comandos[i]) == 0){
            numero_agr = i;
            break;
        }

    }  
        
    switch (numero_agr)
    {    
    case 0:        
        break;
    case 1:        
        break;
    case 2:        
        break;
    case 3:        
        break;
    case 4:        
        break;            
    
    }
    */

    // Caso não seja um comando built-in executamos esse bloco    
        pid_t pid = fork();
     
        if(pid == -1){
            puts("Falha ao executar um novo processo.");
        }
        //Processo filho sendo executado
        else if(pid == 0){

            if(execv(args_comando[0], args_comando) < 0){
                puts("Não foi possível executar o comando.");
                return;
            }

        }else{
            //O processo pai espera o processo filho terminar
            wait(NULL);
            return;
        }

        free(args_comando);
}



int main(){    
    
    inicia_shell();

    while(1){
        char *comando = get_input();
        if(comando != NULL){
            exec_comando(comando);
        }
    }  

    exit(0);
}