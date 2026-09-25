#include "MinHasher.hpp"

using namespace std;

MinHasher::MinHasher(size_t choosenK, size_t choosenNumHash): k(choosenK), numHash(choosenNumHash), q(2305843009213693951), rnd(21){
    signature.assign(choosenNumHash, UINT64_MAX);
    coeff_a.assign(choosenNumHash, 0);
    coeff_b.assign(choosenNumHash, 0);
    hash_coeff();

    word_hashes.assign(choosenK, 0);
    head = 0;
    currentWordHash = 0;
    wordsInBuffer = 0;
    insideWord = false;
};

void MinHasher::hash_coeff() {
	zw = (rnd() % (q - 1)) + 1;
	zs = (rnd() % (q - 1)) + 1;
	
	// zw != zs
	while(zw == zs) {
		zs = (rnd() % (q - 1)) + 1;
	}
	
    for(size_t i = 0; i < numHash; i++) {
        // a deve essere coprimo con q e diverso da 0
        coeff_a[i] = (rnd() % (q - 1)) + 1;
        
        coeff_b[i] = rnd() % q;
    }
};

void MinHasher::process_char(char c) {
	c = tolower(c);
	
    // Fine parola
    if(c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        if(insideWord) {
            word_hashes[head] = currentWordHash;
            head = (head + 1) % k;
            
            if(wordsInBuffer < k) {
                wordsInBuffer++;
            }

            //  Window piena
            if(wordsInBuffer == k) {
                uint64_t x = 0;
                // Si parte dall'elemento più vecchio della window
                size_t idx = head;
                
                // Accumolo polinomiale
                for(size_t i = 0; i < k; ++i) {
                    unsigned __int128 temp = (static_cast<unsigned __int128>(x) * zs) + word_hashes[idx];
                    x = static_cast<uint64_t>(temp % q);
                    
                    idx = (idx + 1) % k;
                }

                // Applicazione delle funzioni di hash
                for(size_t i = 0; i < numHash; i++) {
                    uint64_t hashed = (static_cast<unsigned __int128>(coeff_a[i]) * x + coeff_b[i]) % q;
                    if(hashed < signature[i]) {
                        signature[i] = hashed;
                    }
                }
            }

            currentWordHash = 0;
            insideWord = false;
        }
    } else {
		if(c != '.' && c != '!' && c != '?' && c != ':' && c != ';' && c != ',' && c != '(' && c != ')' && c != '[' && c != ']' && c != '{' && c != '}' && c != '\'' && c != '\"' && c != '/' && c != '-') {
			// Ad ogni carattere letto si aggiorna l'hash
			insideWord = true;
			uint64_t temp = (static_cast<unsigned __int128>(currentWordHash) * zw) + static_cast<unsigned char>(c);
			currentWordHash = static_cast<uint64_t>(temp % q);
		}
    }
}

void MinHasher::newDocument() {
    signature.assign(numHash, UINT64_MAX);
    word_hashes.assign(k, 0);
    head = 0;
    currentWordHash = 0;
    wordsInBuffer = 0;
    insideWord = false;
}

const vector<uint64_t>& MinHasher::getSignature() const {
	return signature;
}

size_t MinHasher::getNumHash() const {
	return numHash;
}
