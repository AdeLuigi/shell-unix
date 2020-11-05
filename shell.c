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

    char *arg_comando;
    
    // Comando que o usuário entrou
    comando = strtok(comando, " ");

    // Argumento do comando.
    arg_comando = strtok(NULL, " ");

    char *argumentos_shell[3] = {comando, arg_comando, NULL};

    // Caso não seja um comando built-in executamos esse bloco
    if(strcmp(comando, ".") || strcmp(comando, "/") == 0){
        pid_t pid = fork();
     
        if(pid == -1){
            puts("Falha ao executar um novo processo.");
        }
        //Processo filho sendo executado
        else if(pid == 0){

            if(execvp(argumentos_shell[0], argumentos_shell) < 0){
                puts("Não foi possível executar o comando.");
                return;
            }

        }else{
            //O processo pai espera o processo filho terminar
            wait(NULL);
            return;
        }
    }
    
    //Lista de comandos built-in possíveis
    char *comandos[NUM_COMANDOS] = {"quit", "fg", "bg", "jobs", "cd"};
    
    // Será o index do argumento chamado, SE esse argumento for válido.
    int numero_agv = -1;    
    
    for (int i = 0; i < NUM_COMANDOS; i++)
    {
        if(strcmp(comando, comandos[i]) == 0){
            numero_agv = i;
            break;
        }

    }
    
    if(numero_agv = -1){
        puts("Comando built-in não reconhecido.");
        return;
    }    
        
    switch (numero_agv)
    {    
    case 0:        
        break;
    
    default:
        break;
    }
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