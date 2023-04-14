# ECE 6780

## Overview

Repo for ECE embedded system final project - Rover

## Group Member  

- Jerry Zhang
- Jared Garey
- Wilson Martinez


## Usage

```
make clean 
make flash  ---- Flash onto connected STM board 
make all    ---- Build
st-util     ---- Open gdb debugeer port
gdb:        ---- To debug
--- target remote localhost:<PORT> 
--- symbol-files  main.elf
```

## Work ethics

```
Master branch contains ready-to-go production code
Dev  branch contains development code. 

In order to ensure we always have a copy of working code, Master branch shouldn't be merged until dev branch is thoroughly tested. 
Normal work flow: 

- git pull origin                            ; sync yourself 
- git checkout dev                           ; go to developement
- write some code ...        
- git add and commit                         ; commit
- git checkout master                        ; Switch to master branch
- git merge dev and fix conflicts            ; Sync production (Make sure your code works !) 
- git push origin master                     ; Production! 

```
MOTOR DIRECTION

The following is the source of true to control the direction of the Rover.
Once we have got the information from the ultrasonic sensor it is necessary to apply a threshold to get the data for the source of true.
The threshold should be define somewhere in between 10 cm (~4 in) to 20 cm (~8 in). Once the threshold is apply we should check a variable 'rover_direction'

L 	F 	R
0	0	0	--> Move Fw
0	0	1	--> Move Fw
0	1	0	--> Turn R 90 degrees
0	1	1	--> Turn L 90 degrees
1	0	0	--> Move Fw
1	0	1	--> Move Fw
1	1	0	--> Turn R 90 degrees
1	1	1	--> Turn R 180 degrees
