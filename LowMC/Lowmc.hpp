// #pragma once

// #include "../Value/value128.h"
// #include "../network/clients.hpp"
// #include "../Operator/operator.h"

// #include<vector>
// #include<bitset>
// #include<utility>
// #include <omp.h>

// const unsigned numofboxes = 31;    // Number of Sboxes
// const unsigned blocksize = 128;   // Block size in bits
// const unsigned keysize = 128; // Key size in bits
// const unsigned rounds = 12; // Number of rounds

// const unsigned identitysize = blocksize - 3*numofboxes;
//                   // Size of the identity part in the Sbox layer
// typedef std::bitset<blocksize> block; 
// typedef std::bitset<keysize> keyblock;
// std::vector<__int128> vSbox =
//     {0x00, 0x01, 0x03, 0x06, 0x07, 0x04, 0x05, 0x02};
// std::vector<__int128> vinvSbox =
//     {0x00, 0x01, 0x07, 0x02, 0x05, 0x06, 0x03, 0x04};


// __int128 to_int128(const std::bitset<128>& bits) {
// uint64_t low = (bits & std::bitset<128>(std::numeric_limits<uint64_t>::max())).to_ullong();  // 获取低 64 位
// uint64_t high = ((bits >> 64) & std::bitset<128>(std::numeric_limits<uint64_t>::max())).to_ullong();  // 获取高 64 位

// return (__int128(high) << 64) | low;  // 拼接高位和低位
// }

// Value gen_rand_key(Clients& clients,size_t t)
// {
//     auto k1=gen_rand_vector(t);
//     Value K1(std::move(k1));K1.set_share_type(1);
//     auto K2=clients.SendRoud(K1);
//     std::vector<int64_t> key(K1.size()*2);
//     for(int i=0;i<K1.size();i++) key[2*i]=K2[i],key[2*i+1]=K1[i];
//     Value Key(std::move(key));Key.set_share_type(1);
//     return Key;
// }

// Value128 conv64to128(const Value& a)
// {
//     std::vector<__int128> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value128 res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// Value conv128to64(const Value128& a)
// {
//     std::vector<int64_t> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// Value8 conv128to8(const Value128& a)
// {
//     std::vector<int8_t> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value8 res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// void XorBox(Value128& a,const std::vector<int8_t>& b)
// {
//     for(int i=0;i<a.size();i++)
//     {
//         a[i]^=vSbox[b[i/2]];
//     }
// }

// void XoriBox(Value128& a,const std::vector<int8_t>& b)
// {
//     for(int i=0;i<a.size();i++)
//     {
//         a[i]^=vinvSbox[b[i/2]];
//     }
// }


// class LowMC
// {

// public:

// LowMC (Clients& clients,const Value& kk) {
//     auto k=conv64to128(kk);
//     key = k;

//     if(clients.getPid()==0) instantiate_LowMC();

//     convkeyMatrices(clients);

//     convLineInvLinMatrices(clients);

//     convLineLinMatrices(clients);

//     convRoundconstants(clients);

//     keyschedule(clients);   

// };

// Value128 encrypt (Clients& clients,const Value& mg)
// {
//     auto c=conv64to128(mg);
//     Xor128(clients,c,expan128(clients,Vroundkeys[0],c.size()));
//     for (unsigned r = 1; r <= rounds; ++r) {
//         auto start1 = std::chrono::high_resolution_clock::now();
//         c =  std::move(Substitution(clients,c));
//         auto end1 = std::chrono::high_resolution_clock::now();
//         auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
//         std::cout << "Function runtime    e1: " << duration1.count() << " ms" << std::endl;
//         auto start2 = std::chrono::high_resolution_clock::now();
//         c =  std::move(MultiplyWithGF2Matrix(clients,VLinMatrices[r-1], c));
//         auto end2 = std::chrono::high_resolution_clock::now();
//         auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
//         std::cout << "Function runtime    e2: " << duration2.count() << " ms" << std::endl;
//         Xor128(clients,c,expan128(clients,Vroundconstants[r-1],c.size()));

//         Xor128(clients,c,expan128(clients,Vroundkeys[r],c.size()));

//     }
//     return c;
// }


// Value decrypt (Clients& clients,const Value128& message)
// {
//     Value128 c = message;
//     for (unsigned r = rounds; r > 0; --r) {
//         Xor128(clients,c,expan128(clients,Vroundkeys[r],c.size()));
//         Xor128(clients,c,expan128(clients,Vroundconstants[r-1],c.size()));
//         c =  MultiplyWithGF2Matrix(clients,VinvLinMatrices[r-1], c);
//         c =  invSubstitution(clients,c);
//     }
//     Xor128(clients,c,expan128(clients,Vroundkeys[0],message.size()));
//     return conv128to64(c);
// }

// Value8 getLast3(const Value128& a,size_t offset)
// {
//     std::vector<int8_t> res(a.size());
//     // #pragma omp parallel for
//     for(int i=0;i<a.size();i++)
//     {
//         res[i]=static_cast<int8_t>((a[i]>>offset)&7);
//     }
//     Value8 val(std::move(res));val.set_fraction_bits(a.get_fraction_bits());val.set_share_type(1);
//     return val;
// }


// std::vector<int8_t> Andcount(Clients& clients,const Value128& a,const Value128& b)
// {
//     std:: vector<int8_t> res(a.size());
//     static int m=b.size();
//     for(int i=0;i<a.size();i++)
//     {
//         int j=i%m;
//         __uint128_t cur=static_cast<__uint128_t>(a[i]&b[j]);
//         int64_t cur1=(cur^(cur>>64));

//         // Compute parity directly
//         res[i] = __builtin_parityll(cur1);
//     }
//     return res;
// }

// void set_key (Clients& clients,Value128 k){
//     key = k;
//     keyschedule(clients);
// }

// void print_matrices();

// private:
// // LowMC private data members //
//     // The Sbox And128 its inverse   


//     std::vector<std::vector<block>> LinMatrices;
//         // Stores the binary matrices for each round
//     std::vector<std::vector<block>> invLinMatrices;
//         // Stores the inverses of LinMatrices
//     std::vector<block> roundconstants;
//         // Stores the round constants
//     Value128 key=make_constB128(0,1);
//         //Stores the master key
//     std::vector<std::vector<keyblock>> KeyMatrices;
//         // Stores the matrices that generate the round keys


//     std::vector<std::vector<Value128>> VLinMatrices;
//         // Stores the binary matrices for each round
//     std::vector<std::vector<Value128>> VinvLinMatrices;
//         // Stores the inverses of LinMatrices
//     std::vector<Value128> Vroundconstants;

//     std::vector<std::vector<Value128>> VKeyMatrices;
//         // Stores the matrices that generate the round keys
//     std::vector<Value128> Vroundkeys;
//         // Stores the round keys
// // LowMC private functions //
//     Value128 Substitution (Clients& clients,const Value128& msg){
//         Value128 temp=msg;
//         //Get the identity part of the message
//         Rshift128(clients,temp,3*numofboxes);
//         //Get the rest through the Sboxes
//         for (unsigned i = 1; i <= numofboxes; ++i) {
//             Lshift128(clients,temp,3);
//             auto last3=getLast3(msg,3*(numofboxes-i));
//             auto idx=reconstruct8(clients,last3);
//             XorBox(temp,idx);
//         }
//         return temp;
//     }
//         // The substitution layer
//     Value128 invSubstitution (Clients& clients,const Value128& msg){
//         Value128 temp=msg;
//         Value128 message=msg;
//         //Get the identity part of the message
//         Rshift128(clients,temp,3*numofboxes);

//         //Get the rest through the invSboxes
//         for (unsigned i = 1; i <= numofboxes; ++i) {
//             Lshift128(clients,temp,3);
//             auto last3=getLast3(message,3*(numofboxes-i));
//             auto idx=reconstruct8(clients,last3);
//             XoriBox(temp,idx);
//         }
//         return temp;
//     }
//         // The inverse substitution layer

//     Value128 MultiplyWithGF2Matrix
//         (Clients& clients,const std::vector<Value128>& matrix, const Value128& msg){
//             std::vector<__int128> temp1(msg.size(),0);
//              // 定义一个局部变量用于存储每个线程的结果
//             std::vector<__int128> local_temp(msg.size(), 0);

//             #pragma omp parallel
//             {
//                 std::vector<__int128> local_temp(msg.size(), 0);

//                 #pragma omp for
//                 for (unsigned i = 0; i < blocksize; ++i) {
//                     auto res = Andcount(clients, msg, matrix[i]);
//                     for (int j = 0; j < local_temp.size(); j++) {
//                         local_temp[j] |= static_cast<__uint128_t>(res[j]) << i;
//                     }
//                 }

//                 // 使用 critical 合并每个线程的结果到主数组
//                 #pragma omp critical
//                 {
//                     for (int j = 0; j < temp1.size(); j++) {
//                         temp1[j] |= local_temp[j];
//                     }
//                 }
//             }
//             Value128 temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
//             // auto res=reconstruct128(clients,temp);
//             // for(int i=0;i<res.size();i++)
//             // {
//             //     std::cout<<res[i]<<" ";
//             // }
//             // std::cout<<std::endl;
//             return temp;
//         }
//         // For the linear layer
//     Value128 MultiplyWithGF2Matrix_Key
//         (Clients& clients,const std::vector<Value128>& matrix, const Value128& k){
//             std::vector<__int128> temp1(k.size(),0);
//             std::vector<__int128> local_temp(k.size(), 0);

//             #pragma omp parallel
//             {
//                 std::vector<__int128> local_temp(k.size(), 0);

//                 #pragma omp for
//                 for (unsigned i = 0; i < blocksize; ++i) {
//                     auto res = Andcount(clients, k, matrix[i]);
//                     for (int j = 0; j < local_temp.size(); j++) {
//                         local_temp[j] |= static_cast<__uint128_t>(res[j]) << i;
//                     }
//                 }

//                 // 使用 critical 合并每个线程的结果到主数组
//                 #pragma omp critical
//                 {
//                     for (int j = 0; j < temp1.size(); j++) {
//                         temp1[j] |= local_temp[j];
//                     }
//                 }
//             }
//         Value128 temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
//         return temp;
//         }
//         // For generating the round keys

//     void keyschedule (Clients& clients){
//         Vroundkeys.clear();
//         for (unsigned r = 0; r <= rounds; ++r) {

//             Vroundkeys.push_back( MultiplyWithGF2Matrix_Key (clients,VKeyMatrices[r], key) );
//         }
//         return;
//     }
//         //Creates the round keys from the master key

// void instantiate_LowMC () {
//     // Create LinMatrices and invLinMatrices
//     LinMatrices.clear();
//     invLinMatrices.clear();
//     for (unsigned r = 0; r < rounds; ++r) {
//         // Create matrix
//         std::vector<block> mat;
//         // Fill matrix with random bits
//         do {
//             mat.clear();
//             for (unsigned i = 0; i < blocksize; ++i) {
//                 mat.push_back( getrandblock () );
//             }
//         // Repeat if matrix is not invertible
//         } while ( rank_of_Matrix(mat) != blocksize );
//         LinMatrices.push_back(mat);
//         invLinMatrices.push_back(invert_Matrix (LinMatrices.back()));
//     }

//     // Create roundconstants
//     roundconstants.clear();
//     for (unsigned r = 0; r < rounds; ++r) {
//         roundconstants.push_back( getrandblock () );
//     }

//     // Create KeyMatrices
//     KeyMatrices.clear();
//     for (unsigned r = 0; r <= rounds; ++r) {
//         // Create matrix
//         std::vector<keyblock> mat;
//         // Fill matrix with random bits
//         do {
//             mat.clear();
//             for (unsigned i = 0; i < blocksize; ++i) {
//                 mat.push_back( getrandkeyblock () );
//             }
//         // Repeat if matrix is not of maximal rank
//         } while ( rank_of_Matrix_Key(mat) < std::min(blocksize, keysize) );
//         KeyMatrices.push_back(mat);
//     }
    
//     return;
// }


// /////////////////////////////
// // Binary matrix functions //
// /////////////////////////////


// unsigned rank_of_Matrix (const std::vector<block> matrix) {
//     std::vector<block> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 1; col <= size; ++col) {
//         if ( !mat[row][size-col] ) {
//             unsigned r = row;
//             while (r < mat.size() && !mat[r][size-col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][size-col] ) mat[i] ^= mat[row];
//         }
//         ++row;
//         if (row == size) break;
//     }
//     return row;
// }


// unsigned rank_of_Matrix_Key (const std::vector<keyblock> matrix) {
//     std::vector<keyblock> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 1; col <= size; ++col) {
//         if ( !mat[row][size-col] ) {
//             unsigned r = row;
//             while (r < mat.size() && !mat[r][size-col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][size-col] ) mat[i] ^= mat[row];
//         }
//         ++row;
//         if (row == size) break;
//     }
//     return row;
// }


// std::vector<block> invert_Matrix (const std::vector<block> matrix) {
//     std::vector<block> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     std::vector<block> invmat(blocksize, 0); //To hold the inverted matrix
//     for (unsigned i = 0; i < blocksize; ++i) {
//         invmat[i][i] = 1;
//     }

//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 0; col < size; ++col) {
//         if ( !mat[row][col] ) {
//             unsigned r = row+1;
//             while (r < mat.size() && !mat[r][col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//                 temp = invmat[row];
//                 invmat[row] = invmat[r];
//                 invmat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][col] ) {
//                 mat[i] ^= mat[row];
//                 invmat[i] ^= invmat[row];
//             }
//         }
//         ++row;
//     }

//     //Transform to identity matrix
//     for (unsigned col = size; col > 0; --col) {
//         for (unsigned r = 0; r < col-1; ++r) {
//             if (mat[r][col-1]) {
//                 mat[r] ^= mat[col-1];
//                 invmat[r] ^= invmat[col-1];
//             }
//         }
//     }

//     return invmat;
// }


//     // keyblock keyXor128 (const keyblock& a, const keyblock& b);
//     // block blockXor128 (const block& a, const block& b);
//     // block keyAnd128 (const keyblock& a, const keyblock& b);
//     // block blockAnd128 (const block& a, const block& b);

//     // block blockLshift128(const block& a, size_t offset);
//     // keyblock keyLshift128(const keyblock& a, size_t offset);

//     // block blockRshift128(const block& a, size_t offset);
//     // keyblock keyRshift128(const keyblock& a, size_t offset);
// // RAnd128om bits functions //
//  block getrandblock () {
//     block tmp = 0;
//     for (unsigned i = 0; i < blocksize; ++i) tmp[i] = getrandbit ();
//     return tmp;
// }

// keyblock getrandkeyblock () {
//     keyblock tmp = 0;
//     for (unsigned i = 0; i < keysize; ++i) tmp[i] = getrandbit ();
//     return tmp;
// }

//     // __int128 keyCount(const block& a);
// void Count(Clients& clients,Value128& a){

//     #pragma omp parallel for
//     for(int i=0;i<a.size();i++)
//     {
//         auto cur=static_cast<__uint128_t>(a[i]);
//         uint64_t lower = static_cast<uint64_t>(cur);
//         uint64_t upper = static_cast<uint64_t>(cur >> 64);

//         // Compute parity directly
//         a[i] = (__builtin_parityll(lower) + __builtin_parityll(upper))%2;
//     }
// }
// void convLineInvLinMatrices(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<__int128> temp((rounds)*blocksize);
//     int num=0;

//     for(int i=0;i<rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=to_int128(invLinMatrices[i][j]);
//         }
//     }


//     Value128 v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo128(v,1);
//     clients.SendTo128(v,2);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VinvLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// else{
//     auto v=clients.recv128(0);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VinvLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
// void convLineLinMatrices(Clients& clients){
//         size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<__int128> temp((rounds)*blocksize);
//     int num=0;

//     for(int i=0;i<rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=to_int128(LinMatrices[i][j]);
//         }
//     }

//     Value128 v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo128(v,1);
//     clients.SendTo128(v,2);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);

//         if(temp1.size()==blocksize)
//         {
//             VLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }

// }
// else{
//     auto v=clients.recv128(0);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
// void convRoundconstants(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<__int128> temp((rounds));
//     int num=0;

//     for(int i=0;i<rounds;i++)
//     {
//         temp[num++]=to_int128(roundconstants[i]);
//     }

//     Value128 v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo128(v,1);
//     clients.SendTo128(v,2);
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp1(2);
//         temp1[0]=temp1[1]=v[i];
//         Value128 v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
//         Vroundconstants.push_back(v);
//     }
// }
// else{
//     auto v=clients.recv128(0);
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp1(2);
//         temp1[0]=temp1[1]=v[i];
//         Value128 v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
//         Vroundconstants.push_back(v);
//     }
// }
// }
// void convkeyMatrices(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<__int128> temp((rounds+1)*blocksize);
//     int num=0;

//     for(int i=0;i<=rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=to_int128(KeyMatrices[i][j]);
//         }
//     }

//     Value128 v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo128(v,1);
//     clients.SendTo128(v,2);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VKeyMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// else{
//     auto v=clients.recv128(0);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VKeyMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
//     // Uses the Grain LSFR as self-shrinking generator to create pseudorandom bits
// // Is initialized with the all 1s state
// // The first 160 bits are thrown away
// bool getrandbit () {
//     static std::bitset<80> state; //Keeps the 80 bit LSFR state
//     bool tmp = 0;
//     //If state has not been initialized yet
//     if (state.none ()) {
//         state.set (); //Initialize with all bits set
//         //Throw the first 160 bits away
//         for (unsigned i = 0; i < 160; ++i) {
//             //Update the state
//             tmp =  state[0] ^ state[13] ^ state[23]
//                        ^ state[38] ^ state[51] ^ state[62];
//             state >>= 1;
//             state[79] = tmp;
//         }
//     }
//     //choice records whether the first bit is 1 or 0.
//     //The second bit is produced if the first bit is 1.
//     bool choice = false;
//     do {
//         //Update the state
//         tmp =  state[0] ^ state[13] ^ state[23]
//                    ^ state[38] ^ state[51] ^ state[62];
//         state >>= 1;
//         state[79] = tmp;
//         choice = tmp;
//         tmp =  state[0] ^ state[13] ^ state[23]
//                    ^ state[38] ^ state[51] ^ state[62];
//         state >>= 1;
//         state[79] = tmp;
//     } while (! choice);
//     return tmp;}


// };



#pragma once

#include "../Value/value128.h"
#include "../network/clients.hpp"
#include "../Operator/operator.h"
#include "Lowmcutil.hpp"

#include<vector>
#include<bitset>
#include<utility>
#include <omp.h>


template <size_t blocksize=64, size_t keysize=128, size_t numofboxes=14, size_t rounds=10>
class LowMC
{

public:
    static constexpr unsigned identitysize = blocksize - 3 * numofboxes;
    using block = std::bitset<blocksize>;
    using keyblock = std::bitset<keysize>;
    size_t N;

void initRandomMatrixE(std::vector<std::vector<uint64_t>>& E) {
    // 使用更快的 xorshift 随机数生成器
    struct xorshift64 {
        uint64_t state;
        explicit xorshift64(uint64_t seed) : state(seed) {}
        uint64_t operator()() {
            state ^= state << 13;
            state ^= state >> 7;
            state ^= state << 17;
            return state;
        }
    };
    
    xorshift64 rng(998244353);
    
    E.resize(blocksize, std::vector<uint64_t>(N, 0));
    for (size_t j = 0; j < blocksize; j++) {
        for (size_t k = 0; k < N; k++) {
            E[j][k] = rng(); // 直接使用生成器，避免分布对象开销
        }
    }
}
std::bitset<blocksize> computeXE(const std::vector<int64_t>& x, const std::vector<std::vector<uint64_t>>& E) {
    std::bitset<blocksize> result;

    for (size_t j = 0; j < blocksize; j++) {
        bool parity = false;
        for (size_t k = 0; k < N; k++) {
            // Compute x[k] & E[j][k], then XOR all bits
            uint64_t masked = x[k] & E[j][k];
            parity ^= (__builtin_parityll(masked)); // Mod 2 sum
        }
        result[j] = parity; // Set j-th bit directly in bitset
    }

    return result;
}
std::vector<block> conv(const std::vector<Value>& a)
{
    N=a.size();
    std::vector<std::vector<uint64_t>> E;
    initRandomMatrixE(E);
    std::vector<block> res(a[0].size()/2);
    if(a.size()>1)
    {
        #pragma omp parallel for
        for(int i=0;i<a[0].size();i+=2)
        {
            std::vector<int64_t> mid(a.size());
            for(int j=0;j<a.size();j++) mid[j]=a[j][i];
            res[i/2]=computeXE(mid,E);
        }
    }
    else
    {
        for(int i=0;i<a[0].size();i+=2)
        {
            res[i/2]=a[0][i];
        }
    }
    return res;
}



std::vector<block> convblock(const std::vector<block>& a)
{
    std::vector<block> res(a.size()/2);
    for(int i=0;i<a.size();i+=2) res[i/2]=a[i];
    return res;
}



LowMC (Clients& clients,const std::vector<keyblock>& k) {
    key = k;
    if(clients.getPid()==0) instantiate_LowMC();

    convkeyMatrices(clients);

    convLineInvLinMatrices(clients);

    convLineLinMatrices(clients);

    convRoundconstants(clients);

    keyschedule(clients);   
    // std::cout<<invLinMatrices[0][0]<<" "<<invLinMatrices[0][1]<<'\n';
};

std::vector<block> encrypt (Clients& clients,const std::vector<Value>& mg)
{
    auto c=conv(mg);
    
    init_per(c.size(),key.size());

    Xor_bitset<blocksize>(clients,c,roundkeys[0],per1);

    for (unsigned r = 1; r <= rounds; ++r) {

        Substitution(clients,c);

        MultiplyWithGF2Matrix(clients,LinMatrices[r-1], c);

        Xor_bitset(clients,c,std::vector<block>{roundconstants[r-1]},per2);

        Xor_bitset(clients,c,roundkeys[r],per1);
    }
    std::vector<block> res(mg[0].size());
    auto c1=clients.SendRoud_bitset(c);
    for(int i=0;i<c.size();i++)
    {
        res[2*i]=c1[i];
        res[2*i+1]=c[i];
    }
    return res;
}


Value decrypt (Clients& clients,const std::vector<block>& message)
{
    auto c=convblock(message);
    for (unsigned r = rounds; r > 0; --r) {
        Xor_bitset(clients,c,roundkeys[r],per1);
        Xor_bitset(clients,c,std::vector<block>{roundconstants[r-1]},per2);
        MultiplyWithGF2Matrix(clients,invLinMatrices[r-1], c);
        invSubstitution(clients,c);
    }
    Xor_bitset<blocksize>(clients,c,roundkeys[0],per1);
    std::vector<int64_t> res(message.size());
    auto c1=clients.SendRoud_bitset(c);
    for(int i=0;i<c.size();i++)
    {
        res[2*i]=c1[i].to_ullong();
        res[2*i+1]=c[i].to_ullong();
    }
    Value R(std::move(res));R.set_share_type(1);
    return R;
}

void set_key (Clients& clients,std::vector<keyblock> k){
    key = k;
    keyschedule(clients);
}

void print_matrices();

private:
// LowMC private data members //
    // The Sbox And128 its inverse   

    std::vector<std::vector<block>> LinMatrices;
        // Stores the binary matrices for each round
    std::vector<std::vector<block>> invLinMatrices;
        // Stores the inverses of LinMatrices
    std::vector<block> roundconstants;
        // Stores the round constants
    std::vector<keyblock> key;
        //Stores the master key
    std::vector<std::vector<keyblock>> KeyMatrices;
        // Stores the matrices that generate the round keys
    std::vector<std::vector<block>> roundkeys;

    std::vector<size_t> per1;
    std::vector<size_t> per2;
 // LowMC private functions //
    void Substitution (Clients& clients, std::vector<block>& msg){
        //Get the identity part of the message


        //Get the rest through the invSboxes
        std::vector<std::bitset<14>> a(msg.size());
        std::vector<std::bitset<14>> b(msg.size());
        std::vector<std::bitset<14>> c(msg.size());

        #pragma omp parallel for
        for(int i=0;i<msg.size();i++)
        {
            for (unsigned j = 1; j <= numofboxes; ++j) {
                size_t offset=3*(numofboxes-j);
                a[i][j-1]=msg[i][offset];
                
                b[i][j-1]=msg[i][offset+1];
                c[i][j-1]=msg[i][offset+2];

            }
        }
        // std::vector<std::bitset<14>> res2(c.size()*2);
        // auto a2=clients.SendRoud_bitset(b);
        // for(int i=0;i<a2.size();i++)
        // {
        //     res2[2*i]=a2[i];
        //     res2[2*i+1]=b[i];
        // }
        // auto cres2=reconstruct_bitset(clients,res2);
        // if(clients.getPid()==2)
        // {
        //       for(int i=0;i<cres2.size();i++)
        //     {
        //         std::cout<<cres2[i]<<" ";
        //     }
        // }
        auto aa=clients.SendRoud_bitset(a);
        auto bb=clients.SendRoud_bitset(b);
        auto cc=clients.SendRoud_bitset(c);
        auto bc=And_bitset_real(clients,b,bb,c,cc);
        auto ac=And_bitset_real(clients,a,aa,c,cc);
        auto ab=And_bitset_real(clients,a,aa,b,bb);
       

        Xor_bitset_real(clients,bc,a);
        Xor_bitset_real(clients,a,b);
        Xor_bitset_real(clients,ac,a);
        Xor_bitset_real(clients,a,c);
        Xor_bitset_real(clients,ab,a);


      
        Rshift_bitset(clients,msg,3*numofboxes);
        Lshift_bitset(clients,msg,3*numofboxes);
        #pragma omp parallel for
        for(int i=0;i<msg.size();i++)
        {
            for(int j=0;j<14;j++)
            {
                size_t offset=3*(numofboxes-j-1);
                msg[i][offset]=bc[i][j];
                msg[i][offset+1]=ac[i][j];
                msg[i][offset+2]=ab[i][j];
            }
        }

    }
        // The substitution layer
    void invSubstitution (Clients& clients,std::vector<block>& msg){
        std::vector<std::bitset<14>> a(msg.size());
        std::vector<std::bitset<14>> b(msg.size());
        std::vector<std::bitset<14>> c(msg.size());

        #pragma omp parallel for
        for(int i=0;i<msg.size();i++)
        {
            for (unsigned j = 1; j <= numofboxes; ++j) {
                size_t offset=3*(numofboxes-j);
                a[i][j-1]=msg[i][offset];
                b[i][j-1]=msg[i][offset+1];
                c[i][j-1]=msg[i][offset+2];
            } 
        }

        auto aa=clients.SendRoud_bitset(a);
        auto bb=clients.SendRoud_bitset(b);
        auto cc=clients.SendRoud_bitset(c);
        auto bc=And_bitset_real(clients,b,bb,c,cc);
        auto ac=And_bitset_real(clients,a,aa,c,cc);
        auto ab=And_bitset_real(clients,a,aa,b,bb);



     
        Xor_bitset_real(clients,ac,b);
        Xor_bitset_real(clients,a,b);
        Xor_bitset_real(clients,bc,a);
        Xor_bitset_real(clients,a,c);
        Xor_bitset_real(clients,ab,a);

        Rshift_bitset(clients,msg,3*numofboxes);
        Lshift_bitset(clients,msg,3*numofboxes);

        #pragma omp parallel for
        for(int i=0;i<msg.size();i++)
        {
            for(int j=0;j<14;j++)
            {
                size_t offset=3*(numofboxes-j-1);
                msg[i][offset]=bc[i][j];
                msg[i][offset+1]=ac[i][j];
                msg[i][offset+2]=ab[i][j];
            }
        }

    }
        // The inverse substitution layer

    void MultiplyWithGF2Matrix
        (Clients& clients,const std::vector<block>& matrix,  std::vector<block>& msg){
            // std::vector<block> temp(msg.size(),0);
            // std::cout<<matrix.size()<<'\n';
            #pragma omp parallel for
            for(int i=0;i<msg.size();i++)
            {        
                block tmp=msg[i];
                for (unsigned j = 0; j < blocksize; ++j) {
                    block cur=tmp&matrix[j];
                    if(blocksize>64)
                    {
                        msg[i][j]=__builtin_parityll(((cur & std::bitset<blocksize>((1ULL << (blocksize/2)) - 1)).to_ullong()^(cur>>(blocksize/2)).to_ullong()));
                    }
                    else{
                        msg[i][j]=__builtin_parityll(cur.to_ullong());
                    }
                   
                }
            }       
        }
        // For the linear layer
    std::vector<block> MultiplyWithGF2Matrix_Key
        (Clients& clients,const std::vector<keyblock>& matrix, const std::vector<keyblock>& k){
            std::vector<block> temp(k.size(),0);
                for (unsigned i = 0; i < blocksize; ++i) {
                    // std::cout<<k.size()<<'\n';
                    auto res = Andcount_bitset(clients, k, std::vector<keyblock>{matrix[i]});
                    // std::cout<<res.size()<<'\n';
                    // if(i==0) std::cout<<res[0]<<" "<<res[1]<<'\n';
                    for (int j = 0; j < temp.size(); j++) {
                        temp[j][i]=res[j];
                    }
                }

        return temp;
        }
        // For generating the round keys

    void keyschedule (Clients& clients){
        roundkeys.clear();
        for (unsigned r = 0; r <= rounds; ++r) {
            roundkeys.push_back( MultiplyWithGF2Matrix_Key (clients,KeyMatrices[r], key) );
        }
        return;
    }
        //Creates the round keys from the master key

void instantiate_LowMC () {
    // Create LinMatrices and invLinMatrices
    LinMatrices.clear();
    invLinMatrices.clear();
    for (unsigned r = 0; r < rounds; ++r) {
        // Create matrix
        std::vector<block> mat;
        // Fill matrix with random bits
        do {
            mat.clear();
            for (unsigned i = 0; i < blocksize; ++i) {
                mat.push_back( getrandblock () );
            }
        // Repeat if matrix is not invertible
        } while ( rank_of_Matrix(mat) != blocksize );
        LinMatrices.push_back(mat);
        invLinMatrices.push_back(invert_Matrix (LinMatrices.back()));
    }
    // Create roundconstants
    roundconstants.clear();
    for (unsigned r = 0; r < rounds; ++r) {
        roundconstants.push_back( getrandblock () );
    }

    // Create KeyMatrices
    KeyMatrices.clear();
    for (unsigned r = 0; r <= rounds; ++r) {
        // Create matrix
        std::vector<keyblock> mat;
        // Fill matrix with random bits
        do {
            mat.clear();
            for (unsigned i = 0; i < blocksize; ++i) {
                mat.push_back( getrandkeyblock () );
            }
        // Repeat if matrix is not of maximal rank
        } while ( rank_of_Matrix_Key(mat) < std::min(blocksize, keysize) );
        KeyMatrices.push_back(mat);
    }
    return;
}


/////////////////////////////
// Binary matrix functions //
/////////////////////////////


void init_per(size_t msg,size_t key)
{
    std::vector<size_t> res(msg);
    size_t m=2*msg/key;
    for(int i=0;i<msg;i++)
    {
        if(i%2==0) res[i]=2*size_t(i/m);
        else res[i]=2*size_t(i/m)+1;
    }
    per1=res;

    per2=std::vector<size_t>(msg,0);
}

unsigned rank_of_Matrix (const std::vector<block> matrix) {
    std::vector<block> mat; //Copy of the matrix 
    for (auto u : matrix) {
        mat.push_back(u);
    }
    unsigned size = mat[0].size();
    //Transform to upper triangular matrix
    unsigned row = 0;
    for (unsigned col = 1; col <= size; ++col) {
        if ( !mat[row][size-col] ) {
            unsigned r = row;
            while (r < mat.size() && !mat[r][size-col]) {
                ++r;
            }
            if (r >= mat.size()) {
                continue;
            } else {
                auto temp = mat[row];
                mat[row] = mat[r];
                mat[r] = temp;
            }
        }
        for (unsigned i = row+1; i < mat.size(); ++i) {
            if ( mat[i][size-col] ) mat[i] ^= mat[row];
        }
        ++row;
        if (row == size) break;
    }
    return row;
}


unsigned rank_of_Matrix_Key (const std::vector<keyblock> matrix) {
    std::vector<keyblock> mat; //Copy of the matrix 
    for (auto u : matrix) {
        mat.push_back(u);
    }
    unsigned size = mat[0].size();
    //Transform to upper triangular matrix
    unsigned row = 0;
    for (unsigned col = 1; col <= size; ++col) {
        if ( !mat[row][size-col] ) {
            unsigned r = row;
            while (r < mat.size() && !mat[r][size-col]) {
                ++r;
            }
            if (r >= mat.size()) {
                continue;
            } else {
                auto temp = mat[row];
                mat[row] = mat[r];
                mat[r] = temp;
            }
        }
        for (unsigned i = row+1; i < mat.size(); ++i) {
            if ( mat[i][size-col] ) mat[i] ^= mat[row];
        }
        ++row;
        if (row == size) break;
    }
    return row;
}


std::vector<block> invert_Matrix (const std::vector<block> matrix) {
    std::vector<block> mat; //Copy of the matrix 
    for (auto u : matrix) {
        mat.push_back(u);
    }
    std::vector<block> invmat(blocksize, 0); //To hold the inverted matrix
    for (unsigned i = 0; i < blocksize; ++i) {
        invmat[i][i] = 1;
    }

    unsigned size = mat[0].size();
    //Transform to upper triangular matrix
    unsigned row = 0;
    for (unsigned col = 0; col < size; ++col) {
        if ( !mat[row][col] ) {
            unsigned r = row+1;
            while (r < mat.size() && !mat[r][col]) {
                ++r;
            }
            if (r >= mat.size()) {
                continue;
            } else {
                auto temp = mat[row];
                mat[row] = mat[r];
                mat[r] = temp;
                temp = invmat[row];
                invmat[row] = invmat[r];
                invmat[r] = temp;
            }
        }
        for (unsigned i = row+1; i < mat.size(); ++i) {
            if ( mat[i][col] ) {
                mat[i] ^= mat[row];
                invmat[i] ^= invmat[row];
            }
        }
        ++row;
    }

    //Transform to identity matrix
    for (unsigned col = size; col > 0; --col) {
        for (unsigned r = 0; r < col-1; ++r) {
            if (mat[r][col-1]) {
                mat[r] ^= mat[col-1];
                invmat[r] ^= invmat[col-1];
            }
        }
    }

    return invmat;
}


    // keyblock keyXor128 (const keyblock& a, const keyblock& b);
    // block blockXor128 (const block& a, const block& b);
    // block keyAnd128 (const keyblock& a, const keyblock& b);
    // block blockAnd128 (const block& a, const block& b);

    // block blockLshift128(const block& a, size_t offset);
    // keyblock keyLshift128(const keyblock& a, size_t offset);

    // block blockRshift128(const block& a, size_t offset);
    // keyblock keyRshift128(const keyblock& a, size_t offset);
// RAnd128om bits functions //
 block getrandblock () {
    block tmp = 0;
    for (unsigned i = 0; i < blocksize; ++i) tmp[i] = getrandbit ();
    return tmp;
}

keyblock getrandkeyblock () {
    keyblock tmp = 0;
    for (unsigned i = 0; i < keysize; ++i) tmp[i] = getrandbit ();
    return tmp;
}


void convLineInvLinMatrices(Clients& clients){
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<block> temp((rounds)*blocksize);
        int num=0;

        for(int i=0;i<rounds;i++)
        {
            for(int j=0;j<blocksize;j++)
            {
                temp[num++]=invLinMatrices[i][j];
            }
        }

        clients.SendTo_bitset(temp,1);
        clients.SendTo_bitset(temp,2);
    }
    else{
        auto temp=clients.recv_bitset<blocksize>(0);
        std::vector<block> temp1;
        for(int i=0;i<temp.size();i++)
        {
            temp1.push_back(temp[i]);
            if(temp1.size()==blocksize)
            {
                invLinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
}
void convLineLinMatrices(Clients& clients){
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<block> temp((rounds)*blocksize);
        int num=0;

        for(int i=0;i<rounds;i++)
        {
            for(int j=0;j<blocksize;j++)
            {
                temp[num++]=LinMatrices[i][j];
            }
        }

        clients.SendTo_bitset(temp,1);
        clients.SendTo_bitset(temp,2);

    }
    else{
        auto temp=clients.recv_bitset<blocksize>(0);
        std::vector<block> temp1;
        for(int i=0;i<temp.size();i++)
        {
            temp1.push_back(temp[i]);
            if(temp1.size()==blocksize)
            {
                LinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
}
void convRoundconstants(Clients& clients){
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<block> temp((rounds));
        int num=0;

        for(int i=0;i<rounds;i++)
        {
            temp[num++]=roundconstants[i];
        }

        clients.SendTo_bitset(temp,1);
        clients.SendTo_bitset(temp,2);
    }
    else{
        auto temp=clients.recv_bitset<blocksize>(0);
        roundconstants=temp;
    }
}
void convkeyMatrices(Clients& clients){
    size_t pid=clients.getPid();
if(pid==0)
{
    std::vector<keyblock> temp((rounds+1)*blocksize);
    int num=0;
    for(int i=0;i<=rounds;i++)
    {
        for(int j=0;j<blocksize;j++)
        {
            temp[num++]=KeyMatrices[i][j];
        }
    }
    clients.SendTo_bitset<keysize>(temp,1);
    clients.SendTo_bitset<keysize>(temp,2);
}
else{
    auto temp=clients.recv_bitset<keysize>(0); 
    std::vector<keyblock> temp1;
    for(int i=0;i<temp.size();i++)
    {
        temp1.push_back(temp[i]);
        if(temp1.size()==blocksize)
        {
            KeyMatrices.push_back(temp1);
            temp1.clear();
        }
    }
}
}
    // Uses the Grain LSFR as self-shrinking generator to create pseudorandom bits
// Is initialized with the all 1s state
// The first 160 bits are thrown away
bool getrandbit () {
    static std::bitset<80> state; //Keeps the 80 bit LSFR state
    bool tmp = 0;
    //If state has not been initialized yet
    if (state.none ()) {
        state.set (); //Initialize with all bits set
        //Throw the first 160 bits away
        for (unsigned i = 0; i < 160; ++i) {
            //Update the state
            tmp =  state[0] ^ state[13] ^ state[23]
                       ^ state[38] ^ state[51] ^ state[62];
            state >>= 1;
            state[79] = tmp;
        }
    }
    //choice records whether the first bit is 1 or 0.
    //The second bit is produced if the first bit is 1.
    bool choice = false;
    do {
        //Update the state
        tmp =  state[0] ^ state[13] ^ state[23]
                   ^ state[38] ^ state[51] ^ state[62];
        state >>= 1;
        state[79] = tmp;
        choice = tmp;
        tmp =  state[0] ^ state[13] ^ state[23]
                   ^ state[38] ^ state[51] ^ state[62];
        state >>= 1;
        state[79] = tmp;
    } while (! choice);
    return tmp;}


};


// #pragma once

// #include "../Value/value128.h"
// #include "../network/clients.hpp"
// #include "../Operator/operator.h"
// #include "Lowmcutil.hpp"

// #include<vector>
// #include<bitset>
// #include<utility>
// #include <omp.h>

// const unsigned numofboxes = 14;    // Number of Sboxes
// const unsigned blocksize = 64;   // Block size in bits
// const unsigned keysize = 128; // Key size in bits
// const unsigned rounds = 12; // Number of rounds

// const unsigned identitysize = blocksize - 3*numofboxes;
//                   // Size of the identity part in the Sbox layer
// typedef std::bitset<blocksize> block; 
// typedef std::bitset<keysize> keyblock;
// std::vector<int64_t> vSbox =
//     {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
// std::vector<int64_t> vinvSbox =
//     {0x00, 0x01, 0x07, 0x02, 0x05, 0x06, 0x03, 0x04};


// __int128 to_int128(const std::bitset<128>& bits) {
// uint64_t low = (bits & std::bitset<128>(std::numeric_limits<uint64_t>::max())).to_ullong();  // 获取低 64 位
// uint64_t high = ((bits >> 64) & std::bitset<128>(std::numeric_limits<uint64_t>::max())).to_ullong();  // 获取高 64 位

// return (__int128(high) << 64) | low;  // 拼接高位和低位
// }

// Value gen_rand_key(Clients& clients,size_t t)
// {
//     auto k1=gen_rand_vector(t);
//     Value K1(std::move(k1));K1.set_share_type(1);
//     auto K2=clients.SendRoud(K1);
//     std::vector<int64_t> key(K1.size()*2);
//     for(int i=0;i<K1.size();i++) key[2*i]=K2[i],key[2*i+1]=K1[i];
//     Value Key(std::move(key));Key.set_share_type(1);
//     return Key;
// }

// Value conv(const Value& a)
// {
//     std::vector<int64_t> res(a.size()/2);
//     for(int i=0;i<a.size();i+=2) res[i/2]=a[i];
//     return Value(std::move(res));
// }

// Value128 conv64to128(const Value& a)
// {
//     std::vector<__int128> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value128 res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// Value conv128to64(const Value128& a)
// {
//     std::vector<int64_t> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// Value8 conv128to8(const Value& a)
// {
//     std::vector<int8_t> c(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         c[i]=a[i];
//     }
//     Value8 res(std::move(c));res.set_fraction_bits(a.get_fraction_bits());res.set_share_type(a.get_share_type());
//     return res;
// }

// void XorBox(Value& a,const std::vector<int8_t>& b)
// {
//     for(int i=0;i<a.size();i++)
//     {
//         a[i]^=vSbox[b[i/2]];
//     }
// }

// void XoriBox(Value& a,const std::vector<int8_t>& b)
// {
//     for(int i=0;i<a.size();i++)
//     {
//         a[i]^=vinvSbox[b[i/2]];
//     }
// }

// std::vector<uint8_t> Andcount(Clients& clients,const Value& a,const Value& b)
// {
//     std:: vector<uint8_t> res(a.size(),0);
//     auto temp=b[0];
//     for(int i=0;i<a.size();i++)
//     {
//         auto cur=a[i]&temp;
//         res[i] = __builtin_parityll(cur);

//     }
//     return res;
// }

// class LowMC
// {

// public:

// LowMC (Clients& clients,const Value& kk) {
//     auto k=conv64to128(kk);
//     key = k;
//     if(clients.getPid()==0) instantiate_LowMC();

//     convkeyMatrices(clients);

//     convLineInvLinMatrices(clients);

//     convLineLinMatrices(clients);

//     convRoundconstants(clients);

//     keyschedule(clients);   

// };

// Value encrypt (Clients& clients,const Value& mg)
// {
//     auto c=(mg.get_share_type()==1)?mg:A2B(clients,mg);
//     init_per(c.size(),key.size());
//     // auto c1=c;
//     std::vector<int64_t> c_copy(c.size()/2);
//     for(int i=0;i<c.size();i+=2) c_copy[i/2]=c[i];
//     Value c1(std::move(c_copy));c1.set_share_type(c.get_share_type());c1.set_fraction_bits(c.get_fraction_bits());
//     Xor64(clients,c1,Vroundkeys[0],per1);
    
//     for (unsigned r = 1; r <= rounds; ++r) {
//         // auto start1 = std::chrono::high_resolution_clock::now();
//         // c1 =  std::move(Substitution(clients,c1));
//         // auto end1 = std::chrono::high_resolution_clock::now();
//         // auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
//         // std::cout << "Function runtime    e1: " << duration1.count() << " ms" << std::endl;

        
//         c1 =  std::move(MultiplyWithGF2Matrix(clients,VLinMatrices[r-1], c1));

//         Xor64(clients,c1,Vroundconstants[r-1],per2);

//         Xor64(clients,c1,Vroundkeys[r],per1);
        

//     }
//     auto c2=clients.SendRoud(c1);
//     for(int i=0;i<c.size();i+=2){
//         c[i]=c2[i/2];
//         c[i+1]=c1[i/2];
//     }
//     return c;
// }


// Value decrypt (Clients& clients,const Value& message)
// {
//     Value c = message;
//     init_per(c.size(),key.size());
//     // auto c1=c;
//     std::vector<int64_t> c_copy(c.size()/2);
//     for(int i=0;i<c.size();i+=2) c_copy[i/2]=c[i];
//     Value c1(std::move(c_copy));c1.set_share_type(c.get_share_type());c1.set_fraction_bits(c.get_fraction_bits());
//     for (unsigned r = rounds; r > 0; --r) {
      
//         Xor64(clients,c1,Vroundkeys[r],per1);
//         Xor64(clients,c1,Vroundconstants[r-1],per2);
//         c1 =  MultiplyWithGF2Matrix(clients,VinvLinMatrices[r-1], c1);
//         // c =  invSubstitution(clients,c);

       
//     }
//     Xor64(clients,c1,Vroundkeys[0],per1);
//     auto c2=clients.SendRoud(c1);
//     for(int i=0;i<c.size();i+=2){
//         c[i]=c2[i/2];
//         c[i+1]=c1[i/2];
//     }
//     return c;
// }

// Value8 getLast3(const Value& a,size_t offset)
// {
//     std::vector<int8_t> res(a.size());
//     // #pragma omp parallel for
//     for(int i=0;i<a.size();i++)
//     {
//         res[i]=static_cast<int8_t>((a[i]>>offset)&7);
//     }
//     Value8 val(std::move(res));val.set_fraction_bits(a.get_fraction_bits());val.set_share_type(1);
//     return val;
// }

// void init_per(size_t msg,size_t key)
// {
//     std::vector<size_t> res(msg);
//     size_t m=2*msg/key;
//     for(int i=0;i<msg;i++)
//     {
//         if(i%2==0) res[i]=2*size_t(i/m);
//         else res[i]=2*size_t(i/m)+1;
//     }
//     per1=res;

//     per2=std::vector<size_t>(msg,0);
// }



// std::vector<int8_t> Andcount128(Clients& clients,const Value128& a,const Value128& b)
// {
//     std:: vector<int8_t> res(a.size());
//     for(int i=0;i<a.size();i++)
//     {
//         __uint128_t cur=static_cast<__uint128_t>(a[i]&b[0]);
//         int64_t cur1=(cur^(cur>>64));

//         // Compute parity directly
//         res[i] = __builtin_parityll(cur1);
//     }
//     return res;
// }

// void set_key (Clients& clients,Value128 k){
//     key = k;
//     keyschedule(clients);
// }

// void print_matrices();

// private:
// // LowMC private data members //
//     // The Sbox And128 its inverse   


//     std::vector<std::vector<block>> LinMatrices;
//         // Stores the binary matrices for each round
//     std::vector<std::vector<block>> invLinMatrices;
//         // Stores the inverses of LinMatrices
//     std::vector<block> roundconstants;
//         // Stores the round constants
//     Value128 key=make_constB128(0,1);
//         //Stores the master key
//     std::vector<std::vector<keyblock>> KeyMatrices;
//         // Stores the matrices that generate the round keys
//     std::vector<size_t> per1;
//     std::vector<size_t> per2;

//     std::vector<std::vector<Value>> VLinMatrices;
//         // Stores the binary matrices for each round
//     std::vector<std::vector<Value>> VinvLinMatrices;
//         // Stores the inverses of LinMatrices
//     std::vector<Value> Vroundconstants;

//     std::vector<std::vector<Value128>> VKeyMatrices;
//         // Stores the matrices that generate the round keys
//     std::vector<Value> Vroundkeys;
//         // Stores the round keys
// // LowMC private functions //
//     Value Substitution (Clients& clients,const Value& msg){
//         Value temp=msg;
//         //Get the identity part of the message
//         Rshift64(clients,temp,3*numofboxes);
//         //Get the rest through the Sboxes
//         for (unsigned i = 1; i <= numofboxes; ++i) {
//             Lshift64(clients,temp,3);
//             auto last3=getLast3(msg,3*(numofboxes-i));
//             auto idx=reconstruct8(clients,last3);
//             XorBox(temp,idx);
//         }
//         return temp;
//     }
//         // The substitution layer
//     Value invSubstitution (Clients& clients,const Value& msg){
//         Value temp=msg;
//         Value message=msg;
//         //Get the identity part of the message
//         Rshift64(clients,temp,3*numofboxes);

//         //Get the rest through the invSboxes
//         for (unsigned i = 1; i <= numofboxes; ++i) {
//             Lshift64(clients,temp,3);
//             auto last3=getLast3(message,3*(numofboxes-i));
//             auto idx=reconstruct8(clients,last3);
//             XoriBox(temp,idx);
//         }
//         return temp;
//     }
//         // The inverse substitution layer

//     Value MultiplyWithGF2Matrix
//         (Clients& clients,const std::vector<Value>& matrix, const Value& msg){
//             std::vector<int64_t> temp1(msg.size(),0);
//             //  // 定义一个局部变量用于存储每个线程的结果
//             // std::vector<int64_t> local_temp(msg.size(), 0);

//             // #pragma omp parallel
//             // {
//             //     std::vector<int64_t> local_temp(msg.size(), 0);
//             //     std:: vector<uint8_t> res(msg.size(),0);
//             //     #pragma omp for
//             //     for (unsigned i = 0; i < blocksize; ++i) {

//             //         auto temp=matrix[i][0];
//             //         for(int i=0;i<msg.size();i++)
//             //         {
//             //             auto cur=msg[i]&temp;
//             //             res[i] = __builtin_parityll(cur);
                
//             //         }
//             //         for (int j = 0; j < local_temp.size(); j++) {
//             //             local_temp[j] |= static_cast<int64_t>(res[j]) << i;
//             //         }
//             //     }

//             //     // 使用 critical 合并每个线程的结果到主数组
//             //     #pragma omp critical
//             //     {
//             //         for (int j = 0; j < temp1.size(); j++) {
//             //             temp1[j] |= local_temp[j];
//             //         }
//             //     }
//             // }
//             // std::vector<int64_t> temp1(msg.size(),0);
//             // // 定义一个局部变量用于存储每个线程的结果
//             // auto start = std::chrono::high_resolution_clock::now();
//             std:: vector<uint8_t> res(msg.size(),0);
//             // #pragma omp parallel for// 并行化外层循环
//             for (unsigned i = 0; i < blocksize; ++i) {

//                 auto temp=matrix[i][0];
//                 #pragma omp parallel for// 并行化外层循环
//                 for(int k=0;k<msg.size();k++)
//                 {
//                     res[k] = __builtin_parityll(msg[k]&temp);
//                 }
//                 for (int j = 0; j < temp1.size(); j++) {
//                         // temp1[j] |= res[j];
//                     temp1[j] |= static_cast<int64_t>(res[j]) << i;  
//                 }
//             }
           
//             //    auto end = std::chrono::high_resolution_clock::now();
//             //    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//             //    std::cout << "Function runtime1: " << duration.count() << " ms" << std::endl;
//                // 使用 critical 合并每个线程的结果到主数组
//             Value temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
//             // auto res=reconstruct128(clients,temp);
//             // for(int i=0;i<res.size();i++)
//             // {
//             //     std::cout<<res[i]<<" ";
//             // }
//             // std::cout<<std::endl;
//             return temp;
//         }
//         // For the linear layer
//     Value MultiplyWithGF2Matrix_Key
//         (Clients& clients,const std::vector<Value128>& matrix, const Value128& k){
//             std::vector<__int128> temp1(k.size(),0);
//             std::vector<__int128> local_temp(k.size(), 0);

//             #pragma omp parallel
//             {
//                 std::vector<__int128> local_temp(k.size(), 0);

//                 #pragma omp for
//                 for (unsigned i = 0; i < blocksize; ++i) {
//                     auto res = Andcount128(clients, k, matrix[i]);
//                     for (int j = 0; j < local_temp.size(); j++) {
//                         local_temp[j] |= static_cast<__uint128_t>(res[j]) << i;
//                     }
//                 }

//                 // 使用 critical 合并每个线程的结果到主数组
//                 #pragma omp critical
//                 {
//                     for (int j = 0; j < temp1.size(); j++) {
//                         temp1[j] |= local_temp[j];
//                     }
//                 }
//             }
//         Value128 temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
//         return conv128to64(temp);
//         }
//         // For generating the round keys

//     void keyschedule (Clients& clients){
//         Vroundkeys.clear();
//         for (unsigned r = 0; r <= rounds; ++r) {

//             Vroundkeys.push_back( MultiplyWithGF2Matrix_Key (clients,VKeyMatrices[r], key) );
//         }
//         return;
//     }
//         //Creates the round keys from the master key

// void instantiate_LowMC () {
//     // Create LinMatrices and invLinMatrices
//     LinMatrices.clear();
//     invLinMatrices.clear();
//     for (unsigned r = 0; r < rounds; ++r) {
//         // Create matrix
//         std::vector<block> mat;
//         // Fill matrix with random bits
//         do {
//             mat.clear();
//             for (unsigned i = 0; i < blocksize; ++i) {
//                 mat.push_back( getrandblock () );
//             }
//         // Repeat if matrix is not invertible
//         } while ( rank_of_Matrix(mat) != blocksize );
//         LinMatrices.push_back(mat);
//         invLinMatrices.push_back(invert_Matrix (LinMatrices.back()));
//     }

//     // Create roundconstants
//     roundconstants.clear();
//     for (unsigned r = 0; r < rounds; ++r) {
//         roundconstants.push_back( getrandblock () );
//     }

//     // Create KeyMatrices
//     KeyMatrices.clear();
//     for (unsigned r = 0; r <= rounds; ++r) {
//         // Create matrix
//         std::vector<keyblock> mat;
//         // Fill matrix with random bits
//         do {
//             mat.clear();
//             for (unsigned i = 0; i < blocksize; ++i) {
//                 mat.push_back( getrandkeyblock () );
//             }
//         // Repeat if matrix is not of maximal rank
//         } while ( rank_of_Matrix_Key(mat) < std::min(blocksize, keysize) );
//         KeyMatrices.push_back(mat);
//     }
    
//     return;
// }


// /////////////////////////////
// // Binary matrix functions //
// /////////////////////////////


// unsigned rank_of_Matrix (const std::vector<block> matrix) {
//     std::vector<block> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 1; col <= size; ++col) {
//         if ( !mat[row][size-col] ) {
//             unsigned r = row;
//             while (r < mat.size() && !mat[r][size-col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][size-col] ) mat[i] ^= mat[row];
//         }
//         ++row;
//         if (row == size) break;
//     }
//     return row;
// }


// unsigned rank_of_Matrix_Key (const std::vector<keyblock> matrix) {
//     std::vector<keyblock> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 1; col <= size; ++col) {
//         if ( !mat[row][size-col] ) {
//             unsigned r = row;
//             while (r < mat.size() && !mat[r][size-col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][size-col] ) mat[i] ^= mat[row];
//         }
//         ++row;
//         if (row == size) break;
//     }
//     return row;
// }


// std::vector<block> invert_Matrix (const std::vector<block> matrix) {
//     std::vector<block> mat; //Copy of the matrix 
//     for (auto u : matrix) {
//         mat.push_back(u);
//     }
//     std::vector<block> invmat(blocksize, 0); //To hold the inverted matrix
//     for (unsigned i = 0; i < blocksize; ++i) {
//         invmat[i][i] = 1;
//     }

//     unsigned size = mat[0].size();
//     //Transform to upper triangular matrix
//     unsigned row = 0;
//     for (unsigned col = 0; col < size; ++col) {
//         if ( !mat[row][col] ) {
//             unsigned r = row+1;
//             while (r < mat.size() && !mat[r][col]) {
//                 ++r;
//             }
//             if (r >= mat.size()) {
//                 continue;
//             } else {
//                 auto temp = mat[row];
//                 mat[row] = mat[r];
//                 mat[r] = temp;
//                 temp = invmat[row];
//                 invmat[row] = invmat[r];
//                 invmat[r] = temp;
//             }
//         }
//         for (unsigned i = row+1; i < mat.size(); ++i) {
//             if ( mat[i][col] ) {
//                 mat[i] ^= mat[row];
//                 invmat[i] ^= invmat[row];
//             }
//         }
//         ++row;
//     }

//     //Transform to identity matrix
//     for (unsigned col = size; col > 0; --col) {
//         for (unsigned r = 0; r < col-1; ++r) {
//             if (mat[r][col-1]) {
//                 mat[r] ^= mat[col-1];
//                 invmat[r] ^= invmat[col-1];
//             }
//         }
//     }

//     return invmat;
// }


//     // keyblock keyXor128 (const keyblock& a, const keyblock& b);
//     // block blockXor128 (const block& a, const block& b);
//     // block keyAnd128 (const keyblock& a, const keyblock& b);
//     // block blockAnd128 (const block& a, const block& b);

//     // block blockLshift128(const block& a, size_t offset);
//     // keyblock keyLshift128(const keyblock& a, size_t offset);

//     // block blockRshift128(const block& a, size_t offset);
//     // keyblock keyRshift128(const keyblock& a, size_t offset);
// // RAnd128om bits functions //
//  block getrandblock () {
//     block tmp = 0;
//     for (unsigned i = 0; i < blocksize; ++i) tmp[i] = getrandbit ();
//     return tmp;
// }

// keyblock getrandkeyblock () {
//     keyblock tmp = 0;
//     for (unsigned i = 0; i < keysize; ++i) tmp[i] = getrandbit ();
//     return tmp;
// }

//     // __int128 keyCount(const block& a);
// void Count(Clients& clients,Value128& a){

//     #pragma omp parallel for
//     for(int i=0;i<a.size();i++)
//     {
//         auto cur=static_cast<__uint128_t>(a[i]);
//         uint64_t lower = static_cast<uint64_t>(cur);
//         uint64_t upper = static_cast<uint64_t>(cur >> 64);

//         // Compute parity directly
//         a[i] = (__builtin_parityll(lower) + __builtin_parityll(upper))%2;
//     }
// }
// void convLineInvLinMatrices(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<int64_t> temp((rounds)*blocksize);
//     int num=0;
//     for(int i=0;i<rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=invLinMatrices[i][j].to_ulong();
//         }
//     }

//     Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo(v,1);
//     clients.SendTo(v,2);
//     std::vector<Value> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VinvLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// else{
//     auto v=clients.recv(0);
//     std::vector<Value> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VinvLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
// void convLineLinMatrices(Clients& clients){
//         size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<int64_t> temp((rounds)*blocksize);
//     int num=0;

//     for(int i=0;i<rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=LinMatrices[i][j].to_ulong();
//         }
//     }

//     Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo(v,1);
//     clients.SendTo(v,2);
//     std::vector<Value> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);

//         if(temp1.size()==blocksize)
//         {
//             VLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }

// }
// else{
//     auto v=clients.recv(0);
//     std::vector<Value> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VLinMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
// void convRoundconstants(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<int64_t> temp((rounds));
//     int num=0;

//     for(int i=0;i<rounds;i++)
//     {
//         temp[num++]=roundconstants[i].to_ulong();
//     }

//     Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo(v,1);
//     clients.SendTo(v,2);
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp1(2);
//         temp1[0]=temp1[1]=v[i];
//         Value v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
//         Vroundconstants.push_back(v);
//     }
// }
// else{
//     auto v=clients.recv(0);
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<int64_t> temp1(2);
//         temp1[0]=temp1[1]=v[i];
//         Value v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
//         Vroundconstants.push_back(v);
//     }
// }
// }
// void convkeyMatrices(Clients& clients){
//     size_t pid=clients.getPid();
// if(pid==0)
// {
//     std::vector<__int128> temp((rounds+1)*blocksize);
//     int num=0;

//     for(int i=0;i<=rounds;i++)
//     {
//         for(int j=0;j<blocksize;j++)
//         {
//             temp[num++]=to_int128(KeyMatrices[i][j]);
//         }
//     }

//     Value128 v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
//     clients.SendTo128(v,1);
//     clients.SendTo128(v,2);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VKeyMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// else{
//     auto v=clients.recv128(0);
//     std::vector<Value128> temp1;
//     for(int i=0;i<v.size();i++)
//     {
//         std::vector<__int128> temp2(2);
//         temp2[0]=temp2[1]=v[i];
//         Value128 v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
//         temp1.push_back(v);
//         if(temp1.size()==blocksize)
//         {
//             VKeyMatrices.push_back(temp1);
//             temp1.clear();
//         }
//     }
// }
// }
//     // Uses the Grain LSFR as self-shrinking generator to create pseudorandom bits
// // Is initialized with the all 1s state
// // The first 160 bits are thrown away
// bool getrandbit () {
//     static std::bitset<80> state; //Keeps the 80 bit LSFR state
//     bool tmp = 0;
//     //If state has not been initialized yet
//     if (state.none ()) {
//         state.set (); //Initialize with all bits set
//         //Throw the first 160 bits away
//         for (unsigned i = 0; i < 160; ++i) {
//             //Update the state
//             tmp =  state[0] ^ state[13] ^ state[23]
//                        ^ state[38] ^ state[51] ^ state[62];
//             state >>= 1;
//             state[79] = tmp;
//         }
//     }
//     //choice records whether the first bit is 1 or 0.
//     //The second bit is produced if the first bit is 1.
//     bool choice = false;
//     do {
//         //Update the state
//         tmp =  state[0] ^ state[13] ^ state[23]
//                    ^ state[38] ^ state[51] ^ state[62];
//         state >>= 1;
//         state[79] = tmp;
//         choice = tmp;
//         tmp =  state[0] ^ state[13] ^ state[23]
//                    ^ state[38] ^ state[51] ^ state[62];
//         state >>= 1;
//         state[79] = tmp;
//     } while (! choice);
//     return tmp;}


// };