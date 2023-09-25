import numpy as np
import matplotlib
import os

import tkinter as tk  # GUI
import tkinter.filedialog as fd  # file dialogs
import pandas as pd

root = tk.Tk()
root.withdraw()
root.wm_attributes('-topmost', 1)

data_file = fd.askopenfilename(
    title='Select rocket data to import',
    filetypes=(('CSV files', '.csv'), ('All files', '.*')),
    parent=root,
)


raw_data = pd.read_csv(data_file)

print(raw_data.iloc[:, 0])

print(raw_data.columns.tolist())

for i in range(raw_data.size()):

    root.mainloop()
