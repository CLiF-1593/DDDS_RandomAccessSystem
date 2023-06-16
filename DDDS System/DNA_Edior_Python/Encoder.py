from FileSystem import FileSystem
import DataConvertor
import SpiderWeb
import NoiseGenerator

def EncoderMain():
    print("[DNA Encoder]")

    print("Select Files")
    file_system = FileSystem()
    file_system.OpenFiles()
    for i in range(file_system.FileNumber()):
        byte = file_system.ReadFile(i)
        binary = DataConvertor.ByteToBinary(byte)
        dna_sequence, vt_code = SpiderWeb.Encode(binary, 8)
        print(binary)
        print(dna_sequence)
        print(vt_code)
        dna_sequence = NoiseGenerator.DNA_Noise(dna_sequence, 0.01)
        print(dna_sequence)
        binary = SpiderWeb.Decode(dna_sequence, vt_code, len(binary))
        print(binary)

if __name__ == '__main__':
    EncoderMain()