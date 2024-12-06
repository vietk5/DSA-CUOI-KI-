import tkinter as tk
from tkinter import ttk, messagebox, simpledialog, scrolledtext
import pickle

M = 62989  # Số bucket của bảng băm


# Cấu trúc từ điển
class Word:
    def __init__(self, word, word_type, meaning):
        self.word = word.lower()
        self.type = word_type
        self.mean = "- " + "\n- ".join(meaning.split("-"))

    def __str__(self):
        return f"Word: {self.word}\nType: {self.type}\nMeaning:\n{self.mean}"


# Bảng băm
class HashTable:
    def __init__(self):
        self.table = [None] * M

    def _hash(self, word):
        h = sum((ord(char) * (idx + 1) for idx, char in enumerate(word.lower())))
        return h % M

    def add_word(self, word):
        h = self._hash(word.word)
        if not self.table[h]:
            self.table[h] = [word]
        else:
            for w in self.table[h]:
                if w.word == word.word:
                    return False  # Từ đã tồn tại
            self.table[h].append(word)
        return True

    def search_word(self, word):
        h = self._hash(word)
        if self.table[h]:
            for w in self.table[h]:
                if w.word == word:
                    return w
        return None

    def delete_word(self, word):
        h = self._hash(word)
        if self.table[h]:
            for idx, w in enumerate(self.table[h]):
                if w.word == word:
                    del self.table[h][idx]
                    return True
        return False

    def edit_word(self, old_word, new_word):
        if self.delete_word(old_word.word):
            return self.add_word(new_word)
        return False

    def load_from_file(self, filename):
        try:
            with open(filename, "rb") as file:
                data = pickle.load(file)
                if isinstance(data, list) and len(data) == M:  # Kiểm tra tính hợp lệ
                    self.table = data
                else:
                    raise ValueError("Invalid data format in file.")
        except FileNotFoundError:
            print(f"File {filename} not found. Starting with an empty dictionary.")
        except Exception as e:
            print(f"Error loading file {filename}: {e}")

    def save_to_file(self, filename):
        with open(filename, "wb") as file:
            pickle.dump(self.table, file)


def convert_txt_to_binary(input_txt, output_bin):
    hash_table = HashTable()
    try:
        with open(input_txt, "r", encoding="utf-8") as txt_file:
            for line in txt_file:
                parts = line.strip().split()
                if len(parts) >= 3:
                    word, word_type, meaning = parts[0], parts[1], " ".join(parts[2:])
                    new_word = Word(word, word_type, meaning)
                    hash_table.add_word(new_word)
    except Exception as e:
        print("Error reading txt file:", e)
    hash_table.save_to_file(output_bin)

# Dùng script này để tạo file `dictionary.bin` mới
# convert_txt_to_binary("input.txt", "dictionary.bin")


# GUI
class DictionaryApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Dictionary App")
        self.root.geometry("600x400")
        self.root.resizable(False, False)
        
        style = ttk.Style()
        style.theme_use("clam")  # Thay đổi theme giao diện
        
        self.hash_table = HashTable()
        self.file = "GiaoDienDictionary.bin"
        self.hash_table.load_from_file(self.file)

        # Tìm kiếm từ
        frame_search = ttk.Frame(root, padding=(10, 5))
        frame_search.pack(fill=tk.X)
        
        self.search_label = ttk.Label(frame_search, text="Enter word:")
        self.search_label.pack(side=tk.LEFT, padx=(0, 5))
        
        self.search_entry = ttk.Entry(frame_search, width=40)
        self.search_entry.pack(side=tk.LEFT, padx=(0, 5))
        
        self.search_button = ttk.Button(frame_search, text="Search", command=self.search_word)
        self.search_button.pack(side=tk.LEFT)

        # Kết quả
        frame_result = ttk.Frame(root, padding=(10, 5))
        frame_result.pack(fill=tk.BOTH, expand=True)

        self.result_text = scrolledtext.ScrolledText(frame_result, wrap=tk.WORD, height=10, width=70, state=tk.DISABLED)
        self.result_text.pack(fill=tk.BOTH, expand=True)

        # Các nút chức năng
        frame_buttons = ttk.Frame(root, padding=(10, 5))
        frame_buttons.pack(fill=tk.X)

        self.add_button = ttk.Button(frame_buttons, text="Add Word", command=self.add_word)
        self.add_button.pack(side=tk.LEFT, padx=5)

        self.edit_button = ttk.Button(frame_buttons, text="Edit Word", command=self.edit_word)
        self.edit_button.pack(side=tk.LEFT, padx=5)

        self.delete_button = ttk.Button(frame_buttons, text="Delete Word", command=self.delete_word)
        self.delete_button.pack(side=tk.LEFT, padx=5)

        self.save_button = ttk.Button(frame_buttons, text="Save Dictionary", command=self.save_dictionary)
        self.save_button.pack(side=tk.RIGHT, padx=5)

    def search_word(self):
        word = self.search_entry.get().strip().lower()
        if not word:
            messagebox.showwarning("Warning", "Please enter a word to search.")
            return

        result = self.hash_table.search_word(word)
        self.result_text.config(state=tk.NORMAL)
        self.result_text.delete(1.0, tk.END)
        if result:
            self.result_text.insert(tk.END, str(result))
        else:
            self.result_text.insert(tk.END, f"'{word}' not found in the dictionary.")
        self.result_text.config(state=tk.DISABLED)

    def add_word(self):
        word = simpledialog.askstring("Add Word", "Enter the English word:")
        if not word:
            return
        word_type = simpledialog.askstring("Add Word", "Enter the type of word:")
        meaning = simpledialog.askstring("Add Word", "Enter the meaning (use '-' to separate meanings):")

        if word and word_type and meaning:
            new_word = Word(word, word_type, meaning)
            if self.hash_table.add_word(new_word):
                messagebox.showinfo("Success", f"Word '{word}' added successfully.")
            else:
                messagebox.showerror("Error", f"Word '{word}' already exists in the dictionary.")

    def edit_word(self):
        old_word = simpledialog.askstring("Edit Word", "Enter the word to edit:")
        if not old_word:
            return
        result = self.hash_table.search_word(old_word.lower())
        if not result:
            messagebox.showerror("Error", f"Word '{old_word}' not found.")
            return

        word_type = simpledialog.askstring("Edit Word", f"Enter new type of word ({result.type}):")
        meaning = simpledialog.askstring("Edit Word", f"Enter new meaning ({result.mean.strip()}):")

        if word_type and meaning:
            new_word = Word(old_word, word_type, meaning)
            if self.hash_table.edit_word(result, new_word):
                messagebox.showinfo("Success", f"Word '{old_word}' updated successfully.")
            else:
                messagebox.showerror("Error", f"Failed to update word '{old_word}'.")

    def delete_word(self):
        word = simpledialog.askstring("Delete Word", "Enter the word to delete:")
        if not word:
            return
        if self.hash_table.delete_word(word.lower()):
            messagebox.showinfo("Success", f"Word '{word}' deleted successfully.")
        else:
            messagebox.showerror("Error", f"Word '{word}' not found.")

    def save_dictionary(self):
        self.hash_table.save_to_file(self.file)
        messagebox.showinfo("Success", "Dictionary saved successfully.")


if __name__ == "__main__":
    root = tk.Tk()
    # convert_txt_to_binary(r"C:\Users\ADMIN\OneDrive\Desktop\DASA\DSA_CK_BAI4\DSA_BAI04\input.txt","Dictionary.bin")
    app = DictionaryApp(root)
    root.mainloop()
