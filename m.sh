#set -x

for word in "$@"
do
    audio=sound/"${word}".mp3  
    [ -f ~/Downloads/"${word}".mp3 ] && mv -f ~/Downloads/"${word}".mp3 "$audio"
    [ -f "${audio}" ] && mplayer >/dev/null 2>&1 "${audio}"
    sleep 1
done

