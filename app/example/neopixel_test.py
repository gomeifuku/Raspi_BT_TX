
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
        

def action(pixels,atime):
    starttime = time.time()

    while 1==1:
        dt=time.time()-starttime
        if dt>ANIM_TIME:
            break
        i=math.floor(ease_in(dt,0,len(pixels),atime))
        pixels[i] = (255, 0, 0)
        pixels.fill((0,0,0))

ANIM_TIME=0.9
LED_MAX_SIZE=8
pixels = neopixel.NeoPixel(board.D18, LED_MAX_SIZE)
pixels.fill((0,0,0))
pixels.show()

while 1==1:
    action(pixels,ANIM_TIME)
