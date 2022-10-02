# t60p-vol-osd

A simple program that monitors for change in volume (via thinkpad volume buttons). Built and tested on a T60p

Build requirements:
- alsa-lib-devel
- dzen

TODO:
- Show volume OSD even if volume at limits (i.e. it doesn't change but volume button is pressed)
- Replace/fix dzen running over itself with multiple quick presses. libxosd may be a good replacement. 
- Add OSD for mute, screen brightness, other? (I know there is a utility called TPB with all this functionality but it polls for changes and uses a little more CPU than I would like) 
