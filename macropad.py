#
# Copyright (C) 2021 Mark Grosen <mark@grosen.org>
# SPDX-License-Identifier: MIT
#

from time import ticks_ms

from micropython import const
from machine import Pin

PIN_ROTA = 17
PIN_ROTB = 18

PIN_SPEAKER = 16
PIN_SPEAKER_SD = 14

class Button:
    """
    Debounced pin handler
    usage e.g.:
    def button_callback(pin):
        print("Button (%s) changed to: %r" % (pin, pin.value()))
    button_handler = Button(pin=Pin(32, mode=Pin.IN, pull=Pin.PULL_UP), callback=button_callback)
    """

    def __init__(self, pin, callback, arg=None, trigger=Pin.IRQ_FALLING, min_ago=200):
        self.callback = callback
        self.min_ago = min_ago
        self.pin = pin
        self._arg = arg if arg else pin
        self._blocked = False
        self._next_call = ticks_ms() + self.min_ago

        pin.irq(trigger=trigger, handler=self.debounce_handler)

    def call_callback(self, arg):
        self.callback(arg)

    def debounce_handler(self, pin):
        if ticks_ms() > self._next_call:
            self._next_call = ticks_ms() + self.min_ago
            self.call_callback(self._arg)

    def value(self):
        return self.pin.value()
