#pragma once

#include "../Operator/operator.h"




Value GroupbyMask(Clients& clients,const std::vector<Value>& a,bool rev=0){

    size_t len=a[0].size()-2;
    std::vector<int64_t> from_top(len),from_button(len);
    auto aa=a[0].get_const_vector();
    std::copy(aa.begin(),aa.end()-2,from_top.begin());
    std::copy(aa.begin()+2,aa.end(),from_button.begin());
    Value FT(std::move(from_top));FT.set_fraction_bits(a[0].get_fraction_bits());FT.set_share_type(a[0].get_share_type());
    Value FB(std::move(from_button));FB.set_fraction_bits(a[0].get_fraction_bits());FB.set_share_type(a[0].get_share_type());
    auto result=Neq(clients,FT,FB);

    for(int i=1;i<a.size();i++){
        std::vector<int64_t> cur_from_top(len),cur_from_button(len);
        auto cur_aa=a[i].get_const_vector();
        std::copy(cur_aa.begin(),cur_aa.end()-2,cur_from_top.begin());
        std::copy(cur_aa.begin()+2,cur_aa.end(),cur_from_button.begin());
        Value cur_FT(std::move(cur_from_top));cur_FT.set_fraction_bits(a[0].get_fraction_bits());cur_FT.set_share_type(a[0].get_share_type());
        Value cur_FB(std::move(cur_from_button));cur_FB.set_fraction_bits(a[0].get_fraction_bits());cur_FB.set_share_type(a[0].get_share_type());
        result=std::move(Or(clients,result,Neq(clients,cur_FT,cur_FB)));
    }
    if(rev==0)
    {
      result.get_fixed_point_vector().push_back(1);
      result.get_fixed_point_vector().push_back(1);
    }
    else
    {
      result.get_fixed_point_vector().insert(result.get_fixed_point_vector().begin(),1);
      result.get_fixed_point_vector().insert(result.get_fixed_point_vector().begin(),1);
    }
    return result;
} 

Value GroupbyMaskTs(Clients& clients,const Value& a,bool rev=0){
  auto a_not=Add(clients,make_constI(clients,1,a.size()/2),neg(clients,a));
  auto r=std::move(a_not.get_fixed_point_vector());
  if(rev==0)
  {
    r.erase(r.end()-2,r.end());
    r.insert(r.begin(),0);  r.insert(r.begin(),0);  
  }
  else
  {
    r.erase(r.begin(),r.begin()+2);
    r.push_back(0);  r.push_back(0);  
  }
  a_not=std::move(r);
  return a_not;
}

Value mask_rev(const Value& a)
{
  auto a_copy=a;
  a_copy.get_fixed_point_vector().insert(a_copy.get_fixed_point_vector().begin(),a_copy[a_copy.size()-1]);
  a_copy.get_fixed_point_vector().insert(a_copy.get_fixed_point_vector().begin(),a_copy[a_copy.size()-2]);
  a_copy.get_fixed_point_vector().erase(a_copy.get_fixed_point_vector().end()-1);
  a_copy.get_fixed_point_vector().erase(a_copy.get_fixed_point_vector().end()-1);

  return a_copy;
}

template<size_t N>
std::vector<std::bitset<N>> mask_rev(const std::vector<std::bitset<N>>& a)
{
  auto a_copy=a;
  a_copy.insert(a_copy.begin(),a_copy[a_copy.size()-1]);
  a_copy.insert(a_copy.begin(),a_copy[a_copy.size()-2]);
  a_copy.erase(a_copy.end()-1);
  a_copy.erase(a_copy.end()-1);

  return a_copy;
}

template<size_t N>
std::vector<std::bitset<N>> GroupbyMaskTs(Clients& clients,const std::vector<std::bitset<N>>& a,bool rev=0){
  auto a_copy=Add_bitset(clients,make_constI_bitset<N>(clients,1,a.size()/2),Aneg_bitset(clients,a));
  if(rev==0)
  { 
    a_copy.erase(a_copy.end()-2,a_copy.end());
    a_copy.insert(a_copy.begin(),0);  a_copy.insert(a_copy.begin(),0);
  }
  else
  {
    a_copy.erase(a_copy.begin(),a_copy.begin()+2);
    a_copy.push_back(0);  a_copy.push_back(0);
  }
  return a_copy;
}



constexpr size_t bit_width(uint32_t n) {
    size_t width = 0;
    while (n > 0) {
        n >>= 1;
        ++width;
    }
    return width;
}

std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> get_index_pair(size_t n){
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ret;
    size_t k = bit_width(n - 1u);
    if(n<2) return ret;

    for (size_t i = 1; i < (k + 1); i++) {
    std::pair<std::vector<size_t>,std::vector<size_t>> it;
        for (size_t j = ((1 << i) - 1); j < n; j += (1 << i)) {
            it.first.push_back(j);
            it.second.push_back(j - (1 << (i - 1)));
        }
        if (!it.first.empty()) {
            ret.push_back(it);
        }
  }

  for (size_t i = (k - 1); i > 0; i--) {
    std::pair<std::vector<size_t>,std::vector<size_t>> it;
    for (size_t j = (3 * (1 << (i - 1)) - 1); j < n; j += (1 << i)) {
      it.first.push_back(j);
      it.second.push_back(j - (1 << (i - 1)));
    }

    if (!it.first.empty()) {
      ret.push_back(it);
    }
  }
  return ret;
}


std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> get_index_pair_rev(size_t n){
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ret;
    size_t k = bit_width(n - 1u);
    if(n<2) return ret;

    for (size_t i = 1; i < (k + 1); i++) {
    std::pair<std::vector<size_t>,std::vector<size_t>> it;
        for (int j =n-1- ((1 << i) - 1); j >=0; j -= (1 << i)) {
            it.first.push_back(j);
            it.second.push_back(j + (1 << (i - 1)));
        }
        if (!it.first.empty()) {
            ret.push_back(it);
        }
  }

  for (size_t i = (k - 1); i > 0; i--) {
    std::pair<std::vector<size_t>,std::vector<size_t>> it;
    for (int j = n-1-(3 * (1 << (i - 1)) - 1); j >=0; j -= (1 << i)) {
      it.first.push_back(j);
      it.second.push_back(j + (1 << (i - 1)));
    }

    if (!it.first.empty()) {
      ret.push_back(it);
    }
  }
  return ret;
} 

std::vector<Value>GroupbySum(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,bool rev=0)
{
    auto gms_B=GroupbyMask(clients,b,rev);
    Value ones(std::move(std::vector<int64_t>(gms_B.size()/2,1)));
    auto gms=Mul_ab1(clients,ones,gms_B,0);
    auto m=GroupbyMaskTs(clients,gms,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
  
    int allsz=0;
    std::vector<Value> res;

    for(int i=0;i<a.size();i++)
    {
      auto v=(a[i].get_share_type()==0)?a[i]:B2A(clients,a[i]);
      // auto v=a[i];

      auto mask=m; 

      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);

        auto newgv=Mul_vector(clients,std::vector<Value>{lg,lg},std::vector<Value>{rg,rv});
        auto newg=newgv[0];
        auto newv=Add(clients,lv,newgv[1]);


        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
    res.push_back(gms_B);
    return res;
}
std::vector<Value>GroupbyXor(Clients& clients,const std::vector<Value>& a,const std::vector<Value>& b,bool rev=0)
{
    auto gms_B=GroupbyMask(clients,b,rev);
    Value ones(std::move(std::vector<int64_t>(gms_B.size()/2,1)));
    auto gms=Mul_ab1(clients,ones,gms_B,0);
    auto m=GroupbyMaskTs(clients,gms,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
  
    int allsz=0;
    std::vector<Value> res;

    for(int i=0;i<a.size();i++)
    {
      auto v=(a[i].get_share_type()==0)?a[i]:B2A(clients,a[i]);
      // auto v=a[i];

      auto mask=m; 

      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);

        auto rvminlv=Add(clients,rv,neg(clients,lv));  
        auto newgv=Mul_vector(clients,std::vector<Value>{lg,lg},std::vector<Value>{rg,rvminlv});
        auto newg=newgv[0];
        auto newv=Add(clients,lv,newgv[1]);


        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
    res.push_back(gms_B);
    return res;
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> GroupbySum(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<Value>& b,bool rev=0)
{
    auto gms_B=GroupbyMask(clients,b,rev);
    auto gms=conv2ToP<N>(clients,gms_B);
    auto m=GroupbyMaskTs(clients,gms,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
    std::cout<<"hehre"<<'\n';
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::bitset<N>>> res;

    for(int i=0;i<a.size();i++)
    {
      auto v=a[i];

      auto mask=m; 

      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);
        auto newg=Mul_bitset(clients,lg,rg);
        auto newv=Add_bitset(clients,lv,Mul_bitset(clients,lg,rv));

        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
       auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    // res.push_back(gms_B);
    return res;
}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> GroupbySum_withmask(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::bitset<N>>& mask,bool rev=0)
{
    auto m=GroupbyMaskTs(clients,mask,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  

    std::vector<std::vector<std::bitset<N>>> res;

    for(int i=0;i<a.size();i++)
    {
      auto v=a[i];

      auto mask=m; 

      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);
        auto newg=Mul_bitset(clients,lg,rg);
        auto newv=Add_bitset(clients,lv,Mul_bitset(clients,lg,rv));

        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
      
    // res.push_back(gms_B);
    return res;
}

// template<size_t N>
// std::vector<std::vector<std::bitset<N>>> GroupbySum_withmask(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::bitset<N>>& gms,bool rev=0)
// {
//   std::vector<std::vector<std::bitset<N>>> a_copy=a;
//   std::vector<std::bitset<N>> gms_copy=gms;
//   if(rev==0) 
//   {
//     for(int i=0;i<a.size();i++) reverse(a_copy[i]);
//     reverse(gms_copy);
//   }
//   auto pem=Permutation_gen_bitset(clients,gms_copy,0);
          
//   std::vector<std::vector<std::bitset<N>>> ps(a.size());

//   for(int i=0;i<a.size();i++){
//     ps[i]=perfix_sum_bitset(clients,a_copy[i]);
//   }
//   std::vector<std::vector<std::bitset<N>>> lastelm(a.size());
//   for(int i=0;i<a.size();i++){
//     std::vector<std::bitset<N>> last(2);
//     last[0]=ps[i][ps[i].size()-2];
//     last[1]=ps[i][ps[i].size()-1];
//     lastelm[i]=expan(clients,last,ps[i].size());
    
//   }


//   for(int i=0;i<a.size();i++) ps[i]=Add_bitset(clients,Mul_bitset(clients,Add_bitset(clients,ps[i],Aneg_bitset(clients,lastelm[i])),gms_copy),lastelm[i]);

//   ps=Permutation_bitset(clients,ps,pem);

//   for(int i=0;i<a.size();i++)
//   { 
//     std::vector<std::bitset<N>> r(ps[i].size());
//     r[0]=-convToint(ps[i][0]),r[1]=-convToint(ps[i][1]);
//     for(int j=2;j<ps[i].size();j+=2)
//     {
//       r[j]=-convToint(ps[i][j])+convToint(ps[i][j-2]);
//       r[j+1]=-convToint(ps[i][j+1])+convToint(ps[i][j-1]);
//     }
//     ps[i]=r;
//   }

//   ps=Permutation_bitset_rev(clients,ps,pem);

//   for(int i=0;i<ps.size();i++)
//   {
//       ps[i].erase(ps[i].end()-1);ps[i].erase(ps[i].end()-1);
//       ps[i].insert(ps[i].begin(),0);ps[i].insert(ps[i].begin(),0);
//   }

//   for(int i=0;i<ps.size();i++) ps[i]=Add_bitset(clients,ps[i],a_copy[i]);
//   for(int i=0;i<ps.size();i++) ps[i]=perfix_sum_bitset(clients,ps[i]);
//   for(int i=0;i<ps.size();i++) reverse(ps[i]);
//   return ps;
// }

std::vector<Value> GroupbySum_withmask(Clients& clients,const std::vector<Value>& a,const Value& mask,bool rev=0)
{
    auto m=GroupbyMaskTs(clients,mask,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
    std::vector<Value> res;

    for(int i=0;i<a.size();i++)
    {
      auto v=(a[i].get_share_type()==0)?a[i]:B2A(clients,a[i]);
      auto mask=m; 
      for(const auto& index:ScanIndex){
          auto lv=Gather(clients,v,index.first);
          auto lg=Gather(clients,mask,index.first);
          auto rv=Gather(clients,v,index.second);
          auto rg=Gather(clients,mask,index.second);
          auto newgv=Mul_vector(clients,std::vector<Value>{lg,lg},std::vector<Value>{rg,rv});
          auto newg=newgv[0];
          auto newv=Add(clients,lv,newgv[1]);
          Scatter(clients,v,newv,index.first);
          Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
    
    return res;
}

std::vector<Value> perfix_sum_seg(Clients& clients,const std::vector<Value>& a,const Value& b,bool rev=0)
{
    if(a[0].size()==2){
      return a;
    }
    Value b_copy;
    if(rev==1)
      b_copy=mask_rev(b);
    else b_copy=b;
    auto m=GroupbyMaskTs(clients,b_copy,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
    std::vector<Value> res;
    for(int i=0;i<a.size();i++)
    {
      auto v=(a[i].get_share_type()==0)?a[i]:B2A(clients,a[i]);
      auto mask=m; 
      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);
        auto rvminlv=Add(clients,rv,neg(clients,lv));  
        auto newgv=Mul_vector(clients,std::vector<Value>{lg,lg},std::vector<Value>{rg,rvminlv});
        auto newg=newgv[0];
        auto newv=Add(clients,lv,newgv[1]);
        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
      
    return res;

}

template<size_t N>
std::vector<std::vector<std::bitset<N>>> perfix_sum_seg(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,const std::vector<std::bitset<N>>& b,bool rev=0)
{
    if(a[0].size()==2){
      return a;
    }
    std::vector<std::bitset<N>> b_copy;
    if(rev==1)
      b_copy=mask_rev(b);
    else b_copy=b;

    auto m=GroupbyMaskTs(clients,b_copy,rev);
    std::vector<std::pair<std::vector<size_t>,std::vector<size_t>>> ScanIndex;
    if(rev==0)
      ScanIndex =get_index_pair(a[0].size()/2);
    else
      ScanIndex =get_index_pair_rev(a[0].size()/2);
  
    std::vector<std::vector<std::bitset<N>>> res;
    for(int i=0;i<a.size();i++)
    {
      auto v=a[i];
      auto mask=m; 
      for(const auto& index:ScanIndex){
        auto lv=Gather(clients,v,index.first);
        auto lg=Gather(clients,mask,index.first);
        auto rv=Gather(clients,v,index.second);
        auto rg=Gather(clients,mask,index.second);
        auto rvminlv=Add_bitset(clients,rv,Aneg_bitset(clients,lv));
        auto newg=Mul_bitset(clients,lg,rg);
        auto newv=Mul_bitset(clients,lg,rvminlv);
        newv=Add_bitset(clients,lv,newv);
        Scatter(clients,v,newv,index.first);
        Scatter(clients,mask,newg,index.first);
      }
      res.push_back(v);
    }
      
    return res;

}

