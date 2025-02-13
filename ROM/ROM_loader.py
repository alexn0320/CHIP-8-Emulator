import sys

'''
    Transforms a byte array into a .chip8 executable file
'''

#appends zeros in the ROM file to reach the desired address for the opcode
def append_zeros(data, address, count):

    for i in range(count):
        data.append(bytes(1))
        address += 0x01

    return (data, address)

#read the file as opcodes on separate lines
def read_file_opcodes(path):
    file = open(path, 'r')
    data = file.readlines();

    result = []
    address = 0x00

    for entry in data:
        #get the useful tokens from each line
        a_entry = entry.replace('\n', '')
        a_entry = a_entry.split(' ')
        tokens = []

        for word in a_entry:
            if word != '':
                tokens.append(word)

        #check for addresses
        if tokens[0][0] == '_':
            a_address = int(tokens[0][1:len(tokens[0]) - 1:], 16)
            
            #add zeros until the desired address is reached
            (d, a) = append_zeros(result, address, a_address - address)
            result = d
            address = a

        #get the 2 bytes from each opcode
        result.append(bytes((int(tokens[len(tokens) - 1][:2:], 16),)))
        result.append(bytes((int(tokens[len(tokens) - 1][2:len(tokens[len(tokens) - 1]):], 16),)))

        address += 0x02

    return (1, result)

def main():
    # read from a file, get the name of the input file and the name of the output file
    if len(sys.argv) == 3:
        data = read_file_opcodes(sys.argv[1])

        if data[0] == 0:
            return;

        r_file = open(sys.argv[2], 'wb')

        for entry in data[1]:
            r_file.write(entry)

if __name__ == '__main__':
    main()
