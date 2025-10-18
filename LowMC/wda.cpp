#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include "Lowmc.h"
#include "../network/clients.hpp"
#include "../Operator/operator.h"
#include "../Value/value.h"

/////////////////////////////
//     LowMC functions     //
/////////////////////////////

Value LowMC::encrypt (Clients& clients,const Value& message) {
    Value c = Xor(clients,message,Vroundkeys[0]);
    for (unsigned r = 1; r <= rounds; ++r) {
        c =  Substitution(clients,c);
        c =  MultiplyWithGF2Matrix(clients,VLinMatrices[r-1], c);
        c = Xor(clients,c,Vroundconstants[r-1]);
        c = Xor(clients,c,Vroundkeys[r]);
    }
    return c;
}


Value LowMC::decrypt (Clients& clients,const Value& message) {
    Value c = message;
    for (unsigned r = rounds; r > 0; --r) {
        c = Xor(clients,c,Vroundkeys[r]);
        c = Xor(clients,c,Vroundconstants[r-1]);
        c =  MultiplyWithGF2Matrix(clients,VinvLinMatrices[r-1], c);
        c =  invSubstitution(clients,c);
    }
    c = Xor(clients,c,Vroundkeys[0]);
    return c;
}


void LowMC::set_key (Clients& clients,Value k) {
    key = k;
    keyschedule(clients);
}

void LowMC::print_matrices() {
    std::cout << "LowMC matrices and constants" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "Block size: " << blocksize << std::endl;
    std::cout << "Key size: " << keysize << std::endl;
    std::cout << "Rounds: " << rounds << std::endl;
    std::cout << std::endl;

    std::cout << "Linear layer matrices" << std::endl;
    std::cout << "---------------------" << std::endl;
    for (unsigned r = 1; r <= rounds; ++r) {
        std::cout << "Linear layer " << r << ":" << std::endl;
        for (auto row: LinMatrices[r-1]) {
            std::cout << "[";
            for (unsigned i = 0; i < blocksize; ++i) {
                std::cout << row[i];
                if (i != blocksize - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Round constants" << std::endl;
    std::cout << "---------------------" << std::endl;
    for (unsigned r = 1; r <= rounds; ++r) {
        std::cout << "Round constant " << r << ":" << std::endl;
        std::cout << "[";
        for (unsigned i = 0; i < blocksize; ++i) {
            std::cout << roundconstants[r-1][i];
            if (i != blocksize - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "Round key matrices" << std::endl;
    std::cout << "---------------------" << std::endl;
    for (unsigned r = 0; r <= rounds; ++r) {
        std::cout << "Round key matrix " << r << ":" << std::endl;
        for (auto row: KeyMatrices[r]) {
            std::cout << "[";
            for (unsigned i = 0; i < keysize; ++i) {
                std::cout << row[i];
                if (i != keysize - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }
        if (r != rounds) {
            std::cout << std::endl;
        }
    }
}


/////////////////////////////
// LowMC private functions //
/////////////////////////////


// block LowMC::blockXor (Clinets& clients,const block& a, const block& b) {
//     block temp;
//     for(int i=0;i<a.size();i++)
//     {
//         temp[i]=Xor(clients,a[i],b[i]);
//     }
//     return temp;
// }

// block LowMC::keyXor (Clinets& clients,const block& a, const block& b) {
//     keyblock temp;
//     for(int i=0;i<a.size();i++)
//     {
//         temp[i]=Xor(clients,a[i],b[i]);
//     }
//     return temp;
// }

// block LowMC::AndBlocks(Clinets& clients,const block& a, const block& b) {
//     block temp;
//     for(int i=0;i<a.size();i++)
//     {
//         temp[i]=And(clients,a[i],b[i]);
//     }
//     return temp;
// }

// block LowMC::keyAnd(Clinets& clients,const keyblock& a, const keyblock& b) {
//     keyblock temp;
//     for(int i=0;i<a.size();i++)
//     {
//         temp[i]=And(clients,a[i],b[i]);
//     }
//     return temp;
// }
// block LowMC::blockLshift(const block& a, size_t offset) {
//     block temp;
//     int64_t carry = 0; // 用于存储从前一个int64_t溢出的位
//     for (int i = 0; i < a.size(); i++) {
//         temp[i] = (a[i] << offset) | carry;
//         carry = a[i] >> (64 - offset); // 计算溢出的位
//     }
//     return temp;
// }

// keyblock LowMC::keyLshift(const keyblock& a, size_t offset) {
//     keyblock temp;
//     int64_t carry = 0; // 用于存储从前一个int64_t溢出的位
//     for (int i = 0; i < a.size(); i++) {
//         temp[i] = (a[i] << offset) | carry;
//         carry = a[i] >> (64 - offset); // 计算溢出的位
//     }
//     return temp;
// }

// block LowMC::blockRshift(const block& a, size_t offset) {
//     block temp;
//     int64_t carry = 0; // 用于存储从前一个int64_t溢出的位
//     for (int i = a.size() - 1; i >= 0; i--) {
//         temp[i] = (a[i] >> offset) | carry;
//         carry = a[i] << (64 - offset); // 计算溢出的位
//     }
//     return temp;
// }

// keyblock LowMC::keyRshift(const keyblock& a, size_t offset) {
//     keyblock temp;
//     int64_t carry = 0; // 用于存储从前一个int64_t溢出的位
//     for (int i = a.size() - 1; i >= 0; i--) {
//         temp[i] = (a[i] >> offset) | carry;
//         carry = a[i] << (64 - offset); // 计算溢出的位
//     }
//     return temp;
// }


Value LowMC::Count(Clients& clients,const Value& a) {

    int64_t count=0;
    std::vector<int64_t> temp(a.size());
    for(int i=0;i<a.size();i++)
    {
        auto cur=a[i];
        int64_t xor_result = 0;

        while (cur) {
            xor_result ^= (cur & 1);
            cur >>= 1;
        }
        temp[i]=xor_result;
    }
    Value res(std::move(temp));res.set_fraction_bits(0);res.set_share_type(1);
    return res;
}



// Value LowMC::Andx7(Clients& clients,const block& a) {

//     auto last4=a[3][0];
//     std::vector<int64_t> res;
    
// }

Value LowMC::Substitution (Clients& clients,const Value& message) {
    Value temp;
    //Get the identity part of the message
    temp = std::move(Rshift(clients,message,3*numofboxes));
    //Get the rest through the Sboxes
    for (unsigned i = 1; i <= numofboxes; ++i) {
        temp =std::move(Lshift(clients,temp,3));
        auto idx=reconstruct(clients,And(clients,(Rshift(clients,message,3*(numofboxes-i))),make_constB(7,message.size()/2)));
        std::vector<int64_t> sb(message.size());
        for(int i=0;i<message.size()/2;i++)
        {
            sb[2*i]=Sbox[idx[i]],sb[2*i+1]=Sbox[idx[i]];
        }
        Value Sb(std::move(sb));Sb.set_fraction_bits(0);Sb.set_share_type(1);
        temp =std::move(Xor(clients, temp,Sb));
    }
    return temp;
}


Value LowMC::invSubstitution (Clients& clients,const Value& message) {
    Value temp;
    //Get the identity part of the message
    temp=std::move(Rshift(clients,message,3*numofboxes));
    //Get the rest through the invSboxes
    for (unsigned i = 1; i <= numofboxes; ++i) {
        temp =std::move(Lshift(clients,temp,3));
        auto idx=reconstruct(clients,And(clients,(Rshift(clients,message,3*(numofboxes-i))),make_constB(7,message.size()/2)));
        std::vector<int64_t> isb(message.size());
        for(int i=0;i<message.size()/2;i++)
        {
            isb[2*i]=invSbox[idx[i]],isb[2*i+1]=invSbox[idx[i]];
        }
        Value ISb(std::move(isb));ISb.set_fraction_bits(0);ISb.set_share_type(1);
        temp =std::move(Xor(clients,temp,ISb));
    }
    return temp;
}


Value LowMC::MultiplyWithGF2Matrix
        (Clients& clients,const std::vector<Value> matrix, const Value message) {
    std::vector<int64_t> temp1(message.size(),0);
    for (unsigned i = 0; i < blocksize; ++i) {
        auto temp2 = Lshift(clients,Count(clients,And(clients,message,expan(clients,matrix[i],message.size()))),i);
        for(int j=0;j<temp2.size();j++) temp1[j]+=temp2[j];
    }
    Value temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
    return temp;
}


Value LowMC::MultiplyWithGF2Matrix_Key
        (Clients& clients,const std::vector<Value> matrix, const Value k) {
    std::vector<int64_t> temp1(k.size(),0);
    for (unsigned i = 0; i < blocksize; ++i) {
        auto temp2 = Lshift(clients,Count(clients,And(clients,k,expan(clients,matrix[i],k.size()))),i);
        for(int j=0;j<temp2.size();j++) temp1[j]+=temp2[j];
    }
    Value temp(std::move(temp1));temp.set_fraction_bits(0);temp.set_share_type(1);
    return temp;
}

void LowMC::keyschedule (Clients& clients) {
    Vroundkeys.clear();
    for (unsigned r = 0; r <= rounds; ++r) {
        Vroundkeys.push_back( MultiplyWithGF2Matrix_Key (clients,VKeyMatrices[r], key) );
    }
    return;
}

void LowMC::instantiate_LowMC () {
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


void LowMC::convLineLinMatrices(Clients& clients)
{
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<int64_t> temp((rounds+1)*blocksize);
        int num=0;

        for(int i=0;i<=rounds;i++)
        {
            for(int j=0;j<blocksize;j++)
            {
                temp[num++]=static_cast<int64_t>(LinMatrices[i][j].to_ulong());
            }
        }

        Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
        clients.SendTo(v,1);
        clients.SendTo(v,2);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VLinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
    else{
        auto v=clients.recv(0);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VLinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
}

void LowMC::convLineInvLinMatrices(Clients& clients)
{
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<int64_t> temp((rounds+1)*blocksize);
        int num=0;

        for(int i=0;i<=rounds;i++)
        {
            for(int j=0;j<blocksize;j++)
            {
                temp[num++]=static_cast<int64_t>(invLinMatrices[i][j].to_ulong());
            }
        }

        Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
        clients.SendTo(v,1);
        clients.SendTo(v,2);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VinvLinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
    else{
        auto v=clients.recv(0);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VinvLinMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
}

void LowMC::convkeyMatrices(Clients& clients)
{
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<int64_t> temp((rounds+1)*blocksize);
        int num=0;

        for(int i=0;i<=rounds;i++)
        {
            for(int j=0;j<blocksize;j++)
            {
                temp[num++]=static_cast<int64_t>(KeyMatrices[i][j].to_ulong());
            }
        }

        Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
        clients.SendTo(v,1);
        clients.SendTo(v,2);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VKeyMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
    else{
        auto v=clients.recv(0);
        for(int i=0;i<v.size();i++)
        {
            std::vector<Value> temp1;
            std::vector<int64_t> temp2(2);
            temp2[0]=temp2[1]=v[i];
            Value v(std::move(temp2));v.set_fraction_bits(0);v.set_share_type(1);
            temp1.push_back(v);
            if(temp1.size()==blocksize)
            {
                VKeyMatrices.push_back(temp1);
                temp1.clear();
            }
        }
    }
}


void LowMC::convRoundconstants(Clients& clients)
{
    size_t pid=clients.getPid();
    if(pid==0)
    {
        std::vector<int64_t> temp((rounds));
        int num=0;

        for(int i=0;i<rounds;i++)
        {
            temp[num++]=static_cast<int64_t>(roundconstants[i].to_ulong());
        }

        Value v(std::move(temp));v.set_fraction_bits(0);v.set_share_type(1);
        clients.SendTo(v,1);
        clients.SendTo(v,2);
        for(int i=0;i<v.size();i++)
        {
            std::vector<int64_t> temp1(2);
            temp1[0]=temp1[1]=v[i];
            Value v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
            Vroundconstants.push_back(v);
        }
    }
    else{
        auto v=clients.recv(0);
        for(int i=0;i<v.size();i++)
        {
            std::vector<int64_t> temp1(2);
            temp1[0]=temp1[1]=v[i];
            Value v(std::move(temp1));v.set_fraction_bits(0);v.set_share_type(1);
            Vroundconstants.push_back(v);
        }
    }
}
/////////////////////////////
// Binary matrix functions //
/////////////////////////////


unsigned LowMC::rank_of_Matrix (const std::vector<block> matrix) {
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


unsigned LowMC::rank_of_Matrix_Key (const std::vector<keyblock> matrix) {
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


std::vector<block> LowMC::invert_Matrix (const std::vector<block> matrix) {
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

///////////////////////
// Pseudorandom bits //
///////////////////////


block LowMC::getrandblock () {
    block tmp = 0;
    for (unsigned i = 0; i < blocksize; ++i) tmp[i] = getrandbit ();
    return tmp;
}

keyblock LowMC::getrandkeyblock () {
    keyblock tmp = 0;
    for (unsigned i = 0; i < blocksize; ++i) tmp[i] = getrandbit ();
    return tmp;
}


// Uses the Grain LSFR as self-shrinking generator to create pseudorandom bits
// Is initialized with the all 1s state
// The first 160 bits are thrown away
bool LowMC::getrandbit () {
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
    return tmp;
}



