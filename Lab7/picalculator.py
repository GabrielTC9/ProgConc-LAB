#   Programa: picalculator.py
#   Autor: Gabriel Trindade Corrêa
#   DRE: 120019606
#   Data: 20/06/2023
#   Descrição: Este programa implementa o cálculo de pi com threads, de acordo com a fórmula da atividade 5 do Lab7.

from threading import Thread
import math
import time

#   Classe das threads
class PiTermCalculatorThread (Thread):

    def __init__(self, id, numTerms, numThreads):
        super().__init__()
        self.threadID = id
        self.numTerms = numTerms
        self.numThreads = numThreads
        self.sumTerms = 0

    def run(self):
        #print("Thread: ", self.threadID)
        for i in range(self.threadID, self.numTerms, self.numThreads):
            self.sumTerms += pow(-1, i)*(1 / (2*i + 1))
    
    def getSumTerms(self):
        return self.sumTerms*4

#   Fluxo principal   
if __name__ == '__main__':
    NUMTERMS = 1000
    NUMTHREADS = 4

    threads = [PiTermCalculatorThread(i, NUMTERMS, NUMTHREADS) for i in range(NUMTHREADS)]
    calcPi = 0.0
    start = time.time()

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()
        calcPi += thread.getSumTerms()

    print("Pi value from math.pi: ", math.pi)
    print("Pi value found with {} terms and {} threads: {}".format(NUMTERMS, NUMTHREADS, calcPi))
    print("Relative error: {:.6f}".format((math.pi - calcPi)/math.pi))
    print("Value found in {:.6f} seconds.".format( time.time()-start))
