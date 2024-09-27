import java.util.Random;

public class Cliente extends Pessoa implements Runnable {
    private Barbearia barbearia;
    private Random random = new Random();

    public Cliente(int id, Barbearia barbearia) {
        this.id = id;
        this.barbearia = barbearia;
    }

    public void run() {
        try {
            while(true) {
                Thread.sleep(random.nextInt(3000) + 3000);
                boolean flagCorte = barbearia.cortaCabelo(this);

                if(!flagCorte) {
                    System.out.println("Cliente " + this.getID() + " tentou entrar na barbearia, mas está lotada... indo dar uma voltinha");
                }
            }

        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }


}
