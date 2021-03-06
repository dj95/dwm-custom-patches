# lemonbarpy 

Easy to use lemonbar parser.


### Features

- Lightweight status line for bspwm with lemonbar and i3status
- Toggle status elements to icon or icon and text with left mouse button
- Select workspace with mouse click
- Transparency
- Toggle over unix socket connection


### Screenshots

![shot1](./img/shot1.png)
![shot2](./img/shot2.png)


### Usage

To toggle one of the elements of your bar with keyboard shortcuts, just add
run `./lbpyctl.py CMD` with CMD as:

- date
- vol
- wlan
- eth
- vpn
- bat


to your hotkey-daemon(e.g. sxhkd).
`lbpyctl.py` will then connect over a unix socket connection to the bar and
send the correct command for the bar.


### Requirements

- Python 3
- my [i3status](https://github.com/dj95/i3status) fork
- libnotify
- python-gobject
- BSPWM(>= 0.9.1)
- Fonts:
  - Ionicons
  - Serif
  - Terminesspowerline
  - Icons


### Installation

- Just clone this repository to any direction.
- Change the I3STATUS_PATH in `src/bspwm.py` to your i3status-installation
- Change the I3STATUS_CONFIG in `src/bspwm.py` to your i3status-config-path


### License

(c) 2016 Daniel Jankowski

This project is licensed under the MIT License.
Check out [LICENSE](./LICENSE) for more information.
