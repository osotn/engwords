# Use fontA (12x24, default), it takes 32 character per line.
# Only fold from busybox can work correctly with UTF-8.
busybox fold -w 8 -s |  ./utf8_to_pos | sed '1s/^/\x1D\x21\x33/' | sed '$ s/$/\x1D\x21\x00/' >> /dev/usb/lp0
