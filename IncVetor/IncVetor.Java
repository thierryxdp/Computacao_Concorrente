/* Disciplina: Computação Concorrente */
/* Prof: Silvana Rossetto */
/* Laboratório: 6 */
/* Codigo: Incrementando um vetor compartilhado em um programa multithreading Java */

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Resource {
    //recurso compartilhado
    private char[] vetor;   // vetor que será incrementado de 1. Usei char por operar com valores pequenos.
    private int tam_vetor;  // armazena o tamanho do vetor
    private int number_threads; // armazena o número de threads

    //construtor
    public Resource(int elements, int nthreads) {
        //inicializa valores que serão compartilhados entre as threads
        this.vetor = new char[elements];
        this.tam_vetor = elements;
        this.number_threads = nthreads;

        //inicializa a estrutura de dados compartilhada entre as threads
        for (int i = 0; i < elements; i++){
            this.vetor[i] = 0;
        }
    }

    //operacao de escrita sobre o recurso compartilhado (será acessado em posições diferentes)
    public void inc(int position){
        this.vetor[position]++;
    }

    //operacao de leitura sobre o recurso compartilhado (será acessado em posições diferentes)
    public int get(int position) {
        return this.vetor[position];
    }

    // função para retornar o tamanho do vetor
    public int tam(){
        return this.tam_vetor;
    }

    // função para retornar o número de threads do programa
    public int n_threads(){
        return this.number_threads;
    }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa
class T extends Thread{
    //identificador da Thread e também posição inicial de cada thread para intercalar entre os elementos do vetor
    private int id;

    //objeto compartilhado com outras threads
    Resource rs;

    //construtor
    public T(int tid, Resource rs){
        this.id = tid;
        this.rs = rs;
    }

    //metodo main da thread
    public void run(){
        // intercala entre as threads de acordo com o id que foi dado, e o passo é igual ao número de threads
        for (int i = this.id; i < rs.tam(); i += rs.n_threads()){
            rs.inc(i); // funcao que incrementa o vetor de acordo com a posição passada
        }
    }
}

//classe da aplicação
class IncVetor{

    public static void main(String[] args){
        // Primeiro checamos se o número de argumentos passados pela linha de comando está certo
        if (args.length != 2){
            System.out.println("Numero errado de argumentos, desejado: <Numero de elementos> <Numero de threads>");
            System.exit(1);
        }
        // Converte os argumentos para inteiros
        int elements = Integer.parseInt(args[0]);
        int nthreads = Integer.parseInt(args[1]);

        // Não podemos ter um número de threads maior que o número de elementos
        if (nthreads > elements) nthreads = elements;

        // Cria as instâncias das threads
        Thread[] threads = new Thread[nthreads];

        //cria uma instancia do recurso compartilhado entre as threads
        Resource rs = new Resource(elements, nthreads);

        // Cria as threads do programa
        for (int i = 0; i < threads.length; i++){
            threads[i] = new T(i, rs);
        }

        double inicio = System.currentTimeMillis(); // variável para cálculo de tempo da execução
        //inicia as threads
        for (int i = 0; i < threads.length; i++){
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i = 0; i < threads.length; i++){
            try { threads[i].join(); }
            catch (InterruptedException e) { return; }
        }

        System.out.println("Tempo total de execucao: " + (System.currentTimeMillis() - inicio)/1000);

        //checagem de corretude do programa concorrente
        for (int i = 0; i < elements; i++){
            if (rs.get(i) != 1){
                System.out.println("Resultado Incorreto!");
                System.exit(1);
            }
        }

        System.out.println("Resultado Correto!");
    }
}