# Use fontA (12x24, default), it takes 32 character per line.
# Only fold from busybox can work correctly with UTF-8.
busybox fold -w 32 -s | ./utf8_to_pos >> ./pos_dev_link
