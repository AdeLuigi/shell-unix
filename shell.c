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

#define clear() printf("\033[H\033[J") 

void inicia_shell(){
    
    clear();
    //puts("Nossa Shell:");
}

void printa_dir_atual(){
    
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
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
        printf("\n");
        exit(0);
    }

    else if(strlen(buf) != 0){
        // Adiciona o comando ao histórico do usuário
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
    
    // Já com os argumentos divididos, testa se são built-in.
    if(strcmp(args_comando[0], "cd") == 0){     
      
      //“cd” não funciona nativamente usando exec..(), por isso executamos com chdir ()
      if(chdir(args_comando[1]) != 0){
            perror("Erro");
        }

        return;
    }

    // Caso não seja um comando built-in executamos esse o bloco abaixo
    else{

        pid_t pid = fork();
        
        if(pid == -1){
            puts("Falha ao executar um novo processo.");
            return;
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

    }

}

int main(){    
    
    inicia_shell();

    while(1){
        
        printa_dir_atual();
        char *comando = get_input();
        
        if(comando != NULL){
         exec_comando(comando); 
        }

        // Pula uma linha entre comandos
        printf("\n");
        
    }  

    exit(0);
}