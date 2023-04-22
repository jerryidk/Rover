# Rover Doc

## Overview

Repo for ECE embedded system final project. An rover developed using STM32 board.

### Function
| Key | Description | 
| --- | ----------- |
| w   | Move Forward|
| a   | Turn Left   |
| s   | Move Backward|
| d   | Turn Right |
| m   | Maza Mode, autononmous driving mode|
| l   | Increase Duration |
| k   | Decrease Duration |
| o   | Increase PWM |
| i   | Decrease PWM |
| g   | PID linear control |
| +   | Increase PID distance |
| -   | Decrease PID distance |

### Stats
The rover connects via a remote usart module to receive commands. There will be a print out the system information which includes

| Var | Description | 
| ----| ------------ |
| Tick | system event count |
| DPS  | Angular velocity |
| Orientation | Rover orientation |
| PWM         | how much Voltage deliver to the motor |
| Duration    | how long the motor should stay at given PWM  |
| Front/Left/Right Distance | Ultrasonic sensor data, how far obstacle are |
| Error | PI control error |
| Output | PI control output pwm |

### Remote USART

- [datasheet](https://www.etechnophiles.com/hc-05-pinout-specifications-datasheet/)
- [connection](https://askubuntu.com/questions/248817/how-to-i-connect-a-raw-serial-terminal-to-a-bluetooth-connection)

Schematic: Select datamode by pull EN low. Rest Pins are self explanatory. 
Commands to connect (pair the bluetooth first, password 0000 or 1234): 

```
    sudo rfcomm connect /dev/rfcomm0 <MAC ADDRESS>
    sudo screen /dev/rfcomm0 9600
```

### Group Member  

- Jerry Zhang
- Jared Garey
- Wilson Martinez

---
## Notes to Dev 

### Basic command
```
make clean 
make flash  ---- Flash onto connected STM board 
make all    ---- Build
st-util     ---- Open gdb debugeer port
gdb:        ---- To debug
--- target remote localhost:<PORT> 
--- symbol-files  main.elf
```

### Work ethics

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
### Code organization

The code is organized by functionality and hardware. the file name usually associates with a specific hardware. Ex. hcsr means ultrasonic sensor driver code. Main application for Rover is in main.c as usual. boot.s contains a bootloader put you into main. 

---
## System Design
 
### Schematic

- PA13, 14,15 should not be used (debugger). Thanks to Wilson PA2, PA3, PA6, PA7, PB0, PB1 can't be used due to linear touch sensor. 
- There are also some other pins can't be used as output,
so test them before you use them. 
- DON"T clear entire PA Moder, it will render your board dead. 

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
| Systick   | Update gyro  | 
| EXTI      | Rotational encoder | 
| TIMER2    | pid control  |
--- 




