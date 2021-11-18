BEGIN="
<html>
    <head>
        <title>MiMeS</title>
        <script src=\"https://cdn.jsdelivr.net/gh/google/code-prettify@master/loader/run_prettify.js\"></script>
        <script type=\"module\" src=\"https://cdn.jsdelivr.net/gh/zerodevx/zero-md@2/dist/zero-md.min.js\"></script>
    </head>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Source+Code+Pro:wght@500&display=swap');
        code {font-family: 'Source Code Pro', monospace; color: rgb(0,0,0);}
    </style>
    
    <body style=\"background-color:rgb\(255, 255, 255\)\;\">
    "

END="
</body>
</html>
"

files=$(find ./ -regextype posix-extended -regex ".*.md")

for i in ${files[@]}; do 
    file=${i/.md/}
    file=${file/.\//}
    echo "writing "$file

    echo $BEGIN>$file.html
    pandoc --mathml $file.md >> $file.html
    echo  $END>> $file.html

done






