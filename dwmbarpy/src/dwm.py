#!/usr/bin/env python3
#
# bspwm.py
#
# (c) 2016 Daniel Jankowski


import gi
import os
import re
import sys
import json
import select
import socket
import _thread
import netifaces
import subprocess


gi.require_version('Notify', '0.7')
from gi.repository import Notify


import utils


I3STATUS_PATH = '/opt/i3status/i3status'
I3STATUS_CONFIG = '/home/neo/Projekte/Python/lemonbarpy/conf/i3status.conf'


class DWM(object):

    def __init__(self):
        super().__init__()

        # Start lemonbar in a subprocess

        # Init notifier for displaying volume stats with dunst
        Notify.init('bar')

        # Panel variables to save intermediate states 
        self.__calendar = None
        self.__volume = None
        self.__vpn = None
        self.__state = None
        self.__spotify = None
        self.spotify_callback_value = None
        self.keyboard_callback_value = None
        self.status = {'date':'', 'spotify':'', 'vol':'', 'wlan':'', 'eth':'', 'vpn':'', 'bat':''}
        self.workspaces = ''
        self.title = ''
        self.current_power = ''
        self.old_power = ''
        
        # Status toggles
        self.__show_eth = False
        self.__show_wlan = False
        self.__show_battery = False
        self.__show_spotify = False


        self.__run = True

    """
    @description
        Generates the status line and writes it into lemonbar.
    """
    def draw_bar(self):
        # Start i3status subprocess to get the status of the system
        s2 = subprocess.Popen([I3STATUS_PATH, '--config', I3STATUS_CONFIG], stdout=subprocess.PIPE)

        # Start utils subprocess for spotify, keyboard-shortcuts etc...
        s3 = subprocess.Popen([os.path.dirname(__file__) + '/utils.py'], stdout=subprocess.PIPE)

        # Register poll to act on the output of both subprocesses
        poll = select.poll()

        # Register subprocess in poll
        poll.register(s2.stdout)
        poll.register(s3.stdout)

        line = ""
        utils = ""

        while self.__run:
            rlist = poll.poll()
            # Iterate through poll events
            for fd, event in rlist:
                # Get stdout of subprocesses
                ws = os.read(fd, 1024).decode('utf-8')

                # If output starts with 'SYS' there are changes in the status line
                # -> regenerate status line
                if ws.startswith('SYS'):
                    stats = ws.split(' °')[1:]
                    self.status = self.generate_status(stats)
                    self.status['spotify'] = utils

                    # If changing to battery or starting on battery, display the battery status
                    if self.current_power != self.old_power and self.current_power == 'BAT':
                        self.__show_battery = True
                    if self.current_power != self.old_power and self.current_power == 'CHR':
                        self.__show_battery = False
                    self.old_power = self.current_power

                    # Write into stdin of lemonbar
                    line = self.generate_line(self.workspaces, self.status, self.title) 
                    self.write_into_bar(line)
                    continue
                elif ws.startswith('CMD'): # Clickable icons
                    ws = ws.lstrip('CMD')
                    if ws.startswith('bat'): # Toggle battery stats
                        self.__show_battery = not self.__show_battery
                    elif ws.startswith('vol'): # Show volume stats in notification
                        notification = Notify.Notification.new("Volume", "Level: {0}".format(self.__volume), "dialog-information")
                        notification.show()
                    elif ws.startswith('wlan'): # Toggle wifi stats
                        self.__show_wlan = not self.__show_wlan
                    elif ws.startswith('eth'): # Toggle ethernet stats
                        self.__show_eth = not self.__show_eth
                    elif ws.startswith('date'):
                        if self.__calendar is None:
                            self.__calendar = subprocess.Popen(['/home/neo/.dotfiles/cal.sh'])
                        else:
                            self.__calendar.terminate()
                            self.__calendar = None
                    elif ws.startswith('ws'): # Open workspace, which is clicked
                        s = subprocess.call(['bspc', 'desktop', '-f', ws[2]])
                    elif ws.startswith('vpn'): # Show VPN in notification
                        notification = Notify.Notification.new("VPN", "Activated: {0}".format(self.__vpn), "dialog-information")
                        notification.show()

                    # Update the status, if to toggle single stats
                    self.status = self.generate_status(self.__state)
                    self.status['spotify'] = utils
                    
                    # Sum up all information in a line for lemonbar with colors and so on
                    line = self.generate_line(self.workspaces, self.status, self.title) 

                    # Write into stdin of lemonbar
                    self.write_into_bar(line)
                    continue

    def generate_line(self, workspaces, status, title):
        line = status['vol'] + status['wlan'] + status['eth'] + status['bat'] + status['date']
        return line

    def shutdown(self):
        print('Shut down bar')
        os.remove('/dev/shm/lemonbarpy.socket')
        self.__run = False

    """
    @description
        Write the line for lemonbar into the stdin of its subprocess and flushes it,
        so lemonbar displays the statusline
    """
    def write_into_bar(self, line):
        s = subprocess.Popen(['xsetroot', '-name', line])
        s.communicate()

    """
    @description
        Generates the status line for lemonbar.
    """
    def generate_status(self, state):
        self.__state = state 

        date = ''
        volume = ''
        wlan = ''
        eth = ''
        battery = ''
        vpn = ''

        for s in state:
            # Volume
            if s.startswith('VOL'):
                s = s.lstrip('VOL')
                # Check if muted
                self.__volume = s
                if s.startswith('muted'):
                    volume = '  '
                else: # If not muted
                    percent = int(s[:-1])
                    # Icon to indicate the volume
                    if percent > 88:
                        volume = '  ⣿ '
                    elif percent <= 88 and percent > 77: 
                        volume = '  ⣷ '
                    elif percent <= 77 and percent > 66: 
                        volume = '  ⣶ '
                    elif percent <= 66 and percent > 55: 
                        volume = '  ⣦ '
                    elif percent <= 55 and percent > 44: 
                        volume = '  ⣤ '
                    elif percent <= 44 and percent > 33: 
                        volume = '  ⣄ '
                    elif percent <= 33 and percent > 22: 
                        volume = '  ⣀ '
                    elif percent <= 22 and percent > 11: 
                        volume = '  ⡀ '
                    elif percent <= 11:
                        volume = '  '
            # Wifi
            elif s.startswith('WLAN'):
                if self.__show_wlan: # With stats
                    if not s.lstrip('WLAN') == 'down': # Wifi down
                        wlan = '  ' + s.lstrip('WLAN') + ' '
                    else: # Wifi connected
                        wlan = ''
                else: # Without stats
                    if not s.lstrip('WLAN') == 'down': # Wifi down
                        wlan = '  '
                    else: # Wifi connected
                        wlan = ''
            # Ethernet
            elif s.startswith('ETH'):
                if self.__show_eth:  # With stats
                    if not s.lstrip('ETH') == 'down': # Eth down
                        eth = '  '+ s.lstrip('ETH') + ''
                    else: # Eth connected
                        eth = ''
                else: # Without stats
                    if not s.lstrip('ETH') == 'down': # Eth down
                        eth = '  '
                    else: # Eth connected
                        eth = ''
            # VPN
            elif s.startswith('VPN'):
                self.__vpn = s.lstrip('VPN')
                vpn_state = ''
                if s.startswith('VPNyes'):
                    interfaces = netifaces.interfaces()
                    for i in interfaces:
                        if i.startswith('ovpn'):
                            vpn_state += netifaces.ifaddresses(i)[2][0]['addr'] + ', '
                    self.__vpn = vpn_state[:-2]
                    vpn = '  '
                else:
                    vpn = '  '
            # Battery
            elif s.startswith('BATT'):
                s = s[4:]
                # If discharging, display the battery status
                self.current_power = re.sub(r'([BATUNKFLCHR]*)\s.*', r'\1', s)

                # If battery is not charging
                if self.__show_battery: # Display battery stats
                    if s.startswith('BAT') or s.startswith('UNK') or s.startswith('FLL'):
                        on_battery = False
                        if s.startswith('BAT'):
                            on_battery = True
                        percent = int(s.split(' ')[1].split(',')[0])
                        s = s.lstrip('BAT ')
                        s = s.lstrip('UNK ')
                        s = s.lstrip('FULL ')
                        #  Change icon related to the actual battery load
                        if percent > 80:
                            battery = '  '
                        elif percent <= 80 and percent > 50:
                            battery = '  '
                        elif percent <= 50 and percent > 30:
                            battery = '  '
                        elif percent <= 30:
                            battery = '  '

                        # Show watt usage when in battery mode
                        if on_battery:
                            with open('/sys/class/power_supply/BAT0/power_now', 'r') as fp:
                                watt = fp.read()
                                watt = re.sub('\.', ',', str(int(watt.rstrip('\n')) / 1000000)[0:4] + 'W')
                            #  Change icon related to the actual battery load
                            if percent > 80:
                                battery = '  ' + s + ' ' + watt + ' '
                            elif percent <= 80 and percent > 50:
                                battery = '  ' + s + ' ' + watt + ' '
                            elif percent <= 50 and percent > 30:
                                battery = '  ' + s + ' ' + watt + ' '
                            elif percent <= 30:
                                battery = '  ' + s + ' ' + watt + ' '
                    elif s.startswith('CHR'): # If it charges, change icon
                        s = s.lstrip('CHR ')
                        battery = '  ' + s + ' '
                else: # Hide battery stats
                    if s.startswith('BAT') or s.startswith('UNK') or s.startswith('FLL'):
                        percent = int(s.split(' ')[1].split(',')[0])
                        s = s.lstrip('BAT ')
                        s = s.lstrip('UNK ')
                        s = s.lstrip('FULL ')
                        #  Change icon related to the actual battery load
                        if percent > 80:
                            battery = '  '
                        elif percent <= 80 and percent > 50:
                            battery = '  '
                        elif percent <= 50 and percent > 30:
                            battery = '  '
                        elif percent <= 30:
                            battery = '  '
                    elif s.startswith('CHR'): # If it charges, change icon
                        s = s.lstrip('CHR ')
                        battery = '  '
            # Date
            elif s.startswith('DATE'):
                date = ' ' + s.lstrip('DATE')[:-1].split(' ')[1] + ' '
        status = {}
        status['date'] = date
        status['vol'] = volume
        status['wlan'] = wlan
        status['eth'] = eth
        status['bat'] = battery
        status['vpn'] = vpn
        return status

def main():
    b = DWM(None)
    b.get_workspaces()
    pass


if __name__ == '__main__':
    main()
