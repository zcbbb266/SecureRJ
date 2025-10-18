#pragma once


#include<vector>
#include<utility>

#include "../network/clients.hpp"
#include "Tool.hpp"
#include <omp.h>  // OpenMP 并行化库

int Log2Ceil(int nbits) {
    if (nbits <= 0) {
        throw std::invalid_argument("nbits must be greater than 0");
    }
    return static_cast<int>(std::ceil(std::log2(nbits)));
}


Value Xor(Clients& clients,const Value& a,const Value& b)
{
    
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size XOR");
    size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int64_t> c(a.size());

    for(int i=0;i<a.size();i++)
    {
        c[i]=a[i]^b[i];
    }

    Value val(std::move(c));val.set_fraction_bits(fp);val.set_share_type(1);
    return val;
}

void Xor128(Clients& clients,Value128& a,const Value128& b)
{
    
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size XOR");

    for(int i=0;i<a.size();i++)
    {
        a[i]^=b[i];
    }
}

Value And(Clients& clients,const Value& a,const Value& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size AND");
     size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int64_t> c(a.size()/2);
    std::vector<int64_t> res(a.size());

    // auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]&b[i])^(a[i]&b[i+1])^(a[i+1]&b[i]));
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Function runtime11111111: " << duration.count() << " ms" << std::endl;
    // auto start1 = std::chrono::high_resolution_clock::now();
    Value c1(std::move(c));c1.set_fraction_bits(fp);

    auto c2=clients.SendRoud(c1);
    // auto end1 = std::chrono::high_resolution_clock::now();
    // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    // std::cout << "Function runtime2222222222222: " << duration1.count() << " ms" << std::endl;
    for(int i=0;i<c1.size();i++)
        res[2*i]=(c2[i]),res[2*i+1]=(c1[i]);
    Value val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(1);
    return val;
}


Value8 And8(Clients& clients,const Value8& a,const Value8& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size AND");
     size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int8_t> c(a.size()/2);
    std::vector<int8_t> res(a.size());

    // auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]&b[i])^(a[i]&b[i+1])^(a[i+1]&b[i]));
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Function runtime11111111: " << duration.count() << " ms" << std::endl;
    // auto start1 = std::chrono::high_resolution_clock::now();
    Value8 c1(std::move(c));c1.set_fraction_bits(fp);

    auto c2=clients.SendRoud8(c1);
    // auto end1 = std::chrono::high_resolution_clock::now();
    // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    // std::cout << "Function runtime2222222222222: " << duration1.count() << " ms" << std::endl;
    for(int i=0;i<c1.size();i++)
        res[2*i]=(c2[i]),res[2*i+1]=(c1[i]);
    Value8 val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(1);
    return val;
}


Value16 And16(Clients& clients,const Value16& a,const Value16& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size AND");
     size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int16_t> c(a.size()/2);
    std::vector<int16_t> res(a.size());

    // auto start = std::chrono::high_resolution_clock::now();

    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]&b[i])^(a[i]&b[i+1])^(a[i+1]&b[i]));
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Function runtime11111111: " << duration.count() << " ms" << std::endl;
    // auto start1 = std::chrono::high_resolution_clock::now();
    Value16 c1(std::move(c));c1.set_fraction_bits(fp);

    auto c2=clients.SendRoud16(c1);
    // auto end1 = std::chrono::high_resolution_clock::now();
    // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    // std::cout << "Function runtime2222222222222: " << duration1.count() << " ms" << std::endl;
    for(int i=0;i<c1.size();i++)
        res[2*i]=(c2[i]),res[2*i+1]=(c1[i]);
    Value16 val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(1);
    return val;
}

Value32 And32(Clients& clients,const Value32& a,const Value32& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size AND");
     size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int32_t> c(a.size()/2);
    std::vector<int32_t> res(a.size());

    // auto start = std::chrono::high_resolution_clock::now();

    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]&b[i])^(a[i]&b[i+1])^(a[i+1]&b[i]));
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Function runtime11111111: " << duration.count() << " ms" << std::endl;
    // auto start1 = std::chrono::high_resolution_clock::now();
    Value32 c1(std::move(c));c1.set_fraction_bits(fp);

    auto c2=clients.SendRoud32(c1);
    // auto end1 = std::chrono::high_resolution_clock::now();
    // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    // std::cout << "Function runtime2222222222222: " << duration1.count() << " ms" << std::endl;
    for(int i=0;i<c1.size();i++)
        res[2*i]=(c2[i]),res[2*i+1]=(c1[i]);
    Value32 val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(1);
    return val;
}



std::vector<Value> And_vector(Clients& clients,const std::vector<Value>& a,const std::vector<Value> &b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size AND");    

    std::vector<std::vector<int64_t>> c(a.size(),std::vector<int64_t>(a[0].size()/2));
    std::vector<std::vector<int64_t>> res(a.size(),std::vector<int64_t>(a[0].size()));
    for(int i=0;i<a.size();i++)
    {
        if(a[i].size()!=b[i].size())
            throw std::invalid_argument("a[i].size must equal b[i].size AND");
        for(int j=0;j<a[i].size();j+=2)
        {
            c[i][j/2]=((a[i][j]&b[i][j])^(a[i][j]&b[i][j+1])^(a[i][j+1]&b[i][j]));
        }
    }

    auto c2=clients.SendRoud_vector(c);

    for(int i=0;i<a.size();i++)
    {
        for(int j=0;j<a[i].size();j+=2)
        {
            res[i][j]=c2[i][j/2];res[i][j+1]=c[i][j/2];
        }
    }

    std::vector<Value> val;
    for(int i=0;i<a.size();i++)
    {
        Value v(std::move(res[i]));v.set_fraction_bits(a[i].get_fraction_bits());v.set_share_type(1);
        val.push_back(v);
    }

    return val;
}


void And128(Clients& clients,Value128& a,const Value128& b)
{
    int m=b.size();
    for(int i=0;i<a.size();i++)
    {
        a[i]&=b[0];
    }
}

Value Or(Clients& clients,const Value& a,const Value& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size OR");

    auto Ones=make_ones_64bit(clients,a.size()/2);

    auto res=std::move(Xor(clients,And(clients,Xor(clients,a,Ones),Xor(clients,b,Ones)),Ones));

    return res;
}

Value Lshift(Clients& clients,const Value& a,size_t offset)
{
    std::vector<int64_t> res(a.size());
    for(int i=0;i<a.size();i++) res[i]=(a[i]<<offset);
    Value val(std::move(res));val.set_fraction_bits(a.get_fraction_bits());val.set_share_type(1);
    return val;
}

Value Rshift(Clients& clients,const Value& a,size_t offset)
{
    std::vector<int64_t> res(a.size());
    for(int i=0;i<a.size();i++) res[i]=(a[i]>>offset);
    Value val(std::move(res));val.set_fraction_bits(a.get_fraction_bits());val.set_share_type(1);
    return val;
}


Value8 Rshift8(Clients& clients,const Value8& a,size_t offset)
{
    std::vector<int8_t> res(a.size());
    for(int i=0;i<a.size();i++) res[i]=(a[i]>>offset);
    Value8 val(std::move(res));val.set_fraction_bits(a.get_fraction_bits());val.set_share_type(1);
    return val;
}



void Lshift128(Clients& clients,Value128& a,size_t offset)
{
    for(int i=0;i<a.size();i++) (a[i]<<=offset);
}

void Rshift128(Clients& clients,Value128& a,size_t offset)
{
    for(int i=0;i<a.size();i++) (a[i]>>=offset);
}

Value ppa_kogge_stone(Clients& clients, const Value& lhs, const Value& rhs,
                      size_t nbits=64) {
  // Generate p & g.
  auto P = Xor(clients, lhs, rhs);
  auto G = And(clients, lhs, rhs);

  for (int idx = 0; idx < Log2Ceil(nbits); ++idx) {
    const size_t offset = 1UL << idx;
    auto G1 = Lshift(clients, G, offset);
    auto P1 = Lshift(clients, P, offset);

    // P1 = P & P1
    // G1 = G ^ (P & G1)
    P1=std::move(And(clients,P,P1));
    G1=std::move(Xor(clients,G,And(clients,P,G1)));

    P = std::move(P1);
    G = std::move(G1);
  }

  // out = (G << 1) ^ p0
  auto C = Lshift(clients, G, 1);
  return Xor(clients, Xor(clients, lhs, rhs), C);
}


Value ppa_kogge_stone_sub(Clients& clients, const Value& lhs, const Value& rhs,
                      size_t nbits=64) {

  // Generate p & g.
  auto rhs_copy=rhs;
  for(int i=0;i<rhs.size();i++) rhs_copy[i]=~rhs_copy[i];

  auto P = Xor(clients, lhs, rhs_copy);
  auto G = And(clients, lhs, rhs_copy);
  P=Lshift(clients,P,1);
  G=Lshift(clients,G,1);

  for(int i=0;i<G.size();i++) G[i]|=1;

  for (int idx = 0; idx < Log2Ceil(nbits); ++idx) {
    const size_t offset = 1UL << idx;
    auto G1 = Lshift(clients, G, offset);
    auto P1 = Lshift(clients, P, offset);

    // P1 = P & P1
    // G1 = G ^ (P & G1)
    P1=std::move(And(clients,P,P1));
    G1=std::move(Xor(clients,G,And(clients,P,G1)));

    P = std::move(P1);
    G = std::move(G1);
  }

  // out = (G << 1) ^ p0
//   auto C = Lshift(clients, G, 1);
  return Xor(clients, Xor(clients, lhs, rhs_copy), G);
}

std::vector<int8_t> Breconstruct8(Clients& clients,const Value8& a)
{

    std::vector<int8_t> a1(a.size()/2),a2(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1]^a1[i/2];
    }
    Value8 A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(1);
   
    auto A3=clients.SendRoud8(A1);
    

    for(int i=0;i<A3.size();i++){
        a2[i]^=A3[i];
    }
    return a2;
}


std::vector<int64_t> Breconstruct(Clients& clients,const Value& a)
{

    std::vector<int64_t> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
        a3[i/2]=a1[i/2]^a2[i/2];
    }
    Value A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(1);
    auto A3=clients.SendRoud(A1);
    for(int i=0;i<A3.size();i++){
        a3[i]^=A3[i];
    }
    return a3;
}

std::vector<__int128> Breconstruct128(Clients& clients,const Value128& a)
{

    std::vector<__int128> a1(a.size()/2),a2(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
        a2[i/2]^=a1[i/2];
    }
    Value128 A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(1);
    auto A3=clients.SendRoud128(A1);
    for(int i=0;i<A3.size();i++){
        a2[i]^=A3[i];
    }
    return a2;
}

std::vector<int64_t> BrevelTo(Clients& clients,const Value& a,size_t To)
{
    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();
    std::vector<int64_t> res(a.size()/2);
    if(Pid==To)
    {
        auto a3=clients.recv((Pid-1+Party_number)%Party_number);
        for(int i=0;i<a.size();i+=2) res[i/2]=(a[i]^a[i+1]^a3[i]);
        return res;
    }
    else if(Pid==(To-1+Party_number)%Party_number) clients.SendTo(a,To);
    return res;
}


std::vector<__int128> BrevelTo128(Clients& clients,const Value128& a,size_t To)
{
    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();
    std::vector<__int128> res(a.size()/2);
    if(Pid==To)
    {
        auto a3=clients.recv128((Pid-1+Party_number)%Party_number);
        for(int i=0;i<a.size();i+=2) res[i/2]=(a[i]^a[i+1]^a3[i]);
        return res;
    }
    else if(Pid==(To-1+Party_number)%Party_number) clients.SendTo128(a,To);
    return res;
}

Value Bneg(Clients& clients,const Value& a)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> v1(a.size()),v2(a.size());
    for(int i=0;i<a.size();i++) v1[i]=~a[i]+1,v2[i]=(1);
    Value val1(std::move(v1));val1.set_fraction_bits(a.get_fraction_bits());val1.set_share_type(1);
    Value val2(std::move(v2));val2.set_fraction_bits(a.get_fraction_bits());val2.set_share_type(1);
    return ppa_kogge_stone(clients,val1,val2);

}

