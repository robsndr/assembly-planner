#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS

#include <vector>
#include <math.h>
#include <map>

namespace math_functions{

template <typename T>
T max(const std::vector<T>& a,bool absoluteValue=false){
    T maximum=0;
    for(unsigned i=0;i<a.size();i++){
        if(absoluteValue){
            if(fabs(a[i])>maximum){
                maximum=fabs(a[i]);
            }
        }else{
            if(a[i]>maximum){
                maximum=a[i];
            }
        }
    }
    return maximum;
}

template <typename T1, typename T2>
typename std::pair<T1,T2> max(const std::map<T1,T2>& a,bool absoluteValue=false){
    std::pair<T1,T2> maximum=*a.begin();
    for(std::pair<T1,T2> key : a){
        if(absoluteValue){
            if(fabs(key.second)>maximum.second){
                maximum=key;
            }
        }else{
            if(key.second>maximum.second){
                maximum=key;
            }
        }
    }
    return maximum;
}

template <typename T>
void nchoosek(int offset,int k,std::vector<std::vector<T> >& combinations,std::vector<T>& current,std::vector<T>& input){
    if (k == 0) {
        combinations.push_back(current);
        return;
    }
    for (int i = offset; i <= input.size() - k; ++i) {
        current.push_back(input[i]);
        nchoosek(i+1, k-1,combinations,current,input);
        current.pop_back();
    }
}

template <typename T>
T sum(const std::vector<T>& a){
    T s=0;
    for(unsigned i=0;i<a.size();i++){
        s+=a[i];
    }
    return s;
}

template <typename T>
T sum(const std::map<T,T>& a){
    T s=0;
    for(std::pair<T,T> t : a){
        s+=t.second;
    }
    return s;
}

long long factorial(long long n);



}

#endif // MATH_FUNCTIONS

