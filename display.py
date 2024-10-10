from machine import Pin, SPI
import asyncio
import gc
from sh1106 import SH1106_SPI

PIN_OLED_CS = 22
PIN_OLED_DC = 24
PIN_OLED_RST = 23

class Display():
    def __init__(self):
        spi = SPI(1, sck=Pin(26, Pin.OUT), mosi=Pin(27, Pin.OUT),
                  polarity=SH1106_SPI.POL, phase=SH1106_SPI.PHA)

        self.display = SH1106_SPI(128, 64, spi, Pin(PIN_OLED_DC, Pin.OUT),
                             cs=Pin(PIN_OLED_CS, Pin.OUT),
                             res=Pin(PIN_OLED_RST, Pin.OUT))
        self.display.rotate(True, True)

    async def handle_display(self, tracks, display, leds, var):
        while True:
            display.fill(0)
            screen=var["screen"]
            if screen==1:
                y=20
                output = []
                display.text("===== PEOS =====", 0, 0, 1)
                display.text("[1]2 3 4 5 6 7 8", 0, 10, 1)
                x = 0
                for track in tracks:
                    value = track['seq'][track['head']]
                    leds.ar[x]=int(leds.rgb_to_hex((value,value,value)), 16)
                    line = f"{track['id']}: {track['head']:#04x} {value:#04x}"
                    output.append(line)
                    display.text(line, 0, y, 1)
                    y=y+10
                    x=x+1
                    if x in (2,5,8,11):
                        x=x+1
            elif screen==2:
                y=20
                output = []
                display.text("==== METERS ====", 0, 0, 1)
                display.text("[1]2 3 4 5 6 7 8", 0, 10, 1)
                x = 0
                for track in tracks:
                    value = track['seq'][track['head']]
                    leds.ar[x]=int(leds.rgb_to_hex((value,value,value)), 16)
                    line = f"{track['id']}: {track['head']:#04x} {value:#04x}"
                    output.append(line)
                    display.text(f"{track['id']}:", 0, y, 1)
                    for i in range(6):
                        display.line(25,y+i+1,int((value/255)*100)+25,y+i+1,1)
                    y=y+10
                    x=x+1
                    if x in (2,5,8,11):
                        x=x+1
            mem=gc.mem_free()
            display.text(f"{mem}", 0, 60, 1)
            display.show()
            leds.sm.put(leds.ar, 8)
            #print(" ".join(output), end='\r')
            await asyncio.sleep(0.05)
