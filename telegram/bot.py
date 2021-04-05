import sys
import subprocess
from telegram import Bot
from telegram import Update
from telegram.ext import Updater
from telegram.ext import MessageHandler
from telegram.ext import Filters
from telegram.ext import CallbackContext

# botfather
# /newbot
# osotn_myBot
# 1074770057:AAF3kJrlXsF4b44jLRtJX9ZEszi0YNiyO_k
TG_TOKEN = "1074770057:AAF3kJrlXsF4b44jLRtJX9ZEszi0YNiyO_k"

def message_handler(update : Update, _: CallbackContext):
    user = update.effective_user
    if user:
        name = user.first_name
    else:
        name = "anonium"

    text = update.effective_message.text
    reply_text = f'ПРИВЕТ: Hello, {name}!\n\n{text}'

    update.message.reply_text(reply_text)

def endwords():
    process = subprocess.run(["cat", "/home/osotn/gitspace/osotn/engwords/translations/hello"], capture_output=True)
    stdout_as_str = process.stdout.decode("utf-8")
    return stdout_as_str

def main():

    print(endwords())

    bot = Bot(
        token=TG_TOKEN,
    )
    updater = Updater(
        bot=bot,
    )

    handler = MessageHandler(Filters.all, message_handler)
    updater.dispatcher.add_handler(handler)

    updater.start_polling()
    updater.idle()

if __name__ == "__main__":
    main()

 


