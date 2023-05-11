/*
    Programa: PiCalc.java
    Autor: Gabriel Trindade Corrêa
    DRE: 120019606
    Data: 24/04/2023
    Descrição: Este programa utiliza threads para calcular o valor de pi.
*/

class Calc extends Thread {
    private int id_thread;
    private int num_threads;
    private int num_terms;
    private double sum_terms;
    
    //--Construtor  
    public Calc(int id_thread, int num_threads, int num_terms) { 
        this.id_thread = id_thread;
        this.num_threads = num_threads;
        this.num_terms = num_terms; 
    }

    //--Retorna soma dos termos calculados pela thread
    public double getSumTerms(){
        return sum_terms;
    }
 
    //--Método executado pela thread
    public void run() {
        for(int i=id_thread; i<num_terms; i+=num_threads){
            double temp = Math.pow(-1,i) * 1/(2*i + 1);
            //System.out.println("Thread "+ id_thread + ": " + temp);
            sum_terms += temp;
        }
         
    }
}

public class PiCalc {
    static int num_terms ;
    static int num_threads ;

    public static void main (String[] args) {
        
        //--Verifica se usuário inseriu número de termos e de threads
        if(args.length!=2){
            System.out.println("Command Error: java PiCalc <number of pi terms> <number of threads>");
            System.exit(1);
        }
        num_terms = Integer.parseInt(args[0]);
        num_threads = Integer.parseInt(args[1]);
        
        //--Reserva espaço para vetor de threads
        Thread[] threads = new Thread[num_threads];
        double pi = 0;

        //--Cria a classe Calc que estende Thread
        for (int i=0; i<threads.length; i++) {
            threads[i] = new Calc(i,num_threads,num_terms);
        }

        //--Inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //--Espera pelo término das threads
        for (int i=0; i<threads.length; i++) {
                try { threads[i].join(); } 
                catch (InterruptedException e) { return; }

                //--Realiza o cálculo dos valores
                double sum_terms = 4 * ((Calc) threads[i]).getSumTerms();
                pi += sum_terms;
        } 

        //--Imprime resultado encontrado de pi
        System.out.println("Valor encontrado com "+num_terms+" elementos e "+num_threads+" threads: " + pi);
        System.out.println("Valor da constante Math.PI: " + Math.PI);
        System.out.println("Erro relativo: " + ((Math.PI - pi)/Math.PI)); 
    }
}
