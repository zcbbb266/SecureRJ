#pragma once

#include "../network/clients.hpp"
#include "../broker/broker.hpp"

Value Ot(Clients& clients,const Value& m1,const Value& m2,const Value& b,size_t sender,size_t receiver)
{
    auto Pid=clients.getPid();
    size_t helper;
    for(int i=0;i<3;i++) if(sender!=i&&receiver!=i) helper=i;
    if(Pid==sender)
    {
        auto seeds=generate_int64_numbers(4);
        clients.SendTo(seeds,helper);
        auto w1=Value(std::move(gen_rand_vector_with_key(m1.size(),seeds[0],seeds[1])));
        auto w2=Value(std::move(gen_rand_vector_with_key(m1.size(),seeds[2],seeds[3])));
        for(int i=0;i<w1.size();i++){
            w1[i]=w1[i]^m1[i];
            w2[i]=w2[i]^m2[i];
        }
        clients.SendTo(w1,receiver);
        clients.SendTo(w2,receiver);
        return Value();
    }
    else if(Pid==helper)
    {
        auto seeds=clients.recv(sender);
        auto b=clients.recv(receiver);
        auto w1=Value(std::move(gen_rand_vector_with_key(b.size(),seeds[0],seeds[1])));
        auto w2=Value(std::move(gen_rand_vector_with_key(b.size(),seeds[2],seeds[3])));
        std::vector<int64_t> temp(b.size());
        for(int i=0;i<b.size();i++)
        {
            if(b[i]==0) temp[i]=w1[i];
            else temp[i]=w2[i];
        }
        Value res(std::move(temp));res.set_fraction_bits(m1.get_fraction_bits());res.set_share_type(m1.get_share_type());
        clients.SendTo(res,receiver);
        return Value();
    }
    else{
        clients.SendTo(b,helper);
        auto k1=clients.recv(sender);
        auto k2=clients.recv(sender);
        auto temp=clients.recv(helper);
        std::vector<int64_t> res(temp.size());
        for(int i=0;i<res.size();i++)
        {
            if(b[i]==0) res[i]=temp[i]^k1[i];
            else res[i]=temp[i]^k2[i];
        } 
        Value R(std::move(res));R.set_fraction_bits(temp.get_fraction_bits());R.set_share_type(temp.get_share_type());
        return R;
    }

}

template<size_t N>
std::vector<std::bitset<N>> Ot(Clients& clients,const std::vector<std::bitset<N>>& m1,const std::vector<std::bitset<N>>& m2,const std::vector<std::bitset<N>>& b,size_t sender,size_t receiver)
{
    auto Pid=clients.getPid();
    size_t helper;
    for(int i=0;i<3;i++) if(sender!=i&&receiver!=i) helper=i;
    if(Pid==sender)
    {
        auto seeds=generate_int64_numbers(4);
        clients.SendTo(seeds,helper);
        auto w1=gen_rand_bitestvector_with_key<N>(m1.size(),seeds[0],seeds[1]);
        auto w2=gen_rand_bitestvector_with_key<N>(m1.size(),seeds[2],seeds[3]);
        for(int i=0;i<w1.size();i++){
            w1[i]=w1[i]^m1[i];
            w2[i]=w2[i]^m2[i];
        }
        clients.SendTo_bitset(w1,receiver);
        clients.SendTo_bitset(w2,receiver);
        return std::vector<std::bitset<N>>();
    }
    else if(Pid==helper)
    {
        auto seeds=clients.recv(sender);
        auto b=clients.recv_bitset<N>(receiver);
        auto w1=gen_rand_bitestvector_with_key<N>(b.size(),seeds[0],seeds[1]);
        auto w2=gen_rand_bitestvector_with_key<N>(b.size(),seeds[2],seeds[3]);
        std::vector<std::bitset<N>> temp(b.size());
        for(int i=0;i<b.size();i++)
        {
            if(b[i]==0) temp[i]=w1[i];
            else temp[i]=w2[i];
        }
        clients.SendTo_bitset(temp,receiver);
        return std::vector<std::bitset<N>>();
    }
    else{
        clients.SendTo_bitset(b,helper);
        auto k1=clients.recv_bitset<N>(sender);
        auto k2=clients.recv_bitset<N>(sender);
        auto temp=clients.recv_bitset<N>(helper);
        std::vector<std::bitset<N>> res(temp.size());
        for(int i=0;i<res.size();i++)
        {
            if(b[i]==0) res[i]=temp[i]^k1[i];
            else res[i]=temp[i]^k2[i];
        } 
        return res;
    }

}