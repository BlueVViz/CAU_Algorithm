/**
 * Homework Assignment #4: Percolation
 *
 * - PercolationStats for simulation of percolation
 *
 * @ Student ID : 20145337
 * @ Name       : Junhyuck Woo
 **/

import java.util.Random;

public class PercolationStats {

    private int mT;             // number of experiments to run
    private double mMean;       // mean of percolation threshold
    private double mStddev;     // standard deviation of percolation threshold

    // perform T independent experiments on an N-by-N grid
    public PercolationStats(int N, int T) {

        if (N <= 0 || T <= 0)
            throw new java.lang.IllegalArgumentException(
                            "N and T must be greater than 1");
        mT = T;

        Random random = new Random(System.currentTimeMillis());

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        try{
            if(T < 30){
                Exception ms = new Exception("Monte Carlo Simulation should do at least 30 times");
                throw ms;
            }
        }catch (Exception me){
            System.out.println("minor Error: " + me.getMessage());
            System.exit(0);
        }


        Percolation perc;                       // Percolation object
        int row, col;                   // To save calculated col and row
        int count = 0;                          // Count how many trials are done
        int size = N * N;                       // Set the sites array size
        double numSite;                     // To count opened site number
        double buf;                          // Buffer value for calculate or save temporary
        double[] devi = new double[mT];         // To calculate deviation
        int[] check = new int[size];            // Create array for random input
        for(int i = 0; i < size; i++)
            check[i] = i;

        do {
            /*
                Reset value for executing repeatedly
                Remove old one using null => GC remove left one
                Recreate percolation object
             */
            numSite = 0;                    // Set 0 to recount number of opened site
            perc = null;                    // Remove old object
            perc = new Percolation(N);      // Recreate object

            /*
                Input should be different every time.
                So shuffle array every times.
                Using Knuth algorithms
             */
            for(int i = 0; i < size; i++){
                int r = random.nextInt(size - i) + i;
                int tmp = check[i];
                check[i] = check[r];
                check[r] = tmp;
            }

            /*
                Try Percolation
                Percolation location of site is made by below formulation
                location = ( col - 1 ) * N + ( row - 1)
                => col = ( location / N ) + 1
                => row = ( location % N ) + 1
             */
            for(int i = 0; i < size && !perc.percolates(); i++){
                col = (check[i] / N) + 1;   // Get col value
                row = (check[i] % N) + 1;   // Get row value
                perc.open(col, row);
                numSite+=1;
            }

            /*
                To calculate deviation and mean,
                accumulate number of opened site on mMean for mean
                save number of opened site on devi array seperately
             */
            mMean += numSite;
            devi[count] = numSite;
            count++;                    // Count how many program try to check performance
        }while(count < mT);             // Repeat it up to #mT

        // Calculate Mean
        mMean /= size;
        mMean /= mT;

        // Calculate Deviation
        for(int i = 0; i< mT; i++){
            buf = devi[i] / size;
            buf -= mMean;
            mStddev += buf*buf;
        }
        mStddev /= (mT - 1);
        mStddev = Math.sqrt(mStddev);
    }

    // sample mean of percolation threshold
    public double mean() {

        return mMean;
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return mStddev;
    }

    // low  endpoint of 95% confidence interval
    public double confidenceLo() {
        return mMean - 1.96 * mStddev / Math.sqrt(mT);
    }

    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return mMean + 1.96 * mStddev / Math.sqrt(mT);
    }

    public static void main(String[] args)    // test client(described below)
    {
        PercolationStats percStats;
        int N = 0;
        int T = 0;
        long start = System.currentTimeMillis();
        double time;

        // you must get two inputs N and T
        if (args.length > 0) {
            try {
                N = Integer.parseInt(args[0]);  // get N
                T = Integer.parseInt(args[1]);  // get T
            } catch (NumberFormatException e) {
                System.err.println("Argument" + args[0] + " and " 
                                            + args[1] + " must be integers.");
                return;
            }
        }

        // run experiment
        percStats = new PercolationStats(N, T);

        // print result
        System.out.println("mean                    = " + percStats.mean());
        System.out.println("stddev                  = " + percStats.stddev());
        System.out.println("95% confidence interval = "
                + percStats.confidenceLo() + ", " + percStats.confidenceHi());

        time = (System.currentTimeMillis() - start) / 1000.0;
        System.out.printf("total time = %f sec (N = %d, T = %d)\n", time, N, T);
    }
}

