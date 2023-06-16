import random

def DNA_Noise(dna_sequence : str, ratio : float):
    if ratio < 0 or ratio > 1: raise "Ratio must be between 0 and 1"
    err_cnt = int(len(dna_sequence) * ratio)
    err_index = []
    index_pool = list(range(len(dna_sequence)))
    for i in range(err_cnt):
        index = random.randrange(len(index_pool))
        err_index.append(index_pool[index])
        index_pool.pop(index)
    dna_table = ['A', 'G', 'C', 'T']
    dna_sequence = list(dna_sequence)
    for i in err_index:
        dna_num = dna_table.index(dna_sequence[i])
        dna_num = (dna_num + random.randint(1,3)) % 4
        dna_sequence[i] = dna_table[dna_num]
    dna_sequence = "".join(dna_sequence)
    return dna_sequence