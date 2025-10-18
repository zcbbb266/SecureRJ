#pragma once

#include <type_traits>
#include <omp.h>
#include "../network/clients.hpp"
#include "../broker/broker.hpp"


template <size_t N>
struct select_type {
    using type = std::conditional_t<
        (N <= 16), int16_t,
        std::conditional_t<
            (N <= 32), int32_t,
            int64_t
        >
    >;
};


Value gen_rand_pair(Clients& clients,size_t size,size_t fb=0)
{
    auto r=gen_rand_vector(size);
    
    Value r1(std::move(r));r1.set_fraction_bits(fb);

    auto r2 = clients.SendRoud(r1);

    std::vector<int64_t> res(2*size);

    for(int i=0;i<r.size();i++) {
        res[2*i]=r2[i];res[2*i+1]=r1[i];
    }

    Value R(std::move(res));R.set_fraction_bits(fb);R.set_share_type(1);

    return R;
}

Value One2(Clients& clients,const Value& a)
{
    auto a2=clients.SendRoud(a);
    std::vector<int64_t> res;
    for(int i=0;i<a.size();i++) 
    {
        res.push_back(a2[i]);res.push_back(a[i]);
    }
    Value R(std::move(res));R.set_fraction_bits(a.get_fraction_bits());
    return R;
}



Value make_constF(Clients& clients,double constNumber,size_t sz,size_t fp=0)
{
    size_t Pid=clients.getPid();
    std::vector<double> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=constNumber;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=constNumber;
        }
    }
    Value res(c,fp);res.set_share_type(0);
    return res;
   
}

Value make_constI(Clients& clients,int64_t constNumber,size_t sz,size_t fp=0)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=constNumber;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=constNumber;
        }
    }
    Value res(std::move(c));res.set_share_type(0);
    return res;
   
}
template<size_t N>
std::vector<std::bitset<N>> make_constI_bitset(Clients& clients,int64_t constNumber,size_t sz,size_t fp=0)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=constNumber;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=constNumber;
        }
    }
    return c;
   
}
Value32 make_constI32(Clients& clients,int64_t constNumber,size_t sz,size_t fp=0)
{
    size_t Pid=clients.getPid();
    std::vector<int32_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=constNumber;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=constNumber;
        }
    }
    Value32 res(std::move(c));res.set_share_type(0);
    return res;
   
}
template<size_t N>
std::vector<std::bitset<N>> make_constIbitset(Clients& clients,int64_t constNumber,size_t sz,size_t fp=0)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=constNumber;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=constNumber;
        }
    }
    return c;
   
}
Value gen_rand_Value(Clients& clients,size_t sz,size_t type)
{
    auto r1=generate_int64_numbers(sz);
    Value R1(std::move(r1));R1.set_fraction_bits(0);R1.set_share_type(0);
    auto R2=clients.SendRoud(R1);
    std::vector<int64_t> res(R1.size()*2);
    for(int i=0;i<R1.size();i++) res[2*i]=R2[i],res[2*i+1]=R1[i];
    Value R(std::move(res));R.set_fraction_bits(0);R.set_share_type(type);
    return R;
}

Value make_constI(Clients& clients,std::vector<int64_t> val)
{
    auto sz=val.size();
    size_t Pid=clients.getPid();

    std::vector<int64_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=val[i];
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=val[i];
        }
    }
    Value res(std::move(c));res.set_share_type(0);
    return res;
}

Value make_dummyA(Clients& clients,size_t sz,size_t seed)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=int64_t(seed)*1e9+i;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=int64_t(seed)*1e9+i;
        }
    }
    Value res(std::move(c));res.set_share_type(0);
    return res;
   
}

Value make_dummy(Clients& clients,size_t sz,size_t seed)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=int64_t(seed)*1e9+i;
            c[2*i+1]=int64_t(seed)*1e9+i;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=int64_t(seed)*1e9+i;
            c[2*i+1]=int64_t(seed)*1e9+i;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=int64_t(seed)*1e9+i;
            c[2*i+1]=int64_t(seed)*1e9+i;
        }
    }
    Value res(std::move(c));res.set_share_type(1);
    return res;
   
}

Value make_constrangA(Clients& clients,size_t sz,size_t st=0)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=i+st;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=i+st;
        }
    }
    Value res(std::move(c));res.set_share_type(0);
    return res;
   
}
template<size_t N>
std::vector<std::bitset<N>> make_constrangA_bitset(Clients& clients,size_t sz,size_t st=0)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> c(2*sz);
    if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=i+st;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=i+st;
        }
    }
    return c;
   
}


Value make_constrang(Clients& clients,size_t sz,size_t offset=0)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(2*sz);
    for(int i=0;i<sz;i++) 
    {
        c[2*i]=i+offset;
        c[2*i+1]=i+offset;
    }
    Value res(std::move(c));res.set_share_type(1);
    return res;
   
}
template<size_t N>
std::vector<std::bitset<N>> make_constrang_bitset(Clients& clients,size_t sz,size_t offset=0)
{
    size_t Pid=clients.getPid();
    std::vector<std::bitset<N>> c(2*sz);
     if(Pid==0)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=i+offset;
            c[2*i+1]=0;
        }
    }
    else if(Pid==1)
    {
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=0;
        }
    }
    else{
        for(int i=0;i<sz;i++) 
        {
            c[2*i]=0;
            c[2*i+1]=i+offset;
        }
    }
    return c;
   
}

void combine(Value& a,Value& b)
{
    a.get_fixed_point_vector().insert(a.get_fixed_point_vector().end(),b.get_fixed_point_vector().begin(),b.get_fixed_point_vector().end());
}

void combine_vector(std::vector<Value>& a,std::vector<Value>& b)
{
    for(int i=0;i<a.size();i++) combine(a[i],b[i]);
}
template<size_t N>
void combine(std::vector<std::bitset<N>>& a,std::vector<std::bitset<N>>& b)
{
    a.insert(a.end(),b.begin(),b.end());
}

void insert_front(Value& a,Value& b)
{
    a.get_fixed_point_vector().insert(a.get_fixed_point_vector().begin(),b.get_fixed_point_vector().begin(),b.get_fixed_point_vector().end());
}

void insert_front_vector(std::vector<Value>& a,std::vector<Value>& b)
{
    for(int i=0;i<a.size();i++) insert_front(a[i],b[i]);
}


void reverse(Value& a)
{
    for(int i=0;i<a.size()/2;i+=2)
    {
        std::swap(a[i],a[a.size()-i-2]);
        std::swap(a[i+1],a[a.size()-i-1]);
    }
}
template<size_t N>
void reverse(std::vector<std::bitset<N>>& a)
{
    for(int i=0;i<a.size()/2;i+=2)
    {
        std::swap(a[i],a[a.size()-i-2]);
        std::swap(a[i+1],a[a.size()-i-1]);
    }
}

void reverse_forward(Clients& clients,Value& a)
{
    for(int i=0;i<((a.size()-2)/2)-1;i+=2)
    {
        std::swap(a[i],a[a.size()-i-4]);
        std::swap(a[i+1],a[a.size()-i-3]);
    }
    auto pid=clients.getPid();
    if(pid==0)
    {
        a[a.size()-2]=1;
        a[a.size()-1]=0;
    }
    else if(pid==1)
    {
        a[a.size()-2]=0;
        a[a.size()-1]=0;
    }
    else{
        a[a.size()-2]=0;
        a[a.size()-1]=1;
    }

}

Value make_constB(int64_t constNumber,size_t sz)
{
    std::vector<int64_t> c(2*sz);
    for(int i=0;i<sz;i++) 
    {
        c[2*i]=constNumber;
        c[2*i+1]=constNumber;
    }
    Value res(std::move(c));res.set_share_type(1);
    return res;
   
}


template<size_t N>
std::vector<std::bitset<N>> make_constB(int64_t constNumber,size_t sz)
{
    std::vector<std::bitset<N>> c(2*sz);
    for(int i=0;i<sz;i++) 
    {
        c[2*i]=constNumber;
        c[2*i+1]=constNumber;
    }
    return c;
   
}



Value make_constB(std::vector<int64_t> val)
{
    std::vector<int64_t> c(2*val.size());
    for(int i=0;i<val.size();i++) 
    {
        c[2*i]=val[i];
        c[2*i+1]=val[i];
    }
    Value res(std::move(c));res.set_share_type(1);
    return res;
   
}

Value128 make_constB128(__int128 constNumber,size_t sz)
{
    std::vector<__int128> c(2*sz);
    for(int i=0;i<sz;i++) 
    {
        c[2*i]=constNumber;
        c[2*i+1]=constNumber;
    }
    Value128 res(std::move(c));res.set_share_type(1);
    return res;
   
}

Value make_ones(Clients& clients,size_t sz)
{
    size_t Pid=clients.getPid();
    std::vector<int64_t> c(sz*2);
    for(int i=0;i<sz;i++)
    {
        c[2*i]=1;
        c[2*i+1]=1; 
    }
    Value res(std::move(c));res.set_fraction_bits(0);res.set_share_type(1);
    return res;
}

Value make_ones_64bit(Clients& clients, size_t sz) {
    size_t Pid = clients.getPid();
    std::vector<int64_t> c(sz*2);
    for (int i = 0; i < sz; i++) {
        c[2*i]=(std::numeric_limits<uint64_t>::max());
        c[2*i+1]=(std::numeric_limits<uint64_t>::max());
    }
    Value res(std::move(c));
    res.set_fraction_bits(0);
    res.set_share_type(1);
    return res;
}

Value128 expan128(Clients& clients,const Value128& a,size_t sz)
{
    if(sz%a.size()!=0) throw std::runtime_error("size error");
    std::vector<__int128> temp(sz);
    for(int i=0;i<sz;i++) 
    {
        temp[i]=a[i%(a.size())];
    }
    Value128 R(std::move(temp));R.set_fraction_bits(a.get_fraction_bits());R.set_share_type(a.get_share_type());
    return R;
}

Value expan(Clients& clients,const Value& a,size_t sz)
{
    if(sz%a.size()!=0) throw std::runtime_error("size error");
    std::vector<int64_t> temp(sz);
    for(int i=0;i<sz;i++) 
    {
        temp[i]=a[i%(a.size())];
    }
    Value R(std::move(temp));R.set_fraction_bits(a.get_fraction_bits());R.set_share_type(a.get_share_type());
    return R;
}

template<size_t N>
std::vector<std::bitset<N>> expan(Clients& clients,const std::vector<std::bitset<N>>& a,size_t sz)
{
    if(sz%a.size()!=0) throw std::runtime_error("size error");
    std::vector<std::bitset<N>> temp(sz);
    for(int i=0;i<sz;i++) 
    {
        temp[i]=a[i%(a.size())];
    }
    return temp;
}



std::vector<Value> pad(Clients& clients,const std::vector<Value>& a,size_t sz)
{
    std::vector<Value> res;
    for(int i=0;i<a.size();i++)
    {
        auto arr=a[i].get_const_vector();
        auto padding=a[i].get_share_type()==0?make_constI(clients,0,sz):make_constB(0,sz);
        arr.insert(arr.end(),padding.get_const_vector().begin(),padding.get_const_vector().end());
        Value r(std::move(arr));r.set_fraction_bits(a[i].get_fraction_bits());r.set_share_type(a[i].get_share_type());
        res.push_back(r);
    }
    return res;
}
template<size_t N>
std::vector<std::vector<std::bitset<N>>> pad(Clients& clients,const std::vector<std::vector<std::bitset<N>>>& a,size_t sz)
{
    std::vector<std::vector<std::bitset<N>>> res;
    for(int i=0;i<a.size();i++)
    {
        auto arr=a[i];
        auto padding=make_constI_bitset<N>(clients,0,sz);
        arr.insert(arr.end(),padding.begin(),padding.end());
        res.push_back(arr);
    }
    return res;
}

void print_int128(__int128 n)
{
    if (n == 0) {
        std::cout << "0";  // 如果为零，直接输出
        return;
    }

    if (n < 0) {  // 如果是负数，输出负号并转为正数
        std::cout << "-";
        n = -n;  // 转换为正数
    }

    if (n / 10 != 0) {  // 递归调用处理更高位
        print_int128(n / 10);
    }

    std::cout << (char)('0' + n % 10);  // 打印当前最低位
}


Value conv32To64(Clients& clients,const Value32& a)
{
    std::vector<int64_t> conv(a.size());
    for(int i=0;i<a.size();i++) conv[i]=static_cast<int64_t>(a[i]); 
    Value R(std::move(conv));R.set_share_type(a.get_share_type());R.set_fraction_bits(a.get_fraction_bits());
    return R;
}

Value32 conv64To32(Clients& clients,const Value& a)
{
    std::vector<int32_t> conv(a.size());
    for(int i=0;i<a.size();i++) conv[i]=static_cast<int32_t>(a[i]); 
    Value32 R(std::move(conv));R.set_share_type(a.get_share_type());R.set_fraction_bits(a.get_fraction_bits());
    return R;
}
template<size_t N>
std::vector<std::bitset<N>> conv64Tobitset(Clients& clients,const Value& a)
{
    std::vector<std::bitset<N>> conv(a.size());
    for(int i=0;i<a.size();i++) conv[i]=a[i]; 
    return conv;
}

template<size_t N>
Value convbitsetTo64(Clients& clients,const std::vector<std::bitset<N>>& a)
{
    std::vector<int64_t> conv(a.size());
    for(int i=0;i<a.size();i++) conv[i]=a[i].to_ulong(); 
    return Value(std::move(conv));
}


template<size_t N>
typename select_type<N>::type revelToint(const std::bitset<N>& a)
{
    typename select_type<N>::type res;
    if(a[N-1]==1)
    {   
        std::bitset<64-N> raw;
        raw.set();
        return static_cast<typename select_type<N>::type>((raw.to_ullong()<<N)^a.to_ullong());
    }
    
    res=static_cast<typename select_type<N>::type>(a.to_ullong());
    return res;
}


template<size_t N>
std::vector<typename select_type<N>::type> convTointV(const std::vector<std::bitset<N>>& a)
{
    std::vector<typename select_type<N>::type> res(a.size());
    for(int i=0;i<a.size();i++)
        res[i]=static_cast<typename select_type<N>::type>(a[i].to_ullong());
    return res;
}

template<size_t N>
typename select_type<N>::type convToint(const std::bitset<N>& a)
{

    return static_cast<typename select_type<N>::type>(a.to_ullong());

}

