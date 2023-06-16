"""
Examples of restriction enzyme sites.
|cite| Richard J. Roberts (1983) Nucleic Acids Research
"""
cut_segments = ["AGCT", "GACGC", "CAGCAG", "GATATC", "GGTACC", "CTGCAG", "GAGCTC",
                "GTCGAC", "AGTACT", "ACTAGT", "GCATGC", "AGGCCT", "TCTAGA"]
"""

"""
start_codon = ["ATG", "GTG", "TTG"]

"""
A codon that signals the termination of the translation.
|cite| Anthony J. F. Griffiths et al. (2005) Macmillan
"""
stop_codon = ["TAG", "TAA", "TGA"]

"""
Illumina sequencers have been reported to have high error rates following a GGC motif.
|cite| William H. Press et al. (2020) Proceedings of the National Academy of Sciences
"""
illumina_segments = ["GCC"]

"""
Similar structure of nucleotide affects Nanopore Sequencing's recognition of it.
|cite| Ian M. Derrington et al. (2010) Proceedings of the National Academy of Sciences
|cite| Ryan R. Wick et al. (2019) Genome biology
"""
nanopore_segments = ["AGA", "GAG", "CTC", "TCT"]

"""
Local GC stability.
"""
balanced_gc_range = [0.5, 0.5]

"""
Well-accepted GC bias.
|cite| Yaniv Erlich and Dina Zielinski (2017) Science
|cite| Zhi Ping et al. (2021) Nature Computational Science
"""
accepted_gc_bias = [0.4, 0.6]

"""
High GC content range.
|cite| Jake L. Weissman et al. (2019) PLoS Genetics
"""
high_gc_bias = [0.5, 0.7]

"""
Low GC content range.
|cite| Anne-Kathrin Dietel et al. (2019) PLoS Genetics
"""
low_gc_bias = [0.1, 0.3]