import tkinter as tk
from tkinter import filedialog

class FileSystem:
    file_path = []

    def OpenFiles(self):
        root = tk.Tk()
        root.withdraw()
        self.file_path = filedialog.askopenfilenames()

    def FileNumber(self):
        return len(self.file_path)

    def ReadFile(self, index : int):
        ret = []
        with open(self.file_path[index], "rb") as file:
            while True:
                data = file.read(1)
                if data == b'':
                    break
                ret.append(data)
        return ret

    def ReadFileAsString(self, index : int):
        with open(self.file_path[index], "r") as file:
            data = file.read().replace('\n', '')
        return data

    def SaveFile(self, index : int, comment : str, data : list):
        with open(self.file_path[index] + comment + ".txt", "wb") as file:
            for i in range(len(data)):
                file.write(data[i])