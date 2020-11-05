#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <readline/readline.h> 
#include <readline/history.h> 

#define clear() printf("\033[H\033[J") 

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

int main(){    
    
    inicia_shell();

    if(get_input() != NULL){
        
    }
    

    exit(0);
}