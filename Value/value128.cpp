#pragma once

#include "value128.h"

// 模板构造函数实现
Value128::Value128(std::vector<double>& input_vector, size_t fraction_bits) {

    if (fraction_bits < 0) {
        throw std::invalid_argument("Fraction bits must be non-negative for non-integer inputs.");
    }

    this->fraction_bits = fraction_bits;

    for (double& Value128 : input_vector) {
        __int128 fixed_point_Value128 = static_cast<__int128>(std::round(static_cast<double>(Value128) * std::pow(2, fraction_bits)));
        fixed_point_vector.push_back(fixed_point_Value128);
    }
};

Value128::Value128(std::vector<__int128>&& input_vector) {
    fixed_point_vector=std::move(input_vector);
    fraction_bits=0;
    shareType=0;
};

Value128::Value128() {
    fraction_bits = 0; 
    fixed_point_vector.clear();
};

const std::vector<__int128>& Value128::get_fixed_point_vector() const {
    return fixed_point_vector;
}

uint32_t Value128::size() const {
    return fixed_point_vector.size();
}

void Value128::set_share_type(bool sp){
    shareType=sp;
}

bool Value128::get_share_type() const {
    return shareType;
}

__int128& Value128::operator[](size_t index){
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

__int128 Value128::operator[](size_t index) const{
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

double Value128::get_Value(size_t index) const {
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }

    if (fraction_bits == 0) {
        return static_cast<__int128>(fixed_point_vector[index]);
    }

    return static_cast<double>(fixed_point_vector[index]) / std::pow(2, fraction_bits);
}


size_t Value128::get_fraction_bits() const {
    return fraction_bits;
}

void Value128::set_fraction_bits(size_t fp) {
    fraction_bits=fp;
}