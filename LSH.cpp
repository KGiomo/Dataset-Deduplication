#include "LSH.hpp"

using namespace std;

LSH::LSH(size_t nBands, size_t nRows, size_t nDocuments): b(nBands), r(nRows), q(2305843009213693951), rnd(55) {
	bands.resize(b);
	
	z = (rnd() % (q - 1)) + 1;
	
	for(size_t i = 0; i < b; i++) {
		// Prealloc
		bands[i].reserve(nDocuments / 2);
	}
}

uint64_t LSH::band_hash(const vector<uint64_t>& signature, size_t startB) {
	uint64_t key = 0;
	
	// Hashing delle r righe
	for(size_t i = startB; i < startB + r; i++) {
		unsigned __int128 temp = (static_cast<unsigned __int128>(key) * z) + signature[i];
		key = static_cast<uint64_t>(temp % q);
	}
	
	return key;
}

unordered_set<uint64_t> LSH::add_signature(const vector<uint64_t>& signature, size_t id) {
	unordered_set<uint64_t> similar_docs;
	
	for(size_t i = 0; i < b; i++) {
		uint64_t key = band_hash(signature, i*r);
		
		vector<uint64_t>& bucket = bands[i][key];
		
		for(size_t j = 0; j < bucket.size(); j++) {
			similar_docs.insert(bucket[j]);
		}
		
		bucket.push_back(id);
	}
	
	return similar_docs;
}

size_t LSH::getB() const {
	return b;
}
