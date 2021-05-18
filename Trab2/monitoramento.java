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
    private int number_sensores;
    private int[] idLeitura;

    public Resource(int sensores){
        this.values = new Valor[60];
        this.position = 0;
        this.number_sensores = sensores;

        for (int i = 0; i < 60; i++){
            values[i] = new Valor();
        }
        this.idLeitura = new int[this.number_sensores];

        for (int i = 0; i < this.number_sensores; i++){
            this.idLeitura[i] = 0;
        }
    }

    public void addValue(int idSensor, int value){
        this.values[this.position].setIdLeitura(this.idLeitura[idSensor]);
        this.values[this.position].setIdSensor(idSensor);
        this.values[this.position].setValue(value);
        //System.out.println("idLeitura: " + this.values[this.position].getIdLeitura() + ". idSensor: " + this.values[this.position].getIdSensor() +
        //        ". Value: " + this.values[this.position].getValue());
        //System.out.println("Position: " + this.position);
        this.position += 1;
        this.position = this.position % 60;
    }

    public Valor getValue(int pos){
        return this.values[pos];
    }

    public void addIdLeitura(int idAtuador){
        this.idLeitura[idAtuador]++;
    }
}

class LE {
    // recurso compartilhado
    private int leitores;
    private int escritores;
    private int prioridade_escrita; // > 0 caso alguma escritora queira escrever e igual a 0 caso não haja

     // construtor
     public LE(){
        this.leitores = 0;
        this.escritores = 0;
        this.prioridade_escrita = 0;
    }

    // aumenta prioridade de escrita em 1, significando que existe mais uma thread escritora querendo escrever
    public void addPrioridade_escrita(){
        this.prioridade_escrita++;
    }

    // remove prioridade da escrita em 1, significando que uma thread escritora começou a escrever
    public void removePrioridade_escrita(){
        this.prioridade_escrita--;
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
            //System.out.println("Escritora quer escrever.");
            le.addPrioridade_escrita();    // adiciona prioridade de escrita
            while (le.getEscritores() > 0 || le.getLeitores() > 0){
                //System.out.println("Escritora bloqueou.");
                try { le.wait(); }
                catch (InterruptedException e) { System.out.println("erro"); return; }
                //System.out.println("Escritora desbloqueou.");
            }
            le.adicionaEscritor();
            le.removePrioridade_escrita();    // tiramos a prioridade de escrita pois a escritora já começou a executar
        }
    }

    public void FimEscrita(){
        synchronized (le) {
            //System.out.println("Escritora terminou de escrever.");
            le.removeEscritor();
            //System.out.println("Desbloqueando todas as Threads.");
            le.notifyAll();
        }
        try {Thread.sleep(1000);}
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}

class Leitora {

    LE le;

    public Leitora(LE le){
        this.le = le;
    }

    public void IniciaLeitura(){
        synchronized (le){
            //System.out.println("Leitora quer ler.");
            while (le.getEscritores() > 0 || le.getPrioridade_escrita() > 0){
                //System.out.println("Leitora bloqueou.");
                try { le.wait(); }
                catch (InterruptedException e) {System.out.println("erro"); return; }
                //System.out.println("Leitora desbloqueou.");
            }
            le.adicionaLeitor();
        }
    }

    public void FimLeitura(){
        synchronized (le){
            //System.out.println("Leitora terminou de ler.");
            le.removeLeitor();
            if (le.getLeitores() == 0) {
                //System.out.println("Desbloqueando todas as Threads.");
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
            if (value > 30){
                System.out.println("Temperatura: " + value);
                Escritora esc = new Escritora(this.le);
                esc.IniciaEscrita();
                rs.addValue(this.id, value);
                esc.FimEscrita();

            }
            try {Thread.sleep(1000);}
            catch (InterruptedException e) { System.out.println("erro"); return; }

        }
    }
}

class Atuador extends Thread{

    // atributos
    private int id;
    private int alerta_vermelho;
    private int alerta_amarelo;
    private double media;
    private int position;
    private int alerta;
    Resource rs;
    LE le;
    //construtor

    public Atuador(int id, Resource rs, LE le){
        this.rs = rs;
        this.id = id;
        this.le = le;
        this.alerta = 0; // zero condição normal, 1 condição amarela e 2 condição vermelha
        this.alerta_vermelho = 0;
        this.alerta_amarelo = 0;
        this.media = 0;
        this.position = 0;
    }

    public void run(){
        while (true){
            Leitora leit = new Leitora(this.le);
            leit.IniciaLeitura();
            for (int i = 0; i < 60; i++){
                if (rs.getValue(i).getValue() == 0) break;

                if (rs.getValue(i).getIdSensor() == this.id){
                    // Se o id do Sensor for igual ao Id do Atuador, nós lemos aquele valor e portanto somamos 1 ao idLeitura.
                    rs.addIdLeitura(this.id);
                    // Adicionamos o valor ao vetor de valores lidos
                    if (position < 15){
                        if (rs.getValue(i).getValue() > 35) alerta_amarelo++;
                        position++;
                    } else {
                        media = media + rs.getValue(i).getValue();
                        position++;
                    }
                    if (rs.getValue(i).getValue() > 35) {
                        alerta_vermelho++;
                        if (alerta_vermelho == 5){
                            alerta = 2;
                        }
                    } else {
                        alerta_vermelho = 0;
                    }
                }
            }

            if (alerta == 2){
                System.out.println("Atuador[" + this.id + "] emitiu Alerta Vermelho!!!");
            } else if (alerta == 0 && alerta_amarelo >= 5){
                System.out.println("Atuador[" + this.id + "] emitiu Alerta Amarelo!!");
            } else {
                System.out.println("Atuador[" + this.id + "] emitiu Alerta Normal!");
            }
            if (this.media == 0){
                System.out.println("Sem sensor disponiveis apos leitura!");
            } else {
                System.out.println("Media dos sensores ainda disponiveis: " + media/(position-15));
            }
            this.alerta = 0;
            this.alerta_vermelho = 0;
            this.alerta_amarelo = 0;
            this.media = 0;
            this.position = 0;
            leit.FimLeitura();
            try {Thread.sleep(2000);}
            catch (InterruptedException e) { System.out.println("erro"); return; }
        }
    }
}


public class monitoramento {

    public static void main(String[] args){

        if (args.length != 1) {
            System.out.println("Numero errado de argumentos, desejado: <Número de Sensores>");
            System.exit(1);
        }
        int sensores = Integer.parseInt(args[0]);

        Resource rs = new Resource(sensores);
        LE le = new LE();
        Thread[] threadsSensores = new Thread[sensores];
        Thread[] threadsAtuadores = new Thread[sensores];


        for (int i = 0; i < threadsSensores.length; i++) {
            threadsSensores[i] = new Sensor(i, rs, le);
            threadsAtuadores[i] = new Atuador(i, rs, le);
        }

        for (int i = 0; i < threadsSensores.length; i++) {
            threadsSensores[i].start();
            threadsAtuadores[i].start();
        }

        //espera pelo termino de todas as threads
        for (int i = 0; i < threadsSensores.length; i++) {
            try {
                threadsAtuadores[i].join();
                threadsSensores[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        System.out.println("Execucao completa!");
    }
}