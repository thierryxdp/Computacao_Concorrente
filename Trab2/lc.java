class Resource {
    // recurso compartilhado
    private int leitores;
    private int escritores;
    private int prioridade_escrita;

    public Resource(){
        this.leitores = 0;
        this.escritores = 0;
        this.prioridade_escrita = 0;
    }

    public void setPrioridade_escrita(int value){
        this.prioridade_escrita = value;
    }

    public int getPrioridade_escrita(){
        return this.prioridade_escrita;
    }

    public void adicionaEscritor(){
        this.escritores++;
    }

    public void removeEscritor(){
        this.escritores--;
    }

    public void adicionaLeitor(){
        this.leitores++;
    }

    public void removeLeitor(){
        this.leitores--;
    }

    public int getLeitores(){
        return this.leitores;
    }

    public int getEscritores(){
        return this.escritores;
    }
}

class Escritora extends Thread{

    private int id;
    Resource rs;

    public Escritora(Resource rs, int id){
        this.rs = rs;
        this.id = id;
    }

    public void run(){
        while(true){
            IniciaEscrita(this.id);
            System.out.println("Escritora[" + id + "] está escrevendo.");
            FimEscrita(this.id);
        }
    }

    public void IniciaEscrita(int id){
        synchronized (rs){
            System.out.println("Escritora[" + id + "] quer escrever.");
            rs.setPrioridade_escrita(1);
            while (rs.getEscritores() > 0 || rs.getLeitores() > 0){
                System.out.println("Escritora[" + id + "] bloqueou.");
                try { rs.wait(); }
                catch (InterruptedException e) { System.out.println("erro"); return; }
                System.out.println("Escritora[" + id + "] desbloqueou.");
            }
            rs.adicionaEscritor();
            rs.setPrioridade_escrita(0);
        }
    }

    public void FimEscrita(int id){
        synchronized (rs) {
            System.out.println("Escritora[" + id + "] terminou de escrever.");
            rs.removeEscritor();
            System.out.println("Desbloqueando todas as Threads.");
            rs.notifyAll();
        }
        try {Thread.sleep(1000);}
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}


class Leitora extends Thread{

    private int id;
    Resource rs;

    public Leitora(Resource rs, int id){
        this.rs = rs;
        this.id = id;
    }

    public void run(){
        while(true){
            IniciaLeitura(this.id);
            System.out.println("Leitora[" + id + "] está lendo.");
            FimLeitura(this.id);
        }
    }

    public void IniciaLeitura(int id){
        synchronized (rs){
            System.out.println("Leitora[" + id + "] quer ler.");
            while (rs.getEscritores() > 0 || rs.getPrioridade_escrita() > 0){
                System.out.println("Leitora[" + id + "] bloqueou.");
                try { rs.wait(); }
                catch (InterruptedException e) {System.out.println("erro"); return; }
                System.out.println("Leitora[" + id + "] desbloqueou.");
            }
            rs.adicionaLeitor();
        }
    }

    public void FimLeitura(int id){
        synchronized (rs){
            System.out.println("Leitora[" + id + "] terminou de ler.");
            rs.removeLeitor();
            if (rs.getLeitores() == 0) {
                System.out.println("Desbloqueando todas as Threads.");
                rs.notifyAll();
            }
        }
        try {Thread.sleep(1000);}
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}


public class lc {

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

        //cria uma instancia do recurso compartilhado entre as threads
        Resource rs = new Resource();
        // Cria as threads do programa
        for (int i = 0; i < nescritoras; i++) {
            threads[i] = new Escritora(rs, i);
        }

        for (int i = nescritoras; i < threads.length; i++){
            threads[i] = new Leitora(rs, i);
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