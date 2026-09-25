#include <cstdint>
#include <vector>
#include <random>
#include <unordered_set>
#include <unordered_map>

class LSH {
public:
	LSH(size_t nBands, size_t nRows, size_t nDocuments);
	std::unordered_set<uint64_t> add_signature(const std::vector<uint64_t>& signature, size_t id);
	size_t getB() const;
private:
	const size_t b;
	const size_t r;
	
	const uint64_t q;
	uint64_t z;
	std::mt19937_64 rnd;
	
	// Ogni banda mappa un hash ad una lista di id di documenti
	std::vector<std::unordered_map<uint64_t, std::vector<uint64_t>>> bands;
	
	uint64_t band_hash(const std::vector<uint64_t>& signature, size_t startB);
};
