#pragma once


#include<vector>
#include<utility>

#include "../Value/value.h"
#include "../network/clients.hpp"
#include "../broker/broker.hpp"

#include "Boolen.hpp"
#include "Arithmetic.hpp"
#include "Tool.hpp"

Value A2B(Clients& clients,const Value& a)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> a1(a.size()/2),a2(a.size());
    
    Value A1;
    if(Pid==0)
    {
        for(int i=0;i<a.size();i+=2)
        {
            a1[i/2]=a[i]+a[i+1];
        }
        Value A(std::move(a1));
        A1=BmakeShare(clients,0,A);
    } 
    else if(Pid==1) A1=BmakeShare(clients,0);
    else A1=BmakeShare(clients,0);

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
    Value A2(std::move(a2));A2.set_fraction_bits(a.get_fraction_bits());A2.set_share_type(1);
    auto res=std::move(ppa_kogge_stone(clients,A1,A2));
    return res;
}



Value B2A(Clients& clients,const Value& a)
{
    size_t Pid=clients.getPid();
    if(Pid==0)
    {
        auto rb=clients.recv(1);
        auto RRR=BmakeShare(clients,1);
        auto temp=ppa_kogge_stone(clients,a,RRR);
        auto ra=BrevelTo(clients,temp,0);
        BrevelTo(clients,temp,2);
        std::vector<int64_t> res(a.size());
        for(int i=0;i<a.size();i+=2)
        {
            res[i]=ra[i/2],res[i+1]=rb[i/2];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(0);
        return Res;
    }
    else if(Pid==1)
    {
        auto r1=gen_rand_vector(a.size()/2);
        auto r2=gen_rand_vector(a.size()/2);
        Value ra(std::move(r1));ra.set_fraction_bits(a.get_fraction_bits());ra.set_share_type(0);
        Value rb(std::move(r2));rb.set_fraction_bits(a.get_fraction_bits());rb.set_share_type(0);
        clients.SendTo(ra,0);
        clients.SendTo(rb,2);
        std::vector<int64_t> R(a.size()/2,0);
        for(int i=0;i<a.size()/2;i++) R[i]=-ra[i]-rb[i];
        Value RR(std::move(R));RR.set_fraction_bits(a.get_fraction_bits());
        auto RRR=BmakeShare(clients,1,RR);
        auto temp=ppa_kogge_stone(clients,a,RRR);
        BrevelTo(clients,temp,0);
        BrevelTo(clients,temp,2);
        std::vector<int64_t> res(a.size());
        for(int i=0;i<a.size();i+=2)
        {
            res[i]=ra[i/2],res[i+1]=rb[i/2];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(0);
        return Res;
    }
    else
    {
        auto ra=clients.recv(1);
        auto RRR=BmakeShare(clients,1);
        auto temp=ppa_kogge_stone(clients,a,RRR);
        BrevelTo(clients,temp,0);
        auto rb=BrevelTo(clients,temp,2);
        std::vector<int64_t> res(a.size());
        for(int i=0;i<a.size();i+=2)
        {
            res[i]=ra[i/2],res[i+1]=rb[i/2];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(0);
        return Res;
    }
    // auto r1=gen_rand_vector(a.size()/2);
    // auto r2=gen_rand_vector(a.size()/2);
    // Value ra(std::move(r1));ra.set_fraction_bits(a.get_fraction_bits());
    // Value rb(std::move(r2));ra.set_fraction_bits(a.get_fraction_bits());
    // Value r1,r2,r3;
    // if(Pid==0)
    // {
    //     r1=std::move(BmakeShare(clients,0,ra));
    //     r2=std::move(BmakeShare(clients,1));
    //     r3=std::move(BmakeShare(clients,2));
    // }
    // else if(Pid==1)
    // {
    //     r1=std::move(BmakeShare(clients,0));
    //     r2=std::move(BmakeShare(clients,1,ra));
    //     r3=std::move(BmakeShare(clients,2));
    // }
    // else
    // {
    //     r1=std::move(BmakeShare(clients,0));
    //     r2=std::move(BmakeShare(clients,1));
    //     r3=std::move(BmakeShare(clients,2,ra));
    // }
    // auto x_add_r=ppa_kogge_stone(clients,ppa_kogge_stone(clients,r1,r2),ppa_kogge_stone(clients,r3,a));

    // auto x_r=Breconstruct(clients,x_add_r);

    // for(int i=0;i<ra.size();i++) ra[i]=-ra[i];

    // Value R=std::move(ra);
    // Value XR(std::move(x_r));R.set_fraction_bits(a.get_fraction_bits());XR.set_share_type(0);

    // if(Pid==0)
    // {
    //     for(int i=0;i<XR.size();i++) XR[i]+=R[i];
    // }

    // if(Pid==2)
    // {
    //     clients.SendTo(R,0);
    //     for(int i=0;i<R.size();i++) R[i]=0;
    // }
    
    // if(Pid==0)
    // {
    //     auto R2=clients.recv(2);
    //     for(int i=0;i<XR.size();i++) XR[i]+=R2[i];
    //     R=std::move(XR);
    // }

    // auto R1=clients.SendRoud(R);

    // std::vector<int64_t> res(a.size());

    // for(int i=0;i<R1.size();i++)
    // {
    //     res[2*i]=R1[i],res[2*i+1]=R[i];
    // }
    // Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(0);

    // return Res;
}



Value trunc(Clients& clients,const Value& a,size_t bit)
{
    size_t Pid=clients.getPid();

    auto rr=gen_rand_pair(clients,a.size()/2,a.get_fraction_bits());
    std::vector<int64_t> rmid;
    for(int i=0;i<rr.size();i++) rmid.push_back(rr[i]>>bit);

    Value r(std::move(rmid));r.set_fraction_bits(a.get_fraction_bits()-bit);

    auto rr2=gen_rand_pair(clients,a.size()/2,a.get_fraction_bits());
    auto rr3=gen_rand_pair(clients,a.size()/2,a.get_fraction_bits());
    auto r2=gen_rand_pair(clients,a.size()/2,a.get_fraction_bits()-bit);
    auto r3=gen_rand_pair(clients,a.size()/2,a.get_fraction_bits()-bit);

    auto rr1=ppa_kogge_stone_sub(clients,rr,ppa_kogge_stone(clients,rr2,rr3));
    auto r1=ppa_kogge_stone_sub(clients,r,ppa_kogge_stone(clients,r2,r3));

    std::vector<int64_t> R1,RR1,R2,RR2;
    if(Pid==0)
    {
        R1=BrevelTo(clients,r1,0);
        BrevelTo(clients,r1,2);
        RR1=BrevelTo(clients,rr1,0);
        BrevelTo(clients,rr1,2);
        R2=BrevelTo(clients,r2,0);
        BrevelTo(clients,r2,1);
        RR2=BrevelTo(clients,rr2,0);
        BrevelTo(clients,rr2,1);
        BrevelTo(clients,r3,1);
        BrevelTo(clients,r3,2);
        BrevelTo(clients,rr3,1);
        BrevelTo(clients,rr3,2);
    } 

    else if(Pid==1)
    {
        BrevelTo(clients,r1,0);
        BrevelTo(clients,r1,2);
        BrevelTo(clients,rr1,0);
        BrevelTo(clients,rr1,2);
        BrevelTo(clients,r2,0);
        R1=BrevelTo(clients,r2,1);
        BrevelTo(clients,rr2,0);
        RR1=BrevelTo(clients,rr2,1);
        R2=BrevelTo(clients,r3,1);
        BrevelTo(clients,r3,2);
        RR2=BrevelTo(clients,rr3,1);
        BrevelTo(clients,rr3,2);
    }

    else
    {
        BrevelTo(clients,r1,0);
        R2=BrevelTo(clients,r1,2);
        BrevelTo(clients,rr1,0);
        RR2=BrevelTo(clients,rr1,2);
        BrevelTo(clients,r2,0);
        BrevelTo(clients,r2,1);
        BrevelTo(clients,rr2,0);
        BrevelTo(clients,rr2,1);
        BrevelTo(clients,r3,1);
        R1=BrevelTo(clients,r3,2);
        BrevelTo(clients,rr3,1);
        RR1=BrevelTo(clients,rr3,2);
    }

    std::vector<int64_t> R,RR;
    for(int i=0;i<a.size()/2;i++)
    {
        R.push_back(R1[i]);
        R.push_back(R2[i]);
        RR.push_back(RR1[i]);
        RR.push_back(RR2[i]);
    }
    Value vR(std::move(R));vR.set_fraction_bits(a.get_fraction_bits()-bit);vR.set_share_type(0);
    Value vRR(std::move(RR));vRR.set_fraction_bits(a.get_fraction_bits());vRR.set_share_type(0);

    auto res=Add_A(clients,a,Aneg(clients,vRR));

    auto a_r=Areconstruct(clients,res);

    for(int i=0;i<a_r.size();i++) a_r[i]=a_r[i]>>bit;
    if(Pid==0)
    {
        for(int i=0;i<vR.size()/2;i++) vR[2*i]+=a_r[i];
    }
    if(Pid==2)
    {
        for(int i=0;i<vR.size()/2;i++) vR[2*i+1]+=a_r[i];
    }

    Value Res=std::move(vR);

    return Res;
}

Value trunc_fast(Clients& clients,const Value& a,size_t bit)
{
    Value a_copy;
   if(a.get_share_type()==0) a_copy=A2B(clients,a);
   else a_copy=a;

   for(int i=0;i<a_copy.size();i++) a_copy[i]>>=bit;
   a_copy.set_fraction_bits(a.get_fraction_bits()-bit);
   return B2A(clients,a_copy);
}

// Value s2tos3(Clients& clients,int toPid,const Value& a){
//     auto aa=a;
//     size_t Pid=clients.getPid();
//     bool is_as=a.get_share_type();
//     if(Pid==(toPid-1+3)%3)
//     {
//         clients.SendTo(a,toPid);
//         auto val2=clients.recv((Pid-1+3)%3);
//         std::vector<int64_t> res(2*val2.size());
//         for(int i=0;i<a.size();i++) {
//             res[2*i]=val2[i];res[2*i+1]=a[i];
//         }
//         Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(a.get_share_type());
//         return Res;
//     }
//     else if(Pid==toPid){

//         auto seeds=generate_int64_numbers(2);
//         clients.SendTo(seeds,(Pid+1)%3);

//         auto val1=clients.recv((toPid-1+3)%3);

//         auto val=gen_rand_vector_with_key(val1.size(),seeds[0],seeds[1]);

//         Value val2(std::move(val));val2.set_fraction_bits(val1.get_fraction_bits());val2.set_share_type(val1.get_share_type());

//         std::vector<int64_t> res(2*val2.size());
//         for(int i=0;i<val1.size();i++) {
//             res[2*i]=val1[i];res[2*i+1]=val2[i];
//         }
//         Value Res(std::move(res));Res.set_fraction_bits(val1.get_fraction_bits());Res.set_share_type(val1.get_share_type());
//         return Res;
//     }
//     else{
//         auto seeds=clients.recv(toPid);
//         auto val=gen_rand_vector_with_key(a.size(),seeds[0],seeds[1]);

//         Value val1(std::move(val));val1.set_fraction_bits(a.get_fraction_bits());val1.set_share_type(a.get_share_type());
//         if(is_as==0) 
//         {
//             for(int i=0;i<a.size();i++) aa[i]-=val1[i];
//         }
//         else{
//             for(int i=0;i<a.size();i++) aa[i]^=val1[i];
//         }
//         clients.SendTo(aa,(Pid+1)%3);
//         std::vector<int64_t> res(2*val1.size());
//         for(int i=0;i<a.size();i++) {
//             res[2*i]=val1[i];res[2*i+1]=aa[i];
//         }
//         Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(a.get_share_type());
//         return Res;
//     }
// }
Value s2tos3(Clients& clients,int toPid,const Value& a){


    size_t Pid=clients.getPid();
    if(Pid==(toPid-1+3)%3)
    {
        auto a1=a;
        auto a2=clients.SendRoud(a1);
        std::vector<int64_t> res(2*a.size());
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(a.get_share_type());
        return Res;
    }
    else if(Pid==toPid){

        auto a1=Value(std::move(std::vector<int64_t>(a.size(),0)));
        auto a2=clients.SendRoud(a1);
        std::vector<int64_t> res(a.size()*2);
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a2.get_fraction_bits());Res.set_share_type(a2.get_share_type());
        return Res;
    }
    else{

        auto a1=a;
        auto a2=clients.SendRoud(a1);
        std::vector<int64_t> res(2*a.size());
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        Value Res(std::move(res));Res.set_fraction_bits(a.get_fraction_bits());Res.set_share_type(a.get_share_type());
        return Res;
    }
}

template<size_t N>
std::vector<std::bitset<N>> s2tos3_bitset(Clients& clients,int toPid,const std::vector<std::bitset<N>>& a){


    size_t Pid=clients.getPid();
    if(Pid==(toPid-1+3)%3)
    {
        auto a1=a;
        auto a2=clients.SendRoud_bitset(a1);
        std::vector<std::bitset<N>> res(2*a.size());
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        return res;
    }
    else if(Pid==toPid){

        auto a1=std::move(std::vector<std::bitset<N>>(a.size(),0));
        auto a2=clients.SendRoud_bitset(a1);
        std::vector<std::bitset<N>> res(a.size()*2);
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        return res;
    }
    else{

        auto a1=a;
        auto a2=clients.SendRoud_bitset(a1);
        std::vector<std::bitset<N>> res(2*a.size());
        for(int i=0;i<a.size();i++) {
            res[2*i]=a2[i];res[2*i+1]=a1[i];
        }
        return res;
    }
}
std::vector<Value> s2tos3_vector(Clients& clients,int toPid,const std::vector<Value>& a){
    std::vector<Value> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        res[i]=s2tos3(clients,toPid,a[i]);
    }
    return res;
}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> s2tos3_vector_bitset(Clients& clients,int toPid,const std::vector<std::vector<std::bitset<N>>>& a){
    std::vector<std::vector<std::bitset<N>>> res(a.size());
    for(int i=0;i<a.size();i++)
    {
        res[i]=s2tos3_bitset(clients,toPid,a[i]);
    }
    return res;
}


template<size_t N>
struct pemed{
    std::vector<Value> a;
    std::vector<std::bitset<N>> b;
};

template<size_t N,size_t M>
struct pemedNM{
    std::vector<std::vector<std::bitset<M>>> a;
    std::vector<std::bitset<N>> b;
};
template<size_t N>
pemed<N> s2tos3_pemed(Clients& clients,int toPid,const pemed<N>& pem){
    auto resa=s2tos3_vector(clients,toPid,pem.a);
    auto resb=s2tos3_bitset(clients,toPid,pem.b);
    return pemed<N>{resa,resb};
}
template<size_t N,size_t M>
pemedNM<N,M> s2tos3_pemedNM(Clients& clients,int toPid,const pemedNM<N,M>& pem){
    auto resa=s2tos3_vector_bitset(clients,toPid,pem.a);
    auto resb=s2tos3_bitset(clients,toPid,pem.b);
    return pemedNM<N,M>{resa,resb};
}
template<size_t N>
std::vector<std::bitset<N>> conv2ToP(Clients& clients,const Value& db){
    size_t pid=clients.getPid();
    std::vector<std::bitset<N>> db1(db.size()),db2(db.size()),db3(db.size());
    for(int i=0;i<db.size();i+=2)
    {
        if(pid==0)
        {
            db1[i]=db[i];db1[i+1]=0;
            db2[i]=0;db2[i+1]=db[i+1];
            db3[i]=0;db3[i+1]=0;
        }
        else if(pid==1)
        {
            db1[i]=0;db1[i+1]=0;
            db2[i]=db[i];db2[i+1]=0;
            db3[i]=0;db3[i+1]=db[i+1];
        }
        else
        {
            db1[i]=0;db1[i+1]=db[i+1];
            db2[i]=0;db2[i+1]=0;
            db3[i]=db[i];db3[i+1]=0;
        }
    }

    auto Add12=Add_bitset(clients,db1,db2);
    auto Mul12=Mul_bitset(clients,db1,db2);
    for(int i=0;i<Mul12.size();i++) Mul12[i]=2*Mul12[i].to_ullong();
    auto db12=Add_bitset(clients,Add12,Aneg_bitset(clients,Mul12));
    auto Add123=Add_bitset(clients,db12,db3);
    auto Mul123=Mul_bitset(clients,db12,db3);
    for(int i=0;i<Mul123.size();i++) Mul123[i]=2*Mul123[i].to_ullong();
    return Add_bitset(clients,Add123,Aneg_bitset(clients,Mul123));
}
Value conv2ToValue(Clients& clients,const Value& db){
    size_t pid=clients.getPid();
    Value db1(std::move(std::vector<int64_t>(db.size(),0)));db1.set_share_type(0);
    Value db2(std::move(std::vector<int64_t>(db.size(),0)));db2.set_share_type(0);
    Value db3(std::move(std::vector<int64_t>(db.size(),0)));db3.set_share_type(0);
    for(int i=0;i<db.size();i+=2)
    {
        if(pid==0)
        {
            db1[i]=db[i];db1[i+1]=0;
            db2[i]=0;db2[i+1]=db[i+1];
            db3[i]=0;db3[i+1]=0;
        }
        else if(pid==1)
        {
            db1[i]=0;db1[i+1]=0;
            db2[i]=db[i];db2[i+1]=0;
            db3[i]=0;db3[i+1]=db[i+1];
        }
        else
        {
            db1[i]=0;db1[i+1]=db[i+1];
            db2[i]=0;db2[i+1]=0;
            db3[i]=db[i];db3[i+1]=0;
        }
    }

    auto Add12=Add_A(clients,db1,db2);
    auto Mul12=i_Mul_A(clients,db1,db2);
    for(int i=0;i<Mul12.size();i++) Mul12[i]=2*Mul12[i];
    auto db12=Add_A(clients,Add12,Aneg(clients,Mul12));
    auto Add123=Add_A(clients,db12,db3);
    auto Mul123=i_Mul_A(clients,db12,db3);
    for(int i=0;i<Mul123.size();i++) Mul123[i]=2*Mul123[i];
    return Add_A(clients,Add123,Aneg(clients,Mul123));
}

template<size_t N>
std::vector<std::bitset<N>> conv2ToP(Clients& clients,const std::vector<std::bitset<8>>& db){
    size_t pid=clients.getPid();
    std::vector<std::bitset<N>> db1(db.size()),db2(db.size()),db3(db.size());
    for(int i=0;i<db.size();i+=2)
    {
        if(pid==0)
        {
            db1[i]=db[i].to_ullong();db1[i+1]=0;
            db2[i]=0;db2[i+1]=db[i+1].to_ullong();
            db3[i]=0;db3[i+1]=0;
        }
        else if(pid==1)
        {
            db1[i]=0;db1[i+1]=0;
            db2[i]=db[i].to_ullong();db2[i+1]=0;
            db3[i]=0;db3[i+1]=db[i+1].to_ullong();
        }
        else
        {
            db1[i]=0;db1[i+1]=db[i+1].to_ullong();
            db2[i]=0;db2[i+1]=0;
            db3[i]=db[i].to_ullong();db3[i+1]=0;
        }
    }

    auto Add12=Add_bitset(clients,db1,db2);
    auto Mul12=Mul_bitset(clients,db1,db2);
    for(int i=0;i<Mul12.size();i++) Mul12[i]=2*Mul12[i].to_ullong();
    auto db12=Add_bitset(clients,Add12,Aneg_bitset(clients,Mul12));
    auto Add123=Add_bitset(clients,db12,db3);
    auto Mul123=Mul_bitset(clients,db12,db3);
    for(int i=0;i<Mul123.size();i++) Mul123[i]=2*Mul123[i].to_ullong();
    return Add_bitset(clients,Add123,Aneg_bitset(clients,Mul123));
}
