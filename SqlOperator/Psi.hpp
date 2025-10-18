#pragma once

#include "../Operator/operator.h"
#include "../LowMC/Lowmc.hpp"
#include "Permutation.hpp"


#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <map>

std::random_device rd;
std::mt19937 gen(rd());  // 使用 Mersenne Twister 算法生成随机数
std::uniform_int_distribution<> dis(0, 1);  // 定义范围 [0, 2]

std::bitset<80> bt64=(1ULL<<64)-1;

template<size_t N>
class CuckooHashTable {
public:
    CuckooHashTable(size_t size) 
    : table(size, -1), tableSize(size), maxRetries(size), num(0) ,insertPositions(size,0){
        cur_num=0;
    }

    void insert(std::bitset<N> key) {
        size_t id=num++;

        for (int i = 0; i < maxRetries; ++i) {
            auto idx=rand_index[cur_num++];
            if(cur_num==rand_index.size()) cur_num=0;

            size_t pos;
            if(idx==0) pos = hash1(key);
            else if(idx==1) pos = hash2(key);

            if (table[pos] == -1) {
                table[pos] = key;
                insertPositions[pos]=id; // 记录插入位置
                return;
            }

            std::bitset<N> oldKey = table[pos];
            size_t oldid=insertPositions[pos];
            table[pos] = key;
            insertPositions[pos]=id; // 记录插入位
            id=oldid;
            key = oldKey;

        }
        fail++;
        // throw std::invalid_argument("cockoo hash table insert fail");

    }

    const std::vector<size_t> Positions() {
       return insertPositions;
    }
size_t hash1(const std::bitset<N>& key) {
    static_assert(N == 64 || N == 80, "Only supports 64 or 80 bits");
    
    // 直接获取64位数据
    uint64_t chunk;
    if constexpr (N==80)
    {
         chunk= (key&bt64).to_ullong();
    }
    else chunk=key.to_ullong();
    // 极简混合：一次异或和乘法
    chunk ^= chunk >> 32;
    chunk *= 0xC6A4A7935BD1E99DULL;
    
    // 如果是80位，简单混合剩余16位
    // if constexpr (N == 80) {
    //     uint16_t extra = 0;
    //     for (int i = 0; i < 16; ++i) {
    //         extra |= (key[64 + i] << i);
    //     }
    //     chunk ^= (extra * 0x9E3779B9);
    // }
    
    return static_cast<size_t>(chunk % tableSize);
}

size_t hash2(const std::bitset<N>& key) {
    static_assert(N == 64 || N == 80, "Only supports 64 or 80 bits");
    
    // 直接获取64位数据
    size_t hash ;
    if constexpr (N==80)
    {
         hash= static_cast<size_t>((key&bt64).to_ullong());
    }
    else hash=static_cast<size_t>(key.to_ullong());
    
    // 极简混合：两次移位和加减
    hash ^= hash >> 16;
    hash += (hash << 5);
    
    // 如果是80位，简单处理剩余位
    // if constexpr (N == 80) {
    //     for (int i = 0; i < 16; ++i) {
    //         hash ^= (key[64 + i] << (i % 16));
    //     }
    // }
    
    return hash % tableSize;
}
    void gen_rand_index(size_t size){
        rand_index.resize(size);
        for(int i=0;i<size;i++) rand_index[i]=dis(gen);
    }

private:
    std::vector<std::bitset<N>> table;
    std::vector<size_t> insertPositions; // 记录每次插入的位置
    size_t tableSize;
    std::map<int,size_t> mp;
    std::vector<size_t> rand_index;
    int maxRetries;
    int num;
    int cur_num; 
    // A more complex hash function using a prime multiplier and a bit shift
public:
    int fail=0;

};

// std::vector<Value> perm(Clients& clients,const std::vector<Value>& a,size_t p,size_t m,std::vector<size_t> pem)
// {
//     size_t Pid=clients.getPid();
//     std::vector<std::vector<int64_t>> a1(a.size(),std::vector<int64_t>(a[0].size()/2,0));
//     for(int i=0;i<a.size();i++)
//     {
//         bool is_as=a[i].get_share_type();
//         if(Pid==p){
//             if(is_as==0){
//                 for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j]+a[i][j+1];
//             }
//             else{
//                 for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j]^a[i][j+1];
//             }
//         }
//         else if(Pid==(p-1+3)%3){
//             for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j];
//         }
//     }

//     std::vector<Value> a2(a1.size());
//     for(int i=0;i<a1.size();i++) a2[i]=std::move(Value(std::move(a1[i])));

//     if(Pid==p)
//     {
//         std::vector<int64_t> v(a[0].size()/2);
//         std::iota(v.begin(),v.end(),0);
//         // std::random_device rd; 
//         // std::mt19937 g(rd());   
//         // std::shuffle(v.begin(),v.end(),g);

//         Value V(std::move(v));
//         clients.SendTo(V,(p-1+3)%3);
        // auto seeds1=generate_int64_numbers(2);
//         auto seeds2=generate_int64_numbers(2);
//         clients.SendTo(seeds1,(p-1+3)%3);
//         clients.SendTo(seeds2,(p+1)%3);
//         auto s=gen_rand_vector_with_key(a[0].size()/2,seeds1[0],seeds1[1]);
//         Value S(std::move(s));
//         // clients.SendTo(S,(p-1+3)%3);
//         auto t=gen_rand_vector_with_key(m,seeds2[0],seeds2[1]);
//         Value T(std::move(t));
//         // clients.SendTo(T,(p+1)%3);

//         std::vector<size_t> rv(V.size());
//         for(int i=0;i<V.size();i++) rv[V[i]]=i;

//         std::vector<int64_t> rep(pem.size());
//         for(int i=0;i<rep.size();i++){
//             rep[i]=rv[pem[i]];
//         } 
//         Value Rep(std::move(rep));
//         clients.SendTo(Rep,(p+1)%3);

//         std::vector<Value> res(a2.size());
//         for(int i=0;i<a2.size();i++) 
//         {
//             std::vector<int64_t> a3(m);
//             bool is_as=a[i].get_share_type();
//             if(is_as==0)
//             {
//                 for(int j=0;j<m;j++) a3[j]=S[Rep[j]]+T[j]+a2[i][pem[j]];
//             }
//             else
//             {
//                 for(int j=0;j<m;j++) a3[j]=S[Rep[j]]^T[j]^a2[i][pem[j]];
//             }
//             Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
//             res[i]=std::move(A3);
//         }
//         return res;
//     }
//     else if(Pid==(p-1+3)%3)
//     {
//         auto V=clients.recv(p);
//         auto seeds=clients.recv(p);
//         auto S=gen_rand_vector_with_key(a[0].size()/2,seeds[0],seeds[1]);
//         std::vector<Value> res(a2.size());
//         for(int i=0;i<a2.size();i++)
//         {
//             std::vector<int64_t> a3(V.size());
//             std::vector<int64_t> mid(m,0);
//             bool is_as=a[i].get_share_type();
//             if(is_as==0)
//             {
//                 for(int j=0;j<V.size();j++) a3[j]=-S[j]+a2[i][V[j]];
//             }
//             else
//             {
//                 for(int j=0;j<V.size();j++){
//                     a3[j]=S[j]^a2[i][V[j]];
//                 }
//             }
//             Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
//             clients.SendTo(A3,(Pid-1+3)%3);
//             res[i]=Value(std::move(mid));
//         }
//         return res;
//     } 
//     else{
//         auto seeds=clients.recv(p);
//         auto T=gen_rand_vector_with_key(m,seeds[0],seeds[1]);
//         // auto T=clients.recv(p);
//         auto Rep=clients.recv(p);
//         std::vector<Value> r(a2.size());
//         for(int i=0;i<a2.size();i++)
//         {

//             auto B=clients.recv((Pid+1)%3);
//             r[i]=std::move(B);
//         }

//         std::vector<Value> res(a2.size());
//         for(int i=0;i<a2.size();i++)
//         {
//             std::vector<int64_t> a3(m);
//             bool is_as=r[i].get_share_type();
//             if(is_as==0)
//             {
//                 for(int j=0;j<m;j++) a3[j]=-T[j]+r[i][Rep[j]];
//             }
//             else
//             {
//                 for(int j=0;j<m;j++) a3[j]=T[j]^r[i][Rep[j]];
//             }
//             Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
//             res[i]=std::move(A3);
//         }
//         return res;
//     }
// }
template<size_t N>
pemed<N> perm(Clients& clients,const pemed<N>& Pemed,size_t p,size_t m,std::vector<size_t> pem,bool isTw=0)
{
    size_t Pid=clients.getPid();

    auto a=Pemed.a;
    auto b=Pemed.b;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int64_t>> a1(a.size(),std::vector<int64_t>(a[0].size()/2));
    std::vector<std::bitset<N>> b1(b.size()/2);
    std::vector<Value> a2(a1.size());
    if(isTw==0)
    {
        for(int i=0;i<a.size();i++)
        {
            bool is_as=a[i].get_share_type();
            if(Pid==p){
                if(is_as==0){
                    for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j]+a[i][j+1];
                }
                else{
                    for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j]^a[i][j+1];
                }
            }
            else if(Pid==(p-1+3)%3){
                for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j];
            }
        }

        for(int i=0;i<a1.size();i++) a2[i]=std::move(Value(std::move(a1[i]))),a2[i].set_share_type(a[i].get_share_type());
            if(Pid==p){
                for(int i=0;i<b.size();i+=2) b1[i/2]=b[i]^b[i+1];
            }
            else if(Pid==(p-1+3)%3){
                for(int i=0;i<b.size();i+=2) b1[i/2]=b[i];
            }
    }
    else a2=a,b1=b;






    std::vector<int64_t> seeds1{1,2};
    std::vector<int64_t> seeds2{3,4};
    std::vector<Value> resa(a2.size());
    auto S=gen_rand_vector_with_key(a2[0].size(),seeds1[0],seeds1[1]);
    
    auto s_bitset=gen_rand_bitestvector_with_key<N>(b1.size(),seeds1[0],seeds1[1]);

    auto T=gen_rand_vector_with_key(m,seeds2[0],seeds2[1]);
    auto t_bitset=gen_rand_bitestvector_with_key<N>(m,seeds2[0],seeds2[1]);
 auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime preprocess: " << duration.count() << " ms" << std::endl;
        start = std::chrono::high_resolution_clock::now();
    if(Pid==p)
    {

        std::vector<std::bitset<N>> resb(m);
        std::vector<int64_t> v(a2[0].size());
        std::iota(v.begin(),v.end(),0);

        Value V(std::move(v));
        std::vector<size_t> rv(V.size());
        for(int i=0;i<V.size();i++) rv[V[i]]=i;

        std::vector<int64_t> rep(pem.size());
        for(int i=0;i<rep.size();i++){
            rep[i]=rv[pem[i]];
        } 
        Value Rep(std::move(rep));
        clients.SendTo(Rep,(p+1)%3);


        for(int i=0;i<a2.size();i++) 
        {
            std::vector<int64_t> a3(m);
            bool is_as=a[i].get_share_type();
            if(is_as==0)
            {
                for(int j=0;j<m;j++) a3[j]=S[Rep[j]]+T[j]+a2[i][pem[j]];
            }
            else
            {
                for(int j=0;j<m;j++) a3[j]=S[Rep[j]]^T[j]^a2[i][pem[j]];
            }
            Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
            resa[i]=std::move(A3);
        }
        for(int i=0;i<m;i++) resb[i]=s_bitset[Rep[i]]^t_bitset[i]^b1[pem[i]];
        
        return pemed<N>{resa,resb};
    }
    else if(Pid==(p-1+3)%3)
    {

        std::vector<std::bitset<N>> resb(b1.size());
        std::vector<int64_t> V(a2[0].size());
        std::iota(V.begin(),V.end(),0);
        for(int i=0;i<a2.size();i++)
        {
            std::vector<int64_t> a3(V.size());
            std::vector<int64_t> mid(m,0);

            bool is_as=a[i].get_share_type();
            if(is_as==0)
            {
                for(int j=0;j<V.size();j++) a3[j]=-S[j]+a2[i][V[j]];
            }
            else
            {
                for(int j=0;j<V.size();j++){
                    a3[j]=S[j]^a2[i][V[j]];
                }
            }
            Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
            clients.SendTo(A3,(Pid-1+3)%3);
            Value Mid(std::move(mid));Mid.set_share_type(a[i].get_share_type());
            resa[i]=Mid;
        }
        for(int i=0;i<V.size();i++) resb[i]=s_bitset[i]^b1[V[i]];
        clients.SendTo_bitset(resb,(Pid-1+3)%3);
        std::vector<std::bitset<N>> mid1(m,0);  

        return pemed<N>{resa,mid1};
    } 
    else{

        std::vector<std::bitset<N>> resb(m);
        auto Rep=clients.recv(p);

        for(int i=0;i<a2.size();i++)
        {    
            auto B=clients.recv((Pid+1)%3);
            std::vector<int64_t> a3(m);
            bool is_as=B.get_share_type();
            if(is_as==0)
            {
                for(int j=0;j<m;j++) a3[j]=-T[j]+B[Rep[j]];
            }
            else
            {
                for(int j=0;j<m;j++) a3[j]=T[j]^B[Rep[j]];
            }
            Value A3(std::move(a3));A3.set_fraction_bits(a[i].get_fraction_bits());A3.set_share_type(a[i].get_share_type());
            resa[i]=std::move(A3);
        }
        auto BB=clients.recv_bitset<N>((Pid+1)%3);
        for(int i=0;i<m;i++) resb[i]=t_bitset[i]^BB[Rep[i]];
        return pemed<N>{resa,resb};
    }
       end = std::chrono::high_resolution_clock::now();
         duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime perm1: " << duration.count() << " ms" << std::endl;
}

template<size_t N,size_t M>
pemedNM<N,M> perm(Clients& clients,const pemedNM<N,M>& Pemed,size_t p,size_t m,std::vector<size_t> pem)
{

    size_t Pid=clients.getPid();

    auto a=Pemed.a;
    auto b=Pemed.b;

    std::vector<std::vector<std::bitset<M> > > a1(a.size(),std::vector<std::bitset<M>>(a[0].size()/2));
    std::vector<std::bitset<N>> b1(b.size()/2);
    for(int i=0;i<a.size();i++)
    {
        if(Pid==p){
            for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=convToint(a[i][j])+convToint(a[i][j+1]);
        }
        else if(Pid==(p-1+3)%3){
            for(int j=0;j<a[i].size();j+=2) a1[i][j/2]=a[i][j];
        }
    }

    if(Pid==p){
        for(int i=0;i<b.size();i+=2) b1[i/2]=b[i]^b[i+1];
    }
    else if(Pid==(p-1+3)%3){
        for(int i=0;i<b.size();i+=2) b1[i/2]=b[i];
    }




    std::vector<int64_t> seeds1{1,2};
    std::vector<int64_t> seeds2{3,4};
    std::vector<std::vector<std::bitset<M>>> resa(a1.size());

    if(Pid==p)
    {

        std::vector<std::bitset<N>> resb(m);
        std::vector<int64_t> v(a[0].size()/2);
        std::iota(v.begin(),v.end(),0);

        Value V(std::move(v));
        std::vector<size_t> rv(V.size());
        for(int i=0;i<V.size();i++) rv[V[i]]=i;

        std::vector<int64_t> rep(pem.size());
        for(int i=0;i<rep.size();i++){
            rep[i]=rv[pem[i]];
        } 
        Value Rep(std::move(rep));
        clients.SendTo(Rep,(p+1)%3);
        auto s=gen_rand_bitestvector_with_key<M>(a[0].size()/2,seeds1[0],seeds1[1]);
       
        auto s_bitset=gen_rand_bitestvector_with_key<N>(b.size()/2,seeds1[0],seeds1[1]);

        auto t=gen_rand_bitestvector_with_key<M>(m,seeds2[0],seeds2[1]);
        auto t_bitset=gen_rand_bitestvector_with_key<N>(m,seeds2[0],seeds2[1]);



        for(int i=0;i<a1.size();i++) 
        {
            std::vector<std::bitset<M>> a3(m);
            for(int j=0;j<m;j++) a3[j]=convToint(s[Rep[j]])+convToint(t[j])+convToint(a1[i][pem[j]]);
            resa[i]=std::move(a3);
        }
        for(int i=0;i<m;i++) resb[i]=s_bitset[Rep[i]]^t_bitset[i]^b1[pem[i]];
    
        return pemedNM<N,M>{resa,resb};
           
    }
    else if(Pid==(p-1+3)%3)
    {
        std::vector<std::bitset<N>> resb(b.size()/2);
        std::vector<int64_t> V(a[0].size()/2);
        std::iota(V.begin(),V.end(),0);
        auto S=gen_rand_bitestvector_with_key<M>(a[0].size()/2,seeds1[0],seeds1[1]);
        auto s_bitset=gen_rand_bitestvector_with_key<N>(b.size()/2,seeds1[0],seeds1[1]);
        for(int i=0;i<a1.size();i++)
        {
            std::vector<std::bitset<M>> a3(V.size());
            std::vector<std::bitset<M>> mid(m,0);

            for(int j=0;j<V.size();j++) a3[j]=-convToint(S[j])+convToint(a1[i][V[j]]);

            clients.SendTo_bitset(a3,(Pid-1+3)%3);
            resa[i]=std::move(mid);
        }
        for(int i=0;i<V.size();i++) resb[i]=s_bitset[i]^b1[V[i]];
        clients.SendTo_bitset(resb,(Pid-1+3)%3);
        std::vector<std::bitset<N>> mid1(m,0);  

        return pemedNM<N,M>{resa,mid1};
    } 
    else{
        std::vector<std::bitset<N>> resb(m);
        auto T=gen_rand_bitestvector_with_key<M>(m,seeds2[0],seeds2[1]);
        auto t_bitset=gen_rand_bitestvector_with_key<N>(m,seeds2[0],seeds2[1]);

        auto Rep=clients.recv(p);

        for(int i=0;i<a1.size();i++)
        {    
            auto B=clients.recv_bitset<M>((Pid+1)%3);
            std::vector<std::bitset<M>> a3(m);
            for(int j=0;j<m;j++) a3[j]=-convToint(T[j])+convToint(B[Rep[j]]);

            resa[i]=std::move(a3);
        }
        auto BB=clients.recv_bitset<N>((Pid+1)%3);
        for(int i=0;i<m;i++) resb[i]=t_bitset[i]^BB[Rep[i]];
        return pemedNM<N,M>{resa,resb};
    }
}

std::vector<Value> duplicate(Clients& clients,const std::vector<Value>& a,size_t p,const std::vector<size_t>& pem)
{
    size_t Pid=clients.getPid();
    if(Pid==p)
    {
        std::vector<int64_t> bb(pem.size());
        bb[0]=0;
        for(int i=1;i<pem.size();i++) bb[i]=(pem[i]==pem[i-1]);
        Value b(std::move(bb));

        auto Fa=clients.recv((p-1+3)%3);
        auto B00=clients.recv((p-1+3)%3);
        for(int i=0;i<b.size();i++) Fa[i]=b[i]^Fa[i];
        clients.SendTo(Fa,(p+1)%3);
        auto Wi=clients.recv((p+1)%3);
        auto M0=clients.recv_vector((p-1+3)%3,a.size());
        auto M1=clients.recv_vector((p-1+3)%3,a.size());

        std::vector<Value> res(a.size());
        for(int i=0;i<a.size();i++)
        {
            std::vector<int64_t> b0(pem.size());    
            b0[0]=B00[0];
            bool is_as=a[i].get_share_type();
            if(is_as)
            {
                for(int j=1;j<pem.size();j++)
                {
                    if(b[j]==0) b0[j]=M0[i][j]^Wi[j]^a[i][pem[j]];
                    else b0[j]=M1[i][j]^Wi[j]^b0[j-1]^a[i][pem[j]];
                } 
            }
            else
            {
                for(int j=1;j<pem.size();j++)
                {
                    if(b[j]==0) b0[j]=M0[i][j]+Wi[j]+a[i][pem[j]];
                    else b0[j]=M1[i][j]+Wi[j]+b0[j-1]+a[i][pem[j]];
                }
            }
            res[i]=std::move(Value(std::move(b0)));
            res[i].set_fraction_bits(a[i].get_fraction_bits());
            res[i].set_share_type(a[i].get_share_type());
        }

        return res;
    }
    else if(Pid==(p-1+3)%3)
    {
        auto b1=gen_rand_vector(pem.size());
        auto w1=gen_rand_vector(pem.size());
        auto w2=gen_rand_vector(pem.size());
        auto b0=gen_rand_vector(1);
        auto fa=gen_rand_vector(pem.size());
        for(int i=0;i<fa.size();i++) fa[i]%=2;
        std::vector<Value> M0(a.size()),M1(a.size());
        std::vector<int64_t> ba(a.size());
        Value W1(std::move(w1));
        Value W2(std::move(w2));
        Value Fa(std::move(fa));
        Value B1(std::move(b1));
        Value B00(std::move(b0));
        clients.SendTo(W1,(p+1)%3);
        clients.SendTo(W2,(p+1)%3);
        clients.SendTo(Fa,p);
        clients.SendTo(B00,p);
        for(int i=0;i<a.size();i++)
        {
            std::vector<int64_t> m0(pem.size(),0),m1(pem.size(),0);
            bool is_as=a[i].get_share_type();
            if(is_as)
            {
                B1[0]=a[i][0]^B00[0];
                ba[i]=B1[0];
                for(int j=1;j<a[i].size();j++)
                {
                    if(Fa[j]==0){
                        m0[i]=a[i][j]^B1[j]^W1[j];
                        m1[i]=B1[j-1]^B1[j]^W2[j];
                    }
                    else 
                    {
                        m0[i]=a[i][j]^B1[j]^W2[j];
                        m1[i]=B1[j-1]^B1[j]^W1[j];
                    }
                }
                B1[0]=a[i][0]^B00[0];
            }
            else{
                B1[0]=a[i][0]-B00[0];
                ba[i]=B1[0];
                for(int j=1;j<a[i].size();j++)
                {
                    if(Fa[j]==0){
                        m0[i]=a[i][j]-B1[j]-W1[j];
                        m1[i]=B1[j-1]-B1[j]-W2[j];
                    }
                    else 
                    {
                        m0[i]=a[i][j]-B1[j]-W2[j];
                        m1[i]=B1[j-1]-B1[j]-W1[j];
                    }
                }
                B1[0]=a[i][0]+B00[0];
            }
            M0[i]=std::move(Value(std::move(m0)));
            M1[i]=std::move(Value(std::move(m1)));
        }
        Value BA(std::move(ba));
        clients.SendTo(BA,(p+1)%3);
        clients.SendTo(B1,(p+1)%3);
        clients.SendTo_vector(M0,p);
        clients.SendTo_vector(M1,p);
        return std::vector<Value>();
    }
    else{
        auto W1=clients.recv((p-1+3)%3);
        auto W2=clients.recv((p-1+3)%3);
        auto Fa=clients.recv(p);
        for(int i=0;i<Fa.size();i++)
        {
            if(Fa[i]==0) Fa[i]=W1[i];
            else Fa[i]=W2[i];
        }
        clients.SendTo(Fa,p);
        auto BA=clients.recv((p-1+3)%3);
        auto B1=clients.recv((p-1+3)%3);
        std::vector<Value> res(a.size());
        for(int i=0;i<a.size();i++)
        {
            Value b0=B1;
            b0[0]=BA[i];
            b0.set_fraction_bits(a[i].get_fraction_bits());
            b0.set_share_type(a[i].get_share_type());
            res[i]=std::move(Value(std::move(b0)));
        }
        return res;
    }
}

// std::vector<Value> OblivSwitch(Clients& clients,const Value& a,size_t p,size_t s,const std::vector<size_t> pem=std::vector<size_t>())
// {
//     size_t Pid=clients.getPid();
//     size_t r;
//     for(int i=0;i<3;i++)
//     {
//         if(i!=p&&i!=s) r=i;
//     }
//     if(Pid==p)
//     {

//     }
// }

template<size_t N>
std::vector<Value> Psi_with_payload(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<Value>& payload_a)
{
    size_t Pid=clients.getPid();
    std::vector<Value> Bs_a,Bs_b;
    for(int i=0;i<a.size();i++)
    {
        if(a[i].get_share_type()==0) Bs_a[i]=std::move(A2B(clients,a[i]));
        else Bs_a[i]=a[i];
    }
    for(int i=0;i<b.size();i++)
    {   
        if(b[i].get_share_type()==0) Bs_b[i]=std::move(A2B(clients,b[i]));   
        else Bs_b[i]=b[i];
    }



    auto key=gen_rand_key(clients,1);

    LowMC<N> cipher(clients,key);
    auto encrypt_a=cipher.encrypt(clients,Bs_a);
    auto encrypt_b=cipher.encrypt(clients,Bs_b);
    size_t cuckoo_hash_sizea=2.5*encrypt_a.size()/2;
    size_t cuckoo_hash_sizeb=2.5*encrypt_b.size()/2;
    std::vector<std::bitset<N>> Ea,Eb;
    if(Pid==0){
        Ea=BBrevelTo_bitset(clients,encrypt_a,0);
        BBrevelTo_bitset(clients,encrypt_b,1);
        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);
        hashTable.gen_rand_index(encrypt_a.size()/2);
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<Ea.size();i++) hashTable.insert(Ea[i]);
 auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1111: " << duration.count() << " ms" << std::endl;
        auto pos=hashTable.Positions();
        std::vector<size_t> realpos(cuckoo_hash_sizea,0);
        for(int i=0;i<Ea.size();i++)
        {
            realpos[pos[i]]=i;
        }
        std::vector<Value> payload_copy_a=payload_a;
        auto payload_copy_a_pem=s2tos3_pemed(clients,2,perm(clients,pemed<N>{payload_copy_a,Ea},0,realpos.size(),realpos,1));

        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),std::vector<size_t>()));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),std::vector<size_t>()));


        auto cp1=eq_bitset(clients,Eb,payload_1.b);
        auto cp2=eq_bitset(clients,Eb,payload_2.b);
        auto ones=Value(std::move(std::vector<int64_t>(Eb.size(),1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);

        compare_conv(clients,cp1);
        compare_conv(clients,cp2);
        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
       return payload;
    }
    else if(Pid==1)
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        Eb=BrevelTo_bitset(clients,encrypt_b,1);
        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);

        std::vector<size_t> p1(Eb.size()),p2(Eb.size());
        for(int i=0;i<Eb.size();i++){
            p1[i]=hashTable.hash1(Eb[i]),p2[i]=hashTable.hash2(Eb[i]);
        }
        std::vector<Value> payload_copy_a=payload_a;

        auto payload_copy_a_pem=s2tos3_pemed(clients,2,perm(clients,pemed<N>{payload_copy_a,Ea},0,cuckoo_hash_sizea,std::vector<size_t>()));

        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),p1));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),p2));



        auto cp1=eq_bitset(clients,Eb,payload_1.b);
        auto cp2=eq_bitset(clients,Eb,payload_2.b);
        auto ones=Value(std::move(std::vector<int64_t>(Eb.size(),1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);

        compare_conv(clients,cp1);
        compare_conv(clients,cp2);
        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
       return payload;
    }
    else 
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        BrevelTo_bitset(clients,encrypt_b,1);
        std::vector<Value> payload_copy_a=payload_a;

        auto payload_copy_a_pem=s2tos3_pemed(clients,2,perm(clients,pemed<N>{payload_copy_a,Ea},0,cuckoo_hash_sizea,std::vector<size_t>()));

        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),std::vector<size_t>()));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,Eb.size(),std::vector<size_t>()));
        auto cp1=eq_bitset(clients,Eb,payload_1.b);
        auto cp2=eq_bitset(clients,Eb,payload_2.b);
        auto ones=Value(std::move(std::vector<int64_t>(Eb.size(),1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);

        compare_conv(clients,cp1);
        compare_conv(clients,cp2);
        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
       return payload;
    }
}
template<size_t N>
std::vector<Value> Psi_with_payload_enc(Clients& clients,const std::vector<std::bitset<N>>& encrypt_a,const std::vector<std::bitset<N>>& encrypt_b,const std::vector<Value>& payload_a)
{
    size_t Pid=clients.getPid();


    size_t cuckoo_hash_sizea=2.5*encrypt_a.size()/2;
    size_t cuckoo_hash_sizeb=2.5*encrypt_b.size()/2;
    std::vector<std::bitset<N>> Ea,Eb;
    if(Pid==0){
        auto start = std::chrono::high_resolution_clock::now();
        Ea=BrevelTo_bitset(clients,encrypt_a,0);
        BrevelTo_bitset(clients,encrypt_b,1);

        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);
        hashTable.gen_rand_index(encrypt_a.size()/2);

        for(int i=0;i<Ea.size();i++) hashTable.insert(Ea[i]);
        std::cout<<hashTable.fail<<'\n';
        auto realpos=hashTable.Positions();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime cock: " << duration.count() << " ms" << std::endl;
        std::vector<Value> payload_copy_a=payload_a;
        start = std::chrono::high_resolution_clock::now();
       auto payload_copy_a_pem=perm(clients,pemed<N>{payload_copy_a,encrypt_a},0,realpos.size(),realpos);  
        end = std::chrono::high_resolution_clock::now();
         duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime perm: " << duration.count() << " ms" << std::endl;
        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>(),1));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>(),1));
        

        
        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);
       


        auto dif=And(clients,cp1,cp2);
        cp1=Xor(clients,cp1,dif);
        auto ones=Value(std::move(std::vector<int64_t>(encrypt_b.size()/2,1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);
 
        compare_conv(clients,cp1);
        compare_conv(clients,cp2);


        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
    
        payload.push_back(Add(clients,cp1_A,cp2_A));
        return payload;
    }
    else if(Pid==1)
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        Eb=BrevelTo_bitset(clients,encrypt_b,1);

        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);

        std::vector<size_t> p1(Eb.size()),p2(Eb.size());
        for(int i=0;i<Eb.size();i++){
            p1[i]=hashTable.hash1(Eb[i]),p2[i]=hashTable.hash2(Eb[i]);
        }
        std::vector<Value> payload_copy_a=payload_a;
        
        auto payload_copy_a_pem=perm(clients,pemed<N>{payload_copy_a,encrypt_a},0,cuckoo_hash_sizea,std::vector<size_t>());   
        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,p1,1));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,p2,1));


        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);

        auto dif=And(clients,cp1,cp2);
        cp1=Xor(clients,cp1,dif);
        auto ones=Value(std::move(std::vector<int64_t>(encrypt_b.size()/2,1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);


        compare_conv(clients,cp1);
        compare_conv(clients,cp2);

        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
        payload.push_back(Add(clients,cp1_A,cp2_A));
       return payload;
    }
    else 
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        BrevelTo_bitset(clients,encrypt_b,1);
        std::vector<Value> payload_copy_a=payload_a;


      auto payload_copy_a_pem=perm(clients,pemed<N>{payload_copy_a,encrypt_a},0,cuckoo_hash_sizea,std::vector<size_t>());
        auto payload_1=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>(),1));

        auto payload_2=s2tos3_pemed(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>(),1));


        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);

        auto dif=And(clients,cp1,cp2);
        cp1=Xor(clients,cp1,dif);

        auto ones=Value(std::move(std::vector<int64_t>(encrypt_b.size()/2,1)));
        auto cp1_A=Mul_ab1(clients,ones,cp1,0);
        auto cp2_A=Mul_ab1(clients,ones,cp2,0);

        compare_conv(clients,cp1);
        compare_conv(clients,cp2);

        std::vector<Value> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            if(payload_1.a[i].get_share_type()==1)
            {
                payload[i]=std::move(Xor(clients,And(clients,payload_1.a[i],cp1),And(clients,payload_2.a[i],cp2)));
            }
            else{
                payload[i]=std::move(Add(clients,Mul(clients,payload_1.a[i],cp1_A),Mul(clients,payload_2.a[i],cp2_A)));
            }
        }
        payload.push_back(Add(clients,cp1_A,cp2_A));
       return payload;
    }
}

template<size_t N,size_t M>
std::vector<std::vector<std::bitset<M>>> Psi_with_payload_enc(Clients& clients,const std::vector<std::bitset<N>>& encrypt_a,const std::vector<std::bitset<N>>& encrypt_b,const std::vector<std::vector<std::bitset<M>>>& payload_a)
{
    size_t Pid=clients.getPid();


    size_t cuckoo_hash_sizea=2.5*encrypt_a.size()/2;
    size_t cuckoo_hash_sizeb=2.5*encrypt_b.size()/2;
    std::vector<std::bitset<N>> Ea,Eb;
    if(Pid==0){ 

        Ea=BrevelTo_bitset(clients,encrypt_a,0);
        BrevelTo_bitset(clients,encrypt_b,1);
        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);
        hashTable.gen_rand_index(encrypt_a.size()/2);
        for(int i=0;i<Ea.size();i++) hashTable.insert(Ea[i]);
                std::cout<<hashTable.fail<<'\n';
        auto realpos=hashTable.Positions();
        std::vector<std::vector<std::bitset<M>>> payload_copy_a=payload_a;
       auto payload_copy_a_pem=s2tos3_pemedNM(clients,2,perm(clients,pemedNM<N,M>{payload_copy_a,encrypt_a},0,realpos.size(),realpos));  
        
        auto payload_1=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>()));

        auto payload_2=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>()));
        
        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);
        std::vector<std::bitset<8>> CP1(cp1.size()),CP2(cp2.size());
        for(int i=0;i<CP1.size();i++) CP1[i]=cp1[i],CP2[i]=cp2[i];

        auto dif=And_bitset(clients,CP1,CP2);
        CP1=Xor_bitset(clients,CP1,dif);
        auto cp1_A=conv2ToP<M>(clients,CP1);
        auto cp2_A=conv2ToP<M>(clients,CP2);


        std::vector<std::vector<std::bitset<M>>> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            payload[i]=std::move(Add_bitset(clients,Mul_bitset(clients,payload_1.a[i],cp1_A),Mul_bitset(clients,payload_2.a[i],cp2_A)));
        }
        payload.push_back(Add_bitset(clients,cp1_A,cp2_A));
        return payload;
    }
    else if(Pid==1)
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        Eb=BrevelTo_bitset(clients,encrypt_b,1);

        CuckooHashTable<N> hashTable(cuckoo_hash_sizea);

        std::vector<size_t> p1(Eb.size()),p2(Eb.size());
        for(int i=0;i<Eb.size();i++){
            p1[i]=hashTable.hash1(Eb[i]),p2[i]=hashTable.hash2(Eb[i]);
        }
        std::vector<std::vector<std::bitset<M>>> payload_copy_a=payload_a;
        auto payload_copy_a_pem=s2tos3_pemedNM(clients,2,perm(clients,pemedNM<N,M>{payload_copy_a,encrypt_a},0,cuckoo_hash_sizea,std::vector<size_t>()));   
        auto payload_1=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,p1));

        auto payload_2=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,p2));


        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);
        std::vector<std::bitset<8>> CP1(cp1.size()),CP2(cp2.size());
        for(int i=0;i<CP1.size();i++) CP1[i]=cp1[i],CP2[i]=cp2[i];

        auto dif=And_bitset(clients,CP1,CP2);
        CP1=Xor_bitset(clients,CP1,dif);
        auto cp1_A=conv2ToP<M>(clients,CP1);
        auto cp2_A=conv2ToP<M>(clients,CP2);


        std::vector<std::vector<std::bitset<M>>> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            payload[i]=std::move(Add_bitset(clients,Mul_bitset(clients,payload_1.a[i],cp1_A),Mul_bitset(clients,payload_2.a[i],cp2_A)));
        }
        payload.push_back(Add_bitset(clients,cp1_A,cp2_A));
        return payload;
    }
    else 
    {
        BrevelTo_bitset(clients,encrypt_a,0);
        BrevelTo_bitset(clients,encrypt_b,1);
        std::vector<std::vector<std::bitset<M>>> payload_copy_a=payload_a;
      auto payload_copy_a_pem=s2tos3_pemedNM(clients,2,perm(clients,pemedNM<N,M>{payload_copy_a,encrypt_a},0,cuckoo_hash_sizea,std::vector<size_t>()));
        auto payload_1=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>()));

        auto payload_2=s2tos3_pemedNM(clients,0,perm(clients,payload_copy_a_pem,1,encrypt_b.size()/2,std::vector<size_t>()));


        auto cp1=eq_bitset<N>(clients,encrypt_b,payload_1.b);
        auto cp2=eq_bitset<N>(clients,encrypt_b,payload_2.b);

        std::vector<std::bitset<8>> CP1(cp1.size()),CP2(cp2.size());
        for(int i=0;i<CP1.size();i++) CP1[i]=cp1[i],CP2[i]=cp2[i];

        auto dif=And_bitset(clients,CP1,CP2);
        CP1=Xor_bitset(clients,CP1,dif);
        auto cp1_A=conv2ToP<M>(clients,CP1);
        auto cp2_A=conv2ToP<M>(clients,CP2);


        std::vector<std::vector<std::bitset<M>>> payload(payload_a.size());
        for(int i=0;i<payload_a.size();i++)
        {
            payload[i]=std::move(Add_bitset(clients,Mul_bitset(clients,payload_1.a[i],cp1_A),Mul_bitset(clients,payload_2.a[i],cp2_A)));
        }
        payload.push_back(Add_bitset(clients,cp1_A,cp2_A));
        return payload;
    }
}




std::vector<Value> Psi_with_payload_one(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<Value>& payload_a)
{
    size_t Pid=clients.getPid();
    std::vector<Value> Bs_a(a.size()),Bs_b(b.size());
    for(int i=0;i<a.size();i++)
    {
        if(a[i].get_share_type()==0) Bs_a[i]=std::move(A2B(clients,a[i]));
        else Bs_a[i]=a[i];
    }
    for(int i=0;i<b.size();i++)
    {   
        if(b[i].get_share_type()==0) Bs_b[i]=std::move(A2B(clients,b[i]));   
        else Bs_b[i]=b[i];
    }


    auto key=gen_rand_key(clients,1);

    LowMC<64> cipher(clients,key);
    auto encrypt_a=cipher.encrypt(clients,Bs_a);

    auto encrypt_b=cipher.encrypt(clients,Bs_b);

    auto res=Psi_with_payload_enc(clients,encrypt_a,encrypt_b,payload_a);

    return res;
}



