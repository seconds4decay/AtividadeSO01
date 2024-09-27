import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.Semaphore;

public class Barbearia {
    private Semaphore cadeirasDisponiveis;
    private Semaphore barbeirosDisponiveis;
    private Queue<Cliente> filaClientes;
    private Random random = new Random();

    public Barbearia(int numCadeiras, int numBarbeiros) {
        cadeirasDisponiveis = new Semaphore(numCadeiras);
        barbeirosDisponiveis = new Semaphore(numBarbeiros);
        filaClientes = new LinkedList<>();
    }

    public boolean cortaCabelo(Cliente c) {
        try {
            if (cadeirasDisponiveis.tryAcquire()) {
                System.out.println("Cliente " + c.getID() + " conseguiu uma cadeira. Aguardando barbeiro.");

                Thread.sleep(random.nextInt(3000) + 3000);
                synchronized (filaClientes) {
                    filaClientes.add(c);
                    filaClientes.notifyAll();
                }

                barbeirosDisponiveis.acquire();
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    public Cliente proximoCliente() {
        synchronized (filaClientes) {

            while (filaClientes.isEmpty()) {

                try {
                    Thread.sleep(random.nextInt(3000) + 3000);
                    filaClientes.wait();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            return filaClientes.poll();
        }
    }
    public void corteTerminado(Cliente c) {

        cadeirasDisponiveis.release();
        barbeirosDisponiveis.release();
        System.out.println("Cliente " + c.getID() + "terminou o corte... saindo da barbearia!");
    }

}
