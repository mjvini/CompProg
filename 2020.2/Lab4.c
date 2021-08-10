#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// declaração de funções da shell
int shell_cd(char** args);
int shell_bg(char** args);
int shell_fg(char** args);
int shell_jobs(char** args);
int shell_help(char** args);
int shell_exit(char** args);

// lista de comandos da shell
char* shell_strings[] = {"cd", "bg", "fg", "jobs", "help", "exit"};

int (*shell_commands[])(char**) = {&shell_cd, &shell_bg, &shell_fg, &shell_jobs, &shell_help, &shell_exit};


// implementação dos comandos


int shell_cd(char** args){

	if(args[1] == NULL)
		fprintf(stderr, "shell: expected argument to \"cd\"\n");

	else{

    	if(chdir(args[1]) != 0)
      		perror("shell: could not change current working directory\n");

  }

  return 1;

}

int shell_bg(char** args){
	
}

int shell_fg(char** args){
	
}

int shell_jobs(char** args){
	
}

int shell_help(char** args){

	printf("Shell: MAB353 - Computadores e Programação.\n");
	printf("Foram implementadas as seguintes funções:\n");

	for (int i = 0; i < 6; i++)
		printf("  %s\n", shell_strings[i]);

	return 1;

}

int shell_exit(char** args){

	return 0;

}

void type_prompt(){

    static int first_time = 1;
    char cwd[PATH_MAX];

    getcwd(cwd, sizeof(cwd));

    // limpa a tela pela primeira vez
	if(first_time){

		const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
		write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;

	}

	printf("%s $", cwd);

}

int shell_launch(char** args){

    pid_t pid;
    int status;

    pid = fork();

    // processo filho
    if(pid == 0){

        if(execvp(args[0], args) == -1)
            perror("failed to launch process");

    }else if(pid < 0){

        perror("shell: failed to create child process");

    // processo pai
    }else{

        do{

            waitpid(pid, &status, WUNTRACED);

        }while(!WIFEXITED(status) && !WIFSIGNALED(status));

    }

    return 1;

}

int shell_run(char** args){

    // nao foi digitado nenhum comando
    if(args[0] == NULL)
        return 1;

    // for() loop retornando a funcao apropriada
    for(int i = 0; i < 6; i++){

        if(strcmp(args[0], shell_strings[i]) == 0)
            return (*shell_commands[i])(args);

    }

    return shell_launch(args);

}

// leitura do input
char* shell_read_line(void){

    int c, position = 0;
    char *buffer = malloc(sizeof(char) * 1024);

    buffer[0] = 0;

    while (1) {

        c = getchar();

        if (c == EOF)
            exit(EXIT_SUCCESS);

        else if (c == '\n'){

            buffer[position] = '\0';
            return buffer;

        }else
            buffer[position] = c;

        position++;

    }
  
}

// retorna os argumentos para as operacoes da shell
char** shell_split_line(char* line){

    int bufsize = 64, position = 0;
    char **tokens = malloc(sizeof(char*)*bufsize);
    char *token, **tokens_backup;

    tokens[0] = 0;

    token = strtok(line, " \n");

    while(token != NULL){

        tokens[position] = token;
        position++;

        if(position >= bufsize){

            bufsize += 64;
            tokens_backup = tokens;
            tokens = realloc(tokens, sizeof(char*)*bufsize);

        if (!tokens){

                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);

        }

        }

        token = strtok(NULL, " \n");

    }

    tokens[position] = NULL;
    return tokens;

}

void shell_loop(){

    char* line;
    char** args;
    int status;

    do{

        type_prompt();
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_run(args);

        free(line);
        free(args);

    }while(status);

}

int main(){

	shell_loop();

	return 0;

}