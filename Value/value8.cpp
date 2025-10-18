#pragma once

#include "value8.h"

// 模板构造函数实现
Value8::Value8(std::vector<double>& input_vector, size_t fraction_bits) {

    if (fraction_bits < 0) {
        throw std::invalid_argument("Fraction bits must be non-negative for non-integer inputs.");
    }

    this->fraction_bits = fraction_bits;

    for (double& Value8 : input_vector) {
        int8_t fixed_point_Value8 = static_cast<int8_t>(std::round(static_cast<double>(Value8) * std::pow(2, fraction_bits)));
        fixed_point_vector.push_back(fixed_point_Value8);
    }
};

Value8::Value8(std::vector<int8_t>&& input_vector) {
    fixed_point_vector=std::move(input_vector);
    fraction_bits=0;
    shareType=0;
};

Value8::Value8() {
    fraction_bits = 0; 
    fixed_point_vector.clear();
};

const std::vector<int8_t>& Value8::get_fixed_point_vector() const {
    return fixed_point_vector;
}

uint32_t Value8::size() const {
    return fixed_point_vector.size();
}

void Value8::set_share_type(bool sp){
    shareType=sp;
}

bool Value8::get_share_type() const {
    return shareType;
}

int8_t& Value8::operator[](size_t index){
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

int8_t Value8::operator[](size_t index) const{
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

double Value8::get_Value(size_t index) const {
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }

    if (fraction_bits == 0) {
        return static_cast<int8_t>(fixed_point_vector[index]);
    }

    return static_cast<double>(fixed_point_vector[index]) / std::pow(2, fraction_bits);
}


size_t Value8::get_fraction_bits() const {
    return fraction_bits;
}

void Value8::set_fraction_bits(size_t fp) {
    fraction_bits=fp;
}