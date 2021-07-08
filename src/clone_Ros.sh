path=`pwd`"/src"


if [ -d "$path/Rosenbrock" ]
then
    echo "Rosenbrock exists. Deleting."
    rm -rf "$path/Rosenbrock"
fi


cd /tmp


git clone --depth=1 --branch=NSC_stable https://github.com/dkaramit/NaBBODES.git

mv "NaBBODES/Rosenbrock" ./

rm -rf NaBBODES
cd Rosenbrock


rm -rf 0-test 2> /dev/null
rm *.cpp 2> /dev/null
rm Ros.hpp 2> /dev/null
rm makefile 2> /dev/null
rm -r .DS_Store 2> /dev/null
# rm Ros_steps.hpp 2> /dev/null
cp Jacobian/Jacobian.hpp Jacobian.hpp 2> /dev/null  
rm -r Jacobian 2> /dev/null
mv METHOD.hpp Ros_METHOD.hpp 

cd $path

mv /tmp/Rosenbrock ./

echo "Rosenbrock copied"