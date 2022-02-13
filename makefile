

all: comp run

comp:
	@clear
	@gcc BankersAlgo.c -o smd

run:
	./smd

clean:
	rm smd


