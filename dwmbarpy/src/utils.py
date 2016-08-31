#!/usr/bin/env python3
#
# dbus-spotify
#
# (c) 2016 Daniel Jankowski


import os
import sys
import dbus
import time
import socket


from threading import Thread, Event


class KeyboardThread(Thread):

    def __init__(self):
        super().__init__()

        self.stop_event = Event()

        try:
            self.__socket = socket.socket(socket.AF_UNIX, socket.SOCK_RAW)
            self.__socket.bind('/dev/shm/dwmbarpy.socket')
            self.__socket.settimeout(0.1)
        except Exception as e: # If this doesnt work, remove existing
            os.remove('/dev/shm/dwmbarpy.socket')
            self.__socket = socket.socket(socket.AF_UNIX, socket.SOCK_RAW)
            self.__socket.bind('/dev/shm/dwmbarpy.socket')
            self.__socket.settimeout(0.1)


    def run(self):
        last_playing = ''
        while not self.stop_event.is_set():
            # Keyboard shortcuts
            try:
                data = self.__socket.recv(1024)
            except:
                continue
            ws = data.decode('utf-8')
            sys.stdout.write(ws)
            sys.stdout.flush()

            #self.stop_event.wait(0.1)


def main():
    spotify_thread = SpotifyThread()
    keyboard_thread = KeyboardThread()

    spotify_thread.start()
    keyboard_thread.start()


if __name__ == '__main__':
    main()
