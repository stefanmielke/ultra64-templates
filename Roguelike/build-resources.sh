mkdir -p src/res
mkdir -p src/res/gfx
mkdir -p src/res/gfx/fonts

# change -w and -h values according to the size of the textures in the folder
for n in $(find resources/gfx/fonts/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u8 -f i4 -w 96 -h 96
done