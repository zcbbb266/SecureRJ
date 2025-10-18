#pragma once

#include "Arithmetic.hpp"
#include "Boolen.hpp"
#include "Conversion.hpp"
#include "Compare.hpp"
#include "BitOperator.hpp"

Value f_Mul_A(Clients& clients,const Value& a,const Value& b,size_t bit=-1)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size");
    size_t fp=a.get_fraction_bits()+b.get_fraction_bits();

    std::vector<int64_t> c(a.size()/2);
    for(int i=0;i<a.size();i+=2)
    {
        c[i/2]=((a[i]*b[i]+a[i]*b[i+1]+a[i+1]*b[i]));
    }

    Value c1(std::move(c));c1.set_fraction_bits(fp);c1.set_share_type(0);

    auto c2=clients.SendRoud(c1);
    std::vector<int64_t> res(a.size());

    for(int i=0;i<c1.size();i++)
        res[2*i]=c2[i],res[2*i+1]=c1[i];
    Value val(std::move(res));val.set_fraction_bits(fp);val.set_share_type(0);

    Value Res;
    if(bit==-1) Res=std::move(trunc_fast(clients,val,fp-clients.get_fxp_bit()));
    else {
        Res=std::move(trunc_fast(clients,val,fp-bit));
    }


    return Res;
}

Value f_Mul(Clients& clients,const Value& a,const Value& b,size_t bit=-1){
    if(a.get_share_type()==1&&b.get_share_type()==0) return f_Mul_A(clients,B2A(clients,a),b,bit);
    if(a.get_share_type()==1&&b.get_share_type()==1) return f_Mul_A(clients,B2A(clients,a),B2A(clients,b),bit);
    if(a.get_share_type()==0&&b.get_share_type()==1) return f_Mul_A(clients,a,B2A(clients,b),bit);
    return f_Mul_A(clients,a,b,bit);
}


Value f_Mul4Div(Clients& clients,const Value& A,const Value& B,size_t bit=-1)
{
    if(A.size()!=B.size())
        throw std::invalid_argument("a.size must equal b.size");
    size_t fp=A.get_fraction_bits()+B.get_fraction_bits();
    auto a=A2B(clients,A);
    auto b=A2B(clients,B);
    std::vector<int64_t> aI(a.size()),bI(a.size()),aF(a.size()),bF(a.size());
    for(int i=0;i<a.size();i++)
    {
        aI[i]=a[i]>>(a.get_fraction_bits());
        bI[i]=b[i]>>(b.get_fraction_bits());
        aF[i]=a[i]&((1<<a.get_fraction_bits())-1);
        bF[i]=b[i]&((1<<b.get_fraction_bits())-1);

    }
    Value AI(std::move(aI));AI.set_fraction_bits(0);AI.set_share_type(1);
    Value BI(std::move(bI));BI.set_fraction_bits(0);BI.set_share_type(1);
    Value AF(std::move(aF));AF.set_fraction_bits(0);AF.set_share_type(1);
    Value BF(std::move(bF));BF.set_fraction_bits(0);BF.set_share_type(1);

    AI=std::move(B2A(clients,AI));
    BI=std::move(B2A(clients,BI));
    AF=std::move(B2A(clients,AF));
    BF=std::move(B2A(clients,BF));

    std::vector<int64_t> cII(a.size()/2),cFF(a.size()/2),cFI(a.size()/2),cIF(a.size()/2);

    for(int i=0;i<a.size();i+=2)
    {
        cII[i/2]=(AI[i]*BI[i]+AI[i]*BI[i+1]+AI[i+1]*BI[i]);
        cFF[i/2]=(AF[i]*BF[i]+AF[i]*BF[i+1]+AF[i+1]*BF[i]);
        cFI[i/2]=(AF[i]*BI[i]+AF[i]*BI[i+1]+AF[i+1]*BI[i]);
        cIF[i/2]=(AI[i]*BF[i]+AI[i]*BF[i+1]+AI[i+1]*BF[i]);
    }

    Value CIF1(std::move(cIF));CIF1.set_fraction_bits(b.get_fraction_bits());CIF1.set_share_type(0);
    Value CFI1(std::move(cFI));CFI1.set_fraction_bits(a.get_fraction_bits());CFI1.set_share_type(0);
    Value CFF1(std::move(cFF));CFF1.set_fraction_bits(fp);CFF1.set_share_type(0);
    Value CII1(std::move(cII));CII1.set_share_type(0);

    auto CII2=clients.SendRoud(CII1);
    auto CFF2=clients.SendRoud(CFF1);
    auto CIF2=clients.SendRoud(CIF1);
    auto CFI2=clients.SendRoud(CFI1);

    std::vector<int64_t> cff(a.size()),cii(a.size()),cfi(a.size()),cif(a.size());

    for(int i=0;i<CII1.size();i++)
    {
        cii[2*i]=CII2[i],cii[2*i+1]=CII1[i];
        cff[2*i]=CFF2[i],cff[2*i+1]=CFF1[i];
        cif[2*i]=CIF2[i],cif[2*i+1]=CIF1[i];
        cfi[2*i]=CFI2[i],cfi[2*i+1]=CFI1[i];
    }
    Value val0(std::move(cii));val0.set_share_type(0);
    Value val1(std::move(cff));val1.set_fraction_bits(fp);val1.set_share_type(0);
    Value val2(std::move(cfi));val2.set_fraction_bits(a.get_fraction_bits());val2.set_share_type(0);
    Value val3(std::move(cif));val3.set_fraction_bits(b.get_fraction_bits());val3.set_share_type(0);

    if(bit==-1) bit=clients.get_fxp_bit();

    auto val4=trunc(clients,val1,fp-bit);
    auto val5=trunc(clients,val2,a.get_fraction_bits()-bit);
    auto val6=trunc(clients,val3,b.get_fraction_bits()-bit);
    auto val7=A2B(clients,val0);
    for(int i=0;i<val7.size();i++) val7[i]=val7[i]<<bit;
    val7=std::move(B2A(clients,val7));
    std::vector<int64_t> OI(val7.size());
    for(int i=0;i<val7.size();i++) OI[i]=val7[i]+val6[i]+val5[i]+val4[i];
    Value res(std::move(OI));res.set_fraction_bits(bit);res.set_share_type(0);
    return res;

}
int64_t reverse_bits(int64_t n) {
    int64_t result = 0;
    for (int i = 0; i < 64; i++) {
        result <<= 1;
        result |= (n & 1);
        n >>= 1;
    }
    return result;
}

Value div_factor(Clients& clients, Value const& a)
{

    auto hb=higest_bit(clients,a);

    for(int i=0;i<hb.size();i++)
    {
        hb[i]=hb[i]<<32;
        hb[i] = reverse_bits(hb[i]);
        hb[i]=hb[i]>>1;
    }
    hb.set_fraction_bits(31);
    return B2A(clients,hb);
}


Value reciprocal_newton_norm(Clients& clients, Value const& a)
{
    size_t iteration_time=30;
    auto Kf_2=make_constF(clients,2.0,a.size()/2,25);
    auto kf_2_9142=make_constF(clients,2.9142,a.size()/2,25);
    auto init_guess=Add_A(clients,kf_2_9142,Aneg(clients,f_Mul(clients,a,Kf_2,25)));

    auto x=std::move(init_guess);
    for(int i=0;i<iteration_time;i++)
    {
        x=std::move(f_Mul(clients,x,Add_A(clients,Kf_2,Aneg(clients,f_Mul(clients,a,x,25))),30));
    } 
    x=std::move(trunc_fast(clients,x,5));
    return x;
}



Value div_A(Clients& clients,const Value& a,const Value& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size");
    size_t fp=a.get_fraction_bits();

    auto b_sign=sign(clients,b);

    auto b_abs=f_Mul(clients,b,b_sign);

    auto b_factor=div_factor(clients,b_abs);
    

    auto b_norm=f_Mul4Div(clients,b_abs,b_factor,25);


    // auto r_b_norm=Areconstruct(clients,b_norm);
    // for(int i=0;i<r_b_norm.size();i++)
    // {
    //     std::cout<<r_b_norm[i]/1024.0/1024.0/32.0<<" ";
    // }
    // std::cout<<std::endl;
    auto b_norm_reciprocal=reciprocal_newton_norm(clients,b_norm);

    // auto r_b_norm_r=reconstruct(clients,b_norm_reciprocal);
    // for(int i=0;i<r_b_norm_r.size();i++)
    // {
    //     std::cout<<r_b_norm_r[i]/1024.0/32.0<<" ";
    // }
    // std::cout<<std::endl;
    auto a1=f_Mul(clients,a,b_sign);

    // auto r_a1=reconstruct(clients,a1);
    // for(int i=0;i<r_a1.size();i++)
    // {
    //     std::cout<<r_a1[i]/1024.0/32.0<<" ";
    // }
    // std::cout<<std::endl;
    auto a2=f_Mul4Div(clients,a1,b_factor);
    auto ans=f_Mul(clients,a2,b_norm_reciprocal);
    return ans;
    // return b_factor;
}


Value div_A4I(Clients& clients,const Value& a,const Value& b)
{
    if(a.size()!=b.size())
        throw std::invalid_argument("a.size must equal b.size");
    size_t fp=a.get_fraction_bits();

    auto b_sign=sign(clients,b);

    auto b_abs=i_Mul_A(clients,b,b_sign);

    auto b_factor=div_factor(clients,b_abs);

    

    auto b_norm=f_Mul(clients,b_abs,b_factor,15);
  

    auto b_norm_reciprocal=reciprocal_newton_norm(clients,b_norm);


    auto a1=i_Mul_A(clients,a,b_sign);
 

    auto a2=f_Mul(clients,a1,b_factor,0);
    auto ans=f_Mul(clients,a2,b_norm_reciprocal);
    return ans;
}

Value floor(Clients& clients,const Value& a)
{
    Value a_copy;
    if(a.get_share_type()==0) a_copy=A2B(clients,a);
    else a_copy=a;
    auto fb=a.get_fraction_bits();
    for(int i=0;i<a.size();i++) a_copy[i]=a_copy[i]>>fb;
    a_copy.set_fraction_bits(0);
    return B2A(clients,a_copy);
}

Value divfloor(Clients& clients,const Value& a,const Value& b)
{
    if(a.get_fraction_bits()!=0||b.get_fraction_bits()!=0) 
    throw std::invalid_argument("a.fb or b.fb must equal to 0");
    auto ones=Value(std::vector<int64_t>(a.size()/2,1));
    auto ab=floor(clients,div_A4I(clients,a,b));

    auto abb=i_Mul_A(clients,ab,b);


    auto is_less=Mul_ab1(clients,ones,ALess(clients,abb,a),0);

    ab=Add_A(clients,is_less,ab);
    abb=i_Mul_A(clients,ab,b);
    auto is_gre=Mul_ab1(clients,ones,AGre(clients,abb,a),0);
    ab=Add_A(clients,ab,Aneg(clients,is_gre));
    return ab;
}

Value MOD(Clients& clients,const Value& a,const Value& b)
{
    if(a.get_fraction_bits()!=0||b.get_fraction_bits()!=0) 
        throw std::invalid_argument("a.fb or b.fb must equal to 0");
    auto ones=Value(std::vector<int64_t>(a.size()/2,1));
    auto ab=floor(clients,div_A4I(clients,a,b));

    auto abb=i_Mul_A(clients,ab,b);


    auto is_less=Mul_ab1(clients,ones,ALess(clients,abb,a),0);

    ab=Add_A(clients,is_less,ab);
    abb=i_Mul_A(clients,ab,b);
    auto is_gre=Mul_ab1(clients,ones,AGre(clients,abb,a),0);
    ab=Add_A(clients,ab,Aneg(clients,is_gre));
    abb=i_Mul_A(clients,ab,b);
    return Add_A(clients,a,Aneg(clients,abb));
}