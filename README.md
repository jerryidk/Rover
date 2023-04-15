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
## Notes

------- 
### Remote USART

- [datasheet](https://www.etechnophiles.com/hc-05-pinout-specifications-datasheet/)
- [connection](https://askubuntu.com/questions/248817/how-to-i-connect-a-raw-serial-terminal-to-a-bluetooth-connection)

Schematic: Select datamode by pull EN low. Rest Pins are self explanatory. 
Commands to connect (pair the bluetooth first, password 0000 or 1234): 

```
    sudo rfcomm connect /dev/rfcomm0 <MAC ADDRESS>
    sudo screen /dev/rfcomm0 9600
```
------ 

### Physical Resource allocation 

---- 
Note, PA13, 14,15 should not be used (debugger). 
Thanks to Wilson: 
PA2, PA3
PA6, PA7
PB0, PB1 
can't be used due to linear touch sensor. 

There are also some other pins can't be used as output,
so test them before you use them. 

PA Moder <- render your board dead. 

| Resource   | Pin  | Mode | Function| 
| ------     | ---- | ---- | ------- | 
| usart      | PC10 | AF1  | TX      |
| usart      | PC11 | AF1  | RX      |
| gyro       | PC0  | OUT  | EN      | 
| gyro       | PB13 | AF0  | SCK     |
| gyro       | PB14 | AF0  | MISO    | 
| gyro       | PB15 | AF0  | MOSI    |
| hcsr1      | PB3  | IN   | ECHO    |  
| hcsr1      | PA8  | OUT  | TRIGGER  |
| hcsr2      | PB4  | IN   | ECHO    |
| hcsr2      | PA9  | OUT  | TRIGGER  |
| hcsr3      | PB5  | IN   | ECHO    |
| hcsr3      | PA10  | OUT  | TRIGGER  |
| motor1     | PC3  | OUT  | RED     |
| motor1     | PC4  | OUT  | BLK     |
| motor2     | PC5  | OUT  | RED     |
| motor2     | PC6  | OUT  | BLK     |
| motor2     | PA4  | AF4  | PWM | TIM14_CH1  |
| motor2     | PB8  | AF2  | PWM | TIM16_CH1  |

### System interrupts

--- 
| Interrupt | Description |
| --------- | ------------|
| Systick   | read gyro and hcsr | 
| EXTI      | read encoder | 
--- 

### TODO

- Encoder ( pick a pin and hook up EXTI interrupt to detect # rev wheel have taken ) 
- PID system ( write a PID system to travel certain distance)
- Finish Motor and build software controller. 
- Build User interface

### MOTOR DIRECTION

The following is the source of true to control the direction of the Rover.
Once we have got the information from the ultrasonic sensor it is necessary to apply a threshold to get the data for the source of true.
The threshold should be define somewhere in between 10 cm (~4 in) to 20 cm (~8 in). Once the threshold is apply we should check a variable 'rover_direction'

| L 	| F 	| R     | Action |
| ---   | ---   | ---   | -----------|
| 0	    | 0	    | 0	    | Move Fw |
| 0	    | 0	    | 1	    | Move Fw |
| 0	    | 1	    | 0	    | Turn R 90 degrees |
| 0	    | 1	    | 1	    | Turn L 90 degrees |
| 1	    | 0	    | 0	    | Move Fw |
| 1	    | 0	    | 1	    | Move Fw |
| 1	    | 1	    | 0	    | Turn R 90 degrees |
| 1	    | 1	    | 1	    | Turn R 180 degrees |

