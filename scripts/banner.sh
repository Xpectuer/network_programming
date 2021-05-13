

input='./1.txt'
while IFS= read -r line
do
	echo $line
done < "$input"


