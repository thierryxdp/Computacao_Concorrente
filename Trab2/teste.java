import java.lang.Math;

public class teste {

    public static void main(String[] args){

        int contador = 63 % 60;
        for (int i = 0; i < 60; i++){
            contador--;
            System.out.print(Math.floorMod(contador, 60) + " ");
        }
        System.out.println("");

    }
}