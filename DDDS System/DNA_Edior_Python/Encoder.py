from FileSystem import FileSystem
import DataConvertor

def EncoderMain():
    print("[DNA Encoder]")
    print("Select Files")
    file_system = FileSystem()
    file_system.OpenFiles()
    for i in range(file_system.FileNumber()):
        byte = file_system.ReadFile(i)
        print(byte)
        binary = DataConvertor.ByteToBinary(byte)
        print(binary)
        print(DataConvertor.BinaryToByte(binary))


if __name__ == '__main__':
    EncoderMain()