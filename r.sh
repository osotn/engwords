#set -x
[ ! -z "${1}" ] && touch words/active/"${1}" && rm -f words/active/"${1}"
 
