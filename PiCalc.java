/*
    Programa: Lab3-PiCalc.java
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 24/04/2023
    Descrição: Este programa utiliza threads para calcular o valor de pi.
*/

class Calc extends Thread {
    private int index;
    private int num_threads;
    private int n;
    private double pi_part;
    
    //--construtor
    public Calc(int index, int num_threads, int n) { 
        this.index = index;
        this.num_threads = num_threads;
        this.n = n; 
    }

    public double getPiPart(){
        return pi_part;
    }
 
    //--metodo executado pela thread
    public void run() {
        for(int i=index; i<n; i+=num_threads){
            double temp = Math.pow(-1,i) * 1/(2*i + 1);
            //System.out.println("Thread "+ index + ": " + temp);
            pi_part += temp;
        }
         
    }
}

public class PiCalc {
    static int n ;//= 0;
    static int num_threads ;//= 0;

    public static void main (String[] args) {
        if(args.length!=2){
            System.out.println("Command Error: java PiCalc <number of pi terms> <number of threads>");
            System.exit(1);
        }
        
        n = Integer.parseInt(args[0]);
        num_threads = Integer.parseInt(args[1]);
        
        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[num_threads];
        double pi = 0;

        //--PASSO 2: cria a classe Calc que estende Thread
        for (int i=0; i<threads.length; i++) {
            threads[i] = new Calc(i,num_threads,n);
        }

        //--PASSO 3: iniciar a thread
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //--PASSO 4: esperar pelo termino das threads
        for (int i=0; i<threads.length; i++) {
                try { threads[i].join(); } 
                catch (InterruptedException e) { return; }

                //Realiza o calculo dos valores
                double pi_part = 4 * ((Calc) threads[i]).getPiPart();
                pi += pi_part;
        } 

        System.out.println("End value: " + pi); 
    }
}
