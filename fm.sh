#set -x

audio=fr/sound/"${1}".mp3  

[ -f ~/Downloads/"${1}".mp3 ] && mv -f ~/Downloads/"${1}".mp3 "$audio"

[ -f "${audio}" ] && mplayer >/dev/null 2>&1 "${audio}"
 
