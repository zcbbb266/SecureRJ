#pragma once

#include "../Operator/operator.h"

Value perfix_sum(Clients& clients,const Value& a,size_t order=1)
{
    std::vector<int64_t> ps(a.size());
    if(order==1)
    {
        ps[0]=a[0],ps[1]=a[1];
        for(int i=2;i<a.size();i+=2)
        {
            ps[i]=a[i]+ps[i-2];
            ps[i+1]=a[i+1]+ps[i-1];
        }
    }
    else{
        ps[ps.size()-2]=a[ps.size()-2],ps[ps.size()-1]=a[ps.size()-1];
        for(int i=a.size()-3;i>=0;i-=2)
        {
            ps[i]=a[i]+ps[i+2];
            ps[i-1]=a[i-1]+ps[i+1];
        }
    }
    Value res(std::move(ps));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
    return res;
}

Value32 perfix_sum_32(Clients& clients,const Value32& a)
{
    std::vector<int32_t> ps(a.size());
    ps[0]=a[0],ps[1]=a[1];
    for(int i=2;i<a.size();i+=2)
    {
        ps[i]=a[i]+ps[i-2];
        ps[i+1]=a[i+1]+ps[i-1];
    }
    Value32 res(std::move(ps));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
    return res;
}
template<size_t N>
std::vector<std::bitset<N>> perfix_sum_bitset(Clients& clients,const std::vector<std::bitset<N>>& a,size_t order=1)
{
    std::vector<std::bitset<N>> ps(a.size());
   
    if(order==1)
    {
        ps[0]=a[0],ps[1]=a[1];
        for(int i=2;i<a.size();i+=2)
        {
            ps[i]=convToint(a[i])+convToint(ps[i-2]);
            ps[i+1]=convToint(a[i+1])+convToint(ps[i-1]);
        }
    }
    else{
        ps[ps.size()-2]=a[ps.size()-2],ps[ps.size()-1]=a[ps.size()-1];
        for(int i=a.size()-3;i>=0;i-=2)
        {
            ps[i]=convToint(a[i])+convToint(ps[i+2]);
            ps[i-1]=convToint(a[i-1])+convToint(ps[i+1]);
        }
    }
    return ps;
}



