mkdir -p src/res
mkdir -p src/res/gfx
mkdir -p src/res/gfx/peons

# change -w and -h values according to the size of the textures in the folder
for n in $(find resources/gfx/peons/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 8 -h 8
done