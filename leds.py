from machine import Pin
import array
import rp2

NUM_LEDS = 12

@rp2.asm_pio(
    sideset_init=rp2.PIO.OUT_LOW,
    out_shiftdir=rp2.PIO.SHIFT_LEFT,
    autopull=True,
    pull_thresh=24,
)

def ws2812():
    # fmt: off
    T1 = 2
    T2 = 5
    T3 = 3
    wrap_target()
    label("bitloop")
    out(x, 1)               .side(0)    [T3 - 1]
    jmp(not_x, "do_zero")   .side(1)    [T1 - 1]
    jmp("bitloop")          .side(1)    [T2 - 1]
    label("do_zero")
    nop()                   .side(0)    [T2 - 1]
    wrap()
    # fmt: on

class Leds():
    def __init__(self):
        self.sm = rp2.StateMachine(0, ws2812, freq=8_000_000, sideset_base=Pin(19))
        self.sm.active(1)
        self.ar = array.array("I", [0 for _ in range(NUM_LEDS)])

    def rgb_to_hex(self, rgb):
        return '0x{:02x}{:02x}{:02x}'.format(rgb[0], rgb[1], rgb[2])
