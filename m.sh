#set -x

for word in "$@"
do
    audio="${word}".mp3

    # Replace spaces on "+" for mp3 name
    if  echo ${audio} | grep -q "+"; then
        old_audio="`echo ${audio} | tr "+" " "`"
        [ -f  ~/Downloads/"${old_audio}" ] && mv -f ~/Downloads/"${old_audio}"  ~/Downloads/"${audio}"
    fi

    [ -f ~/Downloads/"${word}".mp3 ] && mv -f ~/Downloads/"${word}".mp3 sound/"$audio"
    [ -f sound/"${audio}" ] && mplayer >/dev/null 2>&1 sound/"${audio}"
    sleep 1
done

