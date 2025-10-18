#pragma once

#include "Boolen.hpp"
#include "Arithmetic.hpp"
#include "Conversion.hpp"
#include "Compare.hpp"
#include "Fxp.hpp"
#include "../ThreePartyOT/Ot.hpp"

std::vector<int8_t> reconstruct8(Clients& clients,const Value8& a)
{
    if(a.get_share_type()==0) return Areconstruct8(clients,a);
    return Breconstruct8(clients,a);
}

std::vector<int64_t> reconstruct(Clients& clients,const Value& a)
{
    if(a.get_share_type()==0) return Areconstruct(clients,a);
    return Breconstruct(clients,a);
}


std::vector<__int128> reconstruct128(Clients& clients,const Value128& a)
{
    if(a.get_share_type()==0) return Areconstruct128(clients,a);
    return Breconstruct128(clients,a);
}

std::vector<__int128> revelTo128(Clients& clients,const Value128& a,size_t to)
{
    if(a.get_share_type()==0) return ArevelTo128(clients,a,to);
    return BrevelTo128(clients,a,to);
}

Value neg(Clients& clients,const Value& a)
{
    if(a.get_share_type()==0) return Aneg(clients,a);
    return Bneg(clients,a);
}

Value i_Mul(Clients& clients,const Value& a,const Value& b){
    if(a.get_share_type()==1&&b.get_share_type()==0) return i_Mul_A(clients,B2A(clients,a),b);
    if(a.get_share_type()==1&&b.get_share_type()==1) return i_Mul_A(clients,B2A(clients,a),B2A(clients,b));
    if(a.get_share_type()==0&&b.get_share_type()==1) return i_Mul_A(clients,a,B2A(clients,b));
    return i_Mul_A(clients,a,b);
}


Value Add(Clients& clients,const Value& a,const Value& b){
    if(a.get_share_type()==1&&b.get_share_type()==0) return Add_A(clients,B2A(clients,a),b);
    if(a.get_share_type()==1&&b.get_share_type()==1) return Add_A(clients,B2A(clients,a),B2A(clients,b));
    if(a.get_share_type()==0&&b.get_share_type()==1) return Add_A(clients,a,B2A(clients,b));
    return Add_A(clients,a,b);
}

Value Mul(Clients& clients,const Value& a,const Value& b){
    if(a.get_fraction_bits()!=0||b.get_fraction_bits()!=0) return f_Mul(clients,a,b);
    else return i_Mul(clients,a,b);
}

Value Not(Clients& clients,const Value& a)
{
    if(a.get_share_type()==0) return Add(clients,make_constI(clients,1,a.size()/2),neg(clients,a));
    else
    {
        auto a_copy=a;
        for(int i=0;i<a.size();i++) a_copy[i]^=1;
        return a_copy;
    }
}
//a is shared
Value Mul_ab2(Clients& clients,const Value& a,const Value& b)
{
    size_t Pid=clients.getPid();
    if(Pid==0)
    {
        std::vector<int64_t> v1(a.size()/2);
        for(int i=0;i<a.size()/2;i++) v1[i]=a[2*i];
        Value V1(std::move(v1));V1.set_fraction_bits(a.get_fraction_bits());
        auto w1=Mul_ab1(clients,V1,b,0);
        auto w2=Mul_ab1(clients,Value(),b,1);
        return Add(clients,w1,w2);
    } 
    else if(Pid==1)
    {
        std::vector<int64_t> v1(a.size()/2);
        for(int i=0;i<a.size()/2;i++) v1[i]=a[2*i]+a[2*i+1];
        Value V1(std::move(v1));V1.set_fraction_bits(a.get_fraction_bits());
        auto w1=Mul_ab1(clients,Value(),b,0);
        auto w2=Mul_ab1(clients,V1,b,1);
        return Add(clients,w1,w2);
    }
    else{
        auto w1=Mul_ab1(clients,Value(),b,0);
        auto w2=Mul_ab1(clients,Value(),b,1);
        return Add(clients,w1,w2);
    }   
}

Value revelTo(Clients& clients,const Value& a,size_t To)
{
    if(a.get_share_type()==0) return ArevelTo(clients,a,To);
    else return BrevelTo(clients,a,To);
}

std::vector<Value> bitdec(Clients& clients,const Value& a)
{
    int sp=a.get_share_type();
    if(sp==0)
    {
       throw std::runtime_error("you need to first convert to Boolenshare");
    }
    else
    {
        std::vector<int64_t> im(a.size()*128);
        for(int i=0;i<a.size();i+=2)
        {
            for(int j=0;j<64;j++) 
            {
                im[64*i+2*j]=(a[i]>>j)&1;
                im[64*i+2*j+1]=(a[i+1]>>j)&1;
            }
        }   
        Value Im(std::move(im));Im.set_fraction_bits(0);Im.set_share_type(1);
        std::vector<Value> res;
        for(int i=0;i<a.size()/2;i++)
        {
            std::vector<int64_t> one(128);
            for(int j=0;j<128;j++) one[j]=Im[i*128+j];
            Value One(std::move(one));One.set_fraction_bits(0);One.set_share_type(1);
            res.push_back(One);
        }
        return res;
    }
}




Value Gather(Clients& clients,const Value& a,const std::vector<size_t> pos)
{
    std::vector<int64_t> res(2*pos.size());
    for(int i=0;i<pos.size();i++)
    {
        res[2*i]=a[2*pos[i]];res[2*i+1]=a[2*pos[i]+1];
    }
    Value R(std::move(res));R.set_fraction_bits(a.get_fraction_bits());R.set_share_type(a.get_share_type());
    return R;
}
template<size_t N>
std::vector<std::bitset<N>> Gather(Clients& clients,const std::vector<std::bitset<N>>& a,const std::vector<size_t> pos)
{
    std::vector<std::bitset<N>> res(2*pos.size());
    for(int i=0;i<pos.size();i++)
    {
        res[2*i]=a[2*pos[i]];res[2*i+1]=a[2*pos[i]+1];
    }
    return res;
}

std::vector<Value> Gather_vector(Clients& clients,const std::vector<Value>& a,const std::vector<size_t> pos)
{
    std::vector<Value> res(a.size());
    for(int j=0;j<a.size();j++)
    {
        std::vector<int64_t> temp(2*pos.size());
        for(int i=0;i<pos.size();i++)
        {
            temp[2*i]=a[j][2*pos[i]];temp[2*i+1]=a[j][2*pos[i]+1];
        }
        Value R(std::move(temp));R.set_fraction_bits(a[j].get_fraction_bits());R.set_share_type(a[j].get_share_type());
        res[j]=R;
    }
    return res;
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> Gather_vector(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<size_t> pos)
{
    std::vector<std::vector<std::bitset<N>>> res(a.size());
    for(int j=0;j<a.size();j++)
    {
        std::vector<std::bitset<N>> temp(2*pos.size());
        for(int i=0;i<pos.size();i++)
        {
            temp[2*i]=a[j][2*pos[i]];temp[2*i+1]=a[j][2*pos[i]+1];
        }
        res[j]=temp;
    }
    return res;
}
void Scatter(Clients& clients,Value& a,const Value &b,const std::vector<size_t> pos)
{
    for(int i=0;i<pos.size();i++)
    {
        a[2*pos[i]]=b[2*i];
        a[2*pos[i]+1]=b[2*i+1];
    }
}

template<size_t N>
void Scatter(Clients& clients,std::vector<std::bitset<N>>& a,const std::vector<std::bitset<N>> &b,const std::vector<size_t> pos)
{
    for(int i=0;i<pos.size();i++)
    {
        a[2*pos[i]]=b[2*i];
        a[2*pos[i]+1]=b[2*i+1];
    }
}
// Value highestOneBit(SPUContext* ctx, const Value& x) {
//   auto y = _prefix_or(ctx, x);
//   auto y1 = _rshift(ctx, y, 1);
//   return _xor(ctx, y, y1);
// }

Value conditional(Clients& clients,const Value& cond,const Value& a,const Value& b)
{
    // return Add(clients,a,Mul_ab2(clients,Add(clients,b,Aneg(clients,a)),cond));
    auto cond1=cond;
    auto cond2=cond;
    for(int i=0;i<cond2.size();i++) cond2[i]^=1;
    compare_conv(clients,cond1);
    compare_conv(clients,cond2);
    return Xor(clients,And(clients,cond1,b),And(clients,cond2,a));

}

void print_Value(Clients& clients,const Value& a)
{
    auto res=reconstruct(clients,a);
    if(clients.getPid()==0)
    {
        for(int i=0;i<res.size();i++) std::cout<<res[i]<<" ";
        std::cout<<'\n';
    }
}

template<size_t N>
void print_bitset(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    auto res=Areconstruct_bitset(clients,a);
    if(clients.getPid()==0)
    {
        for(int i=0;i<res.size();i++) std::cout<<res[i]<<" ";
        std::cout<<'\n';
    }
}

template<size_t N>
void print_bitset_Boolen(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    auto res=reconstruct_bitset(clients,a);
    if(clients.getPid()==0)
    {
        if(N>64)
            for(int i=0;i<res.size();i++) std::cout<<res[i]<<" ";
        else
            for(int i=0;i<res.size();i++) std::cout<<revelToint(res[i])<<" ";
        std::cout<<'\n';
    }
}
Value revbit(Clients& clients,const Value& a)
{
    if(a.get_share_type()==0)
        return Add(clients,make_constI(clients,1,a.size()/2),neg(clients,a));
    else
        return Xor(clients,make_constB(1,a.size()/2),a);
}