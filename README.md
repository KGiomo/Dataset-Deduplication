# File-Deduplication

Project for the course of Algorithms for Massive Data.  

---

## Dataset Preparation

```bash
mkdir -p temp_dataset
unzip -q tcp_dataset.zip -d temp_dataset
find temp_dataset -type f -name "*.txt" -exec sh -c 'cat "$1"; printf " \n\0"' _ {} \; > dataset_unito.txt
```

---

## C++ Compilation

Compile the C++ source files using `-O3` optimization:

```bash
g++ -O3 -march=native *.cpp -o progetto
```

---

## Execution

Run the executable by streaming the concatenated dataset via standard input. The program outputs execution metrics and saves the detected duplicate document IDs into `duplicati.txt`:

```bash
/usr/bin/time -v ./progetto < dataset_unito.txt
```

---
