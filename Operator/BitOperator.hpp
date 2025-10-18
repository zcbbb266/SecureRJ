#pragma once


#include "Boolen.hpp"
#include "Conversion.hpp"
#include "Compare.hpp"

Value higest_bit(Clients& clients,const Value& a)
{
    Value b;
    if(a.get_share_type()==0)
    {
        b=std::move(A2B(clients,a));
    }
    else b=a;
    
    for(int i=0;i<64;i++)
    {
        auto b1=Rshift(clients,b,1);
        b=std::move(Or(clients,b,b1));
    }
    auto b1=Rshift(clients,b,1);
    auto bb1= Xor(clients,b,b1);
    size_t fb=bb1.get_fraction_bits();

    for(int i=0;i<bb1.size();i++) bb1[i]=bb1[i]>>fb;
    bb1.set_fraction_bits(0);
    return bb1;
}


Value sign(Clients& clients,const Value& a)
{
    auto Msb=msb(clients,a);
    auto MsbA=B2A(clients,Msb);
    auto c_1=make_constI(clients,1,a.size()/2);
    auto c_2=make_constI(clients,2,a.size()/2);
    return Add_A(clients,c_1,Aneg(clients,i_Mul_A(clients,c_2,MsbA)));

}