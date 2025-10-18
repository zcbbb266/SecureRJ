#pragma once

#include "../Value/value128.h"
#include "../network/clients.hpp"
#include "../Operator/operator.h"

#include<vector>
#include<bitset>
#include<utility>
#include <omp.h>

template<size_t N>
void Xor_bitset(Clients& clients,std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b,const std::vector<size_t> per) 
{
    for(int i=0;i<a.size();i++)
    {
        // a[i]^=b[per[i]];
        a[i]^=b[0];

    }
}
template<size_t N>
void Xor_bitset_real(Clients& clients,std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b) 
{
    for(int i=0;i<a.size();i++)
    {
        a[i]=(a[i]^b[i]).to_ulong();
    }
}

template<size_t N>
std::vector<std::bitset<N>> Xor_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b) 
{
    std::vector<std::bitset<N>> c(a.size());
    for(int i=0;i<a.size();i++)
    {
        c[i]=(a[i]^b[i]).to_ulong();
    }
    return c;
}


template<size_t N>
std::vector<std::bitset<N>> And_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b) 
{
    std::vector<std::bitset<N>> c(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]&b[i])^(a[i+1]&b[i])^(a[i]&b[i+1]));
    }
    auto c1=clients.SendRoud_bitset(c);
    std::vector<std::bitset<N>> res(a.size());
    for(int i=0;i<res.size();i+=2)
    {
        res[i]=c1[i/2];
        res[i+1]=c[i/2];
    }
    return res;
}

template<size_t N>
std::vector<std::bitset<N>> And_bitset_real(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& aa,const std::vector<std::bitset<N>>& b,const std::vector<std::bitset<N>>& bb) 
{
    std::vector<std::bitset<N>> c(a.size());
    for(int i=0;i<a.size();i++)
    {
        c[i]=((a[i]&b[i])^(aa[i]&b[i])^(a[i]&bb[i]));
    }
    return c;
}


template<size_t N>
void Rshift_bitset(Clients& clients,std::vector<std::bitset<N>>& a,size_t offset) 
{
    for(int i=0;i<a.size();i++) a[i]>>=offset;
}

template<size_t N>
void Lshift_bitset(Clients& clients,std::vector<std::bitset<N>>& a,size_t offset) 
{
    for(int i=0;i<a.size();i++) a[i]<<=offset;
}

template<size_t N>
std::vector<std::bitset<N>> Rshift_bitset_real(Clients& clients,const std::vector<std::bitset<N>>& a,size_t offset) 
{
    std::bitset<N> btN=(1ULL<<64)-1;
    btN=btN<<16;
    std::vector<std::bitset<N>> c(a.size());
    for(int i=0;i<a.size();i++) c[i]=(a[i]>>offset)^btN;
    return c;
}

template<size_t N>
std::vector<std::bitset<N>> Lshift_bitset_real(Clients& clients,const std::vector<std::bitset<N>>& a,size_t offset) 
{
    std::vector<std::bitset<N>> c(a.size());
    for(int i=0;i<a.size();i++) c[i]=a[i]<<offset;
    return c;
}

template<size_t N>
void getLast3_bitset(const std::vector<std::bitset<N>>& msg,size_t offset,size_t cur,std::vector<std::bitset<14>>& a,std::vector<std::bitset<14>>& b,std::vector<std::bitset<14>>& c) 
{
    for(int i=0;i<msg.size();i++)
    {
        a[i][cur]=msg[i][77-offset];
        b[i][cur]=msg[i][78-offset];
        c[i][cur]=msg[i][79-offset];
    } 
}


template<size_t N>
std::vector<std::bitset<N>> reconstruct_bitset(Clients& clients,const std::vector<std::bitset<N>>& a) 
{
    std::vector<std::bitset<N>> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
    }
    auto A3=clients.SendRoud_bitset<N>(a1);

    for(int i=0;i<A3.size();i++){
        a3[i]=A3[i];
        a3[i]^=a1[i]^a2[i];
    }
    return a3;
}


template<size_t N>
std::vector<std::bitset<N>> BrevelTo_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,size_t To)
{
    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();
    std::vector<std::bitset<N>> res(a.size()/2);
    if(Pid==To)
    {
        auto a3=clients.recv_bitset<N>((Pid-1+Party_number)%Party_number);
        for(int i=0;i<a.size();i+=2) res[i/2]=(a[i]^a[i+1]^a3[i]);
        return res;
    }
    else if(Pid==(To-1+Party_number)%Party_number) clients.SendTo_bitset<N>(a,To);
    return res;
}


std::vector<std::bitset<128>> gen_rand_key(Clients& clients,size_t t)
{

    std::vector<std::bitset<128>> keys(2*t);
    auto k1=gen_rand_vector128(t);
    auto k2=clients.SendRoud_bitset<128>(k1);
    std::vector<std::bitset<128>> key(k1.size()*2);
    for(int i=0;i<k1.size();i++) keys[2*i]=k2[i],keys[2*i+1]=k1[i];

    return keys;
}

template<size_t N>
std::vector<int8_t> Andcount_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b)
{
    std:: vector<int8_t> res(a.size());
    #pragma omp parallel for
    for(int i=0;i<a.size();i++)
    {
        auto cur=a[i]&b[0];
        res[i] = cur.count()&1;
    }
    return res;
}

template<size_t N>
void Andcount_bitset_op(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b,std::vector<std::bitset<N>>& tmp,size_t sz)
{
    auto mt=b[0];
    #pragma omp parallel for
    for(int i=0;i<a.size();i++)
    {        
        auto cur=a[i]&mt;
        tmp[i][sz]=cur.count()&1;
    }
}

void Xor64(Clients& clients,Value& a,const Value& b,const std::vector<size_t> per)
{
    
    for(int i=0;i<a.size();i++)
    {
        a[i]^=b[per[i]];
    }
}

void And64(Clients& clients,Value& a,const Value& b,const std::vector<size_t> per)
{
    int m=b.size();
    for(int i=0;i<a.size();i++)
    {
        a[i]&=b[per[i]];
    }
}
Value expan64(Clients& clients,const Value& a,size_t sz)
{
    if(sz%a.size()!=0) throw std::runtime_error("size error");
    std::vector<int64_t> temp(sz);
    #pragma omp parallel for
    for(int i=0;i<sz;i++) 
    {
        temp[i]=a[i%(a.size())];
    }
    Value R(std::move(temp));R.set_fraction_bits(a.get_fraction_bits());R.set_share_type(a.get_share_type());
    return R;
}

void Lshift64(Clients& clients,Value& a,size_t offset)
{
    for(int i=0;i<a.size();i++) (a[i]<<=offset);
}

void Rshift64(Clients& clients,Value& a,size_t offset)
{
    for(int i=0;i<a.size();i++) (a[i]>>=offset);
}

template<size_t N,size_t M>
std::vector<std::bitset<M>> conv_bitset_FromNToM(const std::vector<std::bitset<N>>& a)
{
    std::vector<std::bitset<M>> res(a.size());
    if(N>M)
    {
        std::bitset<N> btN=(1ULL<<M)-1;
        for(int i=0;i<a.size();i++) res[i]=(a[i]&btN).to_ullong();
    }
    else
    {
        for(int i=0;i<a.size();i++) res[i]=a[i].to_ullong();
    }


    return res;
}
template<size_t N>
Value eq_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b)
{
    std::vector<std::bitset<N>> c(a.size());
    for(int i=0;i<a.size();i++) c[i]=~(a[i]^b[i]);
    std::vector<int64_t> C(a.size());
    if constexpr (N == 80){
        auto low40=conv_bitset_FromNToM<80,64>(c);
        auto high40=conv_bitset_FromNToM<80,64>(Rshift_bitset_real(clients,c,64));
        auto c40=And_bitset(clients,low40,high40);
        for(int i=0;i<c40.size();i++) C[i]=c40[i].to_ullong();
    }
    else if constexpr(N==64)
    {
        for(int i=0;i<c.size();i++) C[i]=c[i].to_ullong();
    }
    else throw std::runtime_error("template support 64 and 80 error");

    std::vector<int32_t> low32(a.size()),high32(a.size());
    for(int i=0;i<a.size();i++) low32[i]=C[i],high32[i]=(C[i]>>32);
    Value32 Low32(std::move(low32));Low32.set_share_type(1);
    Value32 High32(std::move(high32));High32.set_share_type(1);
    Low32=And32(clients,Low32,High32);

    std::vector<int16_t> low16(a.size()),high16(a.size());
    for(int i=0;i<a.size();i++) low16[i]=Low32[i],high16[i]=(Low32[i]>>16);
    Value16 Low16(std::move(low16));Low16.set_share_type(1);
    Value16 High16(std::move(high16));High16.set_share_type(1);
    Low16=And16(clients,Low16,High16);

    std::vector<int8_t> low8(a.size()),high8(a.size());
    for(int i=0;i<a.size();i++) low8[i]=Low16[i],high8[i]=(Low16[i]>>8);
    Value8 Low8(std::move(low8));Low8.set_share_type(1);
    Value8 High8(std::move(high8));High8.set_share_type(1);
    Low8=And8(clients,Low8,High8);

    Low8=And8(clients,Low8,Rshift8(clients,Low8,4));

    Low8=And8(clients,Low8,Rshift8(clients,Low8,2));

    Low8=And8(clients,Low8,Rshift8(clients,Low8,1));

    std::vector<int64_t> res(c.size());
    for(int i=0;i<c.size();i++) res[i]=Low8[i]&1;
    Value R(std::move(res));R.set_share_type(1);
    return R;

}

template<size_t N>
std::vector<std::bitset<N>> ppa_kogge_stone(Clients& clients, const std::vector<std::bitset<N>>& lhs, const std::vector<std::bitset<N>>& rhs) {
  // Generate p & g.
  std::vector<std::bitset<N>> P = Xor_bitset(clients, lhs, rhs);
  std::vector<std::bitset<N>> G = And_bitset(clients, lhs, rhs);
  for (int idx = 0; idx < Log2Ceil(N); ++idx) {
    const size_t offset = 1UL << idx;
    std::vector<std::bitset<N>> G1 = Lshift_bitset_real(clients, G, offset);
    std::vector<std::bitset<N>> P1 = Lshift_bitset_real(clients, P, offset);

    P1=std::move(And_bitset(clients,P,P1));
    G1=std::move(Xor_bitset(clients,G,And_bitset(clients,P,G1)));

    P = std::move(P1);
    G = std::move(G1);
  }


  // out = (G << 1) ^ p0
  Lshift_bitset(clients, G, 1);
  return Xor_bitset(clients, Xor_bitset(clients, lhs, rhs), G);
}

template<size_t N>
std::vector<std::bitset<N>> A2B(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> a1(a.size()/2),a2(a.size());
    std::vector<std::bitset<N>> A1;
    if(Pid==0)
    {
        for(int i=0;i<a.size();i+=2)
        {
            a1[i/2]=convToint(a[i])+convToint(a[i+1]);
        }
        A1=BmakeShare(clients,0,a1);
    } 
    else if(Pid==1) A1=BmakeShare<N>(clients,0);
    else A1=BmakeShare<N>(clients,0);
    for(int i=0;i<a.size();i+=2)
    {
        if(Pid==0)
        {
            a2[i]=0;a2[i+1]=0;
        }
        else if(Pid==1)
        {
            a2[i]=0;a2[i+1]=a[i+1];
        }
        else
        {
            a2[i]=a[i];a2[i+1]=0;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto res=ppa_kogge_stone(clients,A1,a2);
    auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime2222: " << duration.count() << " ms" << std::endl;
    return res;
}