import csv
import sys


def main():

    # TODO: Check for command-line usage
    numberOfArgv = len(sys.argv)
    if numberOfArgv < 2 or numberOfArgv > 3:
        print("Incorrect number of arguments")
        sys.exit(1)

    csvFile = sys.argv[1]
    txtFile = sys.argv[2]

    # TODO: Read database file into a variable
    dnaStr = []
    rows =[]
    with open(f'{csvFile}') as database:
        # reader = csv.reader(database, delimiter=',')
        reader = csv.DictReader(database)
        for dna in reader.fieldnames:
            if dna != 'name':
                dnaStr.append(dna)

        for row in reader:
            rows.append(row)

    # print(rows[0]['name'])
    # print(dnaStr)

    # TODO: Read DNA sequence file into a variable
    sequenceFile = open(f'{txtFile}', "r")
    dnaSequence = sequenceFile.readlines()[0]
    # print(dnaSequence)

    # TODO: Find longest match of each STR in DNA sequence
    longMatchSTR = {}
    for dna in dnaStr:
        longMatchSTR[dna] = longest_match(dnaSequence, dna)

    # print(longMatchSTR)

    # TODO: Check database for matching profiles
    match = False
    for person in rows:
        counter = 0
        # print(person)
        for dna in dnaStr:
            # print(person['name'], person[dna], longMatchSTR[dna])
            if int(person[dna]) == int(longMatchSTR[dna]):
                # print(person['name'], person[dna], longMatchSTR[dna])
                counter += 1
                # print(f"counter:{counter}")


        if counter == len(dnaStr):
            match = True
            print(f"{person['name']}")
            break

    if not match:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
