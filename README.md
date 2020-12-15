# Blind-Aid-device-using-Vl530x-sensor
Interfacing Vl530x using I2c protocol on beaglebone black

This is a wearable device which is worn around waist/chest.This device vibrates when there is an obstacle around the blind person.
The intensity of the vibrator tells about the proximity of the obstacle.

for vibration of motor I've used pwm, where in period is set to 100000000:
     range(mm)  duty_cycle
     20-150     100000000
     150-300    75000000
     300-450    50000000
     450-750    25000000
     above750   0
