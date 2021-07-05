path=`pwd`


if [ -d "$path/Interpolation" ]
then
    echo "Interpolation exists. Deleting."
    rm -rf "$path/Interpolation"
fi


cd /tmp


git clone --depth=1 --branch=master https://github.com/dkaramit/ASAP.git

cd ASAP/Interpolation/C++



rm -rf .git* 
rm -rf .vscode 2> /dev/null


rm *.cpp 2> /dev/null
rm *.run 2> /dev/null
rm *.ipynb 2> /dev/null
rm -r example 2> /dev/null

rm makefile 2> /dev/null

cd $path

mv /tmp/ASAP/Interpolation/C++ ./Interpolation

rm -rf  /tmp/ASAP/


echo "Interpolation copied"