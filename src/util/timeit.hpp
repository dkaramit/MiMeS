#ifndef Timer_included
#define Timer_included

#include<iostream>
#include<chrono>

namespace mimes{ namespace util{
    // this uses the life time of a variable to basically measure time inside a scope! 
    class Timer{
        public:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> finish;
        std::chrono::duration<double> dt;
        Timer(){
            start = std::chrono::high_resolution_clock::now();
        };
        ~Timer(){
            finish = std::chrono::high_resolution_clock::now();
            dt = finish - start;
            std::cerr<<dt.count()<<"\n";
        };
    };
}}


#endif