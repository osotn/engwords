from gi.repository import Gio
window.get_application().add_accelerator("<Primary>E", "win.engwords", None)
action = Gio.SimpleAction(name="engwords");
action.connect('activate', lambda a,p: engwords())
window.add_action(action)

import os
def is_word_unknown(word):
    return not os.path.exists("/home/osotn/gitspace/osotn/engwords/words/active/" + word)

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
    iter = start.copy()
    while end.compare(iter) >= 1:
        c = iter.get_char()
        if (not c.isalpha()) and c not in ("'", '-'):
            break
        iter.forward_char()
        word += c.lower()
    end.set_offset(iter.get_offset())
    return word

def color_unknown_words(sel):
    buffer = sel[0].get_buffer()
    buffer.create_tag("unknown_word", background="#FFFFFF", foreground="#008F8F");
    while sel[1].compare(sel[0]) >= 1:
        ws = get_word(sel)
        #print("1. ", buffer.get_text(ws[0], ws[1], True))
        word = transform_word(ws)
        #print("2. ", word)
        if word and is_word_unknown(word):
            buffer.apply_tag_by_name("unknown_word", ws[0], ws[1])
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

