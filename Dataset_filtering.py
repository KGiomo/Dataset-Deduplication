def read_file(dataset_file, size = 4 * 1024 * 1024):
	# Lettura a finestra di 4 MB, oggetto byte
	buffer = b""
	while True:
		window = dataset_file.read(size)
		if not window:
			if buffer:
				# Per mantenere la funzione in pausa
				yield buffer
			break
		buffer = buffer + window
		files = buffer.split(b'\0')
		buffer = files.pop()
		
		for f in files:
			yield f
		
def dataset_filtering(dataset_file, filtered_file, duplicates_file):
    # Lista dei duplicati
    with open(duplicates_file, "r", encoding="utf-8") as dup_f:
        duplicates = {int(line) for line in dup_f if line.strip()}

    # Filtraggio del dataset, lettura binaria
    with open(dataset_file, "rb",) as in_f, open(filtered_file, "w", encoding="utf-8") as out_f:
        for idDoc, bytesDoc in enumerate(read_file(in_f)):
            # Se l'ID appartiene ai duplicati viene salrato
            if idDoc in duplicates:
                continue

            # Rimozione di eventuali caratteri non validi
            text = bytesDoc.decode("utf-8", errors="ignore").strip()

            if not text:
                continue

            # Scrittura nel file finale deduplicato
            out_f.write(text + "\n|endoftext|>\n")

def main(args):
	dataset_file = "/home/giovanni/Scrivania/ADM/dataset_unito.txt"
	filtered_file = "/home/giovanni/Scrivania/ADM/dataset_filtrato.txt"
	duplicates_file = "/home/giovanni/Scrivania/ADM/duplicati.txt"
	
	dataset_filtering(dataset_file, filtered_file, duplicates_file)

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
