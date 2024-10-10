from machine import Pin
import macropad
        
class Buttons():
    def __init__(self, leds, tracks):
        self.leds = leds
        self.tracks = tracks
        keys = [0] * 12
        for i in range(12):
            keys[i] = macropad.Button(Pin(i+1, Pin.IN, Pin.PULL_UP), self.pushed, arg=i)

    def pushed(self, index):
        u=1
        for i in [2,5,8,11]:
            self.leds.ar[i]=int(self.leds.rgb_to_hex((0,0,0)), 16)
            if index==i:
                self.tracks.var["screen"]=u
                self.leds.ar[index]=int(self.leds.rgb_to_hex((0,0,7)), 16)           
            u=u+1
