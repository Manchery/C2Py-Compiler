all: calc.c expGen.py groundTruth.py autoValidation.sh
	gcc calc.c -g -o calc
	./autoValidation.sh

clean:
	rm calc calc.out exp.in truth.out