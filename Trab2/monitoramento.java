import java.util.Random;

class Valor{
    private int idSensor;
    private int idLeitura;
    private int value;

    public Valor(){
        this.idSensor = 0;
        this.idLeitura = 0;
        this.value = 0;
    }

    public void setIdSensor(int idSensor){
        this.idSensor = idSensor;
    }

    public void setIdLeitura(int idLeitura){
        this.idLeitura = idLeitura;
    }

    public void setValue(int value){
        this.value = value;
    }

    public int getIdSensor(){
        return this.idSensor;
    }
    public int getIdLeitura(){
        return this.idLeitura;
    }
    public int getValue(){
        return this.value;
    }

}
class Resource {
    private Valor[] values;
    private int position;

    public Resource(){
        this.values = new Valor[60];
        this.position = 0;

        for (int i = 0; i < 60; i++){
            values[i] = new Valor();
        }
    }

    public void addValue(int idSensor, int idLeitura, int value){
        this.values[this.position].setIdLeitura(idLeitura);
        this.values[this.position].setIdSensor(idSensor);
        this.values[this.position].setValue(value);
        System.out.println("idLeitura: " + this.values[this.position].getIdLeitura() + ". idSensor: " + this.values[this.position].getIdSensor() +
                ". Value: " + this.values[this.position].getValue());
        System.out.println("Position: " + this.position);
        this.position += 1;
        this.position = this.position % 60;
    }
}

class LE {
    // recurso compartilhado
    private int leitores;
    private int escritores;
    private int prioridade_escrita;

     // construtor
     public LE(){
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

class Escritora {

    LE le;

    public Escritora(LE le){
        this.le = le;
    }

    public void IniciaEscrita(){
        synchronized (le){
            System.out.println("Escritora quer escrever.");
            le.setPrioridade_escrita(1);
            while (le.getEscritores() > 0 || le.getLeitores() > 0){
                System.out.println("Escritora bloqueou.");
                try { le.wait(); }
                catch (InterruptedException e) { System.out.println("erro"); return; }
                System.out.println("Escritora desbloqueou.");
            }
            le.adicionaEscritor();
            le.setPrioridade_escrita(0);
        }
    }

    public void FimEscrita(){
        synchronized (le) {
            System.out.println("Escritora terminou de escrever.");
            le.removeEscritor();
            System.out.println("Desbloqueando todas as Threads.");
            le.notifyAll();
        }
        try {Thread.sleep(1000);}
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}

class Leitora {

    LE le;

    public Leitora(LE rs, int id){
        this.le = le;
    }

    public void IniciaLeitura(int id){
        synchronized (le){
            System.out.println("Leitora quer ler.");
            while (le.getEscritores() > 0 || le.getPrioridade_escrita() > 0){
                System.out.println("Leitora bloqueou.");
                try { le.wait(); }
                catch (InterruptedException e) {System.out.println("erro"); return; }
                System.out.println("Leitora desbloqueou.");
            }
            le.adicionaLeitor();
        }
    }

    public void FimLeitura(){
        synchronized (le){
            System.out.println("Leitora terminou de ler.");
            le.removeLeitor();
            if (le.getLeitores() == 0) {
                System.out.println("Desbloqueando todas as Threads.");
                le.notifyAll();
            }
        }
        try {Thread.sleep(1000);}
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}

class Sensor extends Thread{

    // atributos
    private int id;
    private int id_leitura;

    Resource rs;
    LE le;
    //construtor

    public Sensor(int id, Resource rs, LE le){
        this.rs = rs;
        this.id = id;
        this.le = le;
    }

    public void run(){
        while (true){
            Random rand = new Random();
            int value = rand.nextInt(16) + 25;
            System.out.println("Temperatura: " + value);
            if (value > 30){
                Escritora esc = new Escritora(this.le);
                esc.IniciaEscrita();
                rs.addValue(this.id, 0, value);
                esc.FimEscrita();

            }
            try {Thread.sleep(1000);}
            catch (InterruptedException e) { System.out.println("erro"); return; }

        }
    }
}

class Atuador extends Thread{

}


public class monitoramento {

    public static void main(String[] args){

        if (args.length != 1) {
            System.out.println("Numero errado de argumentos, desejado: <Número de Sensores>");
            System.exit(1);
        }
        int sensores = Integer.parseInt(args[0]);

        Resource rs = new Resource();
        LE le = new LE();
        Thread[] threads = new Thread[sensores];

        for (int i = 0; i < threads.length; i++) {
            threads[i] = new Sensor(i, rs, le);
        }

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

        System.out.println("Execucao completa!");
    }
}