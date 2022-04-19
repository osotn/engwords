# To Terminal
cd ~/gitspace/osotn/engwords && tail -f fifo

# Manage external tools
 - Ctrl   +   Alt   +  /    =  run -d 1 (selected text)    # all words for the selected text
 - Ctrl   +   Alt   +  .    =  run -unfm  (selected line)  # unknown words of the selected line
 - Ctrl   +   Alt   +  ,    =  run -d 500 (current word)
 - Ctrl   +   Alt   +  R    =  run -d 4000 (selected text) # read slow selected text
 

# the same as "Ctrl+Alt+."
 - Ctrl   +  Super  +  /    =  run -unfm  (selected line) 

# POS
 - Ctrl   +   Alt   +  '    =  pos_big.sh (current word)
 - Ctrl   +   Alt   +  ;    =  p_pos.sh   (current word)

 - Ctrl   +   Alt   +  ]    =  pos_big3.sh (current word)
 - Ctrl   +   Alt   +  [    =  pos_big4.sh (current word)

 - Ctrl   +   Alt   +  p    =  pos.sh (selected text) 
 - Ctrl   +   Alt   +  2    =  posb (selected text)

 - Ctrl   +   Alt   +  0    =  echo | pos.sh # feed

 - Ctrl   +   Alt   +  I    =  card format (current word)
 
# card
 - Ctrl   +   Alt   +  C    =  card, print window on screen


# Scripts:

Ctrl+Alt+/
#!/bin/sh
# english text fast read
cd ~/gitspace/osotn/engwords
echo $GEDIT_SELECTED_TEXT | ./engwords -d 1 >>./fifo

Ctrl+Alt+R
#!/bin/sh
# english text slow read (dictate)
cd ~/gitspace/osotn/engwords
echo $GEDIT_SELECTED_TEXT | ./engwords -d 4000 >>./fifo

Ctrl+Super+/
#!/bin/sh
# engwords new
cd ~/gitspace/osotn/engwords
echo `date` >>./fifo
echo >>./fifo
echo $GEDIT_SELECTED_TEXT | ./engwords  -unfm >>./fifo
echo >>./fifo

Ctrl+Alt+.
#!/bin/sh
# engwords selected
cd ~/gitspace/osotn/engwords
echo `date` >>./fifo
echo >>./fifo
echo $GEDIT_SELECTED_TEXT | ./engwords -unfm >>./fifo
echo >>./fifo

Ctrl+Alt+,
#!/bin/sh
# engwords word
cd ~/gitspace/osotn/engwords
echo $GEDIT_CURRENT_WORD | ./engwords -d 500 >>./fifo

Ctrl+Alt+0
#!/bin/sh
# pos print empty line
cd ~/gitspace/osotn/engwords
echo | ./pos.sh

Ctrl+Alt+P
#!/bin/sh
# pos print selected text
cd ~/gitspace/osotn/engwords
echo $GEDIT_SELECTED_TEXT | ./pos.sh


Ctrl+Alt+2
#!/bin/sh
# pos big print selected text
cd ~/gitspace/osotn/engwords
echo $GEDIT_SELECTED_TEXT | ./posb

Ctrl+Alt+;
#!/bin/sh
# pos print engwords word
cd ~/gitspace/osotn/engwords
./p_pos.sh $GEDIT_CURRENT_WORD | sed '$d'

Ctrl+Alt+'
#!/bin/sh
# pos print word big
cd ~/gitspace/osotn/engwords
echo $GEDIT_CURRENT_WORD | ./pos_big.sh

Ctrl+Alt+]
#!/bin/sh
# pos print word big3
cd ~/gitspace/osotn/engwords
echo $GEDIT_CURRENT_WORD | ./pos_big3.sh

Ctrl+Alt+[
#!/bin/sh
# pos print word big4
cd ~/gitspace/osotn/engwords
echo $GEDIT_CURRENT_WORD | ./pos_big4.sh

Ctrl+Alt+I
#!/bin/sh
cd ~/gitspace/osotn/engwords
./p_pos.sh $GEDIT_CURRENT_WORD | sed '$d'
echo | ./pos.sh
echo $GEDIT_CURRENT_WORD | ./pos_big.sh
echo | ./pos.sh
[ -f ./definitions/${GEDIT_CURRENT_WORD} ] && cat ./definitions/${GEDIT_CURRENT_WORD} | ./pos.sh
[ -f ./examples/${GEDIT_CURRENT_WORD} ] && echo "--------------------------------" | cat - ./examples/${GEDIT_CURRENT_WORD} | ./pos.sh
echo | ./pos.sh
echo | ./pos.sh

Ctrl+Alt+C
#!/bin/sh
cd ~/gitspace/osotn/engwords
.echo $GEDIT_CURRENT_WORD | ./run.sh -ibwuf | xargs ./ttf &

from gi.repository import Gio, Gtk, Pango
import os

window.get_application().add_accelerator("<Primary>E", "win.engwords", None)
action = Gio.SimpleAction(name="engwords");
action.connect('activate', lambda a,p: engwords())
window.add_action(action)

# Need to change.
engwords_path = "/home/osotn/gitspace/osotn/engwords"

def is_word_unknown(word):
    return not os.path.exists(engwords_path + "/words/active/" + word)

def is_word_untranslated(word):
    return not os.path.exists(engwords_path + "/translations/" + word)

def is_word_xT(word):
    p = engwords_path + "/words/"
    f = "/" + word + ".txt"
    return os.path.exists(p + "first_3000" + f) or os.path.exists(p + "ielts_general_4000" + f) or os.path.exists(p + "toefl_5000" + f) or os.path.exists(p + "first_10000" + f)

def is_word_yT(word):
    return os.path.exists(engwords_path + "/words/first_20000/" + word + ".txt")

def get_word(sel):
    iter, end = sel
    word_start = iter.copy()
    while end.compare(iter) >= 1:
        c = iter.get_char()
        iter.forward_char()
        if c in (' ', '\t', '\n', '\"', ',', '>'):
            if iter.get_offset() - word_start.get_offset() > 1:
                break
            word_start = iter.copy()
    return (word_start, iter.copy())

def transform_word(sel):
    word = ""
    start, end = sel

    # skip the first "(".
    while start.get_char() == "(":
        start.forward_char();

    iter = start.copy()
    while end.compare(iter) >= 1:
        c = iter.get_char()
        if ((not c.isalpha()) and c not in ("'", '-')) or (ord(c[0]) > 127):
            break
        iter.forward_char()
        word += c.lower()
    end.set_offset(iter.get_offset())
    return word

def remove_all_tags(sel):
    buffer = sel[0].get_buffer()

    buffer.remove_tag_by_name("all_text", sel[0], sel[1]);
    buffer.remove_tag_by_name("unknown_word", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_word", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_unknown_word", sel[0], sel[1]);
    buffer.remove_tag_by_name("unknown_word_xT", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_word_xT", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_unknown_word_xT", sel[0], sel[1]);
    buffer.remove_tag_by_name("unknown_word_yT", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_word_yT", sel[0], sel[1]);
    buffer.remove_tag_by_name("untransl_unknown_word_yT", sel[0], sel[1]);


def color_unknown_words(sel):
    buffer = sel[0].get_buffer()

    buffer.create_tag("all_text",     background="#FFFFFF", foreground="#000000");
    buffer.create_tag("unknown_word", background="#FFFFFF", foreground="#008F8F");
    buffer.create_tag("untransl_word", background="#FFFFFF", foreground="#0000FF");
    buffer.create_tag("untransl_unknown_word", background="#FFFFFF", foreground="#FF0000");
    buffer.create_tag("unknown_word_xT", background="#FFFFFF", foreground="#008F8F", underline=Pango.Underline.SINGLE);
    buffer.create_tag("untransl_word_xT", background="#FFFFFF", foreground="#0000FF", underline=Pango.Underline.SINGLE);
    buffer.create_tag("untransl_unknown_word_xT", background="#FFFFFF", foreground="#FF0000", underline=Pango.Underline.SINGLE);
    buffer.create_tag("unknown_word_yT", background="#DFDFDF", foreground="#008F8F", underline=Pango.Underline.SINGLE);
    buffer.create_tag("untransl_word_yT", background="#DFDFDF", foreground="#0000FF", underline=Pango.Underline.SINGLE);
    buffer.create_tag("untransl_unknown_word_yT", background="#DFDFDF", foreground="#FF0000", underline=Pango.Underline.SINGLE);

    remove_all_tags(sel)
    buffer.apply_tag_by_name("all_text", sel[0], sel[1]);

    while sel[1].compare(sel[0]) >= 1:
        ws = get_word(sel)
        #print("1. ", buffer.get_text(ws[0], ws[1], True))
        word = transform_word(ws)
        #print("2. ", word)
        if word:
            if is_word_unknown(word):
                if is_word_untranslated(word):
                    remove_all_tags(ws)
                    if (is_word_xT(word)):
                        buffer.apply_tag_by_name("untransl_unknown_word_xT", ws[0], ws[1])
                    elif (is_word_yT(word)):
                        buffer.apply_tag_by_name("untransl_unknown_word_yT", ws[0], ws[1])
                    else:
                        buffer.apply_tag_by_name("untransl_unknown_word", ws[0], ws[1])
                else:
                    remove_all_tags(ws)
                    if (is_word_xT(word)):
                        buffer.apply_tag_by_name("unknown_word_xT", ws[0], ws[1])
                    elif (is_word_yT(word)):
                        buffer.apply_tag_by_name("unknown_word_yT", ws[0], ws[1])
                    else:
                        buffer.apply_tag_by_name("unknown_word", ws[0], ws[1])
            else:
                if is_word_untranslated(word):
                    remove_all_tags(ws)
                    if (is_word_xT(word)):
                        buffer.apply_tag_by_name("untransl_word_xT", ws[0], ws[1])
                    elif (is_word_yT(word)):
                        buffer.apply_tag_by_name("untransl_word_yT", ws[0], ws[1])
                    else:
                        buffer.apply_tag_by_name("untransl_word", ws[0], ws[1])
            #print("3. ", buffer.get_text(ws[0], ws[1], True))

def engwords():
    buffer = window.get_active_view().get_buffer()
    bounds = buffer.get_selection_bounds()
    if bounds == ():
        bounds = (buffer.get_start_iter(), buffer.get_end_iter())
    color_unknown_words(bounds)

# Test
sel = (buffer.get_start_iter(), buffer.get_end_iter())
ws = get_word(sel)
buffer.get_text(ws[0], ws[1], True)


