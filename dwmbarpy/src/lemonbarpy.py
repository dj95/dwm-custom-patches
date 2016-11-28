#!/usr/bin/env python3
#
# lemonbarpy
#
# (c) 2016 Daniel Jankowski


import os
import sys
import json
import signal
import argparse


import dwm


CONFIG_FILE = '/home/neo/Projekte/Python/dwmbarpy/conf/lemonbarpy.json'


# TODO: remove this dirty solution
BAR = None


"""
@description
    Shutdown handler to shut down the bar gracefully.
"""
def sigint_handler(signal, frame):
    BAR.shutdown()
    sys.exit(0)


def main():
    b = dwm.DWM()

    global BAR
    BAR = b

    signal.signal(signal.SIGINT, sigint_handler)
    
    b.draw_bar()


if __name__ == '__main__':
    main()
