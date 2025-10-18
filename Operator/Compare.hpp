#pragma once

#include <cstdint>

#include "Boolen.hpp"
#include "Arithmetic.hpp"
#include "Conversion.hpp"
#include "Fxp.hpp"


Value Amsb(Clients &clients,const Value &a)
{
    auto Ca=A2B(clients,a);
    std::vector<int64_t> msb(a.size());
    for(int i=0;i<Ca.size();i++)
    {
        msb[i]=((Ca[i]>>63)&1);
    }

    Value res(std::move(msb));res.set_fraction_bits(0);res.set_share_type(1);
    return res;
}


Value Bmsb(Clients &clients,const Value &a)
{
    std::vector<int64_t> msb(a.size());
    for(int i=0;i<a.size();i++)
    {
        msb[i]=((a[i]>>63)&1);
    }

    Value res(std::move(msb));res.set_fraction_bits(0);res.set_share_type(1);
    return res;
}

Value Aeq(Clients &clients,const Value &a,const Value &b)
{
    auto c=Add_A(clients,a,Aneg(clients,b));
    auto Neg=Amsb(clients,c);
    auto Pos=Amsb(clients,Aneg(clients,c));
    std::vector<int64_t> one(a.size());
    for(int i=0;i<a.size();i++) one[i]=1;
    Value One(std::move(one));One.set_fraction_bits(0);One.set_share_type(1);
    auto res=Xor(clients,Xor(clients,Neg,Pos),One);
    return res;
}

Value Beq(Clients &clients,const Value &a,const Value &b)
{
    // auto c=ppa_kogge_stone(clients,a,Bneg(clients,b));
    // auto Neg=Bmsb(clients,c);
    // auto Pos=Bmsb(clients,Bneg(clients,c));
    // std::vector<int64_t> one(a.size());
    // for(int i=0;i<a.size();i++) one[i]=1;
    // Value One(std::move(one));One.set_fraction_bits(0);One.set_share_type(1);
    // auto res=Xor(clients,Xor(clients,Neg,Pos),One);
    auto c=Xor(clients,a,b);
    for(int i=0;i<c.size();i++) c[i]=~c[i];

    std::vector<int32_t> low32(a.size()),high32(a.size());
    for(int i=0;i<a.size();i++) low32[i]=c[i],high32[i]=(c[i]>>32);
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


    for(int i=0;i<c.size();i++) c[i]=Low8[i]&1;
    return c;
}


Value ANeq(Clients &clients,const Value &a,const Value &b)
{
    auto c=Add_A(clients,a,Aneg(clients,b));
    auto Neg=Amsb(clients,c);
    auto Pos=Amsb(clients,Aneg(clients,c));
    auto res=Xor(clients,Neg,Pos);
    return res;
}

Value BNeq(Clients &clients,const Value &a,const Value &b)
{
    auto res=Beq(clients,a,b);
    for(int i=0;i<res.size();i++) res[i]^=1;
    return res;
}

Value AGeq(Clients &clients,const Value &a,const Value &b)
{
    auto c=Add_A(clients,a,Aneg(clients,b));
    std::vector<int64_t> one(a.size());
    for(int i=0;i<a.size();i++) one[i]=1;
    Value One(std::move(one));One.set_fraction_bits(0);One.set_share_type(1);
    return Xor(clients,One,Amsb(clients,c));
}

Value BGeq(Clients &clients,const Value &a,const Value &b)
{
    auto c=ppa_kogge_stone_sub(clients,a,b);
    std::vector<int64_t> one(a.size());
    for(int i=0;i<a.size();i++) one[i]=1;
    Value One(std::move(one));One.set_fraction_bits(0);One.set_share_type(1);
    return Xor(clients,One,Bmsb(clients,c));
}

Value ALess(Clients &clients,const Value &a,const Value &b)
{
    auto c=Add_A(clients,a,Aneg(clients,b));
    return Amsb(clients,c);
}

Value BLess(Clients &clients,const Value &a,const Value &b)
{
    auto c=ppa_kogge_stone_sub(clients,a,b);
    return Bmsb(clients,c);
}

Value AGre(Clients &clients,const Value &a,const Value &b)
{
    return ALess(clients,b,a);
}

Value BGre(Clients &clients,const Value &a,const Value &b)
{
    return BLess(clients,b,a);
}
Value ALeq(Clients &clients,const Value &a,const Value &b)
{
    auto ones=make_constB(1,a.size()/2);
    return Xor(clients,ones,AGre(clients,a,b));
}

Value BLeq(Clients &clients,const Value &a,const Value &b)
{
    auto ones=make_constB(1,a.size()/2);
    return Xor(clients,ones,BGre(clients,b,a));
}


Value msb(Clients& clients,const Value& a)
{
    if(a.get_share_type()==0) return Amsb(clients,a);
    return Bmsb(clients,a);
}

Value Geq(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0) return AGeq(clients,a,b);
    return BGeq(clients,a,b);
}
Value Gre(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0) return AGre(clients,a,b);
    return BGre(clients,a,b);
}

Value Less(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0) return ALess(clients,a,b);
    return BLess(clients,a,b);
}



Value Leq(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0)  return ALeq(clients,a,b);
    return BLeq(clients,a,b);
}

Value eq(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0) return Aeq(clients,a,b);
    return Beq(clients,a,b);
}


Value Neq(Clients &clients,const Value &a,const Value &b)
{
    if(a.get_share_type()==0) return ANeq(clients,a,b);
    return BNeq(clients,a,b);
}


void compare_conv(Clients &clients,Value &a)
{
    for(int i=0;i<a.size();i++)
    {
        if(a[i]==1) a[i]=-1;
    }
}
Value compare_conv_return(Clients &clients,const Value &a)
{
    Value c=a;
    for(int i=0;i<a.size();i++)
    {
        if(a[i]==0) c[i]=-1;
        else c[i]=0;
    }
    return c;
}
