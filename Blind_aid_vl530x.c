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
while(true) {
uint16_t dist = DistanceSensor_readReg(distanceSensorFD);
if(dist > 20&& dist < 8190) { 
printf("Reading: %u mm", dist);printf("\n");

}
else{
printf("Reading: Nothing detected");printf("\n");
}
struct timespec reqDelay={0,20}; 
nanosleep(&reqDelay, (struct timespec *) NULL); 
}
return 0; }
