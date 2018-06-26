/**
 * Homework Assignment #6: "8-Puzzle"
 *
 *  - Board class for solving "8-Puzzle" Programming Assignment
 *
 *  Compilation:  javac Board.java Queue.java
 *
 * @ Student ID : 20145337
 * @ Name       : Junhyuck Woo
 **/

import java.io.File;
import java.util.Scanner;

public class Board {

    private int[][] tiles;
    private int N;
    /*********************************
     * YOU CAN ADD MORE HERE
     *********************************/
    private int iFor, jFor;    // Using for for-loop
    private int itmp, jtmp;    // Using for 0 location
    private int manhat;        // To contain manhattan value
    private int flag = 0;      // To avoid repeatedly working

    // construct a board from an N-by-N array of blocks
    // (where blocks[i][j] = block in row i, column j)
    public Board(int[][] blocks) {
        if (blocks == null)
            throw new java.lang.NullPointerException();

        N = blocks.length;
        if (N < 2 || N > 128)
            throw new IllegalArgumentException("N must be <= 128");

        tiles = new int[N][N];
        for (int i = 0; i < N; i++)
            System.arraycopy(blocks[i], 0, tiles[i], 0, blocks[i].length);
    }

    // board dimension N
    public int dimension() {
        return N;
    }

    // sum of Manhattan distances between blocks and goal
    public int manhattan() {
        int manhattan = 0;

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        // flag 1 means that manhattan is already calculated
        if(flag == 1)
            return manhat;

        for(iFor = 0; iFor < N; iFor++){
            for(jFor = 0; jFor < N; jFor++){
                int buf = tiles[iFor][jFor] - 1;  // Get number
                int y = buf / N;        // Check real location y
                int x = buf % N;        // Check real location x
                if(buf == -1) {         // Do nothing -> 0
                    itmp = iFor; jtmp = jFor;
                }
                else
                    manhattan += Math.abs(y - iFor) + Math.abs(x - jFor);
            }
        }
        manhat = manhattan; flag = 1; // for optimization
        return manhattan;   // TODO
    }

    // is this board the goal board?
    public boolean isGoal() {

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        // Check every component
        if(tiles[N-1][N-1] != 0)
            return false;

        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                if(tiles[i][j] != i * N + j + 1 && i+j != 2*(N-1))
                    return false;
            }
        }

        return true;   // TODO
    }

    private void swap(int[][] blocks, int r1, int c1, int r2, int c2) {
        if (r1 < 0 || c1 < 0 || r2 < 0 || c2 < 0)
            throw new IndexOutOfBoundsException("row/col index < 0");
        if (r1 >= N || c1 >= N || r2 >= N || c2 >= N)
            throw new IndexOutOfBoundsException("row/col index >= N");

        // swap blocks
        // Swap blocks[r1][c1], blocks[r2][c2]
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        if(blocks == null)
            throw new NullPointerException("Can't swap: No block input");

        int tmp = blocks[r1][c1];
        blocks[r1][c1] = blocks[r2][c2];
        blocks[r2][c2] = tmp;
    }

    // a board that is obtained by exchanging two adjacent blocks in the same row
    public Board twin() {
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            System.arraycopy(tiles[i], 0, blocks[i], 0, tiles[i].length);

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        // Create twin board, swap two components
        if(N == 1){}
        else if(blocks[0][0] != 0 && blocks[0][1] != 0)
            swap(blocks, 0, 0, 0, 1);
        else
            swap(blocks, N - 1, 0, N - 1, 1);

        // Return board
        return new Board(blocks);
    }

    // does this board equal y?
    public boolean equals(Object y) {

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        // Typecast for checking
        Board test = (Board)y;

        // y == null means fail to receive or not correct input
        if(y == null) return false;

        // Board has two variable, titles(board) and N(board size)
        // ignore iFor, jFor => exist for for-loop
        // Check board size
        if(this.N != test.N) return false;

        // Check titles components
        for(iFor = 0; iFor < N; iFor++){
            for(jFor = 0; jFor < N; jFor++){
                if(this.tiles[iFor][jFor] != test.tiles[iFor][jFor])
                    return false;
            }
        }

        // Every component sames, then two objects equal
        return true;    // TODO
    }

    // all neighboring boards
    public Iterable<Board> neighbors() {

        Queue<Board> nbrs = new Queue<Board>();

        // put all neighbor boards into the queue

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        // Variable for stor '0' location

        // Find '0' location
        if(flag != 1){
            for(iFor = 0; iFor < N; iFor++){
                for(jFor = 0; jFor < N; jFor++){
                    if(tiles[iFor][jFor] == 0) {
                        itmp = iFor; jtmp = jFor;
                        break;
                    }
                }
            }
            flag = 1;
        }


        //Swap component up, down, left, right
        // Up
        if(itmp - 1 >= 0){
            swap(tiles, itmp, jtmp, itmp - 1, jtmp);
            nbrs.enqueue(new Board(tiles));
            swap(tiles, itmp, jtmp, itmp - 1, jtmp);

        }
        // Down
        if(itmp + 1 <= N - 1){
            swap(tiles, itmp, jtmp, itmp + 1, jtmp);
            nbrs.enqueue(new Board(tiles));
            swap(tiles, itmp, jtmp, itmp + 1, jtmp);
        }
        // Left
        if(jtmp - 1 >= 0){
            swap(tiles, itmp, jtmp, itmp, jtmp - 1);
            nbrs.enqueue(new Board(tiles));
            swap(tiles, itmp, jtmp, itmp, jtmp - 1);
        }
        // Right
        if(jtmp + 1 <= N - 1){
            swap(tiles, itmp, jtmp, itmp,jtmp + 1);
            nbrs.enqueue(new Board(tiles));
            swap(tiles, itmp, jtmp, itmp,jtmp + 1);
        }

        // Return queue
        return nbrs;
    }

    // string representation of this board (in the output format specified below)
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(N + "\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                s.append(String.format("%2d ", tiles[i][j]));
            }
            s.append("\n");
        }
        return s.toString();
    }

    // unit tests (DO NOT MODIFY)
    public static void main(String[] args) {
        // read the input file
        Scanner in;
        try {
            in = new Scanner(new File(args[0]), "UTF-8");
        } catch (Exception e) {
            System.out.println("invalid or no input file ");
            return;
        }

        // create initial board from file
        int N = in.nextInt();
        int[][] blocks = new int[N][N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                blocks[i][j] = (int)in.nextInt();
                if (blocks[i][j] >= N*N)
                    throw new IllegalArgumentException("value must be < N^2");
                if (blocks[i][j] < 0)
                    throw new IllegalArgumentException("value must be >= 0");
            }
        }

        Board initial = new Board(blocks);

        System.out.println("\n=== Initial board ===");
        System.out.println(" - manhattan = " + initial.manhattan());
        System.out.println(initial.toString());

        Board twin = initial.twin();

        System.out.println("\n=== Twin board ===");
        System.out.println(" - manhattan = " + twin.manhattan());
        System.out.println(" - equals = " + initial.equals(twin));
        System.out.println(twin.toString());

        System.out.println("\n=== Neighbors ===");
        for (Board board : initial.neighbors())
            System.out.println(board);
    }
}

