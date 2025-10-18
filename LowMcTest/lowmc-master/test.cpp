#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <map>
#include <random>
uint32_t murmurHash3_32(const void* key, size_t len, uint32_t seed) {
    const uint8_t* data = (const uint8_t*)key;
    uint32_t h = seed;
    uint32_t k;
    
    while (len >= 4) {
        k = *((uint32_t*)data);
        data += 4;
        len -= 4;
        
        k *= 0xCC9E2D51;
        k = (k << 15) | (k >> (32 - 15));
        k *= 0x1B873593;

        h ^= k;
        h = (h << 13) | (h >> (32 - 13));
        h = h * 5 + 0xE6546B64;
    }

    if (len) {
        k = 0;
        for (size_t i = len; i > 0; --i) {
            k = (k << 8) | data[i - 1];
        }
        k *= 0xCC9E2D51;
        k = (k << 15) | (k >> (32 - 15));
        k *= 0x1B873593;
        h ^= k;
    }

    h ^= len;
    h ^= (h >> 16);
    h *= 0x85EBCA6B;
    h ^= (h >> 13);
    h *= 0xC2B2AE35;
    h ^= (h >> 16);

    return h;
}
std::random_device rd;
std::mt19937 gen(rd());  // 使用 Mersenne Twister 算法生成随机数
std::uniform_int_distribution<> dis(0, 1);  // 定义范围 [0, 2]
std::vector<size_t> rand_index(1000000);
int cur_num=0;
class CuckooHashTable {
    public:
        CuckooHashTable(size_t size) 
        : table(size, -1), tableSize(size), maxRetries(tableSize/2), num(0) ,insertPositions(size,-1){
            fail=0;
        }
    
        void insert(int64_t key) {
            mp[key]=num++;
    
            for (int i = 0; i < maxRetries; ++i) {
                
                // size_t idx=rand_index[cur_num++];
                // if(cur_num==rand_index.size()) cur_num=0;
                size_t idx=dis(gen);
                size_t pos;
                if(idx==0)
                    pos = hash1(key);
                else if(idx==1)
                    pos = hash2(key);
                else
                    pos = hash3(key);
                if (table[pos] == -1) {
                    table[pos] = key;
                    insertPositions[mp[key]]=pos; // 记录插入位置
                    return;
                }
                // Kick out the existing key
                int64_t oldKey = table[pos];
                table[pos] = key;
                insertPositions[mp[key]]=pos; // 记录插入位置
                key = oldKey;
            }
            fail++;
        }
    
        const std::vector<size_t> Positions() {
           return insertPositions;
        }
        // MurmurHash as hash1
        size_t hash1(int64_t key) {
            // std::cout<<key<<" "<<((key * 57885161 + 37) % static_cast<int64_t>(tableSize) + tableSize) % tableSize<<'\n';
            return ((key * 57885161 + 37) % static_cast<int64_t>(tableSize) + tableSize) % tableSize;
        }
    
        // Another hash function to provide a different probing sequence
        size_t hash2(int64_t key) {
            // std::cout<<key<<" "<<((key * 982451653 + 101) % static_cast<int64_t>(tableSize) + tableSize) % tableSize<<'\n';
            return ((key * 982451653 + 101) % static_cast<int64_t>(tableSize) + tableSize) % tableSize;
        }
        size_t hash3(int64_t key) {
            // std::cout<<key<<" "<<((key * 982451653 + 101) % static_cast<int64_t>(tableSize) + tableSize) % tableSize<<'\n';
            return ((key ^ 0xdeadbeef) * 1000000007) % tableSize;
        }
        
    
    private:
        std::vector<int64_t> table;
        std::vector<size_t> insertPositions; // 记录每次插入的位置
        size_t tableSize;
        std::map<int,size_t> mp;
        int maxRetries;
        int num;

        // A more complex hash function using a prime multiplier and a bit shift
    public:
        int fail;
    };
int main() {
    // std::random_device rd;
    // std::mt19937 gen(rd());  // 使用 Mersenne Twister 算法生成随机数
    // std::uniform_int_distribution<> dis(0, 2);  // 定义范围 [0, 2]

    // for(int i=0;i<rand_index.size();i++) rand_index[i]=dis(gen);
    
    CuckooHashTable cuckoo(1.6*1000000); 

    uint32_t inserted = 0;
    uint32_t not_inserted = 0;
    for (int i = 0; i < 1000000; i++) {
        int64_t key = i*23; 
        cuckoo.insert(key);
    }
    std::cout<<cuckoo.fail<<'\n';
    return 0;
}
