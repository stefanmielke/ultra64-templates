mkdir -p src/res
mkdir -p src/res/gfx

for n in $(find resources/gfx/ -name '*.png'); do
    from="$n"
    to=$(echo "$n" | sed "s/resources/src\/res/g; s/png/gen.h/g")
    n64graphics -i $to -g $from -s u16 -f rgba16 -w 32 -h 32
done