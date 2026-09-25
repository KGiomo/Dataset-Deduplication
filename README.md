# Dataset-Deduplication

Project for the course of Algorithms for Massive Data.  

---

This project presents a dataset deduplication pipeline using MinHash and Locality-Sensitive
Hashing (LSH) to optimize LLM training datasets. Built in C++, the system compresses documents into fixed-length signature vectors and prunes the candidate search space from quadratic
O(N^2) to sub-quadratic complexity while operating with an acceptable memory usage. Applying this pipeline to raw text reveals that near-duplicate filtering flags significantly more
redundant entries than rigid exact matching.
Benchmarking a simple Python GPT-2 model on this pruned corpus cut training duration without degrading average loss, demonstrating how LSH speeds up execution.

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
