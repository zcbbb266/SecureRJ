#pragma once

#include "value32.h"

// 模板构造函数实现
Value32::Value32(std::vector<double>& input_vector, size_t fraction_bits) {

    if (fraction_bits < 0) {
        throw std::invalid_argument("Fraction bits must be non-negative for non-integer inputs.");
    }

    this->fraction_bits = fraction_bits;

    for (double& Value32 : input_vector) {
        int32_t fixed_point_Value32 = static_cast<int32_t>(std::round(static_cast<double>(Value32) * std::pow(2, fraction_bits)));
        fixed_point_vector.push_back(fixed_point_Value32);
    }
};

Value32::Value32(std::vector<int32_t>&& input_vector) {
    fixed_point_vector=std::move(input_vector);
    fraction_bits=0;
    shareType=0;
};

Value32::Value32() {
    fraction_bits = 0; 
    fixed_point_vector.clear();
};

const std::vector<int32_t>& Value32::get_fixed_point_vector() const {
    return fixed_point_vector;
}

uint32_t Value32::size() const {
    return fixed_point_vector.size();
}

void Value32::set_share_type(bool sp){
    shareType=sp;
}

bool Value32::get_share_type() const {
    return shareType;
}

int32_t& Value32::operator[](size_t index){
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

int32_t Value32::operator[](size_t index) const{
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

double Value32::get_Value(size_t index) const {
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }

    if (fraction_bits == 0) {
        return static_cast<int32_t>(fixed_point_vector[index]);
    }

    return static_cast<double>(fixed_point_vector[index]) / std::pow(2, fraction_bits);
}


size_t Value32::get_fraction_bits() const {
    return fraction_bits;
}

void Value32::set_fraction_bits(size_t fp) {
    fraction_bits=fp;
}