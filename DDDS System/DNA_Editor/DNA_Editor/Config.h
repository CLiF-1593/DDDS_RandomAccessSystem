#pragma once
#include <vector>
#include <iostream>

#define IsEncoder true

#define ByteToBPS(x) (unsigned char)(x * 4)
#define BPSToByte(x) (unsigned char)(x * 0.25)

#define IsFailed(x) ((x) == -1)

enum Nucleotide { A, G, C, T, X };
