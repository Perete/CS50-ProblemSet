
letter_counter = 0
word_counter = 1
sentence_counter = 0
comma_counter = 0

userInput = input("Text: ")

for letter in userInput:

    if letter.isalpha():
        letter_counter += 1
        # print(letter)
    elif (letter == ' '):
        print(letter)
        word_counter += 1
    elif (letter == '.' or letter == '?' or letter == '!'):
        sentence_counter += 1

print(f"Length: {len(userInput)}")
print(f"Letters: {letter_counter}")
print(f"Words: {word_counter}")
print(f"Sentences: {sentence_counter}")

avg_letters = letter_counter/word_counter
L = avg_letters*100

avg_sentence = sentence_counter/word_counter
S = avg_sentence*100

index = round(0.0588 * L - 0.296 * S - 15.8)
# index = 0.0588 * L - 0.296 * S - 15.8

if index < 1:
    print("Before Grade 1\n")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
