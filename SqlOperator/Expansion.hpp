#pragma once

#include <unordered_map>

#include "../Operator/operator.h"
#include "../LowMC/Lowmc.hpp"
#include "Permutation.hpp"
#include "Groupby.hpp"

std::vector<Value> distribute(Clients& clients,const std::vector<Value>& a,const Value& b,size_t num)
{
    auto rang=make_constrang(clients,num,1);
    auto key=gen_rand_key(clients,1);
    LowMC<> lowmc(clients,key);
    auto b_copy=(b.get_share_type()==0)?A2B(clients,b):b;
    auto start = std::chrono::high_resolution_clock::now();
    auto rp=lowmc.encrypt(clients,std::vector<Value>{rang});

    auto ip=lowmc.encrypt(clients,std::vector<Value>{b_copy});
auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function enc: " << duration.count() << " ms" << std::endl;
    auto rp_r=reconstruct_bitset(clients,rp);
    auto ip_r=reconstruct_bitset(clients,ip);

    auto res=b_copy;
    std::unordered_map<int64_t, int> mp;
    for (std::bitset<64> x : ip_r) mp[x.to_ullong()] = 1;  
    for (int i=0; i<rp_r.size(); i++) {
        if (!mp.count(rp_r[i].to_ullong()))    
            res.get_fixed_point_vector().push_back(rang[2*i]), res.get_fixed_point_vector().push_back(rang[2*i+1]);
    }

    int dif=num-a[0].size()/2;
    auto a_copy=a;
    for(int i=0;i<a.size();i++)
    {
        auto appendix=make_constI(clients,1,dif);
        combine(a_copy[i],appendix);
    }
    auto pem_arr=Permutation(clients,a_copy,res,1);
    
    std::vector<size_t> idx(a[0].size()/2);
    for(int i=0;i<idx.size();i++) idx[i]=i;
    return Gather_vector(clients,pem_arr,idx);
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> pem4expan(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const Value& pem,size_t m)
{
    auto a_copy=a;
    for(int i=0;i<a_copy.size();i++)
    {
        for(int j=0;j<a_copy[i].size()-2;j+=2)
        {
            a_copy[i][j]=a_copy[i][j].to_ullong()-a_copy[i][j+2].to_ullong();
            a_copy[i][j+1]=a_copy[i][j+1].to_ullong()-a_copy[i][j+3].to_ullong();
        }
    }
    auto ones=make_constI_bitset<N>(clients,1,a_copy[0].size()/2);
    a_copy.push_back(ones);
    auto arr=pad<N>(clients,a_copy,m);
    auto pem_arr=Permutation_bitsetAndValue(clients,arr,pem,1);
    std::vector<size_t> idx(m);
    for(int i=0;i<idx.size();i++) idx[i]=i;
    for(int i=0;i<pem_arr.size()-1;i++)
    {
        pem_arr[i]=perfix_sum_bitset(clients,pem_arr[i],0);
    }
    pem_arr=Gather_vector(clients,pem_arr,idx);
    return pem_arr;
}
std::vector<Value> pem4expan(Clients& clients,const std::vector<Value>& a,const Value& pem,size_t m)
{
    auto a_copy=a;
    for(int i=0;i<a_copy.size();i++)
    {
        for(int j=0;j<a_copy[i].size()-2;j+=2)
        {
            a_copy[i][j]-=a_copy[i][j+2];
            a_copy[i][j+1]-=a_copy[i][j+3];
        }
    }
    auto arr=pad(clients,a_copy,m);
    auto pem_arr=Permutation(clients,arr,pem,1);
    std::vector<size_t> idx(m);
    for(int i=0;i<idx.size();i++) idx[i]=i;
    for(int i=0;i<pem_arr.size();i++)
    {
        pem_arr[i]=perfix_sum(clients,pem_arr[i],0);
    }
    pem_arr=Gather_vector(clients,pem_arr,idx);
    return pem_arr;
}

std::vector<Value> Expansion(Clients& clients,const std::vector<Value>& a,const Value& num,const Value& f){
    auto posN=perfix_sum(clients,num);
    auto neg_f=Add(clients,make_constI(clients,1,f.size()/2),Aneg(clients,f));

    std::vector<int64_t> last_number={posN[posN.size()-2],posN[posN.size()-1]};
    Value lsm(std::move(last_number));
    auto m=reconstruct(clients,lsm)[0];
    std::cout<<m<<'\n';
    if(m==0){
        return std::vector<Value>(a.size());
    }
    auto pem_a=Permutation_gen(clients,f,0);
    auto a_copy=Permutation(clients,a,pem_a);
    auto temp=Permutation(clients,std::vector<Value>{num,neg_f},pem_a);
    auto num_pem=temp[0];
    auto neg_f_pem=temp[1];

    posN=A2B(clients,Add(clients,perfix_sum(clients,num_pem),perfix_sum(clients,neg_f_pem)));
    Value pos(std::move(std::vector<int64_t>(posN.size(),0)));pos.set_share_type(1);
    for(int i=0;i<pos.size();i++) pos[i]=posN[i];

    auto rang=make_constrang(clients,m+num.size()/2,1);
    std::cout<<m+num.size()/2<<'\n';
    auto rand_rang=random_shuffle(clients,std::vector<Value>{rang})[0];


    auto key=gen_rand_key(clients,1);

    LowMC<> lowmc(clients,key);
    auto rp=lowmc.encrypt(clients,std::vector<Value>{rand_rang});

    auto ip=lowmc.encrypt(clients,std::vector<Value>{pos});

    auto rp_r=reconstruct_bitset(clients,rp);
    auto ip_r=reconstruct_bitset(clients,ip);
    auto res=pos;
    std::unordered_map<int64_t, int> mp;
    for (std::bitset<64> x : ip_r) mp[x.to_ullong()] = 1;  
    for (int i=0; i<rp_r.size(); i++) {
        if (!mp.count(rp_r[i].to_ullong()))    
            res.get_fixed_point_vector().push_back(rand_rang[2*i]), res.get_fixed_point_vector().push_back(rand_rang[2*i+1]);
    }

    return pem4expan(clients,a_copy,res,m);
} 

template<size_t N>
std::vector<Value> Expansion(Clients& clients,const std::vector<Value>& a,const std::vector<std::bitset<N>>& num,const std::vector<std::bitset<N>>& f){
    auto posN=perfix_sum_bitset(clients,num);
    auto neg_f=Add_bitset(clients,make_constI_bitset<N>(clients,1,f.size()/2),Aneg_bitset(clients,f));

    std::vector<std::bitset<N>> last_number={posN[posN.size()-2],posN[posN.size()-1]};
    auto m=Areconstruct_bitset(clients,last_number)[0];
    std::cout<<m<<'\n';
    if(m==0){
        return std::vector<Value>(a.size());
    }
    auto pem_a=Permutation_gen_bitset(clients,f,0);
    auto a_copy=Permutation_ValueAndbitset(clients,a,pem_a);
    auto temp=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{num,neg_f},pem_a);
    auto num_pem=temp[0];
    auto neg_f_pem=temp[1];

    posN=A2B(clients,Add_bitset(clients,perfix_sum_bitset(clients,num_pem),perfix_sum_bitset(clients,neg_f_pem)));
    Value pos(std::move(std::vector<int64_t>(posN.size(),0)));pos.set_share_type(1);
    for(int i=0;i<pos.size();i++) pos[i]=posN[i].to_ullong();

    auto rang=make_constrang(clients,m+num.size()/2,1);
    std::cout<<m+num.size()/2<<'\n';
    auto rand_rang=random_shuffle(clients,std::vector<Value>{rang})[0];


    auto key=gen_rand_key(clients,1);

    LowMC<> lowmc(clients,key);
    auto rp=lowmc.encrypt(clients,std::vector<Value>{rand_rang});

    auto ip=lowmc.encrypt(clients,std::vector<Value>{pos});

    auto rp_r=reconstruct_bitset(clients,rp);
    auto ip_r=reconstruct_bitset(clients,ip);
    auto res=pos;
    std::unordered_map<int64_t, int> mp;
    for (std::bitset<64> x : ip_r) mp[x.to_ullong()] = 1;  
    for (int i=0; i<rp_r.size(); i++) {
        if (!mp.count(rp_r[i].to_ullong()))    
            res.get_fixed_point_vector().push_back(rand_rang[2*i]), res.get_fixed_point_vector().push_back(rand_rang[2*i+1]);
    }

    return pem4expan(clients,a_copy,res,m);
} 


template<size_t N>
struct expan_valueAndbitset{
    std::vector<Value> a;
    std::vector<std::vector<std::bitset<N>>> b;
};

template<size_t N>
expan_valueAndbitset<N> Expansion_ValueAndBitset(Clients& clients,const std::vector<Value>& a,const std::vector<std::vector<std::bitset<N>>>& b,const std::vector<std::bitset<N>>& num,const std::vector<std::bitset<N>>& f){
    auto posN=perfix_sum_bitset(clients,num);
    auto neg_f=Add_bitset(clients,make_constI_bitset<N>(clients,1,f.size()/2),Aneg_bitset(clients,f));

    std::vector<std::bitset<N>> last_number={posN[posN.size()-2],posN[posN.size()-1]};
    auto m=Areconstruct_bitset(clients,last_number)[0];
    std::cout<<m<<'\n';
    if(m==0){
        return expan_valueAndbitset<N>();
    }
    auto pem_a=Permutation_gen_bitset(clients,f,0);
    auto a_copy=Permutation_ValueAndbitset(clients,a,pem_a);
    auto b_copy=Permutation_bitset(clients,b,pem_a);
    auto temp=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{num,neg_f},pem_a);
    auto num_pem=temp[0];
    auto neg_f_pem=temp[1];

    posN=A2B(clients,Add_bitset(clients,perfix_sum_bitset(clients,num_pem),perfix_sum_bitset(clients,neg_f_pem)));
    Value pos(std::move(std::vector<int64_t>(posN.size(),0)));pos.set_share_type(1);
    for(int i=0;i<pos.size();i++) pos[i]=posN[i].to_ullong();

    auto rang=make_constrang(clients,m+num.size()/2,1);
    std::cout<<m+num.size()/2<<'\n';
    auto rand_rang=random_shuffle(clients,std::vector<Value>{rang})[0];


    auto key=gen_rand_key(clients,1);

    LowMC<> lowmc(clients,key);
    auto rp=lowmc.encrypt(clients,std::vector<Value>{rand_rang});

    auto ip=lowmc.encrypt(clients,std::vector<Value>{pos});

    auto rp_r=reconstruct_bitset(clients,rp);
    auto ip_r=reconstruct_bitset(clients,ip);
    auto res=pos;
    std::unordered_map<int64_t, int> mp;
    for (std::bitset<64> x : ip_r) mp[x.to_ullong()] = 1;  
    for (int i=0; i<rp_r.size(); i++) {
        if (!mp.count(rp_r[i].to_ullong()))    
            res.get_fixed_point_vector().push_back(rand_rang[2*i]), res.get_fixed_point_vector().push_back(rand_rang[2*i+1]);
    }

    return expan_valueAndbitset<N>{pem4expan(clients,a_copy,res,m),pem4expan(clients,b_copy,res,m)};
} 
