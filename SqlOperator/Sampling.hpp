#pragma once

#include <vector>

#include "Join.hpp"

Value DIV4Sampling(Clients& clients,const Value& a,const Value& b)
{
   

    size_t fp=a.get_fraction_bits();

    auto b_sign=sign(clients,b);

    auto b_abs=i_Mul_A(clients,b,b_sign);

    auto b_factor=div_factor(clients,b_abs);

    auto b_norm=f_Mul(clients,b_abs,b_factor,15);

    auto b_norm_reciprocal=reciprocal_newton_norm(clients,b_norm);


    auto a1=i_Mul_A(clients,a,expan(clients,b_sign,a.size()));
    

    auto a2=f_Mul(clients,a1,expan(clients,b_factor,a.size()),5);
    auto ans=f_Mul(clients,a2,expan(clients,b_norm_reciprocal,a.size()));
    return ans;
    
}
Value MOD4Sampling(Clients& clients,const Value& a,const Value& b)
{
    if(a.get_fraction_bits()!=0||b.get_fraction_bits()!=0) 
        throw std::invalid_argument("a.fb or b.fb must equal to 0");
    auto expansB=expan(clients,b,a.size());
    auto ones=Value(std::vector<int64_t>(a.size()/2,1));
    
    auto ab=floor(clients,DIV4Sampling(clients,a,b));

    auto abb=i_Mul_A(clients,ab,expansB);


    auto is_less=Mul_ab1(clients,ones,ALess(clients,abb,a),0);

    ab=Add_A(clients,is_less,ab);
    abb=i_Mul_A(clients,ab,expansB);
    auto is_gre=Mul_ab1(clients,ones,AGre(clients,abb,a),0);
    ab=Add_A(clients,ab,Aneg(clients,is_gre));
    abb=i_Mul_A(clients,ab,expansB);
    return Add_A(clients,a,Aneg(clients,abb));
}

Value gen_rand_idx(Clients& clients,const Value& mx_sz,size_t sz)
{
    auto r=generate_int_numbers(sz);

    auto rr = clients.SendRoud(r);

    std::vector<int64_t> res(2*sz);

    for(int i=0;i<r.size();i++) {
        res[2*i]=rr[i];res[2*i+1]=r[i];
    }

    Value R(std::move(res));R.set_fraction_bits(0);R.set_share_type(0);


    return MOD4Sampling(clients,R,mx_sz);
}

// template<size_t N>
// std::vector<std::vector<std::bitset<N>>> merge4sampling(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,size_t pa,size_t pb)
// {
//     std::vector<Value> Ba(1),Bb(1);
//     Ba[0]=(a[pa].get_share_type()==0)?A2B(clients,a[pa]):a[pa];
//     Bb[0]=(b[pb].get_share_type()==0)?A2B(clients,b[pb]):b[pb];

//     auto onesa=make_constI_bitset<N>(clients,1,Ba[0].size()/2);
//     auto onesb=make_constI_bitset<N>(clients,1,Bb[0].size()/2);


//     auto fa=GroupbyMask(clients,Ba);
//     auto fb=GroupbyMask(clients,Bb);
//     auto Fa=compare_conv_return(clients,fa);
//     auto Fb=compare_conv_return(clients,fb);
//     auto fb_msk=conv2ToP<N>(clients,fb);    
//     auto fa_msk=conv2ToP<N>(clients,fa);    
//     auto Vb=GroupbySum_withmask(clients,std::vector<std::vector<std::bitset<N>>>{onesb},fb_msk)[0];

    
//     auto rangeA=make_constrang(clients,Ba[0].size()/2,1);
//     auto rangeB=make_constrang(clients,Bb[0].size()/2,Ba[0].size()/2+1);
//     auto appxA=And(clients,rangeA,Fa);  
//     auto appxB=And(clients,rangeB,Fb);




//     Ba.push_back(appxA);
//     Bb.push_back(appxB);


//     auto key=gen_rand_key(clients,1);

//     auto Lowmc=LowMC<80>(clients,key);
//     auto enc_a=Lowmc.encrypt(clients,Ba);
//     auto enc_b=Lowmc.encrypt(clients,Bb);


//     auto Rv=Psi_with_payload_enc<80,N>(clients,enc_b,enc_a,std::vector<std::vector<std::bitset<N>>>{Vb});

  


//     auto F=Rv[1];


//     auto V=fa_msk;

//     auto ar=Rv[0];



//     return std::vector<std::vector<std::bitset<N>>>{V,F,ar};
// }

Value expan4filsmapling(Clients& clients,const Value& a,size_t s)
{
    int sz=a.size()*s;
    std::vector<int64_t> res(sz);
    for(int i=0;i<res.size();i+=2)
    {
        int t=i/(2*s);
        res[i]=a[2*t];
        res[i+1]=a[2*t+1];
    }
    return Value(std::move(res));
}

Value expan4smapling(Clients& clients,const Value& a,size_t s)
{
    int sz=a.size()*s;
    std::vector<int64_t> res(sz);
    for(int i=0;i<res.size();i+=2)
    {
        int t=i%(a.size());
        res[i]=a[t];
        res[i+1]=a[t+1];
    }
    return Value(std::move(res));
}
std::vector<Value> expan4smapling_vector(Clients& clients,const std::vector<Value>& a,size_t s)
{
    std::vector<Value> res(a.size());
    for(int i=0;i<a.size();i++) res[i]=expan4smapling(clients,a[i],s),res[i].set_share_type(a[i].get_share_type());
    return res;
}
std::vector<Value> merge4sampling(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,const std::vector<size_t>& pa,const std::vector<size_t>& pb,const std::vector<Value>&payload_a)
{
    std::vector<Value> Ba(pa.size()),Bb(pb.size());
    for(int i=0;i<pa.size();i++)
    {
        Ba[i]=(a[pa[i]].get_share_type()==0)?A2B(clients,a[pa[i]]):a[pa[i]];
        Bb[i]=(b[pb[i]].get_share_type()==0)?A2B(clients,b[pb[i]]):b[pb[i]];
    }

    auto key=gen_rand_key(clients,1);

    auto Lowmc=LowMC<80>(clients,key);
    auto enc_a=Lowmc.encrypt(clients,Ba);
    auto enc_b=Lowmc.encrypt(clients,Bb);

    auto paya=Psi_with_payload_enc(clients,enc_a,enc_b,payload_a);

    paya.erase(paya.end()-1);
    return paya;
}


std::vector<Value> Merge_join4Smpling(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,size_t pa,size_t pb,size_t s,const std::vector<Value>& payload_a)
{
    size_t ns=a[0].size()/2/s;
    std::vector<Value> a_copy=a,b_copy=b;

    if(a[0].get_share_type()==0) a_copy[0]=(A2B(clients,a[0]));

    if(b[0].get_share_type()==0) b_copy[0]=(A2B(clients,b[0]));

    std::vector<int64_t> id(a[0].size()/2);
    for(int i=0;i<a[0].size()/2;i++) id[i]=i%s+1;
    auto idd=make_constB(id);
    a_copy.insert(a_copy.begin(),make_constB(0,a[0].size()/2));
    a_copy.insert(a_copy.begin(),idd);



    auto f=GroupbyMask(clients,std::vector<Value>{b_copy[0]});
    auto fA=conv2ToValue(clients,f);
    auto not_f=Not(clients,f);
    b_copy.insert(b_copy.begin(),not_f);
    b_copy=expan4smapling_vector(clients,b_copy,s);

    auto ranges=make_constrang(clients,b_copy[0].size()/2,1);

    compare_conv(clients,b_copy[0]);

    b_copy[0]=And(clients,b_copy[0],ranges);
    std::vector<int64_t> idb(b_copy[0].size()/2);

    for(int i=0;i<b_copy[0].size()/2;i++) idb[i]=i/ns+1;
    b_copy.insert(b_copy.begin(),make_constB(idb));

    auto& c=b_copy[b_copy.size()-1];

    for(int i=0;i<c.size();i++) c[i]=c[i]*s;
    for(int i=0;i<b_copy[0].size()/2;i++) idb[i]=i/ns;
    c=Add(clients,c,make_constI(clients,idb));


    auto start = std::chrono::high_resolution_clock::now();

    auto mRes=merge4sampling(clients,a_copy,b_copy,std::vector<size_t>{0,1,2},std::vector<size_t>{0,1,2},payload_a);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime7777: " << duration.count() << " ms" << std::endl;



    auto appendix=GroupbySum_withmask(clients,mRes,expan4smapling(clients,fA,s),1);

    return Permutation(clients,appendix,c);
}



Value sortsid(Clients& clients,const Value& rand_eid,const Value& sid,size_t s)
{

    auto ones=make_constI(clients,1,sid.size()/2);
    auto start = std::chrono::high_resolution_clock::now();
    auto vf=GroupbySum(clients,std::vector<Value>{ones},std::vector<Value>{rand_eid});
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime groupbysum: " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    auto f=Mul_ab1(clients,Value(std::move(std::vector<int64_t>(sid.size()/2,1))),vf[1],0);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime mulab1: " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();

    auto DS=perfix_sum_seg(clients,std::vector<Value>{vf[0]},f,1)[0];
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime perfixsumseg: " << duration.count() << " ms" << std::endl;
    auto DR=make_constI(clients,s,sid.size()/2);
    auto J=vf[0];

    int sz=sid.size()*s;
    auto S_pem=expan4smapling(clients,sid,s);
    std::vector<int64_t> offset(S_pem.size()/2);
    for(int i=0;i<offset.size();i++) offset[i]=i%s;
    auto offset_s=make_constI(clients,offset);
    auto number_s=make_constI(clients,s,S_pem.size()/2);

    S_pem=Add(clients,Mul(clients,S_pem,number_s),offset_s);


    auto DS_final=expan4smapling(clients,DS,s);
    auto DR_final=expan4smapling(clients,DR,s);
    auto J_final=expan4smapling(clients,J,s);


    std::vector<int64_t> I(DS_final.size()/2);
    for(int i=0;i<I.size();i++) I[i]=(i%s)+1;
    auto I_final=make_constI(clients,I);


    auto Ones=make_constI(clients,1,DS_final.size()/2);
    auto ranges=make_constrangA(clients,DS_final.size()/2,1);
    auto SGL=Add(clients,Add(clients,ranges,Mul(clients,Add(clients,I_final,neg(clients,Ones)),DS_final)),neg(clients,I_final));
    auto SGR=Add(clients,J_final,neg(clients,Mul(clients,Add(clients,J_final,neg(clients,Ones)),DR_final)));
    auto SG=Add(clients,SGL,SGR);
    auto pem_final=Permutation(clients,std::vector<Value>{S_pem},SG,1)[0];
    return pem_final;

}


// std::vector<Value> Secure_sampling(Clients& clients,const std::vector<Value>& a,size_t s)
// {
//     int n=a[0].size()/2;
//     int floor_ns=n/s;
//     int ceil_ns=(n+s-1)/s;
//     int dif=ceil_ns*s-n;
//     auto a_copy=a;
//     if(dif!=0)
//     {
//         for(int i=0;i<a.size();i++)
//         {
//             auto appendix=gen_rand_Value(clients,dif,a_copy[i].get_share_type());
//             combine(a_copy[i],appendix);
//         }
//     }
//     auto start = std::chrono::high_resolution_clock::now();
//     a_copy=random_shuffle(clients,a_copy);
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function runtime randshuffle: " << duration.count() << " ms" << std::endl;
//     std::vector<int64_t> id(a_copy[0].size()/2);
//     for(int i=0;i<a_copy[0].size()/2;i++) id[i]=i/s;
//     auto eid=make_constI(clients,id);
//     a_copy.insert(a_copy.begin(),eid);

//     auto sz=Value(std::move(std::vector<int64_t>{ceil_ns,ceil_ns}));
//     sz.set_share_type(1);

//     sz=B2A(clients,sz);
//     start = std::chrono::high_resolution_clock::now();
//     auto rand_eid=gen_rand_idx(clients,sz,ceil_ns);
//      end = std::chrono::high_resolution_clock::now();
//     duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function runtime odd_even_merge_sort: " << duration.count() << " ms" << std::endl;
//     start = std::chrono::high_resolution_clock::now();
//     std::vector<int64_t> sid_v(ceil_ns);
//     for(int i=0;i<ceil_ns;i++) sid_v[i]=i;
//     auto sid=make_constI(clients,sid_v);
//     auto pem=radix_sort(clients,std::vector<Value>{rand_eid});
//     // pem=odd_even_merge_sort(clients,std::vector<Value>{rand_eid});
//     end = std::chrono::high_resolution_clock::now();
//     duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function runtime odd_even_merge_sort: " << duration.count() << " ms" << std::endl;

//     // auto pemrev=Permutation(clients,std::vector<Value>{make_constrangA(clients,pem.size()/2)},pem)[0];
//     auto appendix=Permutation_ValueAndbitset(clients,std::vector<Value>{rand_eid,sid},pem);

//     // auto recpem=reconstruct(clients,appendix[0]);
//     // bool sorted=true;
//     // for(int i=1;i<recpem.size();i++)
//     // {
//     //     if(recpem[i]<recpem[i-1]) sorted=false;
//     // }
//     // std::cout<<"sorted: "<<sorted<<std::endl;
  
//     start = std::chrono::high_resolution_clock::now();
//     auto Join_Res=Merge_join4Smpling<24>(clients,a_copy,appendix,0,0);
//     end = std::chrono::high_resolution_clock::now();
//     duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function runtime join: " << duration.count() << " ms" << std::endl;
//     start = std::chrono::high_resolution_clock::now();
//     auto S_pem=sortsid(clients,appendix[0],appendix[1],s);
//     end = std::chrono::high_resolution_clock::now();
//     duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//     std::cout << "Function sortsid: " << duration.count() << " ms" << std::endl;
//     Join_Res.erase(Join_Res.begin());
//     Join_Res=Permutation(clients,Join_Res,S_pem);
//     return Join_Res;
// }


std::vector<Value> Secure_sampling(Clients& clients,const std::vector<Value>& a,size_t s)
{
    int n=a[0].size()/2;
    int floor_ns=n/s;
    int ceil_ns=(n+s-1)/s;
    int dif=ceil_ns*s-n;
    auto a_copy=a;
    if(dif!=0)
    {
        for(int i=0;i<a.size();i++)
        {
            auto appendix=gen_rand_Value(clients,dif,a_copy[i].get_share_type());
            combine(a_copy[i],appendix);
        }
    }


    a_copy=random_shuffle(clients,a_copy);
    auto payload=a_copy;
    std::vector<int64_t> id(a_copy[0].size()/2);
    for(int i=0;i<a_copy[0].size()/2;i++) id[i]=i/s;
    auto eid=make_constB(id);
    a_copy.insert(a_copy.begin(),eid);

    auto sz=Value(std::move(std::vector<int64_t>{ceil_ns,ceil_ns}));
    sz.set_share_type(1);

    sz=B2A(clients,sz);

    auto rand_eid=gen_rand_idx(clients,sz,ceil_ns);

    std::vector<int64_t> sid_v(ceil_ns);
    for(int i=0;i<ceil_ns;i++) sid_v[i]=i;
    auto sid=make_constI(clients,sid_v);
    auto pem=radix_sort(clients,std::vector<Value>{rand_eid});

    
    auto appendix=Permutation_ValueAndbitset(clients,std::vector<Value>{rand_eid,sid},pem);

    auto start = std::chrono::high_resolution_clock::now();
    auto Join_Res=Merge_join4Smpling(clients,a_copy,appendix,0,0,s,payload);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime666: " << duration.count() << " ms" << std::endl;

    return Join_Res;
}


// std::vector<Value> Secure_sampling_compare(Clients& clients,const std::vector<Value>& a,size_t s)
// {
//     int n=a[0].size()/2;
//     int floor_ns=n/s;
//     int ceil_ns=(n+s-1)/s;
//     int dif=ceil_ns*s-n;
//     auto a_copy=a;
//     if(dif!=0)
//     {
//         for(int i=0;i<a.size();i++)
//         {
//             auto appendix=gen_rand_Value(clients,dif,a_copy[i].get_share_type());
//             combine(a_copy[i],appendix);
//         }
//     }

//     a_copy=random_shuffle(clients,a_copy);
//     std::vector<int64_t> id(a_copy[0].size()/2);
//     for(int i=0;i<a_copy[0].size()/2;i++) id[i]=i/s;
//     auto eid=make_constI(clients,id);
//     a_copy.insert(a_copy.begin(),eid);

//     auto sz=Value(std::move(std::vector<int64_t>{ceil_ns,ceil_ns}));
//     sz.set_share_type(1);
//     sz=B2A(clients,sz);


//     auto rand_eid=gen_rand_idx(clients,sz,ceil_ns);
//     // print_Value(clients,rand_eid);
//     std::vector<int64_t> sid_v(ceil_ns);
//     for(int i=0;i<ceil_ns;i++) sid_v[i]=i;
//     auto sid=make_constI(clients,sid_v);

//     auto pem=radix_sort(clients,std::vector<Value>{rand_eid});


//     auto appendix=Permutation(clients,std::vector<Value>{rand_eid,sid},pem);


//     auto Join_Res=Merge_Join(clients,a_copy,appendix,0,0);


    

//     Join_Res.erase(Join_Res.begin());
//     return Permutation(clients,Join_Res,S_pem);
// }

std::vector<Value> Secure_filter_sampling(Clients& clients,const std::vector<Value>& a,const Value& f,size_t s)
{
    int n=a[0].size()/2;
    int floor_ns=n/s;
    int ceil_ns=(n+s-1)/s;
    int dif=ceil_ns*s-n;
    auto a_copy=a;
    auto f_copy=(f.get_share_type()==0)?f:B2A(clients,f);
    std::vector<int64_t> sz{static_cast<int64_t>(s)};
    Value Sz=make_constI(clients,sz);
    if(dif!=0)
    {
        auto zeros=make_constI(clients,0,dif);
        combine(f_copy,zeros);
        for(int i=0;i<a.size();i++)
        {
            auto appendix=gen_rand_Value(clients,dif,a_copy[i].get_share_type());
            combine(a_copy[i],appendix);
        }
    }

    a_copy=random_shuffle(clients,a_copy);
    std::cout<<a_copy[0].size()<<'\n';
    auto start = std::chrono::high_resolution_clock::now();
    auto pem=Permutation_gen(clients,Add(clients,make_constI(clients,1,f_copy.size()/2),neg(clients,f_copy)));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtimepemgen: " << duration.count() << " ms" << std::endl;
    a_copy=Permutation(clients,a_copy,pem);
    f_copy=Permutation(clients,std::vector<Value>{f_copy},pem)[0];
    auto perfixSum=perfix_sum(clients,f_copy);
    std::vector<int64_t> lastone{perfixSum[perfixSum.size()-2],perfixSum[perfixSum.size()-1]};
    Value tot(std::move(lastone));

    // print_Value(clients,tot);
    // print_Value(clients,Sz);
    start = std::chrono::high_resolution_clock::now();

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime div: " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    auto High=divfloor(clients,Add(clients,tot,Add(clients,Sz,neg(clients,make_constI(clients,1,1)))),Sz);


    // print_Value(clients,Low);
    // print_Value(clients,High);

    auto M=Add(clients,Mul(clients,High,Sz),neg(clients,tot));

    
    auto M1=divfloor(clients,M,High);

    
    auto M2=Add(clients,M,neg(clients,Mul(clients,M1,High)));
    // auto M2=MOD(clients,M,High);


    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime Mod: " << duration.count() << " ms" << std::endl;
    // print_Value(clients,M);
    // print_Value(clients,M1);
    // print_Value(clients,M2); 
    start = std::chrono::high_resolution_clock::now();
    std::vector<int64_t> bid(f_copy.size()/(2*s));
    for(int i=0;i<bid.size();i++) bid[i]=i+1;
    auto Bid=make_constI(clients,bid);
    // print_Value(clients,Bid);

    auto bidLow=conv2ToValue(clients,Leq(clients,Bid,expan(clients,High,Bid.size())));
    auto bidM2=conv2ToValue(clients,Leq(clients,Bid,expan(clients,M2,Bid.size())));
    // auto bidLow=Mul_ab1(clients,Value(std::move(std::vector<int64_t>(Bid.size()/2,1))),Leq(clients,Bid,expan(clients,High,Bid.size())),0);
    // auto bidM2=Mul_ab1(clients,Value(std::move(std::vector<int64_t>(Bid.size()/2,1))),Leq(clients,Bid,expan(clients,M2,Bid.size())),0);

    auto g=Add(clients,Mul(clients,bidLow,expan(clients,M1,bidLow.size())),bidM2);


    auto p=perfix_sum(clients,g);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime mul: " << duration.count() << " ms" << std::endl;
    auto gb=A2B(clients,g);
    auto G=expan4filsmapling(clients,g,s);
    auto P=expan4filsmapling(clients,p,s);
    auto GB=expan4filsmapling(clients,gb,s);


    start = std::chrono::high_resolution_clock::now();

    // print_Value(clients,G);
    // print_Value(clients,P);
    // print_Value(clients,P1);

    std::vector<int64_t> o(s);
    for(int i=0;i<o.size();i++) o[i]=i+1;
    auto OOB=make_constB(o);
    OOB=expan(clients,OOB,P.size());
    // auto fd=Mul_ab1(clients,Value(std::move(std::vector<int64_t>(OOB.size()/2,1))),eq(clients,OOB,GB),0);
    auto fd=conv2ToValue(clients,eq(clients,OOB,GB));

    auto ones=make_constI(clients,1,fd.size()/2);
    auto range=make_constrangA(clients,fd.size()/2,1);


    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime mul2: " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();

    auto GG=Add(clients,range,P);


    for(int i=0;i<fd.size();i+=2*s)
    {
        for(int j=2*s-4;j>=0;j-=2)
        {
            fd[i+j]=fd[i+j+2]+fd[i+j];
            fd[i+j+1]=fd[i+j+3]+fd[i+j+1];
        } 
    }
    auto pem_fd=Permutation_gen(clients,fd);
    std::vector<int64_t> e(a_copy[0].size()/2);
    for(int i=0;i<e.size();i++) e[i]=i+1;
    auto E=make_constI(clients,e);
    for(int i=2;i<E.size();i++) E[i]=E[i]+P[i-2];
    auto fdE=Permutation(clients,std::vector<Value>{E,fd},pem_fd);
    fd=fdE[1]; 
    E=fdE[0];

    GG=Add(clients,Mul(clients,fd,E),Mul(clients,GG,Add(clients,ones,neg(clients,fd))));

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime mul3: " << duration.count() << " ms" << std::endl;
    return Permutation(clients,a_copy,GG,1);
}

std::vector<Value> slow_sampling(Clients& clients,const std::vector<Value>& a)
{
    size_t s=1;
    int n=a[0].size()/2;
    int floor_ns=n/s;
    int ceil_ns=(n+s-1)/s;
    int dif=ceil_ns*s-n;
    auto a_copy=a;
    if(dif!=0)
    {
        for(int i=0;i<a.size();i++)
        {
            auto appendix=gen_rand_Value(clients,dif,a_copy[i].get_share_type());
            combine(a_copy[i],appendix);
        }
    }


    a_copy=random_shuffle(clients,a_copy);
    auto payload=a_copy;
    std::vector<int64_t> id(a_copy[0].size()/2);
    for(int i=0;i<a_copy[0].size()/2;i++) id[i]=i/s;
    auto eid=make_constB(id);
    a_copy.insert(a_copy.begin(),eid);

    auto sz=Value(std::move(std::vector<int64_t>{ceil_ns,ceil_ns}));
    sz.set_share_type(1);

    sz=B2A(clients,sz);

    auto rand_eid=gen_rand_idx(clients,sz,ceil_ns);

    std::vector<int64_t> sid_v(ceil_ns);
    for(int i=0;i<ceil_ns;i++) sid_v[i]=i;
    auto sid=make_constI(clients,sid_v);

    


    auto Join_Res=general_Join(clients,a_copy,std::vector<Value>{rand_eid,sid},std::vector<size_t>{0},std::vector<size_t>{0});
    auto start = std::chrono::high_resolution_clock::now();

    auto pem=radix_sort(clients,std::vector<Value>{Join_Res[Join_Res.size()-1]});
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function runtime222: " << duration.count() << " ms" << std::endl;
    // Join_Res=Permutation_ValueAndbitset(clients,Join_Res,pem);
    return Join_Res;
}