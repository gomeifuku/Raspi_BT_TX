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
import ble.ble_main as ble
from gpiozero import Button
from signal import pause
import board
import neopixel
import time
import math
def ease_out(t, b, c, d):
	t /= d
	t = t - 1
	return -c*(t*t*t*t - 1) + b
    
def ease_in_out(t, b, c, d):
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
        i=math.floor(ease_in_out(dt,0,len(pixels),atime))
        pixels[i] = color
        pixels.fill((0,0,0))

    starttime = time.time()
    while 1==1:
        dt=atime-(time.time()-starttime)
        if dt<0:
            break
        i=math.floor(ease_out(dt,0,len(pixels),atime))
        if(i>=len(pixels)):
            i=len(pixels)-1
        if(i<0):
            i=0
        pixels[i] = color
        pixels.fill((0,0,0))

 
isLeft=False

def main():
    ANIM_TIME=0.5
    LED_MAX_SIZE=8
    pixels = neopixel.NeoPixel(board.D18, LED_MAX_SIZE)
    pixels.fill((0,0,0))
    pixels.show()
    bd = ble.BLE_Manager()
    
    def clapping():
        action(pixels,(0,0,255),ANIM_TIME)
        global isLeft
        if isLeft is True:
            bd.sendCWRotation()
        else:
            bd.sendACWRotation()
        isLeft=~isLeft

    action(pixels,(255,0,0),ANIM_TIME)
    action(pixels,(0,255,0),ANIM_TIME)
    action(pixels,(0,0,255),ANIM_TIME)
    
    clap = Button(4)
    clap.when_pressed = clapping
    pause()
    
if __name__ == "__main__":
    main()