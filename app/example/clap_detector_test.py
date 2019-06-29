#!/usr/bin/env python3
# -*- coding: utf-8 -*-
 
# Raspberry Pi gpiozero test for
# Verbal Machines VM-CLAP1 clap sensor
#   scruss - 2017-06
#
# Wiring:
#
#  VM-CLAP1:    Raspberry Pi:
#  =========    =============
#    GND     →   GND
#    PWR     →   3V3
#    OUT     →   GPIO 4
#Sorce:https://chicagodist.com/blogs/news/vm-clap1-sensor-gpiozero-on-raspberry-pi

from gpiozero import Button
from signal import pause
 
import board
import neopixel
import time
import math

def ease_in(t, b, c, d):
	t /= d/2.0
	if t < 1:
		return -c/2.0 * (math.sqrt(1 - t*t) - 1)
	t = t - 2	
	return c/2.0 * (math.sqrt(1 - t*t) + 1) + b
        

def action(pixels,color,atime):
    starttime = time.time()

    while 1==1:
        dt=time.time()-starttime
        if dt>ANIM_TIME:
            break
        i=math.floor(ease_in(dt,0,len(pixels),atime))
        pixels[i] = color
        pixels.fill((0,0,0))

ANIM_TIME=0.7
LED_MAX_SIZE=8
pixels = neopixel.NeoPixel(board.D18, LED_MAX_SIZE)
pixels.fill((0,0,0))
pixels.show()

action(pixels,(255,0,0),ANIM_TIME)
action(pixels,(0,255,0),ANIM_TIME)
action(pixels,(0,0,255),ANIM_TIME)

def clapping():
     action(pixels,(0,0,255),ANIM_TIME)
 
clap = Button(4)
clap.when_pressed = clapping
pause()
 