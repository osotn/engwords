#set -x

for word in "$@"
do
    [ ! -z "${word}" ] && touch words/active/"${word}" && rm -f words/active/"${word}"
done

