# Engwords gedit plugin

from gi.repository import GObject, Gedit
from gi.repository import Gio, Gtk, Pango
import os

class EngWordsActivatable(GObject.Object, Gedit.AppActivatable):
   app = GObject.property(type=Gedit.App)
   __gtype_name__ = "EngWordsAppActivatable"

   def __init__(self):
       GObject.Object.__init__(self)

   def do_activate(self):
       #print("Plugin create app for", self.app)
       self.app.add_accelerator("<Primary>E", "win.engwords", None)

   def do_deactivate(self):
       pass


class EngWordsWindowActivatable(GObject.Object, Gedit.WindowActivatable):
    __gtype_name__ = "EngWordsWindowActivatable"

    window = GObject.property(type=Gedit.Window)

    # Need to change.
    engwords_path = "/home/osotn/gitspace/osotn/engwords"

    def is_word_unknown(self, word):
        return not os.path.exists(self.engwords_path + "/words/active/" + word)

    def is_word_untranslated(self, word):
        return not os.path.exists(self.engwords_path + "/translations/" + word)

    def is_word_xT(self, word):
        p = self.engwords_path + "/words/"
        f = "/" + word + ".txt"
        return os.path.exists(p + "oxford_3000_keys" + f) or \
               os.path.exists(p + "ielts_general_4000" + f) or \
               os.path.exists(p + "toefl_5000" + f) or \
               os.path.exists(p + "first_10000" + f)

    def is_word_yzT(self, word):
        p = self.engwords_path + "/words/"
        f = "/" + word + ".txt"
        return os.path.exists(p + "first_20000" + f) or \
               os.path.exists(p + "first_30000" + f)

    def get_word(self, sel):
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

    def transform_word(self, sel):
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

    def remove_all_tags(self, sel):
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

    def create_all_tags(self, sel):
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

    def color_unknown_words(self, sel):
        buffer = sel[0].get_buffer()

        self.create_all_tags(sel)
        self.remove_all_tags(sel)
        buffer.apply_tag_by_name("all_text", sel[0], sel[1]);

        while sel[1].compare(sel[0]) >= 1:
            ws = self.get_word(sel)
            #print("1. ", buffer.get_text(ws[0], ws[1], True))
            word = self.transform_word(ws)
            #print("2. ", word)
            if word:
                if self.is_word_unknown(word):
                    if self.is_word_untranslated(word):
                        self.remove_all_tags(ws)
                        if (self.is_word_xT(word)):
                            buffer.apply_tag_by_name("untransl_unknown_word_xT", ws[0], ws[1])
                        elif (self.is_word_yzT(word)):
                            buffer.apply_tag_by_name("untransl_unknown_word_yT", ws[0], ws[1])
                        else:
                            buffer.apply_tag_by_name("untransl_unknown_word", ws[0], ws[1])
                    else:
                        self.remove_all_tags(ws)
                        if (self.is_word_xT(word)):
                            buffer.apply_tag_by_name("unknown_word_xT", ws[0], ws[1])
                        elif (self.is_word_yzT(word)):
                            buffer.apply_tag_by_name("unknown_word_yT", ws[0], ws[1])
                        else:
                            buffer.apply_tag_by_name("unknown_word", ws[0], ws[1])
                else:
                    if self.is_word_untranslated(word):
                        self.remove_all_tags(ws)
                        if (self.is_word_xT(word)):
                            buffer.apply_tag_by_name("untransl_word_xT", ws[0], ws[1])
                        elif (self.is_word_yzT(word)):
                            buffer.apply_tag_by_name("untransl_word_yT", ws[0], ws[1])
                        else:
                            buffer.apply_tag_by_name("untransl_word", ws[0], ws[1])
                #print("3. ", buffer.get_text(ws[0], ws[1], True))

    def engwords(self):
        #print("Engwords action")
        #print("hello unknown", self.is_word_unknown("hello"))
        #print("hello untranslated", self.is_word_untranslated("hello"))
        #print("hello is xT", self.is_word_xT("hello"))
        #print("hello is yzT", self.is_word_yzT("hello"))
        buffer = self.window.get_active_view().get_buffer()
        sel = buffer.get_selection_bounds()
        if sel == ():
            sel = (buffer.get_start_iter(), buffer.get_end_iter())
        #self.remove_all_tags(sel)
        #self.create_all_tags(sel)
        #self.remove_all_tags(sel)
        #ws = self.get_word(sel)
        #print(" word 1", buffer.get_text(ws[0], ws[1], True), self.transform_word(ws))
        #ws = self.get_word(sel)
        #print(" word 2", buffer.get_text(ws[0], ws[1], True), self.transform_word(ws))
        self.color_unknown_words(sel)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        #print("Plugin created for", self.window)
        action = Gio.SimpleAction(name="engwords");
        action.connect('activate', lambda a,p: self.engwords())
        self.window.add_action(action)

    def do_deactivate(self):
        #print("Plugin stopped for", self.window)
        pass

    def do_update_state(self):
        # Called whenever the window has been updated (active tab
        # changed, etc.)
        #print("Plugin update for", self.window)
        pass

class EngWordsViewActivatable(GObject.Object, Gedit.ViewActivatable):
    __gtype_name__ = "EngWordsViewActivatable"

    view = GObject.property(type=Gedit.View)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        #print("Plugin created for", self.view)
        pass

    def do_deactivate(self):
        #print("Plugin stopped for", self.view)
        pass

    def do_update_state(self):
        # Called whenever the view has been updated
        #print("Plugin update for", self.view)
        pass


