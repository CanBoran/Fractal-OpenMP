all: compact.cpp
	g++ -fopenmp -o compact compact.cpp
	g++ -fopenmp -o parallel_compact parallel_compact.cpp
	g++ -fopenmp -o sparse sparse.cpp
	g++ -fopenmp -o parallel_sparse parallel_sparse.cpp
