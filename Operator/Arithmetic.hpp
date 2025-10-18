#pragma once


#include<vector>
#include<utility>

#include "../Value/value.h"
#include "../Value/value128.h"
#include "../network/clients.hpp"
#include "../ThreePartyOT/Ot.hpp"
#include "Tool.hpp"


Value Add_A(Clients& clients,const Value& a,const Value& b)
{

    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size ADD");
    // if(a.get_fraction_bits()!=b.get_fraction_bits())
    //     throw std::invalid_argument("a.fb must equal b.fb ADD");
    size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());

    // Value aa=a;
    // Value bb=b;
    // if(aa.get_fraction_bits()<fp)
    // {
    //     size_t dif=fp-aa.get_fraction_bits();
    //     for(int i=0;i<aa.size();i++) aa[i]=(aa[i]<<dif);
    //     aa.set_fraction_bits(fp);
    // }

    // if(bb.get_fraction_bits()<fp)
    // {
    //     size_t dif=fp-bb.get_fraction_bits();
    //     for(int i=0;i<bb.size();i++) bb[i]=(bb[i]<<dif);
    //     bb.set_fraction_bits(fp);
    // }
    std::vector<int64_t> c(a.size());
    if(a.get_fraction_bits()<fp)
    {
        size_t dif=fp-a.get_fraction_bits();
        for(int i=0;i<a.size();i++) c[i]=(a[i]<<dif)+b[i];

    }

    else if(b.get_fraction_bits()<fp)
    {
        size_t dif=fp-b.get_fraction_bits();
        for(int i=0;i<a.size();i++) c[i]=a[i]+(b[i]<<dif);
    }

    else{
        for(int i=0;i<a.size();i++) c[i]=a[i]+b[i];
    }
    Value val(std::move(c));val.set_fraction_bits(fp);val.set_share_type(0);
    return val;
}


Value32 Add_32(Clients& clients,const Value32& a,const Value32& b)
{

    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size ADD");
    // if(a.get_fraction_bits()!=b.get_fraction_bits())
    //     throw std::invalid_argument("a.fb must equal b.fb ADD");
    size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());

    std::vector<int32_t> c(a.size());
    if(a.get_fraction_bits()<fp)
    {
        size_t dif=fp-a.get_fraction_bits();
        for(int i=0;i<a.size();i++) c[i]=(a[i]<<dif)+b[i];

    }

    else if(b.get_fraction_bits()<fp)
    {
        size_t dif=fp-b.get_fraction_bits();
        for(int i=0;i<a.size();i++) c[i]=a[i]+(b[i]<<dif);
    }

    else{
        for(int i=0;i<a.size();i++) c[i]=a[i]+b[i];
    }
    Value32 val(std::move(c));val.set_fraction_bits(fp);val.set_share_type(0);
    return val;
}

template<size_t N>
std::vector<std::bitset<N>> Add_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b)
{

    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size ADD");
    // if(a.get_fraction_bits()!=b.get_fraction_bits())
    //     throw std::invalid_argument("a.fb must equal b.fb ADD");

    std::vector<std::bitset<N>> c(a.size());

    for(int i=0;i<a.size();i++) c[i]=convToint(a[i])+convToint(b[i]);

    return c;
}


std::vector<Value> Mul_vector(Clients& clients,const std::vector<Value>& a,const std::vector<Value> &b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size MUL");    

    std::vector<std::vector<int64_t>> c(a.size(),std::vector<int64_t>(a[0].size()/2));
    std::vector<std::vector<int64_t>> res(a.size(),std::vector<int64_t>(a[0].size()));
    for(int i=0;i<a.size();i++)
    {
        if(a[i].size()!=b[i].size())
            throw std::invalid_argument("a[i].size must equal b[i].size MUL");
        for(int j=0;j<a[i].size();j+=2)
        {
            c[i][j/2]=((a[i][j]*b[i][j]+a[i][j]*b[i][j+1]+a[i][j+1]*b[i][j]));
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
        Value v(std::move(res[i]));v.set_fraction_bits(a[i].get_fraction_bits());v.set_share_type(0);
        val.push_back(v);
    }

    return val;
}



Value i_Mul_A(Clients& clients,const Value& a,const Value& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size MUL");
    size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int64_t> c(a.size()/2);
    std::vector<int64_t> res(a.size());

    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]*b[i]+a[i]*b[i+1]+a[i+1]*b[i]));
    }

    Value c1(std::move(c));c1.set_fraction_bits(fp);c1.set_share_type(0);

    auto c2=clients.SendRoud(c1);


    for(int i=0;i<c1.size();i++)
    {
        res[2*i]=c2[i];res[2*i+1]=c1[i];
    }
    Value val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(0);


    return val;
}


Value32 Mul_32(Clients& clients,const Value32& a,const Value32& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size MUL");
    size_t fp=std::max(a.get_fraction_bits(),b.get_fraction_bits());
    std::vector<int32_t> c(a.size()/2);
    std::vector<int32_t> res(a.size());

    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]*b[i]+a[i]*b[i+1]+a[i+1]*b[i]));
    }

    Value32 c1(std::move(c));c1.set_fraction_bits(fp);c1.set_share_type(0);

    auto c2=clients.SendRoud32(c1);


    for(int i=0;i<c1.size();i++)
    {
        res[2*i]=c2[i];res[2*i+1]=c1[i];
    }
    Value32 val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(0);


    return val;
}

template<size_t N>
std::vector<std::bitset<N>> Mul_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b)
{

    std::vector<std::bitset<N>> c(a.size()/2);
    std::vector<std::bitset<N>> res(a.size());
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=std::bitset<N>(((convToint(a[i])*convToint(b[i])+convToint(a[i])*convToint(b[i+1])+convToint(a[i+1])*convToint(b[i]))));
    }
   

    auto c1=clients.SendRoud_bitset<N>(c);


    for(int i=0;i<c1.size();i++)
    {
        res[2*i]=c1[i];res[2*i+1]=c[i];
    }
   
    return res;
}


template<size_t N>
Value Mul_ValueAndbitset(Clients& clients,const Value& a,const std::vector<std::bitset<N>>& b)
{

    std::vector<int64_t> c(a.size()/2);
    std::vector<int64_t> res(a.size());
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]*b[i].to_ullong()+a[i]*b[i+1].to_ullong()+a[i+1]*b[i].to_ullong()));
    }
   
    Value C(std::move(c));
    auto c1=clients.SendRoud(C);


    for(int i=0;i<c1.size();i++)
    {
        res[2*i]=c1[i];res[2*i+1]=C[i];
    }
   
    return Value(std::move(res));
}




//a is public sender&helper 1&3 / 3&1
Value Mul_ab1(Clients& clients,const Value& a,const Value& b,size_t sender)
{
    size_t receiver=(sender-1+3)%3;
    size_t Pid=clients.getPid();
    size_t helper;
    for(int i=0;i<3;i++) if(i!=sender&&i!=receiver) helper=i;
    if(Pid==sender)
    {
        auto c1=Value(std::move(gen_rand_vector(a.size())));c1.set_fraction_bits(a.get_fraction_bits());
        auto c3=Value(std::move(gen_rand_vector(a.size())));c3.set_fraction_bits(a.get_fraction_bits());
        clients.SendTo(c3,receiver);
        clients.SendTo(c1,helper);
        std::vector<int64_t> m0_temp(a.size()),m1_temp(a.size());
        for(int i=0;i<a.size();i++) {
            m0_temp[i]=(0^b[2*i]^b[2*i+1])*a[i]-c1[i]-c3[i];
            m1_temp[i]=(1^b[2*i]^b[2*i+1])*a[i]-c1[i]-c3[i];
        }
        Value m0(std::move(m0_temp));m0.set_share_type(a.get_share_type());m0.set_fraction_bits(a.get_fraction_bits());
        Value m1(std::move(m1_temp));m1.set_share_type(a.get_share_type());m1.set_fraction_bits(a.get_fraction_bits());
        Ot(clients,m0,m1,Value(),sender,receiver);
        std::vector<int64_t> res(2*c1.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c3[i];res[2*i+1]=c1[i];
        }
        Value R(std::move(res));R.set_fraction_bits(a.get_fraction_bits());R.set_share_type(0);
        return R;
    } 
    else if(Pid==receiver)
    {
        auto c3=clients.recv(sender);
        std::vector<int64_t> b2(c3.size());
        for(int i=0;i<b2.size();i++) {
            b2[i]=b[2*i];
        }
        auto c2=Ot(clients,Value(),Value(),Value(std::move(b2)),sender,receiver);
        clients.SendTo(c2,helper);
        std::vector<int64_t> res(2*c2.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c2[i];res[2*i+1]=c3[i];
        }
        Value R(std::move(res));R.set_fraction_bits(c3.get_fraction_bits());R.set_share_type(0);
        return R;
    }
    else{
        auto c1=clients.recv(sender);
        Ot(clients,Value(),Value(),Value(),sender,receiver);
        auto c2=clients.recv(receiver);
        std::vector<int64_t> res(2*c2.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c1[i];res[2*i+1]=c2[i];
        }
        Value R(std::move(res));R.set_fraction_bits(c1.get_fraction_bits());R.set_share_type(0);
        return R;
    }
}

template<size_t N>
std::vector<std::bitset<N>> Mul_ab1(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>>& b,size_t sender)
{
    size_t receiver=(sender-1+3)%3;
    size_t Pid=clients.getPid();
    size_t helper;
    for(int i=0;i<3;i++) if(i!=sender&&i!=receiver) helper=i;
    if(Pid==sender)
    {
        auto c1=gen_rand_bitsetvector<N>(a.size());
        auto c3=gen_rand_bitsetvector<N>(a.size());
        clients.SendTo_bitset(c3,receiver);
        clients.SendTo_bitset(c1,helper);
        std::vector<std::bitset<N>> m0_temp(a.size()),m1_temp(a.size());
        for(int i=0;i<a.size();i++) {
            m0_temp[i]=(0^convToint(b[2*i])^convToint(b[2*i+1]))*convToint(a[i])-convToint(c1[i])-convToint(c3[i]);
            m1_temp[i]=(1^convToint(b[2*i])^convToint(b[2*i+1]))*convToint(a[i])-convToint(c1[i])-convToint(c3[i]);
        }
        Ot(clients,m0_temp,m1_temp, std::vector<std::bitset<N>>(),sender,receiver);
        std::vector<std::bitset<N>> res(2*c1.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c3[i];res[2*i+1]=c1[i];
        }
        return res;
    } 
    else if(Pid==receiver)
    {
        auto c3=clients.recv_bitset<N>(sender);
        std::vector<std::bitset<N>> b2(c3.size());
        for(int i=0;i<b2.size();i++) {
            b2[i]=b[2*i];
        }
        auto c2=Ot(clients,std::vector<std::bitset<N>>(),std::vector<std::bitset<N>>(),b2,sender,receiver);
        clients.SendTo_bitset(c2,helper);
        std::vector<std::bitset<N>> res(2*c2.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c2[i];res[2*i+1]=c3[i];
        }
        return res;
    }
    else{
        auto c1=clients.recv_bitset<N>(sender);
        Ot(clients,std::vector<std::bitset<N>>(),std::vector<std::bitset<N>>(),std::vector<std::bitset<N>>(),sender,receiver);
        auto c2=clients.recv_bitset<N>(receiver);
        std::vector<std::bitset<N>> res(2*c2.size());
        for(int i=0;i<res.size()/2;i++)
        {
            res[2*i]=c1[i];res[2*i+1]=c2[i];
        }
        return res;
    }
}

std::vector<int8_t> Areconstruct8(Clients& clients,const Value8& a)
{
    std::vector<int8_t> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
    }
    Value8 A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(0);

    auto A3=clients.SendRoud8(A1);

    for(int i=0;i<A3.size();i++){
        a3[i]=A3[i];
        a3[i]+=A1[i]+a2[i];
    }
    return a3;
}

std::vector<int64_t> Areconstruct(Clients& clients,const Value& a)
{
    std::vector<int64_t> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
    }

    Value A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(0);

    auto A3=clients.SendRoud(A1);

    for(int i=0;i<A3.size();i++){
        a3[i]=A3[i];
        a3[i]+=A1[i]+a2[i];
    }

    return a3;
}

std::vector<__int128> Areconstruct128(Clients& clients,const Value128& a)
{
    std::vector<__int128> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
    }
    Value128 A1(std::move(a1));A1.set_fraction_bits(a.get_fraction_bits());A1.set_share_type(0);

    auto A3=clients.SendRoud128(A1);

    for(int i=0;i<A3.size();i++){
        a3[i]+=A3[i];
        a3[i]+=A1[i]+a2[i];
    }
    return a3;
}



template<size_t N>
std::vector<typename select_type<N>::type> Areconstruct_bitset(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    std::vector<std::bitset<N>> a1(a.size()/2),a2(a.size()/2),a3(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        a1[i/2]=a[i],a2[i/2]=a[i+1];
    }

    a3=clients.SendRoud_bitset<N>(a1);
    std::vector<typename select_type<N>::type> res(a.size()/2);
    for(int i=0;i<a3.size();i++){
        res[i]=revelToint(std::bitset<N>(convToint(a3[i])+convToint(a1[i])+convToint(a2[i])));
    }
    return res;
}


std::vector<int64_t> ArevelTo(Clients& clients,const Value& a,size_t To)
{
    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();
    std::vector<int64_t> res(a.size()/2);
    if(Pid==To)
    {
        auto a3=clients.recv((Pid-1+Party_number)%Party_number);
        for(int i=0;i<a.size();i+=2) res[i]=(a[i]+a[i+1]+a3[i]);
        return res;
    }
    else if(Pid==(To-1+Party_number)%Party_number) clients.SendTo(a,To);
    return res;
}

std::vector<__int128> ArevelTo128(Clients& clients,const Value128& a,size_t To)
{
    size_t Pid=clients.getPid();
    size_t Party_number=clients.getPartyNumber();
    std::vector<__int128> res(a.size()/2);
    if(Pid==To)
    {
        auto a3=clients.recv128((Pid-1+Party_number)%Party_number);
        for(int i=0;i<a.size();i+=2) res[i]=(a[i]+a[i+1]+a3[i]);
        return res;
    }
    else if(Pid==(To-1+Party_number)%Party_number) clients.SendTo128(a,To);
    return res;
}

Value Aneg(Clients& clients,const Value& a)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> v1(a.size());
    for(int i=0;i<a.size();i++) v1[i]=(-a[i]);
    Value val1(std::move(v1));val1.set_fraction_bits(a.get_fraction_bits());val1.set_share_type(0);

    return val1;
}

template<size_t N>
std::vector<std::bitset<N>> Aneg_bitset(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> v1(a.size());
    for(int i=0;i<a.size();i++) v1[i]=(-(a[i].to_ullong()));

    return v1;
}


// Value MOD(Clients& clients,const Value& a,const Value& b)
// {
    
// }
