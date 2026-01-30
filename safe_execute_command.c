#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

int safe_execute(char *const args[]) {
    if (args == NULL || args[0] == NULL) {
        return 1;
    }

    pid_t pid;
    int ret = posix_spawnp(&pid, args[0], NULL, NULL, args, environ);
    
    if (ret != 0) {
        return 1;
    }

    int status;

	while (waitpid(pid, &status, 0) == -1) {
    	if (errno != EINTR) {
        	return 1; 
    	}
	}
    return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : 1;
}
