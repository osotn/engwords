import sys
import os
from gtts import gTTS

def main():
    if len(sys.argv) < 2:
        print("Usage: python tts.py <input_file> [slow]")
        return

    input_file = sys.argv[1].strip()

    if input_file == "":
        print("Error: File name cannot be empty.")
        return

    if not os.path.isfile(input_file):
        print(f"Error: File '{input_file}' not found.")
        return

    slow_mode = False
    if len(sys.argv) >= 3 and sys.argv[2].lower() == "slow":
        slow_mode = True

    with open(input_file, "r", encoding="utf-8") as file:
        text = file.read()

    base_name = os.path.splitext(input_file)[0]
    output_file = f"{base_name}_slow.mp3" if slow_mode else f"{base_name}.mp3"

    tts = gTTS(
        text=text,
        lang="en",
        tld="com",     # 🇺🇸 American English
        slow=slow_mode
    )

    tts.save(output_file)

    print("MP3 file created:", output_file)

if __name__ == "__main__":
    main()

