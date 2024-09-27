import java.util.Random;

public class Barbeiro extends Pessoa implements Runnable {
    private Barbearia barbearia;
    private Random random = new Random();

    public Barbeiro(int id, Barbearia barbearia) {
        this.id = id;
        this.barbearia = barbearia;
    }

    public void run()  {
        try {
            while(true) {
                Thread.sleep(random.nextInt(3000) + 3000);
                Cliente cliente = barbearia.proximoCliente();

                if(cliente == null) {
                    System.out.println("Barbeiro " + this.getID() + " indo dormir um pouco... não há clientes na barbearia...");
                } else {
                    System.out.println("Barbeiro " + this.getID() + " acordou! Começando os trabalhos!");
                    System.out.println("Barbeiro " + id + " cortando o cabelo do Cliente " + cliente.getID());

                    Thread.sleep(random.nextInt(3000) + 3000);

                    barbearia.corteTerminado(cliente);
                    System.out.println("Barbeiro " + id + " terminou o corte do Cliente " + cliente.getID());

                }
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

}