from dsw import spiderweb, biofilter
from numpy import array
import BiofilterArgumentPreset

import numpy, sys

#Setting Data
import DataConvertor

observed_length = 4
cut_length = 120
bio_filter = biofilter.LocalBioFilter(observed_length=observed_length,
                                          max_homopolymer_runs=2,
                                          gc_range=BiofilterArgumentPreset.accepted_gc_bias,
                                          undesired_motifs=BiofilterArgumentPreset.illumina_segments)
vertices = spiderweb.find_vertices(observed_length=observed_length, bio_filter=bio_filter).astype(int)
accessor = spiderweb.connect_valid_graph(observed_length=observed_length, vertices=vertices)

# Print Every Array Data
# numpy.set_printoptions(threshold=sys.maxsize)

def Encode(binary : list, vt_code_length : int):
    # Setting Data (Binary Message)
    binary_message = array(binary)

    # Setting Vertices and Accessor
    dna_sequence = ""
    vt_check = []

    # Encoding
    start_index = 0
    for i in range(len(vertices)):
        if vertices[i]:
            try:
                dna_sequence = spiderweb.encode(accessor=accessor, binary_message=binary_message,start_index=i)
                start_index = i
                break
            except: pass
    print("StartIndex : ", start_index)

    if dna_sequence:
        for i in range(0, len(dna_sequence), cut_length):
            vt_check.append(spiderweb.set_vt(dna_sequence[i : min(len(dna_sequence), i + cut_length)], vt_code_length))

    return dna_sequence, vt_check

def Decode(dna_sequence : str, vt_check : list, bit_length : int):
    binary_message = array([])
    start_index = 5

    if dna_sequence:
        for i in range(0, len(dna_sequence), cut_length):
            dna, additional_data = spiderweb.repair_dna(dna_sequence=dna_sequence[i: min(len(dna_sequence), i + cut_length)],
                                                        accessor=accessor, start_index=start_index, observed_length=observed_length,
                                                        vt_check=vt_check.pop(0), has_indel=False)
            print(dna)
            print(additional_data)
    #binary_message = spiderweb.decode(accessor=accessor, dna_sequence=dna_sequence, start_index=start_index, bit_length=bit_length)
    return binary_message