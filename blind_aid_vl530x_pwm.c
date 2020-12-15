#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include<fcntl.h> 
#include<unistd.h> 
#include<sys/ioctl.h> 
#include<linux/i2c.h> 
#include<linux/i2c-dev.h> 
#include<stdbool.h> 
#include<stdint.h>

#define MAX_BUF 64
#define I2C_LINUX_BUS2 "/dev/i2c-2"
#define DISTANCE_SENSOR_DEVICE_ADDRESS 0x29
#define VL53L0X_REG_SYSRANGE_START 0x00 
#define VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK 0x02
#define DISTANCE_SENSOR_READING_MSB 0x1e 
#define NUM_BYTES_READ 2 


static void DistanceSensor_configPins() {
FILE*pipe=popen("config-pin P9_19 i2c","r"); 
int exitCode = WEXITSTATUS(pclose(pipe)); 
if(exitCode!=0){
printf("Program failed: %d", exitCode); 
}
pipe=popen("config-pin P9_20 i2c","r");
exitCode=WEXITSTATUS(pclose(pipe)); 
if(exitCode!=0){
printf("/nProgram failed: %d", exitCode); }
}


static int DistanceSensor_init() 
{ 
DistanceSensor_configPins();
int distanceSensorFD = open(I2C_LINUX_BUS2, O_RDWR); 
int result = ioctl(distanceSensorFD, I2C_SLAVE,DISTANCE_SENSOR_DEVICE_ADDRESS); 
if(result < 0){
perror("I2C: Unable to set I2C device to slave address.");
exit(1); }
return distanceSensorFD; }

static void DistanceSensor_setContinuous(int distanceSensorFD){ 
unsigned char buff[2];
buff[0] = VL53L0X_REG_SYSRANGE_START;
buff[1] = VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK; 
int res=write(distanceSensorFD,buff,2);
if(res != 2) {
perror("I2C: Unable to write i2c register..."); 
exit(1);
} }



static uint16_t DistanceSensor_readReg(int distanceSensorFD) {
char values[NUM_BYTES_READ];
unsigned char startRegAddr = DISTANCE_SENSOR_READING_MSB; 
for(int i=0;i<NUM_BYTES_READ;i++){
unsigned char currAddr = startRegAddr + i;
 int res=write(distanceSensorFD,&(currAddr),sizeof(currAddr));
  if(res != sizeof(currAddr)) {
perror("I2C: Unable to write to i2c register.");
exit(1); }
res=read(distanceSensorFD,&values[i],sizeof(*values));
 if(res != sizeof(*values)) {
perror("I2C: Unable to read from i2c register");
exit(1); }
}
// Convert the reading to distance
uint16_t dist=(((uint16_t)(values[0])<<8)|(uint16_t)(values[1])); 
return dist;
}

int main(){
int distanceSensorFD = DistanceSensor_init();
DistanceSensor_setContinuous(distanceSensorFD);
 int fd_enable,fd_period,fd_dutycycle, len;
 char buf[MAX_BUF]; 
 //enable
  snprintf(buf, sizeof(buf), "/sys/class/pwm/pwmchip4/pwm-4:0/enable");
 fd_enable = open(buf, O_WRONLY);
 if (fd_enable < 0) {
 perror("pwm/enable error");
 return 0;
 } 
 len = snprintf(buf, sizeof(buf), "%d", 1);
 write(fd_enable, buf, len);
 
 //period
 snprintf(buf, sizeof(buf), "/sys/class/pwm/pwmchip4/pwm-4:0/period");
 fd_period = open(buf, O_WRONLY);
 if (fd_period < 0) 
 {perror("pwm/period");return 0;}
 
 len = snprintf(buf, sizeof(buf), "%lu", 100000000); //20ms period for servo
 write(fd_period, buf, len);
 
 // duties
 fd_dutycycle = open("/sys/class/pwm/pwmchip4/pwm-4:0/duty_cycle", O_WRONLY);
 if (fd_dutycycle < 0) {
 perror("pwm4/duty");
 return 0;
 }
 
while(true) {
uint16_t dist = DistanceSensor_readReg(distanceSensorFD);
if(dist > 20 && dist < 150) 
{ 
printf("Reading: %u mm", dist);printf("\n");
len = snprintf(buf, sizeof(buf), "%lu", 100000000);
write(fd_dutycycle, buf, len);
}
else if(dist > 150 && dist < 300) 
{ 
printf("Reading: %u mm", dist);printf("\n");
len = snprintf(buf, sizeof(buf), "%lu", 75000000);
write(fd_dutycycle, buf, len);
}
else if(dist > 300 && dist < 450) 
{ 
printf("Reading: %u mm", dist);printf("\n");
len = snprintf(buf, sizeof(buf), "%lu", 50000000);
write(fd_dutycycle, buf, len);
}
else if(dist > 450 && dist < 750) 
{ 
printf("Reading: %u mm", dist);printf("\n");
len = snprintf(buf, sizeof(buf), "%lu", 25000000);
write(fd_dutycycle, buf, len);
}
else{
printf("Reading: Nothing detected");printf("\n");
printf("Reading: %u mm", dist);printf("\n");
len = snprintf(buf, sizeof(buf), "%lu", 0);
write(fd_dutycycle, buf, len);
}
struct timespec reqDelay={0,2000000}; 
nanosleep(&reqDelay, (struct timespec *) NULL);

}
return 0; }
