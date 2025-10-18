#pragma once

#include <numeric>
#include <random>
#include <algorithm>

#include "../Operator/operator.h"
#include "Perfixsum.hpp"


// std::vector<Value> random_shuffle(Clients& clients,const std::vector<Value>& a)
// {
//     size_t Pid=clients.getPid();

//     int m=a[0].size()/2;
//     std::vector<int64_t> a1(a.size()*(m),0);
//     for(int i=0;i<a.size();i++)
//     {
//         bool is_as=a[i].get_share_type();
//         if(Pid==0){
//             for(int j=0;j<a[0].size();j+=2) a1[i*m+j/2]=a[i][j+1];
//         }
//         else if(Pid==2)
//         {
//             if(is_as==0)
//             {
//                 for(int j=0;j<a[0].size();j+=2) a1[i*m+j/2]=a[i][j+1]+a[i][j];
//             }
//             else
//             {
//                 for(int j=0;j<a[0].size();j+=2) a1[i*m+j/2]=a[i][j+1]^a[i][j];
//             }
            
//         }
//     }
    
//     std::vector<std::vector<int64_t>> seeds(clients.getPartyNumber(),std::vector<int64_t>(clients.getPartyNumber()));
//     for(int k=0;k<=2;k++)
//     {

//         if(Pid==k){
//             auto seed=generate_int64_numbers(1);
//             clients.SendTo(seed,(k-1+3)%3);
//             seeds[k][(k-1+3)%3]=seed[0];
//         }
//         else if(Pid==(k-1+3)%3)
//         {

//             auto seed=clients.recv(k);
//             seeds[k][(k-1+3)%3]=seed[0];
//         }
//     }
//     Value a2(std::move(a1));
//     for(int k=0;k<=2;k++)
//     {

//         if(Pid==k){
//             auto seed=seeds[k][(k-1+3)%3];
//             std::vector<int64_t> v(m);
//             std::iota(v.begin(),v.end(),0);
//             std::mt19937 rng(seed);     // 使用 Mersenne Twister 引擎

//             std::shuffle(v.begin(),v.end(),rng);
//             Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);

//             std::vector<int64_t> res(a2.size());
//             for(int i=0;i<a.size();i++)
//             {
//                 for(int j=0;j<m;j++) res[i*m+j]=a2[i*m+rp[j]];
//             }
//             a2=std::move(res);

//         }
//         else if(Pid==(k-1+3)%3)
//         {

//             auto seed=seeds[k][(k-1+3)%3];
//             std::vector<int64_t> v(m);
//             std::iota(v.begin(),v.end(),0);
//             std::mt19937 rng(seed);     // 使用 Mersenne Twister 引擎

//             std::shuffle(v.begin(),v.end(),rng);
//             Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);

//             std::vector<int64_t> res(a2.size());
//             for(int i=0;i<a.size();i++)
//             {
//                 for(int j=0;j<m;j++) res[i*m+j]=a2[i*m+rp[j]];
//             }

//             Value val(std::move(res));
//             if(k!=2)
//             {   
//                 clients.SendTo(val,(k+1)%3);
//             }
//             else a2=std::move(val);
//         }
//         else{
//             if(k!=2)
//             {
//                 auto val=clients.recv((k-1+3)%3);
//                 a2=std::move(val);

//             }
//         }
       
//     }
//     std::vector<Value> res(a.size());
//     auto start = std::chrono::high_resolution_clock::now();
//     for(int i=0;i<a.size();i++)
//     {
//         std::vector<int64_t> mid(a[0].size()/2);
//         for(int j=0;j<mid.size();j++) mid[j]=a2[i*mid.size()+j];
//         Value M(std::move(mid));M.set_fraction_bits(a[i].get_fraction_bits());M.set_share_type(a[i].get_share_type());
//         res[i]=std::move(s2tos3(clients,0,M));
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
//     return res;
// }

std::vector<std::vector<std::vector<int64_t>>> genRandpem(Clients& clients,size_t m)
{
    size_t Pid=clients.getPid();

    std::vector<std::vector<int64_t>> seeds(clients.getPartyNumber(),std::vector<int64_t>(clients.getPartyNumber()));
    for(int k=0;k<=2;k++)
    {

        if(Pid==k){
            auto seed=generate_int64_numbers(1);
            clients.SendTo(seed,(k-1+3)%3);
            seeds[k][(k-1+3)%3]=seed[0];
        }
        else if(Pid==(k-1+3)%3)
        {

            auto seed=clients.recv(k);
            seeds[k][(k-1+3)%3]=seed[0];
        }

    }
    std::vector<std::vector<std::vector<int64_t>>> pem(clients.getPartyNumber(),std::vector<std::vector<int64_t>>(clients.getPartyNumber()));
    for(int k=0;k<=2;k++)
    {

        if(Pid==k){
            auto seed=seeds[k][(k-1+3)%3];
            std::vector<int64_t> v(m);
            std::iota(v.begin(),v.end(),0);
            std::mt19937 rng(seed);     // 使用 Mersenne Twister 引擎
            // std::shuffle(v.begin(),v.end(),rng);
            pem[k][(k-1+3)%3]=std::move(v);
        }
        else if(Pid==(k-1+3)%3)
        {

            auto seed=seeds[k][(k-1+3)%3];
            std::vector<int64_t> v(m);
            std::iota(v.begin(),v.end(),0);
            std::mt19937 rng(seed);     

            // std::shuffle(v.begin(),v.end(),rng); 
            pem[k][(k-1+3)%3]=std::move(v);
        }
    }
    return pem;
}

std::vector<Value> shuffle4pem(Clients& clients,const std::vector<Value>& a,const std::vector<std::vector<std::vector<int64_t>>> & pem)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    std::vector<Value> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<int64_t> a1((m),0);
        bool is_as=a[i].get_share_type();
        if(Pid==0){
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1];
        }
        else if(Pid==2)
        {
            if(is_as==0)
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]+a[i][j];
            }
            else
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]^a[i][j];
            }
            
        }
        Value a2(std::move(a1));a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
        for(int k=0;k<=2;k++)
        {
    
            if(Pid==k){
                auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[j]=a2[rp[j]];
                a2=std::move(res);a2.set_fraction_bits(a2.get_fraction_bits());a2.set_share_type(a2.get_share_type());
    
            }
            else if(Pid==(k-1+3)%3)
            {
    
                auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[j]=a2[rp[j]];
    
                Value val(std::move(res));
                if(k!=2)
                {   
                    clients.SendTo(val,(k+1)%3);
                }
                else {
                    a2=std::move(val);a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                }
            }
            else{
                if(k!=2)
                {
                    auto val=clients.recv((k-1+3)%3);
                    a2=std::move(val);
                    a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                } 
            }
           
        }
        res[i]=std::move(a2);
    }
    
    return res;
}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> shuffle4pem_bitset(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::vector<std::vector<int64_t>>>& pem)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    std::vector<std::vector<std::bitset<N>>> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<std::bitset<N>> a1((m),0);
        if(Pid==0){
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1];
        }
        else if(Pid==2)
        {
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=convToint(a[i][j+1])+convToint(a[i][j]);
        }
        for(int k=0;k<=2;k++)
        {
    
            if(Pid==k){
                auto v=pem[k][(k-1+3)%3];
                std::vector<std::bitset<N>> res(a1.size());
                for(int j=0;j<m;j++) res[j]=a1[v[j]];
                a1=std::move(res);
            }
            else if(Pid==(k-1+3)%3)
            {
    
                auto v=pem[k][(k-1+3)%3];
    
                std::vector<std::bitset<N>> res(a1.size());
                for(int j=0;j<m;j++) res[j]=a1[v[j]];
                if(k!=2)
                {   
                    clients.SendTo_bitset(res,(k+1)%3);
                }
                else {
                    a1=std::move(res);
                }
            }
            else{
                if(k!=2)
                {
                    auto val=clients.recv_bitset<N>((k-1+3)%3);
                    a1=std::move(val);
                } 
            }
           
        }
        res[i]=std::move(a1);
    }
    
    
    return res;
}


std::vector<Value> unshuffle4pem(Clients& clients,const std::vector<Value>& a,const std::vector<std::vector<std::vector<int64_t>>> & pem)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    std::vector<Value> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<int64_t> a1((m),0);
        bool is_as=a[i].get_share_type();
        if(Pid==2){
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1];
        }
        else if(Pid==1)
        {
            if(is_as==0)
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]+a[i][j];
            }
            else
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]^a[i][j];
            }
            
        }
        Value a2(std::move(a1));a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
        for(int k=2;k>=0;k--)
        {
    
            if(Pid==k){
                auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[rp[j]]=a2[j];
    
                Value val(std::move(res));
                if(k!=0)
                {   
                    clients.SendTo(val,(k+1)%3);
                }
                else {
                    a2=std::move(val);a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                }
               
    
            }
            else if(Pid==(k-1+3)%3)
            {
                 auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[rp[j]]=a2[j];
                a2=std::move(res);a2.set_fraction_bits(a2.get_fraction_bits());a2.set_share_type(a2.get_share_type());
               
            }
            else{
                if(k!=0)
                {
                    auto val=clients.recv(k);
                    a2=std::move(val);
                    a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                } 
            }
           
        }
        res[i]=std::move(a2);
    }
    
    return res;
}


template<size_t N>
std::vector<std::vector<std::bitset<N>>> unshuffle4pem_bitset(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::vector<std::vector<int64_t>>>& pem)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    std::vector<std::vector<std::bitset<N>>> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<std::bitset<N>> a1((m),0);
        if(Pid==2){
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1];
        }
        else if(Pid==1)
        {
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=convToint(a[i][j+1])+convToint(a[i][j]);
        }
        for(int k=2;k>=0;k--)
        {
    
            if(Pid==k){
                auto v=pem[k][(k-1+3)%3];
                std::vector<std::bitset<N>> res(a1.size());
                for(int j=0;j<m;j++) res[v[j]]=a1[j];
                if(k!=0)
                {   
                    clients.SendTo_bitset(res,(k+1)%3);
                }
                else {
                    a1=std::move(res);
                }
            }
            else if(Pid==(k-1+3)%3)
            {
    
                auto v=pem[k][(k-1+3)%3];
    
                std::vector<std::bitset<N>> res(a1.size());
                for(int j=0;j<m;j++) res[v[j]]=a1[j];
                a1=std::move(res);
            }
            else{
                if(k!=0)
                {
                    auto val=clients.recv_bitset<N>(k);
                    a1=std::move(val);
                } 
            }
           
        }
        res[i]=std::move(a1);
    }
    
    
    return res;
}

std::vector<Value> random_shuffle4pem(Clients& clients,const std::vector<Value>& a)
{

    int m=a[0].size()/2;
    auto pem=genRandpem(clients,m);
    
    
    return shuffle4pem(clients,a,pem);
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> random_shuffle4pem_bitset(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    auto pem=genRandpem(clients,m);
    
    
    
    return shuffle4pem_bitset(clients,a,pem);
}


std::vector<Value> random_shuffle(Clients& clients,const std::vector<Value>& a,size_t is_rand=0)
{
    size_t Pid=clients.getPid();


    int m=a[0].size()/2;
    std::vector<std::vector<int64_t>> seeds(clients.getPartyNumber(),std::vector<int64_t>(clients.getPartyNumber()));
    for(int k=0;k<=2;k++)
    {

        if(Pid==k){
            auto seed=generate_int64_numbers(1);
            clients.SendTo(seed,(k-1+3)%3);
            seeds[k][(k-1+3)%3]=seed[0];
        }
        else if(Pid==(k-1+3)%3)
        {

            auto seed=clients.recv(k);
            seeds[k][(k-1+3)%3]=seed[0];
        }

    }
    std::vector<std::vector<std::vector<int64_t>>> pem(clients.getPartyNumber(),std::vector<std::vector<int64_t>>(clients.getPartyNumber()));
    for(int k=0;k<=2;k++)
    {

        if(Pid==k){
            auto seed=seeds[k][(k-1+3)%3];
            std::vector<int64_t> v(m);
            std::iota(v.begin(),v.end(),0);
            std::mt19937 rng(seed);     // 使用 Mersenne Twister 引擎
            if(is_rand) std::shuffle(v.begin(),v.end(),rng);
            pem[k][(k-1+3)%3]=std::move(v);
        }
        else if(Pid==(k-1+3)%3)
        {

            auto seed=seeds[k][(k-1+3)%3];
            std::vector<int64_t> v(m);
            std::iota(v.begin(),v.end(),0);
            std::mt19937 rng(seed);     

            if(is_rand) std::shuffle(v.begin(),v.end(),rng);
            pem[k][(k-1+3)%3]=std::move(v);
        }
    }
    std::vector<Value> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<int64_t> a1((m),0);
        bool is_as=a[i].get_share_type();
        if(Pid==0){
            for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1];
        }
        else if(Pid==2)
        {
            if(is_as==0)
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]+a[i][j];
            }
            else
            {
                for(int j=0;j<a[i].size();j+=2) a1[j/2]=a[i][j+1]^a[i][j];
            }
            
        }
        Value a2(std::move(a1));a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
        for(int k=0;k<=2;k++)
        {
    
            if(Pid==k){
                auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[j]=a2[rp[j]];
                a2=std::move(res);a2.set_fraction_bits(a2.get_fraction_bits());a2.set_share_type(a2.get_share_type());
    
            }
            else if(Pid==(k-1+3)%3)
            {
    
                auto v=pem[k][(k-1+3)%3];
                Value rp(std::move(v));rp.set_fraction_bits(0);rp.set_share_type(0);
    
                std::vector<int64_t> res(a2.size());
                for(int j=0;j<m;j++) res[j]=a2[rp[j]];
    
                Value val(std::move(res));
                if(k!=2)
                {   
                    clients.SendTo(val,(k+1)%3);
                }
                else {
                    a2=std::move(val);a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                }
            }
            else{
                if(k!=2)
                {
                    auto val=clients.recv((k-1+3)%3);
                    a2=std::move(val);
                    a2.set_fraction_bits(a[i].get_fraction_bits());a2.set_share_type(a[i].get_share_type());
                } 
            }
           
        }
        res[i]=std::move(s2tos3(clients,0,a2));
    }
    
    
    return res;
}


std::vector<Value> Permutation(Clients& clients,const std::vector<Value>& a,const Value& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    std::vector<Value> cur;
    for(int i=0;i<a.size();i++) cur.push_back(a[i]);
    // print_Value(clients,cur[0]);
    cur.push_back(pi);
    auto res=random_shuffle4pem(clients,cur);
    std::vector<Value> R(a.size());
    if(Pid==1)
    {
        clients.SendTo(res[res.size()-1],2);
        auto pi=clients.recv(2);
        size_t rs=res.size()-1;
        if(pi.get_share_type()==0)
        {
            for(int i=0;i<pi.size();i++) pi[i]+=res[rs][i]-st;
        }
        else
        {
            for(int i=0;i<pi.size();i++) pi[i]=(pi[i]^res[rs][i])-st;
        }
        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<int64_t> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[pi[j]]=res[i][j];
            }
            R[i]=std::move(Value(std::move(pem)));
            R[i].set_share_type(a[i].get_share_type());
            R[i].set_fraction_bits(a[i].get_fraction_bits());
        }
    }
    else if(Pid==2)
    {
        clients.SendTo(res[res.size()-1],1);
        auto pi=clients.recv(1);
        size_t rs=res.size()-1;
        if(pi.get_share_type()==0)
        {
            for(int i=0;i<pi.size();i++) pi[i]+=res[rs][i]-st;
        }
        else
        {
            for(int i=0;i<pi.size();i++) pi[i]=(pi[i]^res[rs][i])-st;
        }


        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<int64_t> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[pi[j]]=res[i][j];
            }
            R[i]=std::move(Value(std::move(pem)));
            R[i].set_share_type(a[i].get_share_type());
            R[i].set_fraction_bits(a[i].get_fraction_bits());
        }
    }
    else{
        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<int64_t> pem(res[i].size(),0);
            R[i]=std::move(Value(std::move(pem)));
        }
    }
    return s2tos3_vector(clients,0,R);
}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> Permutation_bitset(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::bitset<N>>& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    std::vector<std::vector<std::bitset<N>>> cur;
    for(int i=0;i<a.size();i++) cur.push_back(a[i]);
    // print_Value(clients,cur[0]);
    cur.push_back(pi);
    auto res=random_shuffle4pem_bitset(clients,cur);
    std::vector<std::vector<std::bitset<N>>> R(a.size());
    if(Pid==1)
    {
        clients.SendTo_bitset(res[res.size()-1],2);
        auto pi=clients.recv_bitset<N>(2);
        size_t rs=res.size()-1;
        for(int i=0;i<pi.size();i++) pi[i]=convToint(pi[i])+convToint(res[rs][i])-st;
        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[revelToint(pi[j])]=res[i][j];
            }
            R[i]=std::move(pem);
        }
    }
    else if(Pid==2)
    {
        clients.SendTo_bitset(res[res.size()-1],1);
        auto pi=clients.recv_bitset<N>(1);
        size_t rs=res.size()-1;
        for(int i=0;i<pi.size();i++) pi[i]=convToint(pi[i])+convToint(res[rs][i])-st;


        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[revelToint(pi[j])]=res[i][j];
            }
            R[i]=std::move(pem);
        }
    }
    else{
        for(int i=0;i<res.size()-1;i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size(),0);
            R[i]=std::move(pem);
        }
    }
    return s2tos3_vector_bitset(clients,0,R);
}

template<size_t N>
std::vector<Value> Permutation_ValueAndbitset(Clients& clients,const std::vector<Value>& a,const std::vector<std::bitset<N>>& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    // print_Value(clients,cur[0]);
    auto pem=genRandpem(clients,a[0].size()/2);
    auto res_pi=shuffle4pem_bitset(clients,std::vector<std::vector<std::bitset<N>>>{pi},pem);
    auto res=shuffle4pem(clients,a,pem);

    std::vector<Value> R(a.size());
    if(Pid==1)
    {
        clients.SendTo_bitset(res_pi[0],2);
        auto pi=clients.recv_bitset<N>(2);
        for(int i=0;i<pi.size();i++) pi[i]=convToint(pi[i])+convToint(res_pi[0][i])-st;
        for(int i=0;i<res.size();i++)
        {
            std::vector<int64_t> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[revelToint(pi[j])]=res[i][j];
            }
            R[i]=std::move(pem);
            R[i].set_share_type(res[i].get_share_type());
            R[i].set_fraction_bits(res[i].get_fraction_bits());
        }
    }
    else if(Pid==2)
    {
        clients.SendTo_bitset(res_pi[0],1);
        auto pi=clients.recv_bitset<N>(1);
        for(int i=0;i<pi.size();i++) pi[i]=convToint(pi[i])+convToint(res_pi[0][i])-st;


        for(int i=0;i<res.size();i++)
        {
            std::vector<int64_t> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[revelToint(pi[j])]=res[i][j];
            }
            R[i]=std::move(pem);
            R[i].set_share_type(res[i].get_share_type());
            R[i].set_fraction_bits(res[i].get_fraction_bits());
        }
    }
    else{
        for(int i=0;i<res.size();i++)
        {
            std::vector<int64_t> pem(res[i].size(),0);
            R[i]=std::move(pem);
            R[i].set_share_type(res[i].get_share_type());
            R[i].set_fraction_bits(res[i].get_fraction_bits());
        }
    }
    return s2tos3_vector(clients,0,R);
}


template<size_t N>
std::vector<std::vector<std::bitset<N>>> Permutation_bitsetAndValue(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const Value& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    // print_Value(clients,cur[0]);
    auto pem=genRandpem(clients,a[0].size()/2);
    auto res_pi=shuffle4pem(clients,std::vector<Value>{pi},pem);
    auto res=shuffle4pem_bitset(clients,a,pem);

    std::vector<std::vector<std::bitset<N>>> R(a.size());
    if(Pid==1)
    {
        clients.SendTo(res_pi[0],2);
        auto pi=clients.recv(2);
        for(int i=0;i<pi.size();i++) pi[i]=pi[i]+res_pi[0][i]-st;
        for(int i=0;i<res.size();i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[pi[j]]=res[i][j];
            }
            R[i]=std::move(pem);
        }
    }
    else if(Pid==2)
    {
        clients.SendTo(res_pi[0],1);
        auto pi=clients.recv(1);
        for(int i=0;i<pi.size();i++) pi[i]=pi[i]+res_pi[0][i]-st;


        for(int i=0;i<res.size();i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size());
            for(int j=0;j<res[i].size();j++)
            {
                pem[pi[j]]=res[i][j];
            }
            R[i]=std::move(pem);
        }
    }
    else{
        for(int i=0;i<res.size();i++)
        {
            std::vector<std::bitset<N>> pem(res[i].size(),0);
            R[i]=std::move(pem);
        }
    }
    return s2tos3_vector_bitset(clients,0,R);
}




std::vector<Value> Permutation_rev(Clients& clients,const std::vector<Value>& a,const Value& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    auto pem=genRandpem(clients,a[0].size()/2);
    auto res_pi=s2tos3(clients,0,shuffle4pem(clients,std::vector<Value>{pi},pem)[0]);
    auto rc_res_pi=reconstruct(clients,res_pi);
    std::vector<Value> R(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<int64_t> pem(a[i].size());
        for(int j=0;j<a[i].size();j+=2)
        {
            pem[j]=a[i][2*rc_res_pi[j/2]];
            pem[j+1]=a[i][2*rc_res_pi[j/2]+1];
        }
        R[i]=std::move(pem);
    }
    return s2tos3_vector(clients,1,unshuffle4pem(clients,R,pem));
}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> Permutation_bitset_rev(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::bitset<N>>& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    auto pem=genRandpem(clients,a[0].size()/2);
    auto res_pi=s2tos3_bitset(clients,0,shuffle4pem_bitset(clients,std::vector<std::vector<std::bitset<N>>>{pi},pem)[0]);
    auto rc_res_pi=Areconstruct_bitset(clients,res_pi);
    std::vector<std::vector<std::bitset<N>>> R(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<std::bitset<N>> pem(a[i].size());
        for(int j=0;j<a[i].size();j+=2)
        {
            pem[j]=a[i][2*rc_res_pi[j/2]];
            pem[j+1]=a[i][2*rc_res_pi[j/2]+1];
        }
        R[i]=std::move(pem);
    }
    return s2tos3_vector_bitset(clients,1,unshuffle4pem_bitset(clients,R,pem));
}


template<size_t N>
std::vector<Value> Permutation_ValueAndbitset_rev(Clients& clients,const std::vector<Value>& a,const std::vector<std::bitset<N>>& pi,size_t st=0)
{
    auto Pid=clients.getPid();
    auto pem=genRandpem(clients,a[0].size()/2);
    auto res_pi=s2tos3_bitset(clients,0,shuffle4pem_bitset(clients,std::vector<std::vector<std::bitset<N>>>{pi},pem)[0]);
    auto rc_res_pi=Areconstruct_bitset(clients,res_pi);
    std::vector<Value> R(a.size());
    for(int i=0;i<a.size();i++)
    {
        std::vector<int64_t> pem(a[i].size());
        for(int j=0;j<a[i].size();j+=2)
        {
            pem[j]=a[i][2*rc_res_pi[j/2]];
            pem[j+1]=a[i][2*rc_res_pi[j/2]+1];
        }
        R[i]=std::move(pem);
        R[i].set_share_type(a[i].get_share_type());
        R[i].set_fraction_bits(a[i].get_fraction_bits());
    }
    return s2tos3_vector(clients,1,unshuffle4pem(clients,R,pem));
}




Value Permutation_gen(Clients& clients,const Value& a,bool order=1)
{
    auto not_a=make_constI(clients,1,a.size()/2);
    for(int i=0;i<a.size();i++) not_a[i]=not_a[i]-a[i];
    // Add(clients,ones,neg(clients,a));

    auto is_a=a;
    if(!order) 
    {
        auto tmp=is_a;
        is_a=std::move(not_a);
        not_a=std::move(tmp);
    }
    auto s1=perfix_sum(clients,not_a);
    is_a[0]+=s1[s1.size()-2];
    is_a[1]+=s1[s1.size()-1];
    auto s2=perfix_sum(clients,is_a);

    is_a[0]-=s1[s1.size()-2];
    is_a[1]-=s1[s1.size()-1];
    
    auto ss1=Mul(clients,s1,not_a);
    auto ss2=Mul(clients,s2,is_a);

    auto res1=Add(clients,ss1,ss2);

    // auto res1=Add(clients,s1,s2); 

    auto res=Add(clients,res1,make_constI(clients,-1,a.size()/2));
    return res;
}

template<size_t N>
std::vector<std::bitset<N>> Permutation_gen_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,bool order=1)
{
    auto not_a=make_constIbitset<N>(clients,1,a.size()/2);
    for(int i=0;i<a.size();i++) not_a[i]=convToint(not_a[i])-convToint(a[i]);
    // Add(clients,ones,neg(clients,a));

    auto is_a=a;
    if(!order) 
    {
        auto tmp=is_a;
        is_a=std::move(not_a);
        not_a=std::move(tmp);
    }
    auto s1=perfix_sum_bitset<N>(clients,not_a);
    is_a[0]=convToint(is_a[0])+convToint(s1[s1.size()-2]);
    is_a[1]=convToint(is_a[1])+convToint(s1[s1.size()-1]);
    auto s2=perfix_sum_bitset<N>(clients,is_a);

    is_a[0]=convToint(is_a[0])-convToint(s1[s1.size()-2]);
    is_a[1]=convToint(is_a[1])-convToint(s1[s1.size()-1]);
    auto ss1=Mul_bitset<N>(clients,s1,not_a);
    auto ss2=Mul_bitset<N>(clients,s2,is_a);
    auto res1=Add_bitset<N>(clients,ss1,ss2);

    // auto res1=Add(clients,s1,s2); 

    auto res=Add_bitset<N>(clients,res1,make_constIbitset<N>(clients,-1,a.size()/2));
    return res;
}