// #include<iostream>
// #include<iomanip>

// using namespace std;

// void printArr(int *arr, int len){
//     for(int i = 0; i < len; ++i){
//         cout << setw(3) << arr[i];
//     }
//     cout << endl << endl;
// }

// // 由bitonicSort中的顺序可知,这里传入的arr已是双调序列
// void bitonicMerge(int *arr, int len, bool asd){
//     if (len > 1){
//         int m = len / 2;
//         for(int i = 0; i < m; ++i){
//             if (arr[i] > arr[i + m] == asd)
//         }
//                 // for循环结束后又生成了2个双调序列,分别merge直到序列长度为1
//     bitonicMerge(arr, m, asd); // 都是按照asd进行merge
//     bitonicMerge(arr + m, m, asd);

// }
// }

// void bitonicSort(int *arr, int len, bool asd) { // asd 升序
//     if (len > 1) {
//         int m = len / 2;
//         bitonicSort(arr, m, !asd); // 前半降序
//         bitonicSort(arr + m, len - m, asd); // 后半升序
//         // 前2个sort之后形成了1个双调序列,然后传入merge合并成asd规定的序列
//         bitonicMerge(arr, len, asd); // 合并
//     }
// }

// int main() {
//     int len = 16;
//     auto *arr = new int[len]; // 自动识别类型
//     bool asd = true;

//     // 1.随机生成无序序列
//     srand((unsigned int) time(nullptr));
//     for (int i = 0; i < len; ++i) {
//         arr[i] = (int) random() % 100;
//     }

//     cout << "无序序列:";
//     printArr(arr, len);

//     // 2.双调排序
//     bitonicSort(arr, len, asd);

//     cout << "双调排序:";
//     printArr(arr, len);
// }
#pragma once
#include "../Operator/operator.h"

#include <cmath>


std::vector<std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>>> get_bt_pair(size_t n)
{
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ret1;
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ret2;
    std::vector<std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>>> res;
    size_t k=std::log2(n);
    for(int i=1;i<k;i++)
    {
        std::vector<size_t> r(n/2),l(n/2);
        size_t numr=0,numl=0;
        for(int p=1<<i;p<n;p+=1<<(i+1))
        {
            for(int q=p;q<size_t((1<<i)+p);q++) r[numr++]=q;
        } 
        for(int p=0;p<n;p+=1<<(i+1))
        {
            for(int q=p;q<size_t((1<<i)+p);q++) l[numl++]=q;
        }
        for(int j=i;j>=1;j--)
        {
            std::pair<std::vector<size_t>,std::vector<size_t>> it1,it2;

            for(int p=0;p<numl;p+=1<<j)
            { 
                for(int q=p;q<size_t(p+(1<<(j-1)));q++) it1.first.push_back(l[q]);

            }
            for(int p=1<<(j-1);p<numl;p+=1<<j)
            {   
                for(int q=p;q<size_t(p+(1<<(j-1)));q++) it1.second.push_back(l[q]);
            }
            ret1.push_back(it1);

            for(int p=0;p<numr;p+=1<<j)
            {
                for(int q=p;q<size_t(p+(1<<(j-1)));q++) it2.first.push_back(r[q]);
            }
            for(int p=1<<(j-1);p<numr;p+=1<<j)
            {
                for(int q=p;q<size_t(p+(1<<(j-1)));q++) it2.second.push_back(r[q]);
            }
            ret2.push_back(it2);
        }
    }
    res.push_back(ret1);res.push_back(ret2);
    return res;
}

std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> get_bt_pair1(size_t n)
{
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ret;
    size_t k=std::ceil(std::log2(n));
    for(int j=k;j>=1;j--)
    {
        std::pair<std::vector<size_t>,std::vector<size_t>> it;

        for(int p=0;p<n;p+=1<<j)
        { 
            for(int q=p;q+(1<<(j-1))<std::min(n,size_t(p+(1<<j)));q++) it.first.push_back(q);

        }
        for(int p=1<<(j-1);p<n;p+=1<<j)
        {   
            for(int q=p;q<std::min(n,size_t(p+(1<<(j-1))));q++) it.second.push_back(q);
        }
        ret.push_back(it);
    }
    return ret;
}

Value Btsort(Clients& clients,const std::vector<Value>& a,bool asd=1)
{
    size_t n=a[0].size()/2;
    int t=int(std::log2(n));
    auto idx=get_bt_pair(n);
    if(t%2==0) asd=asd^1;
    auto rang=make_constrang(clients,n);
    std::vector<Value> arr;
    for(int i=0;i<a.size();i++) {
        if(a[i].get_share_type()==1) arr.push_back(B2A(clients,a[i]));
        else arr.push_back(a[i]);
    }
    int num=0;
    for(int i=1;i<t;i++)
    {
        asd=asd^1;
        for(int j=i;j>=1;j--,num++)
        {
            for(int q=0;q<2;q++)
            {
                auto index=idx[q][num];
                auto lv=Gather(clients,arr[0],index.first);
                auto rv=Gather(clients,arr[0],index.second);
                Value res=Geq(clients,lv,rv);
                for(int k=1;k<arr.size();k++)
                {
                    auto lv=Gather(clients,arr[k],index.first);
                    auto rv=Gather(clients,arr[k],index.second);
                    res=std::move(And(clients,res,Geq(clients,lv,rv)));
                }
        
                for(int k=0;k<arr.size();k++)
                {
                    auto lv=Gather(clients,arr[k],index.first);
                    auto rv=Gather(clients,arr[k],index.second);
                    auto newlv=conditional(clients,res,lv,rv);

                    auto newrv=conditional(clients,res,rv,lv);
                    Scatter(clients,arr[k],newlv,index.first);
                    Scatter(clients,arr[k],newrv,index.second);
                }
                auto lr=Gather(clients,rang,index.first);
                auto rr=Gather(clients,rang,index.second);
                auto newlr=conditional(clients,res,lr,rr);
                auto newrr=conditional(clients,res,rr,lr);

                Scatter(clients,rang,newlr,index.first);
                Scatter(clients,rang,newrr,index.second);
            }
            
        }
    }
    auto final_idx=get_bt_pair1(n);
    for(int i=0;i<final_idx.size();i++)
    {
        auto fi=final_idx[i];
        auto lv=Gather(clients,arr[0],fi.first);
        auto rv=Gather(clients,arr[0],fi.second);
        Value res=Geq(clients,lv,rv);
        for(int k=1;k<arr.size();k++)
        {
            auto lv=Gather(clients,arr[k],fi.first);
            auto rv=Gather(clients,arr[k],fi.second);
            res=std::move(And(clients,res,Geq(clients,lv,rv)));
        }

        for(int k=0;k<arr.size();k++)
        {
            auto lv=Gather(clients,arr[k],fi.first);
            auto rv=Gather(clients,arr[k],fi.second);
            auto newlv=conditional(clients,res,lv,rv);

            auto newrv=conditional(clients,res,rv,lv);
            Scatter(clients,arr[k],newlv,fi.first);
            Scatter(clients,arr[k],newrv,fi.second);
        }
        auto lr=Gather(clients,rang,fi.first);
        auto rr=Gather(clients,rang,fi.second);
        auto newlr=conditional(clients,res,lr,rr);
        auto newrr=conditional(clients,res,rr,lr);
        Scatter(clients,rang,newlr,fi.first);
        Scatter(clients,rang,newrr,fi.second);
    }

    return B2A(clients,rang);
}

Value BtMerge(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,bool asd=1)
{
    std::vector<Value> arr;
    for(int i=0;i<a.size();i++)
    {
        Value aa;
        Value bb;
        if(a[i].get_share_type()==0) aa=A2B(clients,a[i]);
        else aa=a[i];
        if(b[i].get_share_type()==0) bb=A2B(clients,b[i]);
        else bb=b[i];
        std::vector<int64_t> mm(aa.size()+bb.size());

        for(int j=0;j<aa.size();j++) mm[j]=aa[j];

        for(int j=0;j<bb.size();j++){

            mm[j+aa.size()]=bb[j];
        }
        Value mg(std::move(mm));mg.set_fraction_bits(a[0].get_fraction_bits());mg.set_share_type(1);
        arr.push_back(mg);
    }
    size_t n=arr[0].size()/2;
    auto idx=get_bt_pair1(n);
    auto rang=make_constrang(clients,n);
    auto final_idx=get_bt_pair1(n);
    for(int i=0;i<final_idx.size();i++)
    {
        auto fi=final_idx[i];
        auto lv=Gather(clients,arr[0],fi.first);
        auto rv=Gather(clients,arr[0],fi.second);
        Value res=Gre(clients,lv,rv);
        for(int k=1;k<arr.size();k++)
        {
            auto lv=Gather(clients,arr[k],fi.first);
            auto rv=Gather(clients,arr[k],fi.second);
            res=std::move(And(clients,res,Gre(clients,lv,rv)));
        }
       if(asd==0) {
           res=std::move(Xor(clients,make_constB(1,res.size()/2),res));
        }
        for(int k=0;k<arr.size();k++)
        {
            auto lv=Gather(clients,arr[k],fi.first);
            auto rv=Gather(clients,arr[k],fi.second);
            auto newlv=conditional(clients,res,lv,rv);
            auto newrv=conditional(clients,res,rv,lv);
            Scatter(clients,arr[k],newlv,fi.first);
            Scatter(clients,arr[k],newrv,fi.second);
        }
        auto lr=Gather(clients,rang,fi.first);
        auto rr=Gather(clients,rang,fi.second);
        auto newlr=conditional(clients,res,lr,rr);
        auto newrr=conditional(clients,res,rr,lr);
        Scatter(clients,rang,newlr,fi.first);
        Scatter(clients,rang,newrr,fi.second);
    }
    return B2A(clients,rang);
}


struct OddEvenSortStage
{
    size_t n;   // total number of elements to sort
    size_t p;   // 1, 2, 4, 8, ..., <n
    size_t k;   // p, p/2, p/4, ..., 1
};

std::vector<OddEvenSortStage> OddEvenSortStages(size_t n)
{
    size_t total_elements = 0;
    for (size_t p = 1; p < n; p <<= 1)
    {
        for (size_t k = p; k >= 1; k >>= 1)
        {
            total_elements++;
        }
    }
    std::vector<OddEvenSortStage> stages(total_elements);
    size_t num=0;
    for(size_t p = 1; p < n; p <<= 1)
    {
        for(size_t k = p; k >= 1; k >>= 1)
        {
            stages[num++]={n, p, k};
        }
    }
    return stages;
}

std::tuple<std::vector<size_t>, std::vector<size_t> > OddEvenSortSequence(OddEvenSortStage stage) {
    

    size_t n = stage.n;
    size_t p = stage.p;
    size_t k = stage.k;


    size_t total_elements = 0;
    for(size_t j = k%p; j < n-k; j += 2*k)
    {
        for(size_t i = 0; i < std::min(k, n-j-k); i++)
        {
            if( (i+j)/(2*p) == (i+j+k)/(2*p) )
            {
                total_elements++;
            }
        }
    }

    std::vector<size_t> s1(total_elements);
    std::vector<size_t> s2(total_elements);
    size_t num=0;
    for(size_t j = k%p; j < n-k; j += 2*k)
    {
        for(size_t i = 0; i < std::min(k, n-j-k); i++)
        {
            if( (i+j)/(2*p) == (i+j+k)/(2*p) )
            {
                s1[num]=i+j;
                s2[num++]=i+j+k;
            }
        }
    }

    return { s1, s2 };
}


Value odd_even_merge_sort(Clients& clients, const std::vector<Value>& a,bool asd=1)
{
    std::vector<Value> arr(a.size());
    for(int i=0;i<a.size();i++)
    {
        if(a[i].get_share_type()==0) arr[i]=A2B(clients,a[i]);
        else arr[i]=a[i];
    }
    size_t numel = arr[0].size()/2;
    auto rang=make_constrang(clients,numel);
    for(auto stage: OddEvenSortStages(numel)) {
        auto [idx1, idx2] = OddEvenSortSequence(stage);
        Value x = Gather(clients,arr[0],idx1);
        Value y = Gather(clients,arr[0],idx2);
        Value res = Gre(clients, x, y);

        for(int i=1;i<arr.size();i++)
        {
            Value x = Gather(clients,arr[i],idx1);
            Value y = Gather(clients,arr[i],idx2);
            res=std::move(And(clients,res,Gre(clients, x, y)));
        }

        if(asd==0) {
           res=std::move(Xor(clients,make_constB(1,res.size()/2),res));
        }
        for(int k=0;k<arr.size();k++)
        {
            auto lv=Gather(clients,arr[k],idx1);
            auto rv=Gather(clients,arr[k],idx2);

            auto newlv=conditional(clients,res,lv,rv);

            auto newrv=conditional(clients,res,rv,lv);
            Scatter(clients,arr[k],newlv,idx1);
            Scatter(clients,arr[k],newrv,idx2);
        }
        auto lr=Gather(clients,rang,idx1);
        auto rr=Gather(clients,rang,idx2);
        auto newlr=conditional(clients,res,lr,rr);
        auto newrr=conditional(clients,res,rr,lr);
        Scatter(clients,rang,newlr,idx1);
        Scatter(clients,rang,newrr,idx2);    
    }

    return B2A(clients,rang);
    // return rang;

}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> radix_sort_conv(Clients& clients,const Value& a)
{
    std::vector<Value> arr;
    if(a.get_share_type()==0) arr=std::move(bitdec(clients,A2B(clients,a)));
    else arr=std::move(bitdec(clients,a));
    std::vector<std::vector<std::bitset<N>>> arr_conv;
    for(int i=0;i<arr[0].size();i+=2)
    {    
        std::vector<int64_t> cur(arr.size()*2);
        for(int j=0;j<arr.size();j++)
        {
            cur[2*j]=arr[j][i];
            cur[2*j+1]=arr[j][i+1];
        }
        // arr_conv.push_back(B2A(clients,cr));
        Value CUR(std::move(cur));
        arr_conv.push_back(conv2ToP<N>(clients,CUR));
    }
    return arr_conv;
}

template<size_t N=24>
std::vector<std::bitset<N>> radix_sort(Clients& clients,const std::vector<Value>& a)
{
    auto cur_bitset=radix_sort_conv<N>(clients,a[0]);
  
    auto res=Permutation_gen_bitset<N>(clients,cur_bitset[0]);
    // print_bitset(clients,res);
    for(int i=1;i<cur_bitset.size();i++)
    {
        auto r=Permutation_gen_bitset(clients,Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{cur_bitset[i]},res)[0]);
        // print_bitset(clients,r);
        res=Permutation_bitset_rev(clients,std::vector<std::vector<std::bitset<N>>>{r},res)[0];
    }
    for(int i=1;i<a.size();i++)
    {
        cur_bitset=std::move(radix_sort_conv<N>(clients,a[i]));
        for(int j=0;j<cur_bitset.size();j++)
        {
            auto r=Permutation_gen_bitset(clients,Permutation_bitset(clients,std::vector<std::vector<std::bitset<N>>>{cur_bitset[j]},res)[0]);
            res=Permutation_bitset_rev(clients,std::vector<std::vector<std::bitset<N>>>{r},res)[0];
        }
    }
    return res;
}



// #pragma once

// #include "../Operator/operator.h"
// #include "Psi.hpp"
// #include "Permutation.hpp"
// #include "Groupby.hpp"
// #include "Sorter.hpp"
// #include "../LowMC/Lowmc.hpp"

// std::vector<Value> merge(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,size_t pa,size_t pb)
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

//     auto p1=Permutation_gen(clients,fa);
//     auto p2=Permutation_gen(clients,fb);


//     auto Va_perm=Permutation(clients,Va,p1);
//     auto Vb_perm=Permutation(clients,Vb,p2); 


//     auto ba_perm=Permutation(clients,std::vector<Value>{Ba},p1);
//     auto bb_perm=Permutation(clients,std::vector<Value>{Bb},p2);
    
//     size_t sza=a[0].size()/2,szb=b[0].size()/2;
//     std::vector<size_t> idxa(sza),idxb(szb);
//     for(int i=0;i<idxa.size();i++) idxa[i]=sza+i;
//     for(int i=0;i<idxb.size();i++) idxb[i]=szb+i;

//     auto va_valid=Gather_vector(clients,Va_perm,idxa);
//     auto vb_valid=Gather_vector(clients,Vb_perm,idxb);


//     auto ba_valid=Gather(clients,ba_perm[0],idxa);
//     auto bb_valid=Gather(clients,bb_perm[0],idxb);

//     auto key=gen_rand_key(clients,1);
//     auto Lowmc=LowMC(clients,key);
//     auto enc_a=Lowmc.encrypt(clients,ba_valid);
//     auto enc_b=Lowmc.encrypt(clients,bb_valid);

//     auto So=Psi_with_payload_enc(clients,ba_valid,bb_valid,enc_a,enc_b,std::vector<Value>{va_valid[1]})[0];
//     auto Rv=Psi_with_payload_enc(clients,bb_valid,ba_valid,enc_b,enc_a,std::vector<Value>{vb_valid[0]})[0];


//     insert_front(So,zerosb);
//     insert_front(Rv,zerosa);

//     std::vector<size_t> idxaa(sza),idxbb(szb);
//     for(int i=0;i<idxaa.size();i++) idxaa[i]=i;
//     for(int i=0;i<idxbb.size();i++) idxbb[i]=i;

//     auto Sopem=Gather(clients,Permutation(clients,std::vector<Value>{So},Vb_perm[Vb_perm.size()-1],1)[0],idxbb);
//     auto Rvpem=Gather(clients,Permutation(clients,std::vector<Value>{Rv},Va_perm[Va_perm.size()-1],1)[0],idxaa);

//     auto Sopem_B=A2B(clients,Sopem);
//     auto zerosB=make_constB(0,Sopem_B.size()/2);

//     Value oneb(std::move(std::vector<int64_t>(Sopem_B.size()/2,1)));
//     auto Sms=Mul_ab1(clients,oneb,eq(clients,Sopem_B,zerosB),0);

//     auto Sms_copy=Add(clients,onesb,neg(clients,Sms));


//     reverse_forward(Sms_copy);
//     reverse(Sopem);


//     auto Sofinal=GroupbySum_withmask(clients,std::vector<Value>{Sopem},Sms_copy)[0];
//     reverse(Sofinal);

//     auto Sk=Mul_ab1(clients,oneb,eq(clients,Sofinal,zerosB),0);
//     auto Sk_copy=Add(clients,onesb,neg(clients,Sk));

//     std::vector<int64_t> C(Rvpem.size());

//     C[0]=C[1]=0;
//     for(int i=2;i<Rvpem.size();i++)
//     {
//         C[i]=C[i-2]+Rvpem[i-2];
//     }

//     std::vector<int64_t> rj(Rvpem.size());
//     for(int i=0;i<Rvpem.size();i++)
//     {
//         rj[i]=C[i]+rangea[i];
//     }

//     Value RJ(std::move(rj));RJ.set_fraction_bits(0);RJ.set_share_type(0);


//     auto SG1=perfix_sum(clients,Sk);
//     print_Value(clients,SG1);
//     auto SG=perfix_sum(clients,Sk_copy);
//     print_Value(clients,SG);
//     print_Value(clients,Sofinal);

//     auto mn=make_constI(clients,Rvpem.size()/2+Sopem.size()/2,Sopem.size()/2);
//     auto sj1=Mul(clients,Add(clients,mn,neg(clients,SG1)),Sk);
//     auto sj2=Mul(clients,Add(clients,Sofinal,SG),Sk_copy);
//     auto SJ=Add(clients,sj1,sj2);

//     print_Value(clients,SJ);
//     combine(RJ,SJ);
//     std::vector<Value> Res(a.size()); 
//     for(int i=0;i<a.size();i++)
//     {
//         Res[i]=a[i];
//         auto B=b[i];
//         combine(Res[i],B);
//     }
//     return Permutation(clients,Res,RJ,1);
// }