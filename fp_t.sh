#set -x
PS1='\[\033[01;32m\]word\[\033[00m\]\$ '
export PS1
PATH=.:$PATH
export PATH

type xclip 1>/dev/null 2>&1 && echo -n $1 | xclip -sel clip

echo -n $2 >fr/phonetics/$1
echo -n $3 >fr/translations/$1

audio=fr/sound/$1.mp3  

[ -f ~/Downloads/$1.mp3 ] && mv -f ~/Downloads/$1.mp3 $audio

[ -f $audio ] && mplayer >/dev/null 2>&1 $audio
[ ! -z $4 ] && touch fr/words/active/$1
 
