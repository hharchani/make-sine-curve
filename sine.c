#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

char *programName;

int getHW(char c) 
{
	char cmd[15];
	int num;
	if(c == 'w' || c == 2) strcpy(cmd, "tput cols");
	else if(c == 'h' || c == 1) strcpy(cmd, "tput lines");
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
	{   
		puts("popen error");
		pclose(pipe);
		exit(0);
	}
	fscanf(pipe, "%d", &num);
	pclose(pipe);
	return num;
}


void showError(int errorStatus)
{
	switch(errorStatus)
	{
		case 1:
			printf("Usage: %s [-F/--full] [-f/--force] [-h/--height height] [-w/--width width] [-c/--cycles cycles]\n", programName);
			exit(1);
		case 2:
			printf("%s:  -h should followed by a number (Height should be a number)\n", programName);
			exit(2);
			break;
		case 3:
			printf("%s:  -w should followed by a number (Width should be a number)\n", programName);
			exit(3);
			break;
		case 4:
			printf("%s:  -t should followed by a number (Number of turns should be a number)\n", programName);
			exit(4);
			break;
		case 5:
			printf("%s:  Height can not exceed terminal window height\n", programName);
			exit(5);
			break;
		case 6:
			printf("%s:  Width can not exceed terminal window width\n", programName);
			exit(6);
			break;
		default:
			printf("%s: Unknown Error Occurred\nExitStatus:%d\n", programName, errorStatus);
			exit(errorStatus);
	}
}

int isNotDigit(char c)
{
	if( c >= '0' && c <= '9') return 0;
	return 1;
}

int stoi(char *s, int errorStatus)
{
	int num,i,len;
	len = strlen(s);
	i=-1;
	num = 0;
	while(++i < len)
	{
		if( isNotDigit( s[len-i-1] ) ) showError(errorStatus);
		num += ( pow(10,i)*(s[len-i-1]-'0') );
	}
	return num;
}


int main(int argc, char **argv)
{
	programName = argv[0];
	int i=0, height, width, turns, force, uheight, uwidth, full, hflag, wflag;
	/* Default Values */
	height = 11;
	width = getHW('w');
	turns = 1;
	force = 0;
	full = 0;
	hflag = wflag = 0;

    while( ++i < argc)
    {
        if( (strcmp(argv[i],"-h")==0) || (strcmp(argv[i], "--height") == 0) )
        {
            hflag = 1;
            if( ++i >= argc)
            {
                showError(2);
            }
            uheight = stoi(argv[i], 2);
        }
        else
            if( (strcmp(argv[i],"-w")==0) || (strcmp(argv[i], "--width") == 0) )
            {
                wflag = 1;
                if( ++i >= argc)
                {
                    showError(3);
                }
                uwidth = stoi(argv[i], 3);
            }
            else
                if( (strcmp(argv[i],"-c")==0) || (strcmp(argv[i], "--cycles") == 0 ) )
                {
                    if( ++i >= argc)
                    {
                        showError(4);
                    }
                    turns = stoi(argv[i], 4);
                }
                else
                    if( ( strcmp(argv[i],"-f")==0) || (strcmp(argv[i], "--force") ==0) )
                    {
                        force = 1;
                    }
                    else
                        if( ( strcmp(argv[i],"-F")==0) || (strcmp(argv[i], "--full") ==0) )
                        {
                            full = 1;
                        }
                        else
                        {
                            showError(1);
                        }
    }

	if( full )
	{
		height = getHW('h')-1;
		width = getHW('w');
		if(hflag) height = uheight;
	}
	else{
		if( force )
		{
			if(hflag) height = uheight;
			if(wflag) width = uwidth;
		}
		else
		{
			if( hflag )
			{
				if ( uheight > getHW('h') ) showError(5);
				else height = uheight;
			}
			if( wflag )
			{
				if( uwidth > getHW('w') ) showError(6);
				else width = uwidth;
			}
		}
	}
    if (height ==1) height = 2;
	if( !(height&2) ) --height;
	width -= width%4;
	char a[height][width];
	int j, h = height/2;
	double unit = (double)turns*2*M_PI/width;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			a[i][j] = ' ';

	for(j=0;j<width;j++)
	{
		a[h+(int)(h*sin(unit*j))][j] = '*';
	}
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
			printf("%c", a[i][j]);
		putchar('\n');
	}
	return 0;
}
