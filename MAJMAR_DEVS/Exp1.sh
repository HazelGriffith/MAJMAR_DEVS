cd bin
for i in 1 3 6
do
	for j in 0 12 24 36 48 60 72 84 96 108 120 132 144 156 168 200000
	do
		for k in {1..100}
		do
			./MAIN $k $j $i
		done
	done
done
