#set -x
PS1='\[\033[01;32m\]word\[\033[00m\]\$ '
export PS1
PATH=.:$PATH
export PATH

type xclip 1>/dev/null 2>&1 && echo -n $1 | xclip -sel clip

echo -n $2 >tr/phonetics/$1
echo -n $3 >tr/translations/$1

audio="${1}".mp3  

# Replace spaces on "+" for mp3 name
if  echo ${audio} | grep -q "+"; then
    old_audio="`echo ${audio} | tr "+" " "`"
    [ -f  ~/Downloads/"${old_audio}" ] && mv -f ~/Downloads/"${old_audio}"  ~/Downloads/"${audio}"
fi

[ -f ~/Downloads/"${audio}" ] && mv -f ~/Downloads/"${audio}" tr/sound/"${audio}"

[ -f tr/sound/"${audio}" ] && mplayer >/dev/null 2>&1 tr/sound/"${audio}"
[ ! -z "${4}" ] && touch tr/words/active/"${1}"

