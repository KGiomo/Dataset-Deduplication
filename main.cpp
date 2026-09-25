#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <cstdint>
#include <vector>

#include "MinHasher.hpp"
#include "LSH.hpp"

using namespace std;

vector<uint64_t> read_signature(fstream& file, uint64_t id, size_t numHash) {
    vector<uint64_t> sig(numHash);
    
    streampos offset = (streampos)id * numHash * sizeof(uint64_t);
    
    // Il puntatore di lettura
    file.seekg(offset);
    
    // Lettura del blocco di byte
    file.read(reinterpret_cast<char*>(sig.data()), numHash * sizeof(uint64_t));
    
    return sig;
}

vector<uint64_t> process_stream(MinHasher& hasher, LSH& lsh, double threshold) {
	char c;
	uint64_t idDoc = 0;
	size_t duplicates = 0;
	
	vector<uint64_t> duplicates_list;
	
	fstream file("signatures.bin", ios::in | ios::out | ios::binary | ios::trunc);
    if (!file.is_open()) {
        cerr << "Can't open signatures.bin!" << endl;
        return {};
    }
    
	while(cin.get(c)) {
		// Fine documento
		if(c == '\0') {
			if(hasher.getSignature()[0] != UINT64_MAX) {				
				const auto& similar_docs = lsh.add_signature(hasher.getSignature(), idDoc);
				
				streampos wOffset = (streampos)idDoc * hasher.getNumHash() * sizeof(uint64_t);
				file.seekp(wOffset);
				file.write(reinterpret_cast<const char*>(hasher.getSignature().data()), hasher.getNumHash() * sizeof(uint64_t));
				file.flush();
				
				const vector<uint64_t>& current_sig = hasher.getSignature();
				
                for(uint64_t current_similar : similar_docs) {					
					vector<uint64_t> similar = read_signature(file, current_similar, current_sig.size());
					
					size_t match = 0;
					for(size_t j = 0; j < current_sig.size(); j++) {
						if(current_sig[j] == similar[j]) {
							match++;
						}
					}
					
					double jaccard = (double)match / current_sig.size();
					
					if(jaccard >= threshold) {
						cout << "Found a duplicate! document " << idDoc << " is similar to document " << current_similar << " Jaccard similarity: " << jaccard << endl;
						duplicates_list.push_back(idDoc);
						duplicates++;
						break; 
					}
				}
				
				idDoc++;
			}
			
			hasher.newDocument();
		} else {
			hasher.process_char(c);
		}
	}
	file.close();
	
    cout << "Duplicates found: " << duplicates << endl;
    cout << "Total number of unique documents: " << idDoc - duplicates << endl;
	
	return duplicates_list;
}

int main (int argc, char **argv){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	size_t k = 3;
	size_t functions = 100;
	size_t bands = 20;
	double threshold = 0.8;
	size_t nDocuments = 61315;
	
	
	LSH lsh(bands, functions/bands, nDocuments);
	MinHasher hasher(k, functions);
	vector<uint64_t> list = process_stream(hasher, lsh, threshold);
	
	std::ofstream output("duplicati.txt");
	for(size_t i = 0; i < list.size(); i++) {
		output << list[i] << "\n";
	}
	output.close();
	
	return 0;
}
