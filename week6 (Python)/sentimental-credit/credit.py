
accumOdd = 0
accumEven = 0
checkSum = 0
next = 1

userCardNumber = input("Card Number: ")
stringLen = len(userCardNumber)
firstDigits = userCardNumber[:2]
firstNumbers = int(userCardNumber[:2])

total_sum = 0
reverse_digits = userCardNumber[::-1]

for i, digit in enumerate(reverse_digits):
    n = int(digit)
    if i % 2 == 1:  # Duplicar cada segundo dígito desde el final
        n *= 2
        if n > 9:
            n -= 9  # Si el resultado es mayor que 9, restamos 9
    total_sum += n

# El número es válido si la suma es divisible por 10
total_sum % 10 == 0

if (firstDigits[0] == '4' and (stringLen == 13 or stringLen == 16) and total_sum % 10 == 0):
    print("VISA")
elif (firstNumbers >= 51 and firstNumbers<=55 and stringLen == 16 and total_sum % 10 ==0):
    print("MASTERCARD")
elif((firstNumbers == 34 or firstNumbers==37) and stringLen == 15 and total_sum % 10 ==0):
    print("AMEX")
else:
    print("INVALID")
