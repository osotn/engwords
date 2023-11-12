echo $* | ./run.sh -t
type xclip 1>/dev/null 2>&1 && echo -n $* | xclip -sel clip
