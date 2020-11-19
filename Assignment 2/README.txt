README

CS-550 Operating Systems Assignment-2, Parts E and F, submitted by-
Name - Bhuvan Chadha
BNumber - B00815783

The submission contains 
1. process_list.c which is the Kernel code for part E
2. user_proc.c which is the User process code for part F
3. A makefile to comile and run the codes
4. A README file

The makefile can be run using 'sudo make' command in the terminal. It will general some kernel module files.
Kernel module can be inserted using 'sudo insmod process_list.ko' command.

The user process file can be run using 'sudo gcc -g user_proc.c'. It will create an a.out file which can be run using 'sudo ./a.out' command which will give the required output.
