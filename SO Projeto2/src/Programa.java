public class Programa {
    public static void main(String[] args) {
        int numBarbeiros = Integer.parseInt(args[0]);
        int numCadeiras = Integer.parseInt(args[1]);
        int numClientes = Integer.parseInt(args[2]);

        Barbearia barbearia = new Barbearia(numCadeiras, numBarbeiros);

        for (int i = 0; i < numBarbeiros; i++) {
            new Thread(new Barbeiro(i + 1, barbearia)).start();
        }

        for (int i = 0; i < numClientes; i++) {
            new Thread(new Cliente(i + 1, barbearia)).start();
        }


    }
}