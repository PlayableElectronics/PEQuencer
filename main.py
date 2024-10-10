import time
import asyncio
#import aiorepl
from display import Display
from tracks import Tracks
from leds import Leds
from buttons import Buttons
      
async def main():        
    display = Display()
    tracks = Tracks()
    leds = Leds()
    buttons = Buttons(leds, tracks)
    leds.ar[2]=int(leds.rgb_to_hex((0,0,7)), 16)
    tasks = [asyncio.create_task(tracks.handle_track(track)) for track in tracks.tracks]
    display_task = asyncio.create_task(display.handle_display(tracks.tracks, display.display, leds, tracks.var))
    # Start the aiorepl task.
    #repl = asyncio.create_task(aiorepl.task())

    await asyncio.gather(*tasks, display_task)

asyncio.run(main())

