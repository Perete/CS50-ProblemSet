
# heigh = int(input("Pyramid Height: "))

while True:
    userInput = input("Pyramid Height: ")
    try:
        heigh = int(userInput)

        if (heigh > 0 and heigh <9):
            break
        
    except ValueError:
        print("Not an Int")



for i in range(heigh):
    str = ""

    for j in range(heigh-i-1):
        str += " "

    for k in range(i+1):
        str += '#'

    str += "  "

    for h in range(i+1):
        str += '#'

    print(str)



