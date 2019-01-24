import random
import subprocess
import matplotlib.pyplot as plt


def execute_test(N, algorithm, generator):
    f = open("test", "w")
    generator["function"](f, N)
    f.close()
    command = "./cmake-build-debug/Codigo --file test --only %s" % algorithm
    p = subprocess.Popen(command.split())
    p.wait()


def test_algorithm(Nrange, algorithms, generator, subplot_args):
    for algorithm in algorithms:
        plt.subplot(*subplot_args)
        subplot_args[2] += 1
        plt.title(generator["name"])
        plt.ylabel("Time in miliseconds")
        plt.xlabel("Ammount of nodes")
        open("Tiempos%s" % algorithm, "w").close()
        for n in Nrange:
            execute_test(n, algorithm, generator)
        f = open("Tiempos%s" % algorithm, "r")
        times = f.read().split('\n')
        f.close()
        times.pop()
        times = map(float, times)
        #for x in range(len(Nrange)): times[x] /= Nrange[x] * Nrange[x]
        plt.plot(Nrange, times)

def generator1(file, N):
    file.write("%i\n" % N)
    for x in range(N):
        for y in range(N):
            file.write(str(1.0))
            if (y != N-1): file.write(" ")
        file.write("\n")
def generator2(file, N):
    file.write("%i\n" % N)
    for x in range(N):
        for y in range(N):
            if (x == y-1):
                file.write(str(.5))
            else:
                file.write(str(0.0))
            if (y != N-1): file.write(" ")
        file.write("\n")

random.seed(1234)
algorithms = ["BellmanFord", "FloydWarshall"]
subplot_args = [2, 2, 1]
test_algorithm(range(3, 150, 4), algorithms, {"name": "Todos unos", "function": generator1}, subplot_args)
test_algorithm(range(3, 150, 4), algorithms, {"name": "Ciclo Cn", "function": generator2}, subplot_args)

plt.show()