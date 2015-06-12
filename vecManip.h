#ifndef VECMANIP
#define VECMANIP

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>


template<typename vec> vec mag(std::vector<vec> dVec){
  return (sqrt(dVec[0]*dVec[0] + dVec[1]*dVec[1] + dVec[2]*dVec[2]));
}

template<typename vec> vec *toUnitVec(vec *dVec){
  vec *uVec = (vec*)malloc(3*sizeof(vec));
  
  uVec[0] = dVec[0] / mag(dVec);
  uVec[1] = dVec[1] / mag(dVec);
  uVec[2] = dVec[2] / mag(dVec);
  
  return uVec;
}

template<typename T> std::vector<T> VecScaleMulti(std::vector<T> vec, T sca){
	std::vector<T> ret;
	for(int i = 0; i < vec.size(); ++i){
		ret.push_back(vec[i]*sca);
	}
	return ret;
}

template<typename T> std::vector<T> VecAdd(std::vector<T> vec1, std::vector<T> vec2){
	std::vector<T> ret;
	if(vec1.size() != vec2.size()){
		std::cerr << "/n/t In std::vector<double> VecAdd: vec1 and vec2 are of different size. Cannot add...\n";
		ret = {0,0,0};
		return ret;
	} else {
		for(int i = 0; i < vec1.size(); ++i){
			ret.push_back(vec1[i]+vec2[i]);
		}
	}
	return ret;
}

template<typename T> std::vector<T> VecSub(std::vector<T> vec1, std::vector<T> vec2){
	std::vector<T> ret;
	if(vec1.size() != vec2.size()){
		std::cerr << "/n/t In std::vector<double> VecAdd: vec1 and vec2 are of different size. Cannot add...\n";
		ret = {0,0,0};
		return ret;
	} else {
		for(int i = 0; i < vec1.size(); ++i){
			ret.push_back(vec1[i]-vec2[i]);
		}
	}
	return ret;
}


#endif