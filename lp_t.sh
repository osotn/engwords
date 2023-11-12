#set -x
PS1='\[\033[01;32m\]word\[\033[00m\]\$ '
export PS1
PATH=.:$PATH
export PATH

type xclip 1>/dev/null 2>&1 && echo -n $1 | xclip -sel clip

echo -n $2 >la/phonetics/$1
echo -n $3 >la/translations/$1

audio="${1}".mp3

# Replace spaces on "+" for mp3 name
if  echo ${audio} | grep -q "+"; then
    old_audio="`echo ${audio} | tr "+" " "`"
    [ -f  ~/Downloads/"${old_audio}" ] && mv -f ~/Downloads/"${old_audio}"  ~/Downloads/"${audio}"
fi

[ -f ~/Downloads/"${audio}" ] && mv -f ~/Downloads/"${audio}" la/sound/"${audio}"

[ -f la/sound/"${audio}" ] && mplayer >/dev/null 2>&1 la/sound/"${audio}"
[ ! -z $4 ] && touch la/words/active/$1
