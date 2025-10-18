#include "network/client.hpp"
#include "Value/value.h"
#include "Value/value128.h"
#include "network/clients.hpp"
#include "broker/broker.hpp"
#include "Operator/operator.h"
#include "LowMC/Lowmc.hpp"
#include "SqlOperator/Permutation.hpp"
#include "SqlOperator/Groupby.hpp"
#include "SqlOperator/Sorter.hpp"
#include "ThreePartyOT/Ot.hpp"
#include "SqlOperator/Psi.hpp"
#include "SqlOperator/Join.hpp"
#include "SqlOperator/Expansion.hpp"
#include "SqlOperator/Sampling.hpp"

#include <thread>
#include <vector>
#include <memory>
#include <chrono> 


// int main(int argc, char* argv[]) {
//     int Party_number = *argv[1] - '0';
//     int pid = *argv[2] - '0';
//     std::cout<<Party_number<<" "<<pid<<'\n';
//     auto clients = client_behavior(Party_number, pid);
    

//     if (pid == 0) clients[0][1]->send("nima 1");
//     if (pid == 1) clients[1][0]->send("nima 0");
//     if (pid == 2) clients[2][0]->send("nima 0 froawdawddwddddddddddddddddddddddddddddddddddm 3");
//     std::this_thread::sleep_for(std::chrono::seconds(1)); 
//     return 0;
// }

void test1(size_t Pid,size_t Party_number,size_t port)
{
     Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        std::vector<int64_t> a(1000000,10);
        Value vala(std::move(a));

        auto val1=AmakeShare(clients,0,vala);
        auto val2=AmakeShare(clients,1);
        auto conv1=conv64Tobitset<32>(clients,val1);
        auto conv2=conv64Tobitset<32>(clients,val2);
        auto res=Mul_bitset<32>(clients,conv1,conv2);
        auto res2=Mul_bitset<32>(clients,conv1,conv2);
        // auto res=Mul(clients,val1,val2);
        // auto res=clients.SendRoud_bitset<32>(conv1);
        // auto res=div_A4I(clients,val1,val2);
        
        // auto tres=reconstruct(clients,res);
        // auto tres=Areconstruct_bitset<64>(clients,res);

    }
    else if(Pid==1)
    {
        std::vector<int64_t> a(1000000,20);
        Value vala(std::move(a));
        auto val1=AmakeShare(clients,0);
        auto val2=AmakeShare(clients,1,vala);
        auto conv1=conv64Tobitset<32>(clients,val1);
        auto conv2=conv64Tobitset<32>(clients,val2);
        auto res=Mul_bitset<32>(clients,conv1,conv2);
        auto res2=Mul_bitset<32>(clients,conv1,conv2);
        // auto res=Mul(clients,val1,val2);
        // auto res=clients.SendRoud_bitset<32>(conv1);
        // auto res=div_A4I(clients,val1,val2);
        // auto tres=reconstruct(clients,res);
        // auto tres=Areconstruct_bitset<64>(clients,res);

    }
    else if(Pid==2)
    {
        auto val1=AmakeShare(clients,0);
        auto val2=AmakeShare(clients,1);
        auto conv1=conv64Tobitset<32>(clients,val1);
        auto conv2=conv64Tobitset<32>(clients,val2);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Mul_bitset<32>(clients,conv1,conv2);
        auto res2=Mul_bitset<32>(clients,conv1,conv2);
        // auto res=Mul(clients,val1,val2);
        // auto res=clients.SendRoud_bitset<32>(conv1);
        // auto res=div_A4I(clients,val1,val2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto tres=reconstruct(clients,res);
        // for(int i=0;i<10;i++)
        // {
        //     std::cout<<tres[i]/1024.0/32.0<<" ";
        // }
        // auto tres=Areconstruct_bitset<64>(clients,res);
        // for(int i=0;i<100;i++) std::cout<<tres[i]<<" ";
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); // 等待对方监听启动
    clients.close();
}

void test2(size_t Pid,size_t Party_number,size_t port)
{
     Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        std::vector<int64_t> a={-1,2,3,-1,5};
        // std::vector<int64_t> a=gen_rand_vector(1000000);
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0,vala);
        auto val2=BmakeShare(clients,1);
        auto res=ppa_kogge_stone_sub(clients,val1,val2);
        // auto res=And(clients,val1,val2);
        // auto res=And(clients,val1,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        std::vector<int64_t> a={6,7,8,-459,5};
        // std::vector<int64_t> a=gen_rand_vector(1000000); 
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1,vala);
        auto res=ppa_kogge_stone_sub(clients,val1,val2);
        // auto res=And(clients,val1,val2);

        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=ppa_kogge_stone_sub(clients,val1,val2);
        // auto res=And(clients,val1,val2);
        // auto res=And(clients,val1,val2);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]<<" ";
        } 
    }
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}



void test3(size_t Pid,size_t Party_number)
{
     Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        std::vector<int64_t> a={1,222,333,452};
        // std::vector<int64_t> a=gen_rand_vector(1000000);
        Value vala(std::move(a));
        auto val1=AmakeShare(clients,0,vala);
        auto res=A2B(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        
        auto val1=AmakeShare(clients,0);
        auto res=A2B(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=AmakeShare(clients,0);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=A2B(clients,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]<<" ";
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test4(size_t Pid,size_t Party_number)
{
     Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        // auto a=gen_rand_vector(1000000);
        std::vector<int64_t> a={1542,2873,3783,4783,4535};
        // std::vector<int64_t> a={1,1,1,1,1};
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0,vala);
        auto val2=neg(clients,val1);
        auto res=B2A(clients,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        
        auto val1=BmakeShare(clients,0);
        auto val2=neg(clients,val1);
        auto res=B2A(clients,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=neg(clients,val1);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=B2A(clients,val2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]<<" ";
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test5(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        std::vector<int64_t> a={-1542,2873,-3783,4783,4535};
        // std::vector<int64_t> a={1,1,1,1,1};
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0,vala);
        auto res=msb(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        
        auto val1=BmakeShare(clients,0);
        auto res=msb(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto res=msb(clients,val1);
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]<<" ";
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test6(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        std::vector<int64_t> a={1542,453,3783,4783,-7864};
        // std::vector<int64_t> a=gen_rand_vector(1000000);
        // std::vector<int64_t> a={1,1,1,1,1};
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0,vala);
        auto val2=BmakeShare(clients,1);
        auto res=eq(clients,val1,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        std::vector<int64_t> a={45345,453,7863,4533,-7864};
        // std::vector<int64_t> a=gen_rand_vector(1000000);
        Value vala(std::move(a));
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1,vala);
        auto res=eq(clients,val1,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=eq(clients,val1,val2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        auto tres=reconstruct(clients,res);   
        for(int i=0;i<tres.size();i++)
        { 
            std::cout<<tres[i]<<" ";
        }
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test7(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        auto val=make_constF(clients,10.23,1000000,15);
        auto tres=reconstruct(clients,val);
    }
    else if(Pid==1)
    {
        auto val=make_constF(clients,10.23,1000000,15);
        auto tres=reconstruct(clients,val);
    }
    else if(Pid==2)
    {
        auto val=make_constF(clients,10.23,1000000,15);
        auto tres=reconstruct(clients,val);   
        for(int i=0;i<10;i++)
        {
            std::cout<<tres[i]<<" ";
        }
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test8(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        std::vector<double> a={1437.2,1};
        // std::vector<int64_t> a={1,1,1,1,1};
        Value vala(a,10);
        auto val1=BmakeShare(clients,0,vala);
        auto val2=BmakeShare(clients,1);
        auto res=higest_bit(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        std::vector<double> a={64,2};
        Value vala(a,10);
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1,vala);
        auto res=higest_bit(clients,val1);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=BmakeShare(clients,1);
        auto res=higest_bit(clients,val1);
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]<<" ";
        }
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test9(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        std::vector<double> a={-1453.45321,455852234.7832,4848234.7832,443.15614,45.5432,4536.0};
        Value vala(a,15);
        auto val1=AmakeShare(clients,0,vala);
        auto val2=AmakeShare(clients,1);
        auto res=f_Mul(clients,val1,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==1)
    {
        std::vector<double> a={25846,421852234.7832,3.415,458524.1561,5.2,6.0};
        Value vala(a,15);
        auto val1=AmakeShare(clients,0);
        auto val2=AmakeShare(clients,1,vala);
        auto res=f_Mul(clients,val1,val2);
        auto tres=reconstruct(clients,res);
    }
    else if(Pid==2)
    {
        auto val1=AmakeShare(clients,0);
        auto val2=AmakeShare(clients,1);
        auto res=f_Mul(clients,val1,val2);
        auto tres=reconstruct(clients,res);
        for(int i=0;i<tres.size();i++)
        {
            std::cout<<tres[i]/1024.0/32<<" ";
        }
        std::cout<<std::endl;

    }

    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test10(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        auto a=gen_rand_vector(1000000);
        Value vala(std::move(a));

        auto val1=AmakeShare(clients,0,vala);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Mul(clients,val1,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto b=gen_rand_vector(100000);
        // Value valb(std::move(b));
        // auto val2=AmakeShare(clients,0,valb);
        // auto start1 = std::chrono::high_resolution_clock::now();
        // for(int i=0;i<100;i++)
        // {
        //     auto res=reconstruct(clients,val2);
        // }
        // auto end1 = std::chrono::high_resolution_clock::now();
        // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
        // std::cout << "Function runtime11: " << duration1.count() << " ms" << std::endl;
    }
    else if(Pid==1)
    {
        auto val1=AmakeShare(clients,0);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Mul(clients,val1,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime2: " << duration.count() << " ms" << std::endl;
        // auto b=gen_rand_vector(100000);
        // Value valb(std::move(b));
        // auto val2=AmakeShare(clients,0);
        // auto start1 = std::chrono::high_resolution_clock::now();
        // for(int i=0;i<100;i++)
        // {
        //     auto res=reconstruct(clients,val2);
        // }
        // auto end1 = std::chrono::high_resolution_clock::now();
        // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
        // std::cout << "Function runtime22: " << duration1.count() << " ms" << std::endl;
    }
    else if(Pid==2)
    {
        auto val1=AmakeShare(clients,0);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Mul(clients,val1,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime3: " << duration.count() << " ms" << std::endl;
        // auto b=gen_rand_vector(100000);
        // Value valb(std::move(b));
        // auto val2=AmakeShare(clients,0);
        // auto start1 = std::chrono::high_resolution_clock::now();
        // for(int i=0;i<100;i++)
        // {
        //     auto res=reconstruct(clients,val2);
        // }
        // auto end1 = std::chrono::high_resolution_clock::now();
        // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
        // std::cout << "Function runtime33: " << duration1.count() << " ms" << std::endl;
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test11(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        auto a=gen_rand_vector(1000000);
        auto b=gen_rand_vector(1000000);
        // auto a=std::vector<int64_t>{10,2,13,10,17,12,2,6};
        // auto b=std::vector<int64_t>{7,4,84,45,25,41,4,84};
        Value vala(std::move(a));
        Value valb(std::move(b));
        auto val1=BmakeShare(clients,0,vala);
        auto val3=BmakeShare(clients,0,valb);
        auto val2=gen_rand_key(clients,1);
        LowMC<80> cipher(clients,val2);
        auto ccres=cipher.encrypt(clients,std::vector<Value>{val1,val3});
        // auto cres=cipher.decrypt(clients,ccres);
        // auto res=reconstruct_bitset(clients,ccres);
    }
    else if(Pid==1)
    {
        auto val1=BmakeShare(clients,0);
        auto val3=BmakeShare(clients,0);
        auto val2=gen_rand_key(clients,1);
        
        LowMC<80> cipher(clients,val2);
        auto ccres=cipher.encrypt(clients,std::vector<Value>{val1,val3});
        // auto cres=cipher.decrypt(clients,ccres);
        // auto res=reconstruct_bitset(clients,ccres);

    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        // auto val2=BmakeShare(clients,0);
        auto val3=BmakeShare(clients,0);
        auto val2=gen_rand_key(clients,1);

        LowMC<80> cipher(clients,val2);
        auto start = std::chrono::high_resolution_clock::now();
        auto ccres=cipher.encrypt(clients,std::vector<Value>{val1,val3});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto cres=cipher.decrypt(clients,ccres);
        // auto res=reconstruct_bitset(clients,ccres);

        // for(int i=0;i<res.size();i++)
        // {
        //     std::cout<<res[i]<<" ";
        // }
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test12(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
if(Pid==0)
    {
        std::vector<int64_t> temp1(2000000,0);
        int64_t t=random()%10000;
        std::vector<int64_t> a(1000000,t);
        Value vala(std::move(a));
        auto val=BmakeShare(clients,0,vala);
        // auto val=conv64to128(BmakeShare(clients,0,vala));
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<64;i++)
        {
            std:: vector<uint8_t> res(val.size(),0);
            auto temp=val[0];
            for(int i=0;i<a.size();i++)
            {
                auto cur=val[i]&temp;
                res[i] = __builtin_parityll(cur);
        
            }
            for (int j = 0; j < temp1.size(); j++) {
                temp1[j] |= static_cast<int64_t>(res[j]) << i;
            }
        }
        std::cout<<static_cast<int>(temp1[t])<<'\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    }
    else if(Pid==1)
    {
        std::vector<int64_t> temp1(2000000,0);
        int64_t t=random()%10000;
        std::vector<int64_t> a(1000000,t);
        Value vala(std::move(a));
        auto val=BmakeShare(clients,0);
        // auto val=conv64to128(BmakeShare(clients,0,vala));
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<64;i++)
        {
            std:: vector<uint8_t> res(val.size(),0);
            auto temp=val[0];
            for(int i=0;i<a.size();i++)
            {
                auto cur=val[i]&temp;
                res[i] = __builtin_parityll(cur);
        
            }
            for (int j = 0; j < temp1.size(); j++) {
                temp1[j] |= static_cast<int64_t>(res[j]) << i;
            }
        }
        std::cout<<static_cast<int>(temp1[t])<<'\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    }
    else
    {
        std::vector<int64_t> temp1(2000000,0);
        int64_t t=random()%10000;
        std::vector<int64_t> a(1000000,t);
        Value vala(std::move(a));
        auto val=BmakeShare(clients,0);
        // auto val=conv64to128(BmakeShare(clients,0,vala));
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<64;i++)
        {
            std:: vector<uint8_t> res(val.size(),0);
            auto temp=val[0];
            for(int i=0;i<a.size();i++)
            {
                auto cur=val[i]&temp;
                res[i] = __builtin_parityll(cur);
        
            }
            for (int j = 0; j < temp1.size(); j++) {
                temp1[j] |= static_cast<int64_t>(res[j]) << i;
            }
        }
        std::cout<<static_cast<int>(temp1[t])<<'\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test13(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        //1000000 353ms
        auto a=std::vector<int64_t>(1000000,1);
        // auto a=std::vector<int64_t>{1,2,3,4,5,6};
        Value vala(std::move(a));
        auto a1=std::vector<int64_t>{100,54,984,498,845,674};
        Value vala1(std::move(a1));
        std::vector<int64_t> b(1000000);
        std::iota(b.begin(),b.end(),0);

        // std::vector<int64_t> b={1,3,0,4,5,2};
        Value valb(std::move(b));
        auto val1=AmakeShare(clients,0,vala);
        auto val3=BmakeShare(clients,0,vala1);
        auto val2=AmakeShare(clients,0,valb);
        // std::vector<Value> val;
        // val.push_back(val1);
        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        std::vector<std::vector<std::bitset<24>>> val;
        val.push_back(conv1);
        // val.push_back(val3);
        // auto pem=genRandpem(clients,conv1.size()/2);
        // auto cres1=shuffle4pem_bitset<64>(clients,val,pem);
        // auto cres2=unshuffle4pem_bitset<64>(clients,s2tos3_vector_bitset(clients,0,cres1),pem);
        auto cres=Permutation_bitset<24>(clients,val,conv2);
        // auto cres=Permutation_bitset_rev<24>(clients,val,conv2);
        // print_bitset(clients,cres[0]);
    }
    else if(Pid==1)
    {
        auto val1=AmakeShare(clients,0);
        auto val3=BmakeShare(clients,0);
        auto val2=AmakeShare(clients,0);
        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        std::vector<std::vector<std::bitset<24>>> val;
        val.push_back(conv1);
        // val.push_back(val3);
        // auto pem=genRandpem(clients,conv1.size()/2);
        // auto cres1=shuffle4pem_bitset<64>(clients,val,pem);
        // auto cres2=unshuffle4pem_bitset<64>(clients,s2tos3_vector_bitset(clients,0,cres1),pem);
        auto cres=Permutation_bitset<24>(clients,val,conv2);
        // print_bitset(clients,s2tos3_bitset(clients,1,cres2[0]));
        // print_bitset(clients,cres[0]);


    }
    else if(Pid==2)
    {

        auto val1=AmakeShare(clients,0);
        auto val3=BmakeShare(clients,0);
        auto val2=AmakeShare(clients,0);
        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        std::vector<std::vector<std::bitset<24>>> val;
        val.push_back(conv1);
        // val.push_back(val3);
        auto start = std::chrono::high_resolution_clock::now();
        // auto pem=genRandpem(clients,conv1.size()/2);
        // auto cres1=shuffle4pem_bitset<64>(clients,val,pem);
        // auto cres2=unshuffle4pem_bitset<64>(clients,s2tos3_vector_bitset(clients,0,cres1),pem);
        // auto cres=Permutation_bitset_rev<24>(clients,val,conv2);
        auto cres=Permutation_bitset<24>(clients,val,conv2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_bitset(clients,cres[0]);

    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}



void test14(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        // auto a=gen_rand_vector(500000);
        auto a=std::vector<int64_t>{1,1,2,3,3,4,4,4,5,6,6};
        Value vala(std::move(a));
        // auto b=gen_rand_vector(500000);

        std::vector<int64_t> b={1,3,0,4,5,2,2,2,7,8,8};
        Value valb(std::move(b));
        auto val1=BmakeShare(clients,0,vala);
        auto val2=AmakeShare(clients,0,valb);
        std::vector<std::vector<std::bitset<24>>> val3;
        std::vector<Value> val4;
        auto conv1=conv64Tobitset<24>(clients,val2);
        val3.push_back(conv1);val4.push_back(val1);
        auto cres=GroupbySum(clients,val3,val4,1);
        print_bitset(clients,cres[0]);

        // print_Value(clients,cres[0]);
        // print_Value(clients,cres[1]);
        // auto res2=reconstruct(clients,cres[1]);
    }
    else if(Pid==1)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=AmakeShare(clients,0);
        std::vector<std::vector<std::bitset<24>>> val3;
        std::vector<Value> val4;
        auto conv1=conv64Tobitset<24>(clients,val2);
        val3.push_back(conv1);val4.push_back(val1);
        auto cres=GroupbySum(clients,val3,val4,1);
        print_bitset(clients,cres[0]);
        // print_Value(clients,cres[0]);
        // print_Value(clients,cres[1]);
        // auto res2=reconstruct(clients,cres[1]);
    }
    else if(Pid==2)
    {
        auto val1=BmakeShare(clients,0);
        auto val2=AmakeShare(clients,0);
        std::vector<std::vector<std::bitset<24>>> val3;
        std::vector<Value> val4;
        auto conv1=conv64Tobitset<24>(clients,val2);
        val3.push_back(conv1);val4.push_back(val1);

        auto start = std::chrono::high_resolution_clock::now();
        auto cres=GroupbySum(clients,val3,val4,1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        print_bitset(clients,cres[0]);

        // print_Value(clients,cres[0]);
        // print_Value(clients,cres[1]);
        // auto res2=reconstruct(clients,cres[1]);
        // for(int i=0;i<res2.size();i++)
        // {
        //     print_int128(res2[i]);
        //     std::cout<<" ";
        // }
    }
       std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test15(size_t Pid,size_t Party_number,size_t port)
{
    std::vector<double> times;
    std::vector<double> comm;
    double pre=0;
    std::vector<size_t> datasz={5000,8192,16384,32768,50000,65536,131072,262144,500000};
    Clients clients(Party_number,Pid,port);
    Value val1,val2;
for(int i=0;i<datasz.size();i++)
{
    auto sz=datasz[i];
    if(Pid==0)
    {
        auto a=gen_rand_vector(sz);
        
        Value vala(std::move(a));
        auto b=gen_rand_vector(sz);


        Value valb(std::move(b));
        val1=BmakeShare(clients,0,vala);
        val2=BmakeShare(clients,0,valb);

    }
    else if(Pid==1)
    {
        val1=BmakeShare(clients,0);
        val2=BmakeShare(clients,0);
    }
    else if(Pid==2)
    {
        val1=BmakeShare(clients,0);
        val2=BmakeShare(clients,0);

    }
    std::vector<Value> valaa;std::vector<Value> valbb;
    // for(int j=0;j<=i;j++)
    // {
        valaa.push_back(val1);valbb.push_back(val2);
    // }


    auto start = std::chrono::high_resolution_clock::now();
    auto cres=BtMerge(clients,valaa,valbb,1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    times.push_back(duration);
    std::cout<<duration<<'\n';
    comm.push_back(clients.CommCost()-pre);
    pre=clients.CommCost();
}
        for(int i=0;i<times.size();i++)
    {
        std::cout<<times[i]<<' ';
    }
    std::cout<<'\n';
    for(int i=0;i<times.size();i++)
    {
        std::cout<<comm[i]<<' ';
    }
    // Clients clients(Party_number,Pid,port);
    // if(Pid==0)
    // {
    //     // auto a=gen_rand_vector(1000000);
    //     std::vector<int64_t> a={1,1,1,1,1,1,1,1,-158,-5464,-45894,-894899};
        
    //     // std::vector<int64_t> a={5868,1038,2575,7878,12968,5124,19798,2759,8962,487,2554,5527,15890,19919,3288,11875,5509,15866,17142,8920};
    //     // auto a=std::vector<int64_t>{10356271499182,88042823231835,174940302968664894,861479603093249486,-504801432502811770 ,-188772840965089798 ,292491955487256783 ,-458040940225437445, -604058476677963351, -701929045144345242 ,-235394412361062955 ,-29404073658832048 ,12766992776671982 ,510291345664598896 ,384100886977695671 };
    //     Value vala(std::move(a));
    //     // auto b=gen_rand_vector(5000);

    //     std::vector<int64_t> b={-10,-8,-6,2,2,1000,9854151};

    //     // std::vector<int64_t> c={1,1,1,1,1,1,1,1,-158,-5464,-45894,-894899,-10,-8,-6,2,2,1000,9854151};
    //     std::vector<int64_t> c={5868,1038,2575,7878,12968,5124,19798,2759,8962,487,2554,5527,15890,19919,3288,11875,5509,15866,17142,8920};

    //     Value valb(std::move(b));
    //     Value valc(std::move(c)); 
    //     auto val1=BmakeShare(clients,0,vala);
    //     auto val2=BmakeShare(clients,0,valb);
    //     auto val3=BmakeShare(clients,0,valc);
    //     std::vector<Value> valaa;std::vector<Value> valbb;
    //     valaa.push_back(val1);valbb.push_back(val2);
    //     // auto cres=BtMerge(clients,valaa,valbb,1);
    //     auto cres=odd_even_merge_sort(clients,std::vector<Value>{val3},1);
    //     // auto cres=odd_even_merge_sort(clients,std::vector<Value>{val1},1);
    //     auto res1=Permutation(clients,std::vector<Value>{make_constrangA(clients,val3.size()/2)},cres)[0];
    //     auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val3},res1)[0]);
    //     // auto res=reconstruct(clients,cres);
    // }
    // else if(Pid==1)
    // {
    //     auto val1=BmakeShare(clients,0);
    //     auto val2=BmakeShare(clients,0);
    //     auto val3=BmakeShare(clients,0);
    //     std::vector<Value> valaa;std::vector<Value> valbb;
    //     valaa.push_back(val1);valbb.push_back(val2);
    //     // auto cres=BtMerge(clients,valaa,valbb,1);
    //     auto cres=odd_even_merge_sort(clients,std::vector<Value>{val3},1);
    //     // auto cres=odd_even_merge_sort(clients,std::vector<Value>{val1},1);
    //     auto res1=Permutation(clients,std::vector<Value>{make_constrangA(clients,val3.size()/2)},cres)[0];
    //     auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val3},res1)[0]);
    //     // auto res=reconstruct(clients,cres);
    // }
    // else if(Pid==2)
    // {
    //     auto val1=BmakeShare(clients,0);
    //     auto val2=BmakeShare(clients,0);
    //     auto val3=BmakeShare(clients,0);
    //     std::vector<Value> valaa;std::vector<Value> valbb;
    //     valaa.push_back(val1);valbb.push_back(val2);
    //     auto start = std::chrono::high_resolution_clock::now();
    //     // auto cres=BtMerge(clients,valaa,valbb,1);
    //     auto cres=odd_even_merge_sort(clients,std::vector<Value>{val3},1);
        
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    //     // print_Value(clients,cres);
    //     // auto res=reconstruct(clients,cres);
    //     auto res1=Permutation(clients,std::vector<Value>{make_constrangA(clients,val3.size()/2)},cres)[0];
    //     auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val3},res1)[0]);
    //     for(int i=0;i<res.size();i++)
    //     {
    //         print_int128(res[i]);
    //         std::cout<<" ";
    //     }
    // }


    // std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test16(size_t Pid,size_t Party_number,size_t port)
{
    
    // Clients clients(Party_number,Pid,port);
    // if(Pid==0)
    // {
    //     // auto a=gen_rand_vector(1000000);
    //     auto a=std::vector<int64_t>(1000000,1); 

    //     // auto a=std::vector<int64_t>{1,3,0,4,5,2,2,2};
    //     // auto a=std::vector<int64_t>{5,3,1,4,0,2};
    //     Value vala(std::move(a));
    //     // auto b=gen_rand_vector(524288);

    //     // std::vector<int64_t> b={1,3,0,4,5,2,2,2};

    //     auto val1=AmakeShare(clients,0,vala);
    //     auto val2=AmakeShare(clients,1);
    //     // auto conv=conv64Tobitset<17>(clients,val1);
    //     // auto cv=conv64To32(clients,val1);
    //     // auto cres=Permutation_gen_32(clients,cv);
    //     // auto cres=Permutation_gen_bitset<17>(clients,conv);
    //     // auto cres=radix_sort_conv(clients,val1);

    //     auto cres=radix_sort(clients,std::vector<Value>{val1});
    //     // print_Value(clients,cres);
    //     // auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val1},cres)[0]);
    // }
    // else if(Pid==1)
    // {
    //     std::vector<int64_t> b=std::vector<int64_t>(1000000,1);
    //     Value valb(std::move(b));
    //     auto val1=AmakeShare(clients,0);
    //     auto val2=AmakeShare(clients,1,valb);
    //     // auto conv=conv64Tobitset<17>(clients,val1);
    //     // auto cv=conv64To32(clients,val1);
    //     // auto cres=Permutation_gen_32(clients,cv);
    //     // auto cres=Permutation_gen_bitset<17>(clients,conv);
    //     // auto cres=radix_sort_conv(clients,val1);
    //     auto cres=radix_sort(clients,std::vector<Value>{val1});
    //     // print_Value(clients,cres);
    //     // auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val1},cres)[0]);

    // }
    // else if(Pid==2)
    // {
    //     auto val1=AmakeShare(clients,0);
    //     auto val2=AmakeShare(clients,1);
    //     auto conv=conv64Tobitset<17>(clients,val1);
    //     // auto cv=conv64To32(clients,val1);
    //     auto start = std::chrono::high_resolution_clock::now();
    //     // auto cres=Permutation_gen_32(clients,cv);
    //     // auto cres=Permutation_gen_bitset<17>(clients,conv);
    //     // auto cres=radix_sort_conv(clients,val1);
    //     auto cres=radix_sort(clients,std::vector<Value>{val1});
    //     // print_Value(clients,cres);
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    //     // auto res=reconstruct(clients,Permutation(clients,std::vector<Value>{val1},cres)[0]);
    //     // for(int i=0;i<res.size();i++)
    //     // {
    //     //     print_int128(res[i]);
    //     //     std::cout<<" ";
    //     // }
    //     // std::cout<<'\n';
    // }
    std::vector<double> times;
    std::vector<double> comm;
    double pre=0;
    std::vector<size_t> datasz={1000000};
    Clients clients(Party_number,Pid,port);
    Value val;
for(int i=0;i<datasz.size();i++)
{
    auto sz=datasz[i];
    if(Pid==0)
    {

        auto a=std::vector<int64_t>(sz,1); 


        Value vala(std::move(a));

        val=BmakeShare(clients,0,vala);

    }
    else if(Pid==1)
    {
        val=BmakeShare(clients,0);


    }
    else if(Pid==2)
    {
        val=BmakeShare(clients,0);



    }
    auto start = std::chrono::high_resolution_clock::now();
    auto cres=radix_sort<24>(clients,std::vector<Value>{val});
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    times.push_back(duration);
    std::cout<<duration<<'\n';
    comm.push_back(clients.CommCost()-pre);
    pre=clients.CommCost();
}
    for(int i=0;i<times.size();i++)
    {
        std::cout<<times[i]<<' ';
    }
    std::cout<<'\n';
    for(int i=0;i<times.size();i++)
    {
        std::cout<<comm[i]<<' ';
    }
    // std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test17(size_t Pid,size_t Party_number)
{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        //1000000 155ms
        auto a=gen_rand_vector(1000000);
        // auto a=std::vector<int64_t>{1,3,0,4,5,2,2,2};
        // auto a=std::vector<int64_t>{5,3,1,4,0,2};
        Value vala(std::move(a));
        // auto b=gen_rand_vector(524288);
        auto b=gen_rand_vector(1000000);

        // std::vector<int64_t> b={1,3,0,4,5,2,2,2};
        // std::vector<int64_t> b={3,4,0,1,2,5};
        Value valb(std::move(b));
        Ot(clients,vala,valb,Value(),0,2);
    }
    else if(Pid==1)
    {
        Ot(clients,Value(),Value(),Value(),0,2);
    }
    else if(Pid==2)
    {
        // auto a=std::vector<int64_t>{1,0,1,1,0,0};
        auto a=gen_rand_vector(10000);
        for(int i=0;i<a.size();i++) a[i]=a[i]&1;
        Value vala(std::move(a)); 
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Ot(clients,Value(),Value(),vala,0,2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // for(int i=0;i<res.size();i++)
        // {
        //     print_int128(res[i]);
        //     std::cout<<" ";
        // }
        // std::cout<<'\n';
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test18(size_t Pid,size_t Party_number)

{
    Clients clients(Party_number,Pid);
    if(Pid==0)
    {
        // 1000000 553ms
        auto a=gen_rand_vector(1000000);
        // auto a=std::vector<int64_t>{1,3,0,4,5,2,2,2};
        // auto a=std::vector<int64_t>{5,3,1,4,0,2};
        Value vala(std::move(a));vala.set_share_type(0);
        // auto b=gen_rand_vector(524288);
        auto b=gen_rand_vector(1000000);

        // std::vector<int64_t> b={1,3,0,4,5,2,2,2};
        // std::vector<int64_t> b={0,1,1,0,1,0};
        Value valb(std::move(b));
        auto val1=AmakeShare(clients,0,vala);
        auto val2=BmakeShare(clients,0,valb);
        for(int i=0;i<val2.size();i++)
        {
            val2[i]&=1;  
        } 
        auto res=Mul_ab2(clients,val1,val2);
        // print_Value(clients,res);  
    }
    else if(Pid==1)
    {
        // auto a=gen_rand_vector(10000);
        auto val1=AmakeShare(clients,0);
        auto val2=BmakeShare(clients,0);
        for(int i=0;i<val2.size();i++)
        {
            val2[i]&=1;  
        } 
        auto res=Mul_ab2(clients,val1,val2);
        // print_Value(clients,res);
    }
    else if(Pid==2)
    {
        // auto a=gen_rand_vector(10000);
        auto val1=AmakeShare(clients,0);
        auto val2=BmakeShare(clients,0);
        for(int i=0;i<val2.size();i++)
        {
            val2[i]&=1;  
        } 
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Mul_ab2(clients,val1,val2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test19(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        auto a=std::vector<int64_t>(1,0);
        Value vala(std::move(a));
        vala.set_share_type(1);
        std::vector<Value> res(100);
        for(int i=0;i<100;i++)
        {
            res[i]=clients.SendRoud(vala);
            // std::cout<<res[i][0]<<'\n';
        }

    }
    else if(Pid==1)
    {
        auto a=std::vector<int64_t>(1,0);

        Value vala(std::move(a));
        vala.set_share_type(1);
        std::vector<Value> res(100);
        for(int i=0;i<100;i++)
        {
            res[i]=clients.SendRoud(vala);
        }

    }
    else if(Pid==2)
    {
        auto a=std::vector<int64_t>(1,0); 
        Value vala(std::move(a));
        vala.set_share_type(1);
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Value> res(100);
        for(int i=0;i<100;i++)
        {
            res[i]=clients.SendRoud(vala);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;

        // for(int i=0;i<res.size();i++)
        // {
        //     std::cout<<res[i]<<" ";
        // }
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test20(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        //1000000 107ms
        auto a=std::vector<int64_t>(1000000,0);
        // auto a=std::vector<int64_t>{10,2,13,144,17,12,2,6};
        Value vala(std::move(a));vala.set_share_type(0);
        auto res=s2tos3(clients,2,vala);
        // print_Value(clients,res);
    }
    else if(Pid==1)
    {
        auto a=std::vector<int64_t>(1000000,1);
        // auto a=std::vector<int64_t>{10,2,13,144,17,12,2,6};
        Value vala(std::move(a));
        vala.set_share_type(0);
        auto res=s2tos3(clients,2,vala);
        // print_Value(clients,res);
    }
    else if(Pid==2)
    {
        auto a=std::vector<int64_t>(1000000,0); 
        Value vala(std::move(a));
        vala.set_share_type(0);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=s2tos3(clients,2,vala);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test21(size_t Pid,size_t Party_number,size_t port)
{
    // Clients clients(Party_number,Pid,port);
    // if(Pid==0)
    // {
    //     //1000000 145ms
    //     // auto a=std::vector<int64_t>(1000000,0);
    //     auto a=std::vector<int64_t>{10,2,13,144,17,12,2,6};

    //     // auto b=std::vector<size_t>(500000,0);

    //     Value vala(std::move(a));vala.set_share_type(0);
    //     Value val=BmakeShare(clients,0,vala);
    //     auto start = std::chrono::high_resolution_clock::now();
    //     auto res=perm(clients,std::vector<Value>{val},1,6,std::vector<size_t>());
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    //     auto cres=s2tos3(clients,0,res[0]);
    //     print_Value(clients,cres);
    // }
    // else if(Pid==1)
    // {
    //     // auto a=std::vector<int64_t>(1000000,1);
    //     auto a=std::vector<int64_t>{10,2,13,144,17,12,2,6};
    //     auto b=std::vector<size_t>{2,3,2,3,2,1};
    //     Value vala(std::move(a));
    //     Value val=BmakeShare(clients,0);
    //     vala.set_share_type(0);
    //     auto start = std::chrono::high_resolution_clock::now();
    //     // auto res=perm(clients,std::vector<Value>{val},1,6,b);
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    //     auto cres=s2tos3(clients,0,res[0]);
    //     print_Value(clients,cres);
    // }
    // else if(Pid==2)
    // {
    //     auto a=std::vector<int64_t>(1000000,0); 
    //     Value vala(std::move(a));
    //     vala.set_share_type(0);
    //     Value val=BmakeShare(clients,0);
    //     auto start = std::chrono::high_resolution_clock::now();
    //     auto res=perm(clients,std::vector<Value>{val},1,6,std::vector<size_t>());
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
    //     auto cres=s2tos3(clients,0,res[0]);
    //     print_Value(clients,cres);
    // }


    // std::cout<<clients.CommCost()<<"MB"<<'\n';
    // std::this_thread::sleep_for(std::chrono::seconds(5)); 
    // clients.close();
}

void test22(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {
        //1000000 145ms
        auto a=std::vector<int64_t>(500000);
        for(int i=0;i<a.size();i++) a[i]=i+1;
        auto b=std::vector<int64_t>(500000);
        for(int i=0;i<b.size();i++) b[i]=i+1;
        auto c=std::vector<int64_t>(500000);
        for(int i=0;i<c.size();i++) c[i]=i+1;
        auto d=std::vector<int64_t>(500000);
        for(int i=0;i<d.size();i++) d[i]=i+1;
        // auto a=std::vector<int64_t>{10,2,13,144,17,12,284,6};
        // auto b=std::vector<int64_t>{11,2,123,14,16,12,41,37};
        // // // auto b=std::vector<size_t>(500000,0);
        // auto c=std::vector<int64_t>{10,11,14,12,19,17,123,321};

        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);
        Value valc(std::move(c));valc.set_share_type(0);
        Value vald(std::move(d));vald.set_share_type(0);

        Value val=BmakeShare(clients,0,vala);
        Value val1=BmakeShare(clients,0,valb);
        Value val2=BmakeShare(clients,0,valc);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Psi_with_payload_one(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<Value>{val2});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto cres=s2tos3(clients,2,res[0]);
        // print_Value(clients,res[0]);
    }
    else if(Pid==1)
    {

        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        Value val2=BmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Psi_with_payload_one(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<Value>{val2});

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto cres=s2tos3(clients,2,res[0]);
        // print_Value(clients,res[0]);
    }
    else if(Pid==2)
    {
        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        Value val2=BmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Psi_with_payload_one(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<Value>{val2});

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // auto cres=s2tos3(clients,2,res[0]);
        // print_Value(clients,res[0]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test23(size_t Pid,size_t Party_number,size_t port)
{
    std::vector<double> times;
    std::vector<double> comm;
    double pre=0;
    std::vector<size_t> datasz={5000,25000,50000,250000,500000};
    Clients clients(Party_number,Pid,port);

    for(int i=0;i<datasz.size();i++)
    {
        Value val,val1;
        auto sz=datasz[i];
        if(Pid==0)
        {

            // auto a=std::vector<int64_t>{10,10,3,3,1,1,2,7,4};
            // auto b=std::vector<int64_t>{10,2,2,1,1,3,4,9};
            auto a=gen_rand_vector(sz); 
            auto b=gen_rand_vector(sz);

            Value vala(std::move(a));vala.set_share_type(0);
            Value valb(std::move(b));valb.set_share_type(0);

            val=BmakeShare(clients,0,vala);
            val1=BmakeShare(clients,0,valb);

            
            // print_Value(clients,res[0]);
        }
        else if(Pid==1) 
        {

            val=BmakeShare(clients,0);
            val1=BmakeShare(clients,0);
            // print_Value(clients,res[0]);
        }
        else if(Pid==2)
        {
            val=BmakeShare(clients,0);
            val1=BmakeShare(clients,0);
            // print_Value(clients,res[0]);
        }
        // std::vector<Value> d1,d2;
        // std::vector<size_t> p1,p2;
        // for(int j=0;j<=i;j++)
        // {
        //     d1.push_back(val);
        //     d2.push_back(val1);
        //     p1.push_back(j);
        //     p2.push_back(j);
        // }
        auto start = std::chrono::high_resolution_clock::now();
        // auto res=merge(clients,d1,d2,p1,p2);
        
        auto res=merge(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        times.push_back(duration);
        comm.push_back(clients.CommCost()-pre);
        pre=clients.CommCost();

        // std::cout<<clients.CommCost()<<"MB"<<'\n';
    }
    for(int i=0;i<times.size();i++)
    {
        std::cout<<times[i]<<' ';
    }
    std::cout<<'\n';
    for(int i=0;i<times.size();i++)
    {
        std::cout<<comm[i]<<' ';
    }
    // if(Pid==0)
    // {

    //     // auto a=std::vector<int64_t>{10,10,3,3,1,1,2,7,4};
    //     // auto b=std::vector<int64_t>{10,2,2,1,1,3,4,9};
    //     auto a=gen_rand_vector(500000); 
    //     auto b=gen_rand_vector(500000);

    //     Value vala(std::move(a));vala.set_share_type(0);
    //     Value valb(std::move(b));valb.set_share_type(0);

    //     val=BmakeShare(clients,0,vala);
    //     val1=BmakeShare(clients,0,valb);

        
    //     // print_Value(clients,res[0]);
    // }
    // else if(Pid==1) 
    // {

    //     Value val=BmakeShare(clients,0);
    //     Value val1=BmakeShare(clients,0);
    //     // print_Value(clients,res[0]);
    // }
    // else if(Pid==2)
    // {
    //     val=BmakeShare(clients,0);
    //     val1=BmakeShare(clients,0);
    //     // print_Value(clients,res[0]);
    // }
    // auto start = std::chrono::high_resolution_clock::now();
    // auto res=merge(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;

    // std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
 void test24(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{10,7,3,3,1,9};
        // auto b=std::vector<int64_t>{2,2,1,1,0,2};
        // auto f=std::vector<int64_t>{1,1,1,1,0,1};
        auto a=gen_rand_vector(500000);
        auto b=std::vector<int64_t>(500000,1);
        auto f=std::vector<int64_t>(500000,1);

        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);
        Value valf(std::move(f));valf.set_share_type(0);

        Value val=AmakeShare(clients,0,vala);
        Value val1=AmakeShare(clients,0,valb);
        Value val2=AmakeShare(clients,0,valf);

        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Expansion(clients,std::vector<Value>{val},conv1,conv2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }
    else if(Pid==1)
    {

        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);
        Value val2=AmakeShare(clients,0);
        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Expansion(clients,std::vector<Value>{val},conv1,conv2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }
    else if(Pid==2)
    {
        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);
        Value val2=AmakeShare(clients,0);
        auto conv1=conv64Tobitset<24>(clients,val1);
        auto conv2=conv64Tobitset<24>(clients,val2);
        auto start = std::chrono::high_resolution_clock::now();
        auto res=Expansion(clients,std::vector<Value>{val},conv1,conv2);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test25(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        auto a=std::vector<int64_t>{6,6,10,10,3,3,1,1,2,7,4};
        auto b=std::vector<int64_t>{9,10,2,2,1,1,3,4};
        // auto a=std::vector<int64_t>{0,1,2};
        // auto b=std::vector<int64_t>{0,0,0,1,1,1,2,2,2};
        // auto a=std::vector<int64_t>(50000,1);
        // auto b=std::vector<int64_t>(50000,0);
        // a[0]=10086;
        // b[0]=198654;
        // a[1]=2;
        // b[1]=2;
        // auto c=std::vector<int64_t>{8,7,6,5,4,3,2,1};
        // auto a=gen_rand_vector(500000);
        // auto b=std::vector<int64_t>(500000,1);


        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);
        // Value valc(std::move(c));valc.set_share_type(0);

        Value val=BmakeShare(clients,0,vala);
        Value val1=BmakeShare(clients,0,valb);
        // Value val2=AmakeShare(clients,0,valc);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Merge_Join<24>(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        print_Value(clients,res[1]);
    }
    else if(Pid==1)
    {

        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Merge_Join<24>(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        print_Value(clients,res[1]);
    }
    else if(Pid==2)
    {
        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=Merge_Join<24>(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        print_Value(clients,res[1]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test26(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        auto a=std::vector<int64_t>{382933069,599489438,19,50,20};
        auto b=std::vector<int64_t>{1024,3,2,2,100};  
        // auto a=std::vector<int64_t>(100000,1);
        // auto b=std::vector<int64_t>(100000,1);

        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);

        Value val=AmakeShare(clients,0,vala);
        Value val1=AmakeShare(clients,0,valb);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=MOD(clients,val,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        print_Value(clients,res);
    }
    else if(Pid==1)
    {

        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=MOD(clients,val,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        print_Value(clients,res);
    }
    else if(Pid==2)
    {
        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=MOD(clients,val,val1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        print_Value(clients,res);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test27(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        auto a=std::vector<int64_t>{10};

        Value vala(std::move(a));vala.set_share_type(0);

        Value val=AmakeShare(clients,0,vala);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=gen_rand_idx(clients,val,100000);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res);
    }
    else if(Pid==1)
    {

        Value val=AmakeShare(clients,0);


        auto start = std::chrono::high_resolution_clock::now();
        auto res=gen_rand_idx(clients,val,100000);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res);
    }
    else if(Pid==2)
    {
        Value val=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=gen_rand_idx(clients,val,100000);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test28(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    std::vector<double> times;
    std::vector<double> comm;
    double pre=0;
    std::vector<size_t> datasz={1000000};
    std::vector<size_t> blocksize={1,2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100};
for(int i=0;i<blocksize.size();i++)
    {
        Value val;
        auto sz=blocksize[i];
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{10,10,3,3,1,1,2,7,4,998,135,456};
        auto a=std::vector<int64_t>(1000000,1);


        Value vala(std::move(a));vala.set_share_type(0);

        val=AmakeShare(clients,0,vala);


    }
    else if(Pid==1)
    {

        val=AmakeShare(clients,0);


    }
    else if(Pid==2)
    {
        val=AmakeShare(clients,0);


    } 

    auto start = std::chrono::high_resolution_clock::now();
        auto res=Secure_sampling(clients,std::vector<Value>{val},sz);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        times.push_back(duration);
        comm.push_back(clients.CommCost()-pre);
        pre=clients.CommCost();

        // std::cout<<clients.CommCost()<<"MB"<<'\n';
    }
    for(int i=0;i<times.size();i++)
    {
        std::cout<<times[i]<<' ';
    }
    std::cout<<'\n';
    for(int i=0;i<times.size();i++)
    {
        std::cout<<comm[i]<<' ';
    }
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}


void test29(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    std::vector<double> times;
    std::vector<double> comm;
    double pre=0;
    std::vector<size_t> datasz={10000,100000,1000000};
    std::vector<size_t> blocksize={1,2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100};
    // std::vector<size_t> blocksize={3};
    
    for(int i=0;i<blocksize.size();i++)
    {
        Value val,valF;
        auto sz=blocksize[i];
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{10,10,3,3,1,1,2,7,4,998,135,456,778,314,654};
        auto a=std::vector<int64_t>(1000000,1);
        // auto f=std::vector<int64_t>{0,1,0,0,1,1,0,0,0,1,0,0,1,1,1};
        auto f=std::vector<int64_t>(1000000,1);


        Value vala(std::move(a));vala.set_share_type(0);
        Value valf(std::move(f));valf.set_share_type(0);
        val=AmakeShare(clients,0,vala);
        valF=AmakeShare(clients,0,valf);

        // print_Value(clients,res[0]);
    }
    else if(Pid==1)
    {

         val=AmakeShare(clients,0);
         valF=AmakeShare(clients,0);

        // print_Value(clients,res[0]);
    }
    else if(Pid==2)
    {
         val=AmakeShare(clients,0);
         valF=AmakeShare(clients,0);

        // print_Value(clients,res[0]);
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto res=Secure_filter_sampling(clients,std::vector<Value>{val},valF,sz);
    // print_Value(clients,res[0]);

     auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        times.push_back(duration);
        comm.push_back(clients.CommCost()-pre);
        pre=clients.CommCost();
    }
    for(int i=0;i<times.size();i++)
    {
        std::cout<<times[i]<<' ';
    }
    std::cout<<'\n';
    for(int i=0;i<times.size();i++)
    {
        std::cout<<comm[i]<<' ';
    }
    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test30(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{6,6,10,10,3,3,1,1,2,7,4};
        // auto b=std::vector<int64_t>{9,10,2,2,1,1,3,4};
        // auto a=std::vector<int64_t>{0,1,2};
        // auto b=std::vector<int64_t>{0,0,0,1,1,1,2,2,2};
        auto a=std::vector<int64_t>(500000,1);
        auto b=std::vector<int64_t>(500000,0);
        a[0]=10086;
        b[0]=198654;
        a[1]=2;
        b[1]=2;
        // auto c=std::vector<int64_t>{8,7,6,5,4,3,2,1};
        // auto a=gen_rand_vector(500000);
        // auto b=std::vector<int64_t>(500000,1);


        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);
        // Value valc(std::move(c));valc.set_share_type(0);

        Value val=AmakeShare(clients,0,vala);
        Value val1=AmakeShare(clients,0,valb);
        // Value val2=AmakeShare(clients,0,valc);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=LeftJoin(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }
    else if(Pid==1)
    {

        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=LeftJoin(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }
    else if(Pid==2)
    {
        Value val=AmakeShare(clients,0);
        Value val1=AmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=LeftJoin(clients,std::vector<Value>{val},std::vector<Value>{val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void test31(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{6,6,10,10,3,3,1,1,2,7,4};
        // auto b=std::vector<int64_t>{9,10,2,2,1,1,3,4};
        // auto a=std::vector<int64_t>{0,1,2};
        // auto b=std::vector<int64_t>{0,0,0,1,1,1,2,2,2};
        auto a=std::vector<int64_t>(1000000,1);
        auto b=std::vector<int64_t>(1000000,0);
        // a[0]=10086;
        // b[0]=198654;
        // a[1]=2;
        // b[1]=2;
        // auto c=std::vector<int64_t>{8,7,6,5,4,3,2,1};
        // auto a=gen_rand_vector(500000);
        // auto b=std::vector<int64_t>(500000,1);


        Value vala(std::move(a));vala.set_share_type(0);
        Value valb(std::move(b));valb.set_share_type(0);
        // Value valc(std::move(c));valc.set_share_type(0);

        Value val=BmakeShare(clients,0,vala);
        Value val1=BmakeShare(clients,0,valb);
        // Value val2=AmakeShare(clients,0,valc);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val1,val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }
    else if(Pid==1)
    {

        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val1,val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }
    else if(Pid==2)
    {
        Value val=BmakeShare(clients,0);
        Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val1,val1},std::vector<size_t>{0},std::vector<size_t>{0});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void test32(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    if(Pid==0)
    {

        // auto a=std::vector<int64_t>{6,6,10,10,3,3,1,1,2,7,4};
        // auto b=std::vector<int64_t>{9,10,2,2,1,1,3,4};
        // auto a=std::vector<int64_t>{0,1,2};
        // auto b=std::vector<int64_t>{0,0,0,1,1,1,2,2,2};
        auto a=std::vector<int64_t>(1000000,1);
        // auto b=std::vector<int64_t>(1000000,0);
        // a[0]=10086;
        // b[0]=198654;
        // a[1]=2;
        // b[1]=2;
        // auto c=std::vector<int64_t>{8,7,6,5,4,3,2,1};
        // auto a=gen_rand_vector(500000);
        // auto b=std::vector<int64_t>(500000,1);


        Value vala(std::move(a));vala.set_share_type(0);
        

        // Value valb(std::move(b));valb.set_share_type(0);
        // Value valc(std::move(c));valc.set_share_type(0);

        Value val=BmakeShare(clients,0,vala);
        // Value val1=BmakeShare(clients,0,valb);
        // Value val2=AmakeShare(clients,0,valc);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=slow_sampling(clients,std::vector<Value>{val});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }
    else if(Pid==1)
    {

        Value val=BmakeShare(clients,0);
        // Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=slow_sampling(clients,std::vector<Value>{val});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }
    else if(Pid==2)
    {
        Value val=BmakeShare(clients,0);
        // Value val1=BmakeShare(clients,0);
        // Value val2=AmakeShare(clients,0);

        auto start = std::chrono::high_resolution_clock::now();
        auto res=slow_sampling(clients,std::vector<Value>{val});
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
        // print_Value(clients,res[0]);
        // print_Value(clients,res[1]);
    }


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void Q3(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    std::vector<size_t> datasz={600000,1200000,1800000,2400000,3000000};
    for(int i=0;i<datasz.size();i++)
    {
        auto sz=datasz[i];
        Value val;
        Value val1;
        Value val2;
        if(Pid==0)
        {

            auto a=std::vector<int64_t>(sz,1);
            auto b=std::vector<int64_t>(sz/40,2);
            auto c=std::vector<int64_t>(sz/4,3);

            Value vala(std::move(a));vala.set_share_type(0);
            Value valb(std::move(b));valb.set_share_type(0);
            Value valc(std::move(c));valc.set_share_type(0);

             val=BmakeShare(clients,0,vala);
             val1=BmakeShare(clients,0,valb);
             val2=BmakeShare(clients,0,valc);


          
            // print_Value(clients,res[0]);
            // print_Value(clients,res[1]);
        }
        else if(Pid==1)
        {


             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);

        }
        else if(Pid==2)
        {
             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);
        }
        auto start = std::chrono::high_resolution_clock::now();
        auto midres1=general_Join(clients,std::vector<Value>{val1},std::vector<Value>{val2,val2},std::vector<size_t>{0},std::vector<size_t>{0});
        auto midres2=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val2},std::vector<size_t>{0},std::vector<size_t>{0});

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime: "<<sz<<" " << duration.count() << " ms" << std::endl;
    }
    


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void Q7(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    std::vector<size_t> datasz={600000,1200000,1800000,2400000,3000000};
    for(int i=0;i<datasz.size();i++)
    {
        auto sz=datasz[i];
        Value val;
        Value val1;
        Value val2;
        Value val3;
        
        if(Pid==0)
        {

            auto a=std::vector<int64_t>(sz,1);
            auto b=std::vector<int64_t>(sz/40,2);
            auto c=std::vector<int64_t>(sz/4,3);
            auto d=std::vector<int64_t>(sz/600,4);

            Value vala(std::move(a));vala.set_share_type(0);
            Value valb(std::move(b));valb.set_share_type(0);
            Value valc(std::move(c));valc.set_share_type(0);
            Value vald(std::move(d));valc.set_share_type(0);

             val=BmakeShare(clients,0,vala);
             val1=BmakeShare(clients,0,valb);
             val2=BmakeShare(clients,0,valc);
             val3=BmakeShare(clients,0,vald);

        }
        else if(Pid==1)
        {


             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);
             val3=BmakeShare(clients,0);

        }
        else if(Pid==2)
        {
             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);
             val3=BmakeShare(clients,0);

        }
        auto start = std::chrono::high_resolution_clock::now();
        auto midres1=general_Join(clients,std::vector<Value>{val1},std::vector<Value>{val2,val2},std::vector<size_t>{0},std::vector<size_t>{0});
        auto midres2=general_Join(clients,std::vector<Value>{val2},std::vector<Value>{val,val,val},std::vector<size_t>{0},std::vector<size_t>{0});
        auto midres3=general_Join(clients,std::vector<Value>{val3},std::vector<Value>{val,val},std::vector<size_t>{0},std::vector<size_t>{0});


        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime: "<<sz<<" " << duration.count() << " ms" << std::endl;
    }
    


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}
void Q10(size_t Pid,size_t Party_number,size_t port)
{
     Clients clients(Party_number,Pid,port);
    std::vector<size_t> datasz={600000,1200000,1800000,2400000,3000000};
    for(int i=0;i<datasz.size();i++)
    {
        auto sz=datasz[i];
        Value val;
        Value val1;
        Value val2;
        if(Pid==0)
        {

            auto a=std::vector<int64_t>(sz,1);
            auto b=std::vector<int64_t>(sz/40,2);
            auto c=std::vector<int64_t>(sz/4,3);

            Value vala(std::move(a));vala.set_share_type(0);
            Value valb(std::move(b));valb.set_share_type(0);
            Value valc(std::move(c));valc.set_share_type(0);

             val=BmakeShare(clients,0,vala);
             val1=BmakeShare(clients,0,valb);
             val2=BmakeShare(clients,0,valc);

        }
        else if(Pid==1)
        {


             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);

        }
        else if(Pid==2)
        {
             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);
        }
        auto start = std::chrono::high_resolution_clock::now();
        auto midres1=general_Join(clients,std::vector<Value>{val1},std::vector<Value>{val2,val2},std::vector<size_t>{0},std::vector<size_t>{0});
        auto midres2=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val2},std::vector<size_t>{0},std::vector<size_t>{0});

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime: "<<sz<<" " << duration.count() << " ms" << std::endl;
    }
    


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

void SRJ(size_t Pid,size_t Party_number,size_t port)
{
    Clients clients(Party_number,Pid,port);
    std::vector<size_t> datasz={600000,1200000,1800000,2400000,3000000};
    for(int i=0;i<datasz.size();i++)
    {
        auto sz=datasz[i];
        Value val;
        Value val1;
        Value val2;
        if(Pid==0)
        {

            auto a=std::vector<int64_t>(sz,1);
            auto b=std::vector<int64_t>(sz/40,2);
            auto c=std::vector<int64_t>(sz/4,3);

            Value vala(std::move(a));vala.set_share_type(0);
            Value valb(std::move(b));valb.set_share_type(0);
            Value valc(std::move(c));valc.set_share_type(0);

             val=BmakeShare(clients,0,vala);
             val1=BmakeShare(clients,0,valb);
             val2=BmakeShare(clients,0,valc);

        }
        else if(Pid==1)
        {


             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);

        }
        else if(Pid==2)
        {
             val=BmakeShare(clients,0);
             val1=BmakeShare(clients,0);
             val2=BmakeShare(clients,0);
        }

        auto start = std::chrono::high_resolution_clock::now();

        auto midres1=general_Join(clients,std::vector<Value>{val1},std::vector<Value>{val2,val2},std::vector<size_t>{0},std::vector<size_t>{0});
        auto midres2=general_Join(clients,std::vector<Value>{val,val},std::vector<Value>{val2},std::vector<size_t>{0},std::vector<size_t>{0});

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Function runtime: "<<sz<<" " << duration.count() << " ms" << std::endl;
    }
    


    std::cout<<clients.CommCost()<<"MB"<<'\n';
    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    clients.close();
}

int main(int argc,char *argv[]) {
    int Party_number=*argv[1]-'0';
    int Pid=*argv[2]-'0';
    int port=10000*(*argv[3]-'0')+1000*(*argv[4]-'0')+100*(*argv[5]-'0')+10*(*argv[6]-'0')+(*argv[7]-'0');
    omp_set_num_threads(8); 
    // test1(Pid,Party_number,port);
    // test2(Pid,Party_number,port);
    // test3(Pid,Party_number);
    // test4(Pid,Party_number);
    // test5(Pid,Party_number);
    // test6(Pid,Party_number,port);
    // test7(Pid,Party_number);
    // test8(Pid,Party_number);
    // test9(Pid,Party_number);
    // test10(Pid,Party_number);
    // test11(Pid,Party_number,port);
    // test12(Pid,Party_number,port);
    // test13(Pid,Party_number,port);
    // test14(Pid,Party_number,port);
    // test15(Pid,Party_number,port);

    // test16(Pid,Party_number,port);
    // test17(Pid,Party_number);
    // test18(Pid,Party_number); 
    // test19(Pid,Party_number,port);
    // test20(Pid,Party_number,port);
    // test21(Pid,Party_number,port);
    // test22(Pid,Party_number,port);
    // test23(Pid,Party_number,port); 
    // test24(Pid,Party_number,port);
    // test25(Pid,Party_number,port);
    // test26(Pid,Party_number,port);
    // test27(Pid,Party_number,port);
    // test28(Pid,Party_number,port);
    test29(Pid,Party_number,port);
    // test30(Pid,Party_number,port);

    // test31(Pid,Party_number,port);
    // test32(Pid,Party_number,port);

    // Q3(Pid,Party_number,port);
    // Q7(Pid,Party_number,port);
    // Q10(Pid,Party_number,port);




 

    // std::vector<__int128> a(1000000,0);
    // std::vector<int8_t> b(1000000,1);
    // auto start = std::chrono::high_resolution_clock::now();

}