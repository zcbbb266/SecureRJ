#include <iostream>
#include <bitset>
#include <vector>
#include <chrono>
#include<utility>

struct Xorshift128 {
    uint64_t state[2];

    Xorshift128(uint64_t seed1, uint64_t seed2) {
        state[0] = seed1;
        state[1] = seed2;
    }

    uint64_t next() {
        uint64_t x = state[0];
        uint64_t y = state[1];
        state[0] = y;
        x ^= x << 23;
        state[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
        return state[1] + y;
    }
 // 生成 128 位的 bitset 随机数
    std::bitset<128> gen128() {
        uint64_t high = next();  // 高64位
        uint64_t low = next();   // 低64位
        
        // 直接通过两个 uint64_t 来构造 128 位的 bitset
        std::bitset<128> result;
        result = std::bitset<128>(high);  // 填充高64位
        result |= std::bitset<128>(low) << 64;  // 将低64位填充到后面
        
        return result;
    }
};

Xorshift128 rng(12345678, 87654321);

int64_t generate_random_int64() {
    return static_cast<int64_t>(rng.next());
}

std::bitset<128> generate_random_128() {
    return rng.gen128();
}

// int64_t generate_random_int64() {
//     std::random_device rd;  
//     std::mt19937_64 gen(rd()); 
    
//     std::uniform_int_distribution<int64_t> dis(INT64_MIN, INT64_MAX);
    
//     return dis(gen);
// }

std::vector<int64_t> gen_rand_vector(size_t sz)
{
    std::vector<int64_t> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=generate_random_int64();
    return rand;
}

std::vector<std::bitset<128>> gen_rand_vector128(size_t sz)
{
    std::vector<std::bitset<128>> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=generate_random_128();
    return rand;
}

int main()
{
    auto res1=gen_rand_vector128(1000000);
    auto res2=gen_rand_vector128(100);
    int64_t sum=0;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<size_t> precomputed_indices(res1.size());
    size_t m=res1.size()/res2.size();
        for (int i = 0; i < res1.size(); i++) {
        precomputed_indices[i] = i / m;
    }

for (int j = 0; j < 128; j++) {
    for (int i = 0; i < res1.size(); i++) {
        res1[i] ^= res2[precomputed_indices[i]];
    }
}
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<__int128> a1(1000000,11516516540ll),a2(1000000,1844515160ll);
    for(int j=0;j<128;j++)
    {
        for(int i=0;i<a1.size();i++)
        {
            int64_t cur1=(a1[i]^(a1[i]>>64));

            // Compute parity directly
            sum+=__builtin_parityll(cur1);
        }
    }
     auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    std::cout << "Function runtime1: " << duration1.count() << " ms" << std::endl;

}