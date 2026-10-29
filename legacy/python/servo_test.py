import time
import RPi.GPIO as GPIO
GPIO.setwarnings(False)

GPIO.setmode(GPIO.BOARD)
GPIO.setup(18, GPIO.OUT)
pwm=GPIO.PWM(18, 50)

a = 0.1
pwm.start(3)

while True:
    pwm.ChangeDutyCycle(2)
    time.sleep(1)
    pwm.ChangeDutyCycle(12)
    time.sleep(1)
