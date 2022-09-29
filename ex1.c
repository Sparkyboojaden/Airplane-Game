/*
===========================Sources used to create===========================
- https://www.youtube.com/channel/UC6qj_bPq6tQ6hLwOBpBQ42Q <---- CodeVault on youtube gave great guides on processes and forking
- https://www.geeksforgeeks.org/wait-system-call-c/ <---- For forking with a wait
============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



//A global fuel variable that can be changed by the signal.
int fuel = 100;

//This signal says which bomber has dropped bombs
void sig1(){
    pid_t plane = getpid();
    printf("Bomber %i to base, bombs away! \n", plane);
}

//this signal sets the fuel of the given bomber to full (100)
void sig2(){
    fuel = 100;
}

int main(int argc, char*argv[])
{
    //for status pids
    int mcstuff[50];
    int loc = 0;
    //for the while loop to keep prompting user what to do
    int x = 1;
    int count = 0;
    //loop prints out for the user to select a command for the base/child processes to run
    while(x != 0){
        int input = 0;
        printf("Select the following by enter the number of the command you wish to execute \n");
        printf("1. launch \n");
        printf("2. bomb \n");
        printf("3. status \n");
        printf("4. refuel \n");
        printf("5. quit \n");
        scanf("%i", &input);  
        //checks for a valid input being put in
        if(input > 0 && input < 6){
            //launch
            if(input == 1){
                //launches a new bomber
                pid_t pid = fork();
                if(pid==0){
                    //instead of using execlp, I just use the code in here bc I used an online compiler
                    //https://www.onlinegdb.com/online_c_compiler#
                    //The below command is what I would run if I wanted to call my program "launch" and store the values in there
                    //execlp("./launch","launch",NULL);
                    
                    //gets value of the bomber
                    int n = getpid();
                    //loop to run while fuel is > 0
                    while(fuel > -6){
                        sleep(3);
                        //if the fuel is out
                        if(fuel < 0)
                        {
                            printf("Bomber %i to base, I HAVE CRASHED!\n", n);
                            kill(n, SIGUSR2);
                            fuel = 0;
                            return 0;
                        //counts down the fuel amount by each bomber currently flying
                        }else{
                            printf("Bomber %i to base, %i fuel left\n", n, fuel);
                            fuel = fuel - 15;
                        }
                        //sig1 waiting to be called (bomb)
                        signal(SIGUSR1, sig1);
                        //sig2 waiting to be called (refuel)
                        signal(SIGUSR2, sig2);
                    }
                //parent, adds values to the status array     
                }else{
                    mcstuff[loc] = pid; 
                    loc = loc + 1;
                }
            }
            //bomb
            if(input == 2){
                int plane;
                //prompts user for the plane to bomb with
                printf("Please enter a plane N to bomb with \n");
                scanf("%i", &plane);
                for (int i = 0; i < loc; i++) {  
                    if(mcstuff[i] == plane)
                    {
                        //calls the sig1
                        kill(plane, SIGUSR1);   
                    }//if the value is not in the current N's, it does not execute. 
                }  
                
            }
            //status, just prints out the array
            if(input == 3){
                printf("Process list: ");
                for (int i = 0; i < loc; i++) {   
                    printf("%i ", mcstuff[i]);     
                }    
                printf("\n");
            }
            //refuel
            if(input == 4){
                int plane2;
                //prompts user for the plane to refuel
                printf("Please enter a plane N to bomb with \n");
                scanf("%i", &plane2);
                for (int i = 0; i < loc; i++) {  
                    if(mcstuff[i] == plane2)
                    {
                        //calls the sig1
                        kill(plane2, SIGUSR2);   
                    }//if the value is not in the current N's, it does not execute. 
                } 
                //calls the sig2
            }
            //quit, just exits the program when 5 (quit) is entered
            if(input == 5){
                x = 0;
                printf("Quitting!");
                exit(1);
            }
        }else{
            printf("Please enter a value between 1 and 5. Restart the program to run again. \n");
            x = 0;
            exit(1);
        }
    }
    return 0;
}