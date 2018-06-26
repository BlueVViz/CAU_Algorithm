/**
 * Homework Assignment #6: "8-Puzzle"
 *
 *  - Solver class for solving "8-Puzzle" Programming Assignment
 *
 *  Compilation:  javac Solver.java Board.java
 *  Execution:    java Solver inputfile.txt
 *  Dependencies: MinPQ
 *
 * @ Student ID : 20145337
 * @ Name       : Junhyuck Woo
 **/

import java.io.File;
import java.util.Scanner;

public class Solver {

    private boolean solved;
    private Node solution;
    /*********************************
     * YOU CAN ADD MORE HERE
     *********************************/
    private Node proof;

    // search node
    private class Node implements Comparable<Node> {
        private Board board;
        private int moves;
        private Node prev;

        public Node(Board board, int moves, Node prev) {
            if (board == null)
                throw new java.lang.NullPointerException();
            this.board = board;
            this.moves = moves;
            this.prev = prev;
        }

        // calculate distance of this search node
        public int distance() {

        /*********************************
         * PUT YOUR CODE HERE
         *********************************/

            return board.manhattan();   // TODO
        }

        // calculate priority of this search node
        public int priority() {

        /*********************************
         * PUT YOUR CODE HERE
         *********************************/
     
            return distance() + moves;   // TODO
        }

        // compare node by priority (implements Comparable<Node>)
        public int compareTo(Node that) {

        /*********************************
         * PUT YOUR CODE HERE
         *********************************/
            // Input node is null, fail to get or wrong input
            if(that == null)
                throw new java.lang.NullPointerException("Node is empty, can't compare");
            // case 1: input priority is big
            if(this.priority() < that.priority()){
                return -1;
            }
            // case 2: input priority is small
            if(this.priority() > that.priority()){
                return +1;
            }

            // case 3: same priority
            return 0;   // TODO
        }
    }

    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        if (initial == null)
            throw new java.lang.NullPointerException();

        solved = false;
        solution = null;

        // create initial search node (and it's twin)
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        Board board = initial;
        Board twinBoard = initial.twin();
        solution = new Node(board, 0, null);
        proof = new Node(twinBoard, 0, null);

        // priority queue
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        MinPQ<Node> minPQ = new MinPQ<Node>();
        MinPQ<Node> minPQtwin = new MinPQ<Node>();


        // insert the initial search node into a priority queue
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        minPQ.insert(solution);
        minPQtwin.insert(proof);

        // solve the puzzle
    /*********************************
     * PUT YOUR CODE HERE
     *********************************/

        while(true){
            // Get Node from queue
            solution = minPQ.delMin();      // Solution node
            proof = minPQtwin.delMin();     // Twin Node
            board = solution.board;         // Get board from node - solution
            twinBoard = proof.board;        // Get board from node - proof

            // Twin node
            // distance = 0, manhattan = 0, => equal with goal
            // Or check whether it is goal
            if(proof.distance() == 0 || twinBoard.isGoal()){
                solved = false;
                break;
            }

            // Solution Node
            // Same purpose
            if(solution.distance() == 0 || board.isGoal()){
                solved = true;
                break;
            }

            // Move one step
            solution.moves++;
            proof.moves++;

            // Create neighbor nodes - solution
            Iterable<Board> navers = board.neighbors();
            for(Board naver : navers){
                // Check with previous board
                if(solution.prev != null && naver.equals(solution.prev.board)){
                    continue;
                }
                // Create Node
                Node newNode = new Node(naver, solution.moves, solution);
                // Insert node in queue
                minPQ.insert(newNode);
            }
            // Create neighbor nodes - solution
            Iterable<Board> twinNavers = twinBoard.neighbors();
            for(Board twinNaver : twinNavers){
                // Check with previous board
                if(proof.prev != null && twinNaver.equals(proof.prev.board)){
                    continue;
                }
                // Create Node
                Node newNode = new Node(twinNaver, proof.moves, proof);
                // Insert node in queue
                minPQtwin.insert(newNode);
            }
        }
        // Terminate
        return;
    }

    // is the initial board solvable?
    public boolean isSolvable() {

    /*********************************
     * PUT YOUR CODE HERE
     *********************************/
     
        return solved;   // TODO
    }
    // min number of moves to solve initial board; -1 if unsolvable
    public int moves() {

     /*********************************
     * PUT YOUR CODE HERE
     *********************************/
        if(isSolvable()){
            return solution.moves;
        }
        else {
            return -1;  // TODO*/
        }
    }

    // sequence of boards in a intest solution; null if unsolvable
    public Iterable<Board> solution() {
        if (solution == null)
            return null;
        Stack<Board> sol = new Stack<Board>();
        Node searchNode = solution;
        while (searchNode != null) {
            sol.push(searchNode.board);
            searchNode = searchNode.prev;
        }
        return sol;
    }

    // solve a slider puzzle (given below)
    public static void main(String[] args) {

        // to calculate running time
        long start = System.currentTimeMillis();
        double time;

        // read the input file
        Scanner in;
        String filename = args[0];
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
                blocks[i][j] = in.nextInt();
                if (blocks[i][j] >= N*N)
                    throw new IllegalArgumentException("value must be < N^2");
                if (blocks[i][j] < 0)
                    throw new IllegalArgumentException("value must be >= 0");
            }
        }

        // initial board
        Board initial = new Board(blocks);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            System.out.println("No solution possible");
        else {
            System.out.println("Minimum number of moves = " + solver.moves() + "\n");
            for (Board board : solver.solution())
                System.out.println(board);
        }

        // calculate running time
        time = (System.currentTimeMillis() - start) / 1000.0;
        System.out.println("time = "+ time + "sec");
        System.out.println("Minimum number of moves = " + solver.moves() + "\n");
    }
}

