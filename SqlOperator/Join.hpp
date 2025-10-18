#pragma once

#include "../Operator/operator.h"
#include "Psi.hpp"
#include "Permutation.hpp"
#include "Groupby.hpp"
#include "Sorter.hpp"
#include "../LowMC/Lowmc.hpp"
#include "Expansion.hpp"

#include <future> 

template<size_t N>
std::vector<std::bitset<N>> merge_pem(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,std::vector<size_t> pa,std::vector<size_t> pb)
{
    if(pa.size()!=pb.size()) throw std::invalid_argument("pa.size must equal pb.size merge");
    std::vector<Value> Ba(pa.size()),Bb(pb.size());
    for(int i=0;i<pa.size();i++)
    {
         Ba[i]=(a[pa[i]].get_share_type()==0)?A2B(clients,a[pa[i]]):a[pa[i]];
    }

    for(int i=0;i<pb.size();i++)
    {
        Bb[i]=(b[pb[i]].get_share_type()==0)?A2B(clients,b[pb[i]]):b[pb[i]];
    }



    auto onesa=make_constI_bitset<N>(clients,1,Ba[0].size()/2);
    auto onesb=make_constI_bitset<N>(clients,1,Bb[0].size()/2);
    auto rangeA=make_constrang(clients,Ba[0].size()/2,1);
    auto rangeB=make_constrang(clients,Bb[0].size()/2,Ba[0].size()/2+1);

    auto fa=GroupbyMask(clients,Ba);
    auto fb=GroupbyMask(clients,Bb);    

    auto Fa=compare_conv_return(clients,fa);
    auto Fb=compare_conv_return(clients,fb);

    auto appxA=And(clients,rangeA,Fa);
    auto appxB=And(clients,rangeB,Fb);


    auto fb_msk=conv2ToP<N>(clients,fb);

    auto vb=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{onesb},fb_msk)[0];


    Ba.push_back(appxA);
    Bb.push_back(appxB);
    
    auto key=gen_rand_key(clients,1);
    auto Lowmc=LowMC<80>(clients,key);
    auto enc_a=Lowmc.encrypt(clients,Ba);
    auto enc_b=Lowmc.encrypt(clients,Bb);
    // std::vector<size_t> idx1(a[0].size()/2),idx2(b[0].size()/2);
    // for(int i=0;i<idx1.size();i++) idx1[i]=i;
    // for(int i=0;i<idx2.size();i++) idx2[i]=i+idx1.size();
    // for(int i=0;i<Ba.size();i++) combine(Ba[i],Bb[i]);
    // auto enc_ab=Lowmc.encrypt(clients,Ba);
    // auto enc_a=Gather(clients,enc_ab,idx1);
    // auto enc_b=Gather(clients,enc_ab,idx2);


    auto rangea=make_constrangA_bitset<N>(clients,Ba[0].size()/2,1);
    
    auto Rv=Psi_with_payload_enc<80,N>(clients,enc_b,enc_a,std::vector<std::vector<std::bitset<N>>>{vb})[0];
    std::vector<std::bitset<N>> rj(Rv.size());

    rj[0]=rj[1]=0;

    for(int i=2;i<rj.size();i++)
    {
        rj[i]=convToint(rj[i-2])+convToint(Rv[i-2]);
    }

    for(int i=0;i<rj.size();i++)
    {
        rj[i]=convToint(rj[i])+convToint(rangea[i]);
    }
    auto So=Psi_with_payload_enc<80,N>(clients,enc_a,enc_b,std::vector<std::vector<std::bitset<N>>>{rj});
  

    auto rev_fb=mask_rev(fb_msk);
    auto Sofinal=GroupbySum_withmask(clients,So,rev_fb,1);


    auto Sk=Sofinal[1];

    auto Sk_copy=Add_bitset(clients,onesb,Aneg_bitset(clients,Sk));
    auto SG=perfix_sum_bitset(clients,Sk_copy);

    auto mn=make_constI_bitset<N>(clients,rj.size()/2+So[0].size()/2+1,So[0].size()/2);
    auto sj1=Mul_bitset(clients,Add_bitset(clients,mn,Aneg_bitset(clients,SG)),Sk_copy);
    auto sj2=Mul_bitset(clients,Add_bitset(clients,Sofinal[0],vb),Sk);
    auto sj=Add_bitset(clients,sj1,sj2);

    combine(rj,sj);
    return rj;
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> merge4Join(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<size_t> pa,const std::vector<size_t> pb)
{

    if(pa.size()!=pb.size()) throw std::invalid_argument("pa.size must equal pb.size merge");
    std::vector<Value> Ba(pa.size()),Bb(pb.size());
    for(int i=0;i<pa.size();i++)
    {
         Ba[i]=(a[pa[i]].get_share_type()==0)?A2B(clients,a[pa[i]]):a[pa[i]];
    }

    for(int i=0;i<pb.size();i++)
    {
        Bb[i]=(b[pb[i]].get_share_type()==0)?A2B(clients,b[pb[i]]):b[pb[i]];
    }

    auto onesa=make_constI_bitset<N>(clients,1,Ba[0].size()/2);
    auto onesb=make_constI_bitset<N>(clients,1,Bb[0].size()/2);
    auto rangeA=make_constrang(clients,Ba[0].size()/2,1);
    auto rangeB=make_constrang(clients,Bb[0].size()/2,Ba[0].size()/2+1);

    auto fa=GroupbyMask(clients,Ba);
    auto fb=GroupbyMask(clients,Bb);    

    auto Fa=compare_conv_return(clients,fa);
    auto Fb=compare_conv_return(clients,fb);

    auto appxA=And(clients,rangeA,Fa);
    auto appxB=And(clients,rangeB,Fb);

    auto fa_msk=conv2ToP<N>(clients,fa);
    auto fb_msk=conv2ToP<N>(clients,fb);
    auto va=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{onesa},fa_msk)[0];
    auto vb=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{onesb},fb_msk)[0];

    Ba.push_back(appxA);
    Bb.push_back(appxB);
    
    auto dummmy1=make_dummy(clients,a[0].size()/2,1);
    auto dummmy2=make_dummy(clients,b[0].size()/2,2);
    
    auto key=gen_rand_key(clients,1);
    auto Lowmc=LowMC<80>(clients,key);
    auto enc_a=Lowmc.encrypt(clients,Ba);
    auto enc_b=Lowmc.encrypt(clients,Bb);

    auto rangea=make_constrangA_bitset<N>(clients,Ba[0].size()/2,1);

    auto Rv=Psi_with_payload_enc<80,N>(clients,enc_b,enc_a,std::vector<std::vector<std::bitset<N>>>{vb});
  
    std::vector<std::bitset<N>> rj(Rv[0].size());

    rj[0]=rj[1]=0;

    for(int i=2;i<rj.size();i++)
    {
        rj[i]=convToint(rj[i-2])+convToint(Rv[0][i-2]);
    }

    for(int i=0;i<rj.size();i++)
    {
        rj[i]=convToint(rj[i])+convToint(rangea[i]);
    }

    auto So=Psi_with_payload_enc<80,N>(clients,enc_a,enc_b,std::vector<std::vector<std::bitset<N>>>{rj,va});
    

    std::vector<std::vector<std::bitset<N>>> appendix(4);
    auto F=Rv[1];
    combine(F,So[2]);


    auto rev_fb=mask_rev(fb_msk);
    auto Sofinal=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{So[0],So[2]},rev_fb,1);

    auto Sk=Sofinal[1];

    auto Sk_copy=Add_bitset(clients,onesb,Aneg_bitset(clients,Sk));
    auto SG=perfix_sum_bitset(clients,Sk_copy);

    auto mn=make_constI_bitset<N>(clients,rj.size()/2+So[0].size()/2+1,So[0].size()/2);
    auto sj1=Mul_bitset(clients,Add_bitset(clients,mn,Aneg_bitset(clients,SG)),Sk_copy);
    auto sj2=Mul_bitset(clients,Add_bitset(clients,Sofinal[0],vb),Sk);
    auto sj=Add_bitset(clients,sj1,sj2);

    combine(rj,sj);

    auto V=fa_msk;
    combine(V,fb_msk);

    auto al=va;
    combine(al,So[1]);

    auto ar=Rv[0];
    combine(ar,vb);



    return std::vector<std::vector<std::bitset<N>>>{V,F,al,ar,rj,vb};
}

std::vector<Value> merge(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<size_t> pa,const std::vector<size_t> pb)
{
    auto pem=merge_pem<24>(clients,a,b,pa,pb);
    std::vector<Value> Res(a.size()); 
    for(int i=0;i<a.size();i++)
    {
        Res[i]=a[i];
        auto B=b[i];
        combine(Res[i],B);
    }
    return Permutation_ValueAndbitset(clients,Res,pem,1);
}
struct merge_message{
    std::vector<Value> V;
    std::vector<Value> Vpem;
};

struct merge_cur{
    std::vector<Value> org;
    std::vector<Value> V;
    std::vector<Value> Vpem;
};


template<size_t N>
std::vector<Value> Merge_Join(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,std::vector<size_t> pa,std::vector<size_t> pb)
{
    
    auto mRes=merge4Join<N>(clients,a,b,pa,pb);

    auto appendix_pem=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{mRes[0],mRes[1],mRes[2],mRes[3]},mRes[4],1);
    auto appendix=perfix_sum_seg(clients,std::vector<std::vector<std::bitset<N>>>{appendix_pem[1],appendix_pem[2],appendix_pem[3]},appendix_pem[0],1);

    auto zeros=make_constI_bitset<N>(clients,0,a[0].size()/2);
    auto ones=make_constI_bitset<N>(clients,1,b[0].size()/2);
    combine(zeros,ones);


    auto rangea=make_constrangA_bitset<N>(clients,a[0].size()/2);
    auto rangeb=make_constrangA_bitset<N>(clients,b[0].size()/2);
    combine(rangea,rangeb);

    auto res=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{zeros,rangea},mRes[4],1);
    auto pemgen=Permutation_gen_bitset(clients,res[0]);

    std::vector<size_t> indx1(a[0].size()/2),indx2(b[0].size()/2);
    for(int i=0;i<a[0].size()/2;i++) indx1[i]=i;
    for(int i=0;i<b[0].size()/2;i++) indx2[i]=a[0].size()/2+i;


    auto pemall=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{res[1]},pemgen)[0];

    rangeb=make_constrangA_bitset<N>(clients,b[0].size()/2);
    auto pem2=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{rangeb},Gather(clients,pemall,indx2))[0];


    auto appendix_final=Permutation_bitset(clients,appendix,pemgen);

    auto appendix_final1=Gather_vector(clients,appendix_final,indx1);
    auto appendix_final2=Gather_vector(clients,appendix_final,indx2);

    auto a_pem=a;
    auto b_pem=Permutation_ValueAndbitset(clients,b,pem2);
    auto j=Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{mRes[5]},pem2)[0];


    auto A_final=Expansion(clients,a_pem,appendix_final1[2],appendix_final1[0]);

    if(A_final[0].size()==0){
        return std::vector<Value>(a.size()+b.size());
    }
    std::vector<std::vector<std::bitset<N>>> b_appendix(4);
    b_appendix[0]=appendix_final2[1];
    b_appendix[1]=appendix_final2[2];
    b_appendix[2]=make_constrangA_bitset<N>(clients,b[0].size()/2);
    b_appendix[3]=j;

    auto B_ST=Expansion_ValueAndBitset(clients,b_pem,b_appendix,appendix_final2[1],appendix_final2[0]);
    b_pem=std::move(B_ST.a);
    b_appendix=std::move(B_ST.b);
    if(b_pem[0].size()==0){
        return std::vector<Value>(a.size()+b.size());
    }



    auto Ones=make_constI_bitset<N>(clients,1,b_pem[0].size()/2);
    auto Ranges=make_constrangA_bitset<N>(clients,b_pem[0].size()/2,1);
    auto I=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{Ones},b_appendix[4])[0];

    auto DL=std::move(b_appendix[0]);
    auto DR=std::move(b_appendix[1]);
    auto J=std::move(b_appendix[3]);

    auto SGL=Add_bitset(clients,Add_bitset(clients,Ranges,Mul_bitset(clients,Add_bitset(clients,I,Aneg_bitset(clients,Ones)),DR)),Aneg_bitset(clients,I));
    auto SGR=Add_bitset(clients,J,Aneg_bitset(clients,Mul_bitset(clients,Add_bitset(clients,J,Aneg_bitset(clients,Ones)),DL)));
    auto SG=Add_bitset(clients,SGL,SGR);
    


    auto B_final=Permutation_ValueAndbitset(clients,b_pem,SG,1);

    A_final.insert(A_final.end(),B_final.begin(),B_final.end());
    return A_final;
}

std::vector<Value> general_Join(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,std::vector<size_t> pa,std::vector<size_t> pb)
{
    if(pa.size()!=pb.size()) throw std::invalid_argument("pa.size must equal pb.size merge");
    std::vector<Value> Ba(pa.size()),Bb(pb.size());
    auto zeros=make_constI(clients,0,a[0].size()/2);
    auto ones=make_constI(clients,1,b[0].size()/2);
    for(int i=0;i<pa.size();i++) Ba[i]=a[pa[i]];

    for(int i=0;i<pb.size();i++) Bb[i]=b[pb[i]];
    combine(zeros,ones);
    combine_vector(Ba,Bb);
    auto pem=radix_sort(clients,Ba);
    auto Ba_pem=Permutation_ValueAndbitset(clients,Ba,pem);
    auto zeros_pem=Permutation_ValueAndbitset(clients,std::vector<Value>{zeros},pem)[0];
    auto ar=GroupbySum(clients,std::vector<Value>{zeros_pem},Ba_pem);
    auto al=GroupbySum(clients,std::vector<Value>{revbit(clients,zeros_pem)},Ba_pem);
    ar=GroupbyXor(clients,ar,Ba_pem,1);
    al=GroupbyXor(clients,al,Ba_pem,1);

    auto pembit=Permutation_gen(clients,zeros_pem);
    Ba_pem=Permutation(clients,Ba_pem,pembit);
    al=Permutation(clients,al,pembit);
    ar=Permutation(clients,ar,pembit);
    std::vector<size_t> idx1(a[0].size()/2);
    std::vector<size_t> idx2(b[0].size()/2);
    for(int i=0;i<idx1.size();i++) idx1[i]=i;
    for(int i=0;i<idx2.size();i++) idx2[i]=i+idx1.size();
    auto Baa=Gather_vector(clients,Ba_pem,idx1);
    auto Bbb=Gather_vector(clients,Ba_pem,idx2);
    auto ala=Gather_vector(clients,al,idx1)[0];
    auto alb=Gather_vector(clients,al,idx2)[0];
    auto ara=Gather_vector(clients,ar,idx1)[0];
    auto arb=Gather_vector(clients,ar,idx2)[0];
    auto onesa=make_constI(clients,1,a[0].size()/2);
    auto onesb=make_constI(clients,1,b[0].size()/2);
    auto eqa=conv2ToValue(clients,eq(clients,ara,onesa));
    auto eqb=conv2ToValue(clients,eq(clients,alb,onesb));
    auto rangb=make_constrang(clients,b[0].size()/2);
    eqa=revbit(clients,eqa);
    eqb=revbit(clients,eqb);

    ara=Mul(clients,ara,eqa);
    alb=Mul(clients,alb,eqb);

    auto A_final=Expansion(clients,Baa,onesa,onesa);
    auto fj=GroupbySum(clients,std::vector<Value>{onesb},Bbb);
    Bbb.push_back(fj[1]);
    Bbb.push_back(eqb);
    Bbb.push_back(rangb);
    Bbb.push_back(fj[0]);
    auto B_final=Expansion(clients,Bbb,onesb,onesb);
    auto Onesb=make_constI(clients,1,B_final[0].size()/2);
    auto Rangb=make_constrang(clients,B_final[0].size()/2);

    std::vector<Value> b_appendix(4);
    b_appendix[3]=B_final[B_final.size()-1];
    B_final.erase(B_final.end()-1);
    b_appendix[2]=B_final[B_final.size()-1];
    B_final.erase(B_final.end()-1);
    b_appendix[1]=B_final[B_final.size()-1];
    B_final.erase(B_final.end()-1);
    b_appendix[0]=B_final[B_final.size()-1];
    B_final.erase(B_final.end()-1);

    auto DL=std::move(b_appendix[0]);
    auto DR=std::move(b_appendix[1]);
    auto J=std::move(b_appendix[3]);
    auto I=GroupbySum(clients,std::vector<Value>{Onesb},std::vector<Value>{b_appendix[2]})[0];

    auto SGL=Add(clients,Add(clients,Rangb,Mul(clients,Add(clients,I,Aneg(clients,Onesb)),DR)),Aneg(clients,I));
    auto SGR=Add(clients,J,Aneg(clients,Mul(clients,Add(clients,J,Aneg(clients,Onesb)),DL)));
    auto SG=Add(clients,SGL,SGR);
    


    // B_final=Permutation(clients,Bbb,SG,1);
    A_final.insert(A_final.end(),B_final.begin(),B_final.end());
    return A_final;
}
std::vector<Value> LeftJoin(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<size_t>& pa,const std::vector<size_t>& pb,const std::vector<Value>& payload_b=std::vector<Value>())
{
    
    auto payload_copy=payload_b;
    if(pa.size()!=pb.size()) throw std::invalid_argument("pa.size must equal pb.size merge");
    std::vector<Value> Ba(pa.size()),Bb(pb.size());
    for(int i=0;i<pa.size();i++)
    {
         Ba[i]=(a[pa[i]].get_share_type()==0)?A2B(clients,a[pa[i]]):a[pa[i]];
    }

    for(int i=0;i<pb.size();i++)
    {
        Bb[i]=(b[pb[i]].get_share_type()==0)?A2B(clients,b[pb[i]]):b[pb[i]];
    }


    auto onesb=make_constI(clients,1,Bb[0].size()/2);
    auto rangeA=make_constrang(clients,Ba[0].size()/2,1);
    auto rangeB=make_constrang(clients,Bb[0].size()/2,Ba[0].size()/2+1);

    payload_copy.push_back(onesb);

    auto fa=GroupbyMask(clients,Ba);
    auto fb=GroupbyMask(clients,Bb);    

    auto Fa=compare_conv_return(clients,fa);
    auto Fb=compare_conv_return(clients,fb);

    auto appxA=And(clients,rangeA,Fa);
    auto appxB=And(clients,rangeB,Fb);

    auto fa_msk=conv2ToValue(clients,fa);
    auto fb_msk=conv2ToValue(clients,fb);
    auto pay=GroupbySum_withmask(clients,payload_copy,fb_msk);


    Ba.push_back(appxA);
    Bb.push_back(appxB);
    auto key=gen_rand_key(clients,1);
    auto Lowmc=LowMC<80>(clients,key);
    auto enc_a=Lowmc.encrypt(clients,Ba);
    auto enc_b=Lowmc.encrypt(clients,Bb);

    auto Rv=Psi_with_payload_enc(clients,enc_b,enc_a,pay);
    Rv.erase(Rv.end()-1);

    auto rev_fa=mask_rev(fa_msk);
    payload_copy=GroupbySum_withmask(clients,Rv,rev_fa,1);
    return payload_copy;
}


// std::vector<Value> Join(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,size_t pa,size_t pb)
// {
//     auto ones=make_constI(clients,0,a[0].size()/2);
//     auto zeros=make_constI(clients,1,b[0].size()/2);
    
//     std::vector<Value> Ba={a[pa],ones},Bb={b[pb],zeros};

//     auto mRes=merge(clients,Ba,Bb,0,0);

//     auto C=GroupbyMask(clients,std::vector<Value>{mRes[0]});
//     auto tl=GroupbySum_withmask(clients,std::vector<Value>{Add(clients,make_constI(clients,1,mRes[1].size()/2),mRes[1])},C);
//     auto tr=GroupbySum_withmask(clients,std::vector<Value>{mRes[1]},C);

//     auto aL=perfix_sum_seg(clients,std::vector<Value>{tl},C);
//     auto aR=perfix_sum_seg(clients,std::vector<Value>{tr},C);

// }

// Value merge_pem(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,size_t pa,size_t pb)
// {

//     auto Ba=(a[pa].get_share_type()==0)?A2B(clients,a[pa]):a[pa];
//     auto Bb=(b[pb].get_share_type()==0)?A2B(clients,b[pb]):b[pb];

//     auto onesa=make_constI(clients,1,Ba.size()/2);
//     auto onesb=make_constI(clients,1,Bb.size()/2);

//     auto zerosa=make_constI(clients,0,Ba.size()/2);
//     auto zerosb=make_constI(clients,0,Bb.size()/2);


//     auto Va=GroupbySum(clients,std::vector<Value>{onesa},std::vector<Value>{Ba});

//     auto Vb=GroupbySum(clients,std::vector<Value>{onesb},std::vector<Value>{Bb});

//     auto fa=std::move(Va[Va.size()-1]);
//     auto fb=std::move(Vb[Vb.size()-1]);
//     auto fb_copy=fb;
//     auto fbv_copy=Vb[0]; 

//     Va.erase(Va.end()-1);
//     Vb.erase(Vb.end()-1);

//     combine(fa,zerosa);
//     combine(fb,zerosb);
    
//     auto dummmy1=make_dummy(clients,a[0].size()/2,1);
//     auto dummmy2=make_dummy(clients,b[0].size()/2,2);

//     for(int i=0;i<Va.size();i++)
//     {
//         combine(Va[i],dummmy1);
//         combine(Vb[i],dummmy2);
//     }

//     combine(Ba,dummmy1);
//     combine(Bb,dummmy2);

//     auto rangea=make_constrangA(clients,Ba.size()/2,1);
//     auto rangeb=make_constrangA(clients,Bb.size()/2,1);

//     Va.push_back(rangea);
//     Vb.push_back(rangeb);

//     auto p2=Permutation_gen(clients,fb);

//     auto p1=Permutation_gen(clients,fa);

//     Va.push_back(Ba);
//     Vb.push_back(Bb);
//     auto Va_perm=Permutation(clients,Va,p1);
//     auto Vb_perm=Permutation(clients,Vb,p2); 


//     auto ba_perm=Value(std::move(Va_perm[Va_perm.size()-1]));
//     auto bb_perm=Value(std::move(Vb_perm[Vb_perm.size()-1]));

//     ba_perm.set_share_type(1);
//     bb_perm.set_share_type(1);
//     ba_perm.set_fraction_bits(Ba.get_fraction_bits());
//     bb_perm.set_fraction_bits(Bb.get_fraction_bits());

//     Va_perm.erase(Va_perm.end()-1);
//     Vb_perm.erase(Vb_perm.end()-1);

//     auto start = std::chrono::high_resolution_clock::now();
//     size_t sza=a[0].size()/2,szb=b[0].size()/2;
//     std::vector<size_t> idxa(sza),idxb(szb);
//     for(int i=0;i<idxa.size();i++) idxa[i]=sza+i;
//     for(int i=0;i<idxb.size();i++) idxb[i]=szb+i;

//     auto va_valid=Gather_vector(clients,Va_perm,idxa);
//     auto vb_valid=Gather_vector(clients,Vb_perm,idxb);


//     auto ba_valid=Gather(clients,ba_perm,idxa);
//     auto bb_valid=Gather(clients,bb_perm,idxb);

//     auto key=gen_rand_key(clients,1);

//     auto Lowmc=LowMC(clients,key);
//     auto enc_a=Lowmc.encrypt(clients,ba_valid);
//     auto enc_b=Lowmc.encrypt(clients,bb_valid);

//     auto Rv=Psi_with_payload_enc(clients,bb_valid,ba_valid,enc_b,enc_a,std::vector<Value>{vb_valid[0]});
  
//     std::vector<int64_t> rp(Rv[0].size());

//     rp[0]=rp[1]=0;

//     for(int i=2;i<rp.size();i++)
//     {
//         rp[i]=rp[i-2]+Rv[0][i-2];
//     }

//     for(int i=0;i<rp.size();i++)
//     {
//         rp[i]+=va_valid[1][i];
//     }

//     Value RP(std::move(rp));RP.set_fraction_bits(0);RP.set_share_type(0);


//     auto So=Psi_with_payload_enc(clients,ba_valid,bb_valid,enc_a,enc_b,std::vector<Value>{RP});
    

//     insert_front(So[0],zerosb);
//     insert_front(So[1],zerosb);
//     insert_front(Rv[0],zerosa);

//     std::vector<size_t> idxaa(sza),idxbb(szb);
//     for(int i=0;i<idxaa.size();i++) idxaa[i]=i;
//     for(int i=0;i<idxbb.size();i++) idxbb[i]=i;

//     auto Sopem=Gather_vector(clients,Permutation(clients,So,Vb_perm[Vb_perm.size()-1],1),idxbb);
//     auto Rvpem=Gather_vector(clients,Permutation(clients,std::vector<Value>{Rv[0]},Va_perm[Va_perm.size()-1],1),idxaa);

//     std::vector<int64_t> C(Rvpem[0].size());

//     C[0]=C[1]=0;
//     for(int i=2;i<Rvpem[0].size();i++)
//     {
//         C[i]=C[i-2]+Rvpem[0][i-2];
//     }

//     std::vector<int64_t> rj(Rvpem[0].size());
//     for(int i=0;i<Rvpem[0].size();i++)
//     {
//         rj[i]=C[i]+rangea[i];
//     }

//     Value RJ(std::move(rj));RJ.set_fraction_bits(0);RJ.set_share_type(0);

//     reverse_forward(fb_copy);
//     reverse(Sopem[0]);
//     reverse(Sopem[1]);

//     auto Sofinal=GroupbySum_withmask(clients,Sopem,fb_copy);

//     reverse(Sofinal[0]);
//     reverse(Sofinal[1]);

//     auto Sk=Sofinal[1];

//     auto Sk_copy=Add(clients,onesb,neg(clients,Sk));
//     auto SG=perfix_sum(clients,Sk_copy);


//     auto mn=make_constI(clients,Rvpem[0].size()/2+Sopem[0].size()/2+1,Sopem[0].size()/2);
//     auto sj1=Mul(clients,Add(clients,mn,neg(clients,SG)),Sk_copy);
//     auto sj2=Mul(clients,Add(clients,Sofinal[0],fbv_copy),Sk);
//     auto SJ=Add(clients,sj1,sj2);

//     combine(RJ,SJ);
//     return RJ;
// }

