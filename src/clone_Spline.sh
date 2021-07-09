path=`pwd`"/src"


if [ -d "$path/Interpolation" ]
then
    echo "Interpolation exists. Deleting."
    rm -rf "$path/Interpolation"
fi


cd /tmp


# git clone --depth=1 --branch=stable https://github.com/dkaramit/SimpleSplines.git
git clone --depth=1 --branch=stable git@github.com:dkaramit/SimpleSplines.git

cd SimpleSplines



rm -rf .git* 
rm -rf .vscode 2> /dev/null


rm *.cpp 2> /dev/null
rm *.run 2> /dev/null
rm *.ipynb 2> /dev/null
rm -r example 2> /dev/null

rm makefile 2> /dev/null

cd $path

mv /tmp/SimpleSplines ./Interpolation

rm -rf  /tmp/SimpleSplines/


echo "Interpolation copied"