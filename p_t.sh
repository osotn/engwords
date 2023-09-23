#set -x
PATH=.:$PATH
export PATH

type xclip 1>/dev/null 2>&1 && echo -n "${1}" | xclip -sel clip

echo -n "${2}" >phonetics/"${1}"
echo -n "${3}" >translations/"${1}"

audio="${1}".mp3

# Replace spaces on "+" for mp3 name
if  echo ${audio} | grep -q "+"; then
    old_audio="`echo ${audio} | tr "+" " "`"
    [ -f  ~/Downloads/"${old_audio}" ] && mv -f ~/Downloads/"${old_audio}"  ~/Downloads/"${audio}"
fi

[ -f ~/Downloads/"${audio}" ] && mv -f ~/Downloads/"${audio}" sound/"${audio}"


[ -f sound/"${audio}" ] && mplayer >/dev/null 2>&1 sound/"${audio}"

[ ! -z "${4}" ] && touch words/active/"${1}"
 
