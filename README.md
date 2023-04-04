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
Note, PA14,15 should not be used (debugger). 

| Resource   | Pin  | Mode | Function| 
| ------     | ---- | ---- | ------- | 
| usart      | PC10 | AF1  | TX      |
| usart      | PC11 | AF1  | RX      |

| gyro       | PC0  | OUT  | EN      | 
| gyro       | PB13 | AF0  | SCK     |
| gyro       | PB14 | AF0  | MISO    | 
| gyro       | PB15 | AF0  | MOSI    |

| motor1     | PA0  | OUT  | BLK     |
| motor1     | PA1  | OUT  | RED     | 
| motor1     | PB10 | AF2  | ENCODER |
| motor1     | PB1  | AF0  | PWM     |

| motor2     | PC7  | OUT  | BLK     |
| motor2     | PA3  | OUT  | RED     |
| motor2     | PC6  | AF0  | ENCODER |
| motor2     | PA2  | AF0  | PWM     |

| hcsr1      | PB3  | IN   | ECHO    |  
| hcsr1      | PA5  | OUT  | TRIGER  |

| hcsr2      | PB4  | IN   | ECHO    |
| hcsr2      | PA6  | OUT  | TRIGER  |

| hcsr3      | PB5  | IN   | ECHO    |
| hcsr3      | PA7  | OUT  | TRIGER  |
---- 
| Timer | Type    | Frequency   | PIN | Desciption |     
| ----- | ------- | ----------  | ------ | --------   | 
| 7     | Basic   | 1MHZ (1us) | N/A | HCSR measure  |
| 2     | Encoder | TBD | PB10 | Motor1 encoder|
| 3     | Encoder | TBD | PC6  | Motor2 encoder|
| 14    | PMW     | TBD | PB1  | Motor pwm|
| 15    | PMW     | TBD | PA2  | Motor pwm|
---- 

### System interrupts

--- 
| Interrupt | Frequency | Description |
| --------- | --------- | ----------- |
| Systick   | 10HZ (100ms) | Update all info| 
--- 

### System Limitation

---

hcsr sensor effective range: 10cm - 80cm. 

motor speed effective range: 

---



