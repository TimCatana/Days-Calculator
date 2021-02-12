all: daysCalculatorA daysCalculatorB daysCalculatorC daysCalculatorD daysCalculatorE

daysCalculatorA: daysCalculatorA.c
	gcc -ansi -o daysCalculatorA daysCalculatorA.c

daysCalculatorB: daysCalculatorB.c
	gcc -ansi -o daysCalculatorB daysCalculatorB.c

daysCalculatorC: daysCalculatorC.c
	gcc -ansi -o daysCalculatorC daysCalculatorC.c

daysCalculatorD: daysCalculatorD.c
	gcc -ansi -o daysCalculatorD daysCalculatorD.c

daysCalculatorE: daysCalculatorE.c
	gcc -ansi -o daysCalculatorE daysCalculatorE.c

clean:
	rm daysCalculatorA daysCalculatorB daysCalculatorC daysCalculatorD daysCalculatorE
