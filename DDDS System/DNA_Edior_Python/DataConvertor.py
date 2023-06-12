
def ByteToBinary(byte : list):
    binary = []
    for i in range(len(byte)):
        b = byte[i]
        bin_tmp = []
        b = int.from_bytes(b, "big")
        while b:
            bin_tmp.append(b % 2)
            b //= 2
        bin_tmp += [0] * (8 - len(bin_tmp))
        binary += reversed(bin_tmp)
    return binary

def BinaryToByte(binary : list):
    byte = []
    for i in range(0, len(binary), 8):
        num = binary[i : i + 8]
        value = 0
        add = 1
        for j in range(len(num) - 1, -1, -1):
            if num[j]:
                value += add
            add *= 2
        byte.append(value.to_bytes(1, "little"))
    return byte