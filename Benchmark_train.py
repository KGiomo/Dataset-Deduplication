import gc
import os
import time
import torch
from transformers import AutoTokenizer, GPT2Config, GPT2LMHeadModel, logging

os.environ["OMP_NUM_THREADS"] = "1"
os.environ["MKL_NUM_THREADS"] = "1"

torch.set_num_threads(1)

logging.set_verbosity_error()

def benchmark_train(file_path, label):
    print(f"\nBenchmark on {label}")

    tokenizer = AutoTokenizer.from_pretrained("gpt2")
    
    tokens = []
    chunk_size = 100000
    max_chunks = 50  

    # Lettura di un numero fisso di blocchi
    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        for _ in range(max_chunks):
            chunk = f.read(chunk_size)
            if not chunk:
                break
            chunk_tokens = tokenizer.encode(chunk, add_special_tokens=False)
            tokens.extend(chunk_tokens)

    print(f"Analysed token: {len(tokens):,}")

    # Tensor
    seq_len = 128
    batch_size = 2
    total_length = (len(tokens) // seq_len) * seq_len
    inputs = torch.tensor(tokens[:total_length], dtype=torch.long).view(-1, seq_len)

    config = GPT2Config(
        vocab_size=tokenizer.vocab_size,
        n_positions=seq_len,
        n_embd=32,  
        n_layer=1,  
        n_head=1, 
    )

    model = GPT2LMHeadModel(config)
    optimizer = torch.optim.AdamW(model.parameters(), lr=1e-3)

    model.train()
    start_time = time.time()
    total_loss = 0.0
    
    # Calcolo proporzionale dei batch:
    total_available_batches = len(inputs) // batch_size
    
    # 10% dei batch
    num_batches = max(int(total_available_batches * 0.10), 10)

    print(f"Execution on {num_batches} batch:")

    for i in range(num_batches):
        batch = inputs[i * batch_size : (i + 1) * batch_size]
        optimizer.zero_grad()

        outputs = model(batch, labels=batch)
        loss = outputs.loss
        loss.backward()
        optimizer.step()

        total_loss += loss.item()

    elapsed = time.time() - start_time
    avg_loss = total_loss / num_batches if num_batches > 0 else 0

    print(f"Elapsed time: {elapsed:.4f} s")
    print(f"Average loss: {avg_loss:.4f}")

    del model, optimizer, inputs, tokens
    gc.collect()

    return


def main():
    benchmark_train("dataset_unito.txt", "Originale")
    benchmark_train("dataset_filtrato.txt", "Filtrato (LSH)")


if __name__ == "__main__":
    main()
