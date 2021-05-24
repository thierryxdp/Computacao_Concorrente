import java.util.Random;
import java.lang.Math;

class Valor{
    // Para cada medição precisamos armazenar o id do Sensor que fez a medição, quantas medições
    // já foram feitas por aquele Sensor e o valor em si medido.
    private int idSensor;
    private int idLeitura;
    private int value;

    public Valor(){
        this.idSensor = 0;
        this.idLeitura = 0;
        this.value = 0;
    }

    // Define um valor medido
    public void setValor(int idSensor, int idLeitura, int value){
        this.idSensor = idSensor;
        this.idLeitura = idLeitura;
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
    // Os valores armazenados são: um vetor de 60 posições do objeto Valor, que guarda no máximo 60 medições feitas;
    // uma variável que guarda a posição atual para escrita e o número de sensores que o programa tem,
    // que também corresponde ao número de atuadores.
    private Valor[] values;
    private int position;
    private int number_sensores;

    public Resource(int sensores){
        this.values = new Valor[60];
        this.position = 0;
        this.number_sensores = sensores;

        for (int i = 0; i < 60; i++){
            values[i] = new Valor();
        }
    }

    // Adiciona um valor ao vetor, ou seja, o Sensor fez uma medição que é válida a ser armazenada
    public void addValue(int idSensor, int value, int idLeitura){
        this.values[this.position].setValor(idSensor, idLeitura, value);
        /*
        System.out.println("idLeitura: " + this.values[this.position].getIdLeitura() + ". idSensor: " +
                this.values[this.position].getIdSensor() + ". Value: " + this.values[this.position].getValue());
        System.out.println("Position: " + this.position);

        for (int i = 0; i < 60; i++){
            System.out.print(this.values[i].getValue() + " ");
        }
        System.out.println("");
        */

        this.position += 1;
        this.position = this.position % 60;
    }

    public Valor getValue(int pos){
        return this.values[pos];
    }

}

class LE {
    // recurso compartilhado
    private int leitores; // número de leitores executando
    private int escritores; // número de escritores executando
    private int prioridade_escrita; // > 0 caso alguma escritora queira escrever e igual a 0 caso não haja

    // construtor que inicializa as variáveis com 0, é desnecessário pois por padrão o Java já faz isso.
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

class Escritora{

    // Id da Escritora e objeto da área de dados compartilhada
    private int id;
    LE le;

    // Inicializa de acordo com os parâmetros passados na instanciação
    public Escritora(LE le, int id){
        this.le = le;
        this.id = id;
    }


    public void IniciaEscrita(int id){
        synchronized (le){          // exclusão mútua através do recurso compartilhado
            //System.out.println("le.escritorPrioridade(" + id + ")");
            le.addPrioridade_escrita();    // adiciona prioridade de escrita
            while (le.getEscritores() > 0 || le.getLeitores() > 0){ // checa se existem leitores ou escritor executando
                //System.out.println("le.escritorBloqueado(" + id + ")");
                try { le.wait(); }  // caso existam escritores ou leitores já executando a thread se bloqueia
                catch (InterruptedException e) { System.out.println("erro"); return; }
            }
            //System.out.println("le.escritorEscrevendo(" + id + ")");
            le.adicionaEscritor();  // podemos adicionar o escritor
            le.removePrioridade_escrita();    // tiramos a prioridade de escrita pois a escritora já começou a executar
        }
    }

    public void FimEscrita(int id){
        synchronized (le) {         // exclusão mútua através do recurso compartilhado
            //System.out.println("le.escritorSaindo(" + id + ")");
            le.removeEscritor();    // remove o escritor pois ele já terminou de escrever
            le.notifyAll();         // notifica caso algum escritor ou leitores tenham sido bloqueados
        }
        try {Thread.sleep(1000);}   // para visualizar melhor a alternância entre as threads
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}

class Leitora{

    // Id da Escritora e objeto da área de dados compartilhada
    private int id;
    LE le;

    // Inicializa de acordo com os parâmetros passados na instanciação
    public Leitora(LE le, int id){
        this.le = le;
        this.id = id;
    }


    public void IniciaLeitura(int id){
        synchronized (le){          // exclusão mútua através do recurso compartilhado
            while (le.getEscritores() > 0 || le.getPrioridade_escrita() > 0){   // caso tenha alguma thread escritora executando ou querendo executar bloqueia
                //System.out.println("le.leitorBloqueado(" + id + ")");
                try { le.wait(); }
                catch (InterruptedException e) {System.out.println("erro"); return; }
            }
            //System.out.println("le.leitorLendo(" + id + ")");
            le.adicionaLeitor();    // conseguiu passar pela condição e adiciona o leitor
        }
    }

    public void FimLeitura(int id){
        synchronized (le){          // exclusão mútua através do recurso compartilhado
            //System.out.println("le.leitorSaindo(" + id + ")");
            le.removeLeitor();      // leitora já terminou de ler e pode ser removida
            if (le.getLeitores() == 0) {
                le.notifyAll();     // se for a única thread leitora executando libera a thread escritora que queria escrever se for o caso.
            }
        }
        try {Thread.sleep(1000);}   // visualizar melhor a alternância entre as threads
        catch (InterruptedException e) { System.out.println("erro"); return; }
    }
}

class Sensor extends Thread{

    // atributos
    private int id;
    private int idLeitura;

    Resource rs;
    LE le;
    //construtor

    public Sensor(int id, Resource rs, LE le){
        this.rs = rs;
        this.id = id;
        this.le = le;
        this.idLeitura = 0;
    }

    public void run(){
        while (true){
            Random rand = new Random();
            int value = rand.nextInt(16) + 25;
            if (value > 30){
                Escritora esc = new Escritora(this.le, this.id);
                esc.IniciaEscrita(this.id);
                System.out.println("sa.sensor(" + this.id + ", " + value +  ")");
                rs.addValue(this.id, value, this.idLeitura);
                this.idLeitura++;
                //System.out.println("Sensor(" + this.id + ") terminou a escrita");
                esc.FimEscrita(this.id);

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
    Resource rs;
    LE le;
    //construtor

    public Atuador(int id, Resource rs, LE le){
        this.rs = rs;
        this.id = id;
        this.le = le;
        this.alerta_vermelho = 0;
        this.alerta_amarelo = 0;
        this.media = 0;
        this.position = 0;
    }

    public void run(){
        while (true){
            Leitora leit = new Leitora(this.le, this.id);
            leit.IniciaLeitura(this.id);
            //System.out.println("sa.atuadorlendo(" + this.id + ")");
            int ultimo_Valor = 0;
            for (int i = 0; i < 60; i++){
                if (rs.getValue(i).getValue() == 0) break;
                if (rs.getValue(i).getIdSensor() == this.id){
                    if (rs.getValue(i).getIdLeitura() >= ultimo_Valor) {
                        ultimo_Valor = i;
                    }
                }

            }
            //System.out.println("Posicao colocada por ultimo do Sensor(" + this.id + "): " + ultimo_Valor);
            int contador = ultimo_Valor % 60;
            for (int i = 0; i < 60; i++){

                if (rs.getValue(contador).getIdSensor() == this.id && rs.getValue(contador).getValue() != 0){

                    if (rs.getValue(contador).getValue() > 35){
                        if (position < 15) alerta_amarelo++;
                        if (position < 5) alerta_vermelho++;
                    }

                    this.media = this.media + rs.getValue(contador).getValue();
                    this.position++;

                }
                contador--;
                contador = Math.floorMod(contador, 60);
            }

            if (alerta_vermelho >= 5){
                System.out.println("sa.atuadorAlertaVermelho(" + this.id + ")");
            } else if (alerta_amarelo >= 5){
                System.out.println("sa.atuadorAlertaAmarelo(" + this.id + ")");
            } else {
                System.out.println("sa.atuadorAlertaNormal(" + this.id + ")");
            }

            if (this.media == 0){
                System.out.println("sa.atuadorMedia(" + this.id + ", " + 0 + ")");
            } else {
                System.out.println("sa.atuadorMedia(" + this.id + ", " + this.media/this.position + ")");
            }

            this.alerta_vermelho = 0;
            this.alerta_amarelo = 0;
            this.media = 0;
            this.position = 0;

            //System.out.println("Atuador(" + this.id + ") terminou de ler.");
            leit.FimLeitura(this.id);
            try {Thread.sleep(2000);}
            catch (InterruptedException e) { System.out.println("erro"); return; }
        }
    }
}


public class monitoramento {

    public static void main(String[] args){

        if (args.length != 1) {
            System.out.println("Numero errado de argumentos, desejado: <Numero de Sensores>");
            System.exit(1);
        }
        int sensores = Integer.parseInt(args[0]);

        if (sensores > 4){
            System.out.println("Numero de sensores nao e o recomendado, por favor execute novamente com um numero menor.");
            System.exit(1);
        }

        //inicia log de saida
        System.out.println("# -*- coding: utf-8 -*-");
        System.out.println("import verificaSA");
        System.out.println("sa = verificaSA.SA(" + sensores + ")");

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