#pragma once

#include <random>
#include <cstdint>
#include <vector>

#include "../Value/value.h"
#include "../network/clients.hpp"


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






Value generate_int64_numbers(size_t sz) {
    std::vector<int64_t> numbers(sz);
    std::random_device rd;
    std::mt19937_64 gen(rd()); // 64位梅森旋转算法
    std::uniform_int_distribution<int64_t> dis;
    for (int i = 0; i < sz; i++) {
        numbers[i] = dis(gen);
    }
    return Value(std::move(numbers));
}

Value generate_int_numbers(size_t sz) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<uint32_t> dist(0, 3e8);
    std::vector<int64_t> numbers(sz);
    for (int i = 0; i < sz; i++) {
        numbers[i] = dist(gen);
    }
    return Value(std::move(numbers));
}

int64_t generate_random_int64() {
    return static_cast<int64_t>(rng.next());
}

int64_t generate_random_int64_with_key(int64_t key1,int64_t key2) {
    Xorshift128 randV(key1, key2);
    return static_cast<int64_t>(randV.next());
}

template<size_t N>
std::bitset<N> generate_random_bitset_with_key(int64_t key1,int64_t key2)
{

    return std::bitset<N>(0);
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
template<size_t N>
std::vector<std::bitset<N>> gen_rand_bitsetvector(size_t sz)
{
    std::vector<std::bitset<N>> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=0;
    return rand;
}

std::vector<int64_t> gen_rand_vector_with_key(size_t sz,int64_t key1,int64_t key2)
{
    std::vector<int64_t> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=generate_random_int64_with_key(key1,key2);
    return rand;
}
template<size_t N>
std::vector<std::bitset<N>> gen_rand_bitestvector_with_key(size_t sz,int64_t key1,int64_t key2)
{
    std::vector<std::bitset<N>> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=generate_random_bitset_with_key<N>(key1,key2);
    return rand;
}

std::vector<std::bitset<128>> gen_rand_vector128(size_t sz)
{
    std::vector<std::bitset<128>> rand(sz);
    for(int i=0;i<sz;i++) rand[i]=generate_random_128();
    return rand;
}

Value gen_rand_Value(Clients& clients,size_t sz)
{
    auto r1=gen_rand_vector(sz);
    Value R1(std::move(r1));R1.set_fraction_bits(0);R1.set_share_type(0);
    auto R2=clients.SendRoud(R1);
    std::vector<int64_t> res(R1.size()*2);
    for(int i=0;i<R1.size();i++) res[2*i]=R2[i],res[2*i+1]=R1[i];
    Value R(std::move(res));R.set_fraction_bits(0);R.set_share_type(0);
    return R;
}


Value AmakeShare(Clients& clients,size_t Owner_id,const Value& a=Value())
{

    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();

    std::vector<int64_t> a1,a2,a3(a.size()),A1(2*a.size()),A2(2*a.size()),A3(2*a.size());
    if(Pid==Owner_id){
        a1=gen_rand_vector(a.size());
        a2=gen_rand_vector(a.size());
        for(int i=0;i<a.size();i++)
        {
            a3[i]=(a[i]-a1[i]-a2[i]);
        }

        for(int i=0;i<a1.size();i++)
        {
            A1[2*i]=a1[i],A1[2*i+1]=a2[i];
            A2[2*i]=a2[i],A2[2*i+1]=a3[i];
            A3[2*i]=a3[i],A3[2*i+1]=a1[i];
        }

        Value va1(std::move(A1));va1.set_fraction_bits(a.get_fraction_bits());va1.set_share_type(0);
        Value va2(std::move(A2));va2.set_fraction_bits(a.get_fraction_bits());va2.set_share_type(0);
        Value va3(std::move(A3));va3.set_fraction_bits(a.get_fraction_bits());va3.set_share_type(0);
        clients.SendTo(va3,(Pid-1+Party_number)%Party_number);
        clients.SendTo(va2,(Pid+1+Party_number)%Party_number);
        return va1;
    }
    else{

        return clients.recv(Owner_id);
    }
}

Value BmakeShare(Clients& clients,size_t Owner_id,const Value& a=Value())
{

   size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();

    std::vector<int64_t> a1,a2,a3(a.size()),A1(2*a.size()),A2(2*a.size()),A3(2*a.size());
    if(Pid==Owner_id){
        a1=gen_rand_vector(a.size());
        a2=gen_rand_vector(a.size());
        for(int i=0;i<a.size();i++)
        {
            a3[i]=(a[i]^a1[i]^a2[i]);
        }

        for(int i=0;i<a1.size();i++)
        {
            A1[2*i]=a1[i],A1[2*i+1]=a2[i];
            A2[2*i]=a2[i],A2[2*i+1]=a3[i];
            A3[2*i]=a3[i],A3[2*i+1]=a1[i];
        }

        Value va1(std::move(A1));va1.set_fraction_bits(a.get_fraction_bits());va1.set_share_type(1);
        Value va2(std::move(A2));va2.set_fraction_bits(a.get_fraction_bits());va2.set_share_type(1);
        Value va3(std::move(A3));va3.set_fraction_bits(a.get_fraction_bits());va3.set_share_type(1);
        clients.SendTo(va2,(Pid+1+Party_number)%Party_number);
        clients.SendTo(va3,(Pid-1+Party_number)%Party_number);

        return va1;
    }
    else{

        return clients.recv(Owner_id);
    }
}

template<size_t N>
std::vector<std::bitset<N>> BmakeShare(Clients& clients,size_t Owner_id,const std::vector<std::bitset<N>>& a=std::vector<std::bitset<N>>())
{

   size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();

    std::vector<std::bitset<N>> a1(a.size(),0),a2(a.size(),0),a3(a.size(),0),A1(2*a.size(),0),A2(2*a.size(),0),A3(2*a.size(),0);
    if(Pid==Owner_id){
        for(int i=0;i<a.size();i++)
        {
            a3[i]=a[i];
        }

        for(int i=0;i<a1.size();i++)
        {
            A1[2*i]=a1[i],A1[2*i+1]=a2[i];
            A2[2*i]=a2[i],A2[2*i+1]=a3[i];
            A3[2*i]=a3[i],A3[2*i+1]=a1[i];
        }
        clients.SendTo_bitset(A2,(Pid+1+Party_number)%Party_number);
        clients.SendTo_bitset(A3,(Pid-1+Party_number)%Party_number);

        return A1;
    }
    else{

        return clients.recv_bitset<N>(Owner_id);
    }
}