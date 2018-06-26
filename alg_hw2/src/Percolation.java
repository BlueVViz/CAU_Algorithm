/**
 * Homework Assignment #4: Percolation
 *
 *  - Percolation data structure
 *
 * @ Student ID : 20145337
 * @ Name       : Junhyuck Woo
 **/

import java.util.Scanner;
import java.io.File;

public class Percolation {

    private static final boolean SITE_BLOCKED = false;
    private static final boolean SITE_OPEN = true;
    private boolean[] sites;          // sites[i] = state of site i
    private int mN;                   // remember the input N
    private int topIdx;               // idx of the special top
    private int bottomIdx;            // idx of the special bottom
    private WeightedQuickUnionUF uf;
    /*********************************
     * YOU CAN ADD MORE HERE
     *********************************/
    /*
        Variable Location:
        Calculate appropriate location of structure's site
        => Argument uses 2 dimensions form,
           but Array uses one dimension.

        Another WeightedQuickUnionUF defined for isFull
        If we just call isFull using UF, there is a BackWash bug.
        To remove the bug, I should create another same data structure.
        It's quite waste, but I don't have other options.
     */
    private int location = 0;
    private WeightedQuickUnionUF flow;

    // create N-by-N grid, with all sites blocked
    public Percolation(int N) {
        if (N <= 0)
            throw new IllegalArgumentException("N must be >0");
        mN = N;
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
    /*
        Basically, We need board to check everything.
        As you see the Uf and flow, Its size is different with sites.
        Because, this data structure will union with
     */
        sites = new boolean[mN*mN];
        for(int i = 0; i < mN * mN; i++)                    // Initialize board as Blocked status.
            sites[i] = SITE_BLOCKED;
        uf = new WeightedQuickUnionUF(mN * mN + 2);     // Original Data structure.
        flow = new WeightedQuickUnionUF(mN * mN + 2);   // To check remove backwash bug.
        topIdx = mN * mN;                                   // To reduce calculation of topIdx, save it.
        bottomIdx = mN * mN + 1;                            // To reduce calculation of bottomIdx, save it.
    }

    private void checkIndex(int i, int j) {
        if (i < 1 || i > mN)
            throw new IndexOutOfBoundsException("row index i out of bounds");
        if (j < 1 || j > mN)
            throw new IndexOutOfBoundsException("column index j out of bounds");
    }

    // open site(row i, column j) if it is not open already
    public void open(int i, int j) {
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
     /*
        Check Index area.
        If arguments are overflowed or not suited
        exit the program.
     */
        try{
            checkIndex(i, j);
        }catch (IndexOutOfBoundsException e){
            System.out.println(e.getMessage());
            System.exit(0);
        }
        /*
            If selected position is opened,
            then union each point.
         */
        if( !isOpen(i, j) ){
            sites[location] = SITE_OPEN;
            // If first row is opened then union with topIdx (UF, flow)
            if(i == 1){
                uf.union(topIdx,location);
                flow.union(topIdx,location);
            }
            // Upper point is opened (UF, flow)
            if(i > 1 && sites[location - mN]){
                flow.union(location, location -mN);
                uf.union(location, location - mN);
            }
            // lower point is opened (UF, flow)
            if(i < mN &&sites[location + mN]){
                flow.union(location,location + mN);
                uf.union(location,location + mN);
            }
            // Right point is opened (UF, flow)
            if(j < mN && sites[location + 1] == SITE_OPEN){
                flow.union(location,location + 1);
                uf.union(location,location + 1);
            }
            // Left point is opened (UF, flow)
            if(j > 1 && sites[location-1] == SITE_OPEN){
                flow.union(location,location-1);
                uf.union(location,location-1);
            }
            /*
                If bottom line is opened, then union with bottomIdx(UF).
                Because Union bottomIdx and location make backwash bug
                I don't union those.
             */
            if(i == mN)
                uf.union(bottomIdx,location);
        }
    }

    // is site(row i, column j) open?
    public boolean isOpen(int i, int j) {
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        /*
            Check Index area.
            If arguments are overflowed or not suited
            exit the program.
         */

        try{
            checkIndex(i, j);
        }catch (IndexOutOfBoundsException e){
            System.out.println(e.getMessage());
            System.exit(0);
        }
        location = (i - 1) * mN + (j - 1);
        if(sites[location] == SITE_OPEN)
            return SITE_OPEN;
        else
            return SITE_BLOCKED;   // TODO: modify
    }

    // is site(row i, column j) full?
    public boolean isFull(int i, int j) {
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        /*
            Check Index area.
            If arguments are overflowed or not suited
            exit the program.
         */
        try{
            checkIndex(i, j);
        }catch (IndexOutOfBoundsException e){
            System.out.println(e.getMessage());
            System.exit(0);
        }
        location = (i - 1) * mN + (j - 1);
        // Check liquid can flow top to a specific point
        if(flow.connected(topIdx, location))
            return true;
        else
            return false;   // TODO: modify
        }

    // does the system percolate?
    public boolean percolates() {
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
    /*
        Check connection of topIdx and bottomIdx
        If it is connected, return true to finish function
        If not, return false.
     */
        if(uf.connected(topIdx,bottomIdx))
            return true;
        else
            return false;   // TODO: modify
    }

    // test client(optional)
    public static void main(String[] args)
    {
        Scanner in;
        int N = 0;
        long start = System.currentTimeMillis();

        try {
            // get input file from argument
            in = new Scanner(new File(args[0]), "UTF-8");
        } catch (Exception e) {
            System.out.println("invalid or no input file ");
            return;
        }

        N = in.nextInt();         // N-by-N percolation system
        System.out.printf("N = %d\n", N);

        // repeatedly read in sites to open and draw resulting system
        Percolation perc = new Percolation(N);

        while (in.hasNext()) {
            int i = in.nextInt();   // get i for open site (i,j)
            int j = in.nextInt();   // get j for open site (i,j)
            perc.open(i, j);        // open site (i,j)
            System.out.printf("open(%d, %d)\n", i, j);
        }
        if (perc.percolates()) {
            System.out.println("This system percolates");
        } else {
            System.out.println("This system does NOT percolate");
        }

        double time = (System.currentTimeMillis() - start) / 1000.0;
        System.out.println("running time = "+ time + " sec");
    }
}

