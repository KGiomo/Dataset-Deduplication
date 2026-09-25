#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <vector>
#include <cstdint>
#include <random>

class MinHasher {
public:
	MinHasher(size_t choosenK, size_t choosenNumHash);
	void process_char(char c);
	void newDocument();
	const std::vector<uint64_t>& getSignature() const;
	size_t getNumHash() const;
private:
	void hash_coeff();
	
	const size_t k;
	const size_t numHash;
	std::vector<uint64_t> coeff_a;
	std::vector<uint64_t> coeff_b;
	const uint64_t q;
	// Base per i caratteri
	uint64_t zw;
	// Base per le parole
	uint64_t zs;
	std::mt19937_64 rnd;
	std::vector<uint64_t> signature;
	
	// Buffer circolare
	std::vector<uint64_t> word_hashes; 
	size_t head;                 
	uint64_t currentWordHash; 
	size_t wordsInBuffer;           
	bool insideWord;                
};
