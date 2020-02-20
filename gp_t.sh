#set -x
PS1='\[\033[01;32m\]word\[\033[00m\]\$ '
export PS1
PATH=.:$PATH
export PATH

echo -n $2 >de/phonetics/$1
echo -n $3 >de/translations/$1

audio=de/sound/$1.mp3  

[ -f ~/Downloads/$1.mp3 ] && mv -f ~/Downloads/$1.mp3 $audio

[ -f $audio ] && mplayer >/dev/null 2>&1 $audio
[ ! -z $4 ] && touch de/words/active/$1
 
