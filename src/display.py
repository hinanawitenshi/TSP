import matplotlib.pyplot as plt
import numpy
import sys
import os
import math

class Result:
    def __init__(self, seed, algorithm, parms, schema, arr, answer, best, path,
        period):
        self.seed = seed
        self.algorithm = algorithm
        self.parms = parms
        self.schema = schema
        self.arr = arr
        self.answer = answer
        self.best = best
        self.path = path
        self.period = period

def loadcsv(fpath):

    with open(fpath) as f:
        seed = int(next(f)[2:-1])
        algorithm = next(f)[2:-1]
        parms = []
        if algorithm == 'GREEDY' or algorithm == 'SA':
            parms = [ float(x) for x in (next(f)[2:-1].split(',')) ]
        schema = next(f)[2:-1].split(',')
        line = ''
        while (line != '\n'):
            line = next(f)
        with open(fpath + '.tmp', 'w') as tmp:
            line = ''
            while (line != '\n'):
                line = next(f)
                tmp.write(line[2:])
        line = next(f)[2:-1]
        answer = float(line.split('/')[0])
        best = float(line.split('/')[1])
        period = float(next(f)[2:-1])
    
    arr = numpy.loadtxt(fpath, delimiter=',')
    path = numpy.loadtxt(fpath + '.tmp', delimiter=',')

    os.remove(fpath + '.tmp')

    return Result(seed, algorithm, parms, schema, arr, answer, best, path,
        period)

def plot(result):
    iters = result.arr[:,0]
    nrows = int(math.ceil((len(result.schema)) / 2))
    ncols = 2
    fig, axs = plt.subplots(nrows, ncols)
    fig.suptitle(result.algorithm + '(Result=%.3f/%.3f,TotalTime=%.3f)' % \
        (result.answer, result.best, result.period))
    axs = axs.reshape(nrows, ncols)
    axs[0, 0].plot(result.path[:,0], result.path[:,1])
    for i in range(1, len(result.schema)):
        ax = axs[int(i / 2), i % 2]
        ax.plot(iters, result.arr[:,i])
        ax.set(xlabel=result.schema[0], ylabel=result.schema[i])
    plt.show()

if __name__ == '__main__':

    fpath = sys.argv[1]

    plot(loadcsv(fpath))
