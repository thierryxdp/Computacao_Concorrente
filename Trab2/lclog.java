class Resource {

    // recurso compartilhado
    private int leitores; // número de leitores executando
    private int escritores; // número de escritores executando
    private int prioridade_escrita; // 1 se há prioridade para escrita e 0 caso não haja

    // construtor que inicializa as variáveis com 0, é desnecessário pois por padrão o Java já faz isso.
    public Resource(){
        this.leitores = 0;
        this.escritores = 0;
        this.prioridade_escrita = 0;
    }

    // define prioridade para escrita, o recomendado é passar como valor 0 (sem prioridade) ou 1 (com prioridade)
    public void setPrioridade_escrita(int value){
        this.prioridade_escrita = value;
    }

    // retorna valor da prioridade
    public int getPrioridade_escrita(){
        return this.prioridade_escrita;
    }

    // Soma em 1 o número de escritores
    public void adicionaEscritor(){
        this.escritores++;
    }

    // Diminui em 1 o número de escritores
    public void removeEscritor(){
        this.escritores--;
    }

    // Soma em 1 o número de leitores
    public void adicionaLeitor(){
        this.leitores++;
    }

    // Diminui em 1 o número de leitores
    public void removeLeitor(){
        this.leitores--;
    }

    // retorna o número de leitores
    public int getLeitores(){
        return this.leitores;
    }

    // retorna o número de escritores
    public int getEscritores(){
        return this.escritores;
    }
}

class Escritora extends Thread{

    // Id da Escritora e objeto da área de dados compartilhada
    private int id;
    Resource rs;

    // Inicializa de acordo com os parâmetros passados na instanciação
    public Escritora(Resource rs, int id){
        this.rs = rs;
        this.id = id;
    }

    // Tarefa que a thread Escritora está encarregada de fazer
    public void run(){
        while(true){    // executa indeterminadamente
            IniciaEscrita(this.id); // adiciona escritor
            FimEscrita(this.id);    // remove escritor
        }
    }

    public void IniciaEscrita(int id){
        synchronized (rs){          // exclusão mútua através do recurso compartilhado
            System.out.println("le.escritorPrioridade(" + id + ")");
            rs.setPrioridade_escrita(1);    // seta prioridade de escrita com o valor 1
            while (rs.getEscritores() > 0 || rs.getLeitores() > 0){ // checa se existem leitores ou escritor executando
                System.out.println("le.escritorBloqueado(" + id + ")");
                try { rs.wait(); }  // caso existam escritores ou leitores já executando a thread se bloqueia
                catch (InterruptedException e) { System.out.println("erro"); return; }
            }
            System.out.println("le.escritorEscrevendo(" + id + ")");
            rs.adicionaEscritor();  // podemos adicionar o escritor
            rs.setPrioridade_escrita(0);    // tiramos a prioridade de escrita pois a escritora já começou a executar
        }
    }

    public void FimEscrita(int id){
        synchronized (rs) {         // exclusão mútua através do recurso compartilhado
            System.out.println("le.escritorSaindo(" + id + ")");
            rs.removeEscritor();    // remove o escritor pois ele já terminou de escrever
            rs.notifyAll();         // notifica caso algum escritor ou leitores tenham sido bloqueados
        }
        try {Thread.sleep(1000);}   // para visualizar melhor a alternância entre as threads
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}


class Leitora extends Thread{

    // Id da Escritora e objeto da área de dados compartilhada
    private int id;
    Resource rs;

    // Inicializa de acordo com os parâmetros passados na instanciação
    public Leitora(Resource rs, int id){
        this.rs = rs;
        this.id = id;
    }

    // Tarefa que a thread Leitora está encarregada de fazer
    public void run(){
        while(true){
            IniciaLeitura(this.id);
            FimLeitura(this.id);
        }
    }


    public void IniciaLeitura(int id){
        synchronized (rs){          // exclusão mútua através do recurso compartilhado
            while (rs.getEscritores() > 0 || rs.getPrioridade_escrita() > 0){   // caso tenha alguma thread escritora executando ou querendo executar bloqueia
                System.out.println("le.leitorBloqueado(" + id + ")");
                try { rs.wait(); }
                catch (InterruptedException e) {System.out.println("erro"); return; }
            }
            System.out.println("le.leitorLendo(" + id + ")");
            rs.adicionaLeitor();    // conseguiu passar pela condição e adiciona o leitor
        }
    }

    public void FimLeitura(int id){
        synchronized (rs){          // exclusão mútua através do recurso compartilhado
            System.out.println("le.leitorSaindo(" + id + ")");
            rs.removeLeitor();      // leitora já terminou de ler e pode ser removida
            if (rs.getLeitores() == 0) {
                rs.notifyAll();     // se for a única thread leitora executando libera a thread escritora que queria escrever se for o caso.
            }
        }
        try {Thread.sleep(1000);}   // visualizar melhor a alternância entre as threads
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}


public class lclog {

    public static void main(String[] args) {
        // Primeiro checamos se o número de argumentos passados pela linha de comando está certo
        // Passaremos o número de threads leitoras e o número de threads escritoras, nessa ordem.
        if (args.length != 2) {
            System.out.println("Numero errado de argumentos, desejado: <Num Threads Leitoras> <Num Threads Escritoras>");
            System.exit(1);
        }
        // Converte os argumentos para inteiros
        int nleitoras = Integer.parseInt(args[0]);
        int nescritoras = Integer.parseInt(args[1]);

        // Cria as instâncias das threads
        Thread[] threads = new Thread[nescritoras + nleitoras];

        //inicia log de saida
        System.out.println("# -*- coding: utf-8 -*-");
        System.out.println("import verificaLE");
        System.out.println("le = verificaLE.LE()");

        //cria uma instancia do recurso compartilhado entre as threads
        Resource rs = new Resource();
        // Cria as threads do programa
        for (int i = 0; i < nescritoras; i++) {
            threads[i] = new Escritora(rs, i);
        }

        for (int i = nescritoras; i < threads.length; i++){
            threads[i] = new Leitora(rs, i-nescritoras);
        }
        //inicia as threads
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
        System.out.println("Execução completa!");
    }
}