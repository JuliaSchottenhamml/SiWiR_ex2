set title 'Solution'
set xlabel 'x'
set ylabel 'y'
splot "data/solution.txt" with pm3d
set output 'img/solution.jpg'
pause -1
