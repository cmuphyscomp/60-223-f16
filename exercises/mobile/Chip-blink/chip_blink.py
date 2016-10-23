import Adafruit_GPIO as GPIO
import time

led_pin = "CSID6"

# gets a gpio object
gpio = GPIO.get_platform_gpio()

# set up the pinmode 
gpio.setup(led_pin, GPIO.OUT)

#blink the led
while True:
    gpio.set_high(led_pin)
    time.sleep(1)
    gpio.set_low(led_pin)
    time.sleep(1)
