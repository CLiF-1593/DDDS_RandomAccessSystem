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
        with open(self.file_path[index], "rb") as f:
            while True:
                data = f.read(1)
                if data == b'':
                    break
                ret.append(data)
        return ret

    def SaveFile(self, index : int, comment : str, data : list):
        with open(self.file_path[index] + comment + ".txt", "wb") as f:
            for i in range(len(data)):
                f.write(data[i])