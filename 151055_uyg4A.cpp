#include <iostream>						//cout function.
#include <stdio.h>						//C functions.
#include <stdlib.h>						//C functions.
#include <unistd.h>						//Linux-C interactions.
#include <ctime>						//time functions.
#include <time.h>						//time functions.
#include <sys/stat.h>						
#include <sys/wait.h>						//wait function.
#include <thread>						//pthread(threadChecker)
#include <math.h>						//To be able to use pow for equations.

using namespace std;

char *input;
struct stat attr;						//To check updates on input file.

long int readFile()
{
	FILE *fp = fopen(input, "r");			//Input txt open in read mode.
	long int readNumber;
	fscanf(fp, "%li", &readNumber);			//readNumber=inp.txt.Text().
    	fclose(fp);						//Close inp.txt file after reading.
   	 return readNumber;						//Return long int readNumber.
}

void alarmClock(int count)					//Alarm Handling.
{
	
	alarm(count);						//Scheduling an alarm for count seconds.
	cout<<"\tALARM CLOCK!!!"<<endl; 			//Times up notification.
	if(count==4)
	cout<<"\tCASE B"<<endl;
	else if (count==6)
	cout<<"\tCASE C"<<endl;
}

void signalCases(int sig) 					//Signal Handling.
{
    switch (sig)
    {
    case SIGINT:
        signal(SIGINT, signalCases);				//Handling Ctrl+C
        cout<<"CTRL C";

    case SIGTSTP:						//Handling Ctrl+Z
        signal(SIGTSTP, signalCases);
    	cout<<"CTRL Z";
    }
    cout<<"\tSignal Handled!"<<endl;
}

int digitCounter(long int number)				//Counting how many digits that number have.
{
    int digit = 0;						//Return Value (int)digit.
    while (number > 0)						//Counting digits by dividing to "10" each step.
    {
        number = number / 10;
        digit++;
    }
    return digit;
}
void checkModification(char *path) 				//Checking Modifications for txt file.
{
    while (true)
    {
        stat(input, &attr);
        if ((time(NULL)) == (attr.st_mtime))
        {
	    cout<<"Last Save:\t"<<ctime(&attr.st_mtime);	//Printing Last Save Time.
	    sleep(1);
	    long int number = readFile();			//Getting the value from txt to (long int) number.
            int digit = digitCounter(number);			//Sending number to digitCounter function and counting digits.(int) digit.
            if (digit < 12)					//Number should be 12 digits.
            {							
                cout << endl << "Number cannot be lower than 100 000 000 000" << endl;
            }
	    else if(digit > 12)
	    {
		cout << endl << "Number cannot be larger than 999 999 999 999" << endl;
	    }
	    
            return ;

        }
        printf("\r%s","Waiting for save..."); 
        fflush(stdout);                               
    }
}
//To catch 90 from 12 digit number.
long int countryControl(long int number,int digit){

	if(digit==12)
	return number/(pow(10,10));
	
	else
	return -1;
}
//To catch 222 from 12 digit number.
long int cityControl(long int number,int digit){

	if (digit==12){
	long int temp;
	temp=countryControl(number,digitCounter(number)*pow(10,3));
	return ((number/(pow(10,7))-temp));
	}
	else 
	return -1;
}

int main(int argc, char *argv[])
{
    
    input = realpath(argv[1], NULL); 	// input = path.
											// Creating pipe-> pipe[0] read and pipe[1] write.
    int pfd[2];			
    pipe(pfd); 			
    cout<<"Pipe has been created..."<<endl;
    long int number = 0;

    signal(SIGALRM, alarmClock);							//Signal Handling.
    char log[250];									//System(log)...
    sprintf(log, "ls -d $PWD/PhoneNumber.txt");
    pid_t pid = fork();								//Forking.

    
    if (pid == 0)									//Child,Writer.
    { 	
    	cout<<"Child\t-> My PID: "<<getpid()<<"\t"<<"PPID: "<<getppid()<<endl;	//Printing PID and PPID.
        thread th_Checker(checkModification, input);					//Modification time checking.
        th_Checker.join();								//Thread joins.
        cout<<endl;
        cout<<"\t->Parsing and printing Phone Number"<<endl;				//Printing
        
        number=readFile();								//Getting "Number" from PhoneNumber.txt
       	
        close(pfd[0]);									//Pipe.
        system(log);									//Printing path.
        write(pfd[1], &number, sizeof(number));      			//Sending(Write) number and sizeof number from pipe(Child).
        sleep(4);   									//Waiting for 4 seconds (sleep).   
    }

    else if (pid > 0)									//Parent,Reader.
    { 
    	cout<<"Parent\t-> My PID: "<<getpid()<<"\t"<<"PPID: "<<getppid()<<endl;	//Printing PID and PPID.
        close(pfd[1]);									//Pipe.
        read(pfd[0], &number, sizeof(number));					//Reading "Number"and sizeof number from pipe(Child).
        cout << "Incoming Message from Child:\t"<<number<< endl;			//Print "Number".
              
        int tr=countryControl(number,digitCounter(number));
        int esk=cityControl(number,digitCounter(number));
        
            	//Reversing the number.
            	long int reverse;							
            	long int reverseNumber=0;						//Return reversed Number variable.
            	reverse=number;

            	while(reverse>0)
		{
			int digit=reverse%10;						//Math.h
			reverseNumber=reverseNumber*10+digit;
			reverse=reverse/10;
		}
 	if (number != 0&&digitCounter(number)==12){
            	//Case A. 
            if(tr == 90 && esk == 222)
            {
               cout<<"\tCountry: Turkey"<<endl;
               cout<<"\tCity: Eskisehir"<<endl;
            }  
        	//Case B.
            else if (reverseNumber>number)
            {                            
                alarmClock(4);
                cout << "Terminating PID: " << pid << " PPID: " << getpid() << endl;
                kill(pid, SIGKILL);								//Kill parent process.
                sleep(4);
            }
            	//Case C.
            else if (tr!= 90)
            {			
            		signal(SIGINT, signalCases);
               	signal(SIGTSTP, signalCases);      		     
				
       		alarmClock(6);								//Case C 6 second alarm.
       		wait(NULL);
            }
 
        }
  
    return 0;
}
}



