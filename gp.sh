echo $* | ./run.sh -g
type xclip 1>/dev/null 2>&1 && echo -n $* | xclip -sel clip
