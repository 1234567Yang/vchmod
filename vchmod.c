#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>

#include <sys/stat.h>

#include <errno.h>

#define ARR9(a) a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]


struct termios old, new;
int reset_console(){
	// set back
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
}



// cursor_pos starts with 0
int print_user_access_control(int arr[9], int cursor_pos)
// int ur, int uw, int ux, int gr, int gw, int gx, int or, int ow, int ox,
{
	char arr_c[9];
	for(int i = 0; i < 9; i++){
		if(arr[i] == 0) arr_c[i] = '-';
		else if(arr[i] == 1) arr_c[i] = 'a';
		else{
			perror("An error occured: arr is neither 0 or 1.\n");
			reset_console();
			exit(EXIT_FAILURE);
		}
	}
	int oct_val = arr[0]*4 + arr[1]*2 + arr[2];      // user
	oct_val = oct_val * 8 + arr[3]*4 + arr[4]*2 + arr[5];  // group  
	oct_val = oct_val * 8 + arr[6]*4 + arr[7]*2 + arr[8];  // other
	
	// printf("     %c %c %c     %c %c %c     %c %c %c     |     Oct: %o\r", ARR9(arr_c),oct_val);
	printf("     ");
    for (int i = 0; i < 9; i++) {
        if (i == cursor_pos) {
            printf("\x1b[7m%c\x1b[0m ", arr_c[i]);  // 反显
        } else {
            printf("%c ", arr_c[i]);
        }
        if (i == 2 || i == 5) printf("    ");
    }
    printf("    |     Oct: %o\r", oct_val);
	

	fflush(stdout);
}


int main(int argc, char *argv[]){
/*
    if(arc <= 1){
        printf("You need to assign at least 1 file\n");
        exit(EXIT_FAILURE);
    }
*/
	int which_arg = -1;
	for(int i = 0; i < argc; i++){
		if(argv[i][0] == '\0') continue;
		if(argv[i][0] == '-') continue;
		which_arg = i;
		break;
	}
	if(which_arg == -1){
		perror("No vaild arg.\n"); exit(EXIT_FAILURE); 
	}

	struct stat st;

	if (stat(argv[which_arg], &st) == -1) { perror("Stat can not get the information.\n"); exit(EXIT_FAILURE); }

	int arr[9];
	int cursor_pos = 0;
    



	tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);    

	printf("\r\nq - Quit; Enter - Save; Arrows - Move/Switch; a - Allowed\n\n\r");
	printf("     User      Group     Other     \n\r");
	printf("     R W X     R W X     R W X     \n\r");
	

	mode_t mode = st.st_mode;

	mode_t masks[] = {

    	S_IRUSR, S_IWUSR, S_IXUSR,

    	S_IRGRP, S_IWGRP, S_IXGRP,

    	S_IROTH, S_IWOTH, S_IXOTH

	};


	for (int i = 0; i < 9; i++) {
    	arr[i] = (mode & masks[i]) ? 1 : 0;
	}

	print_user_access_control(arr, cursor_pos);

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (c == '\x1B') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;
        
            if (seq[0] == '[') {
                if (seq[1] == 'D') {
                    // Left arrow
                    cursor_pos = (cursor_pos - 1 >= 0) ? (cursor_pos - 1) : 0;
                } else if (seq[1] == 'C') {
                    // Right arrow
                    cursor_pos = (cursor_pos + 1 <= 8) ? (cursor_pos + 1) : 8;
                }else{
					// goto FLAG_SWITCH_STATE;
					arr[cursor_pos] = abs(arr[cursor_pos] - 1);
				}
            }
        }
		else
		{
			// FLAG_SWITCH_STATE:
		}


		
		print_user_access_control(arr, cursor_pos);

    }
	reset_console();

	printf("\n");

	return 0;	
}

