#ifndef Ros_METHOD
#define Ros_METHOD
#include<array>

template<class LD>
struct ROS3w{
    static constexpr unsigned int s=3;
    static constexpr unsigned int p=2;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0,2/3.,4/3.};
    static constexpr arr b={0.25,0.25,0.5 };
    static constexpr arr bstar={ 0.746704703274 , 0.1144064078371 , 0.1388888888888 };
    static constexpr LD gamma=0.4358665215084;
    
    static constexpr arr2 a={
        arr{0,0,0},
        arr{2/3.,0,0},
        arr{2/3.,2/3.,0}

    };

    static constexpr arr2 g={
        arr{0,0,0},
        arr{0.3635068368900,0,0},
        arr{-0.8996866791992,-0.1537997822626,0}

    };
};
/*--------------------------------------------------------------------------------------------------------*/
template<class LD>
struct ROS34PW2{
    static constexpr unsigned int s=4;
    static constexpr unsigned int p=3;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0, 0.87173304301691800777, 0.73157995778885237526, 1};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={2.4212380706095346e-1 , -1.2232505839045147 , 1.5452602553351020 , 4.3586652150845900e-1};
    static constexpr arr bstar={ 3.7810903145819369e-1 , -9.6042292212423178e-2 , 0.5 , 2.1793326075422950e-1};
    static constexpr LD gamma=4.3586652150845900e-1; 
    
    static constexpr arr2 a={
        arr{0,0,0,0},
        arr{8.7173304301691801e-1,0,0,0},
        arr{8.4457060015369423e-1,-1.1299064236484185e-1,0,0},
        arr{0,0,1,0}
    };
        
    static constexpr arr2 g={
        arr{0,0,0,0},
        arr{-8.7173304301691801e-1,0,0,0},
        arr{-9.0338057013044082e-1,5.4180672388095326e-2,0,0},
        arr{2.4212380706095346e-1,-1.2232505839045147,5.4526025533510214e-1,0}
    };
};
/*--------------------------------------------------------------------------------------------------------*/
template<class LD>
struct RODASPR2{
    static constexpr unsigned int s=6;
    static constexpr unsigned int p=4;
    
    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0, 0.9375, 0.49816697385844987966, 0.68907842168064792343, 0.99999999999999997224, 0.99999999999999991673};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={5.1944159827788361e-1,3.9955867781540699e-2,-4.7356407303732290e-1,9.4907420451383284e-1,-3.4740759753593431e-1,3.125e-1  };
    static constexpr arr bstar={-1.7746585073632790e-1,-5.8241418952602364e-1,6.8180612588238165e-1,7.6557391437996980e-1,3.125e-1,0 };
    static constexpr LD gamma=3.125e-1; 
    
    static constexpr arr2 a={
        arr{0,0,0,0,0,0},
        arr{9.375e-1,0,0,0,0,0},
        arr{-4.7145892646261345e-2,5.4531286650471122e-1,0,0,0,0},
        arr{4.6915543899742240e-1,4.4490537602383673e-1,-2.2498239334061121e-1 ,0,0,0},
        arr{1.0950372887345903,6.3223023457294381e-1,-8.9232966090485821e-01,1.6506213759732410e-1,0,0},
        arr{-1.7746585073632790e-1,-5.8241418952602364e-1,6.8180612588238165e-1,7.6557391437996980e-1,3.125e-1,0}
    };

    static constexpr arr2 g={
        arr{0,0,0,0,0,0},
        arr{-9.3750000000000000e-01,0,0,0,0,0},
        arr{-9.7580572085994507e-02,-5.8666328499964138e-01,0,0,0,0},
        arr{-4.9407065013256957e-01,-5.6819726428975503e-01,5.0318949274167679e-01,0,0,0},
        arr{-1.2725031394709183,-1.2146444240989676,1.5741357867872399,6.0051177678264578e-01,0,0},
        arr{6.9690744901421153e-01,6.2237005730756434e-01,-1.1553701989197045,1.8350029013386296e-01,-6.5990759753593431e-01,0}
    };
};

#endif