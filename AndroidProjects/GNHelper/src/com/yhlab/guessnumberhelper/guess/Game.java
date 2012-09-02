package com.yhlab.guessnumberhelper.guess;


public class Game {
    
    public static final int CANDIDATE_ZERO = 0;
    public static final int CANDIDATE_ONE = 1;
    public static final int CANDIDATE_MORE = 2;
    public static final int CANDIDATE_MORE_LUCKY_ONE = 3;

    private GuessEngine engine;

    /**
     * Constructs a new instance of <code>Game</code>.
     * 
     * @param digitCount
     *            the number of digits
     * @param chooser
     *            the guess chooser adopted
     */
    public Game(int digitCount, int symbolCount, IGuessChooser chooser) {
        if (digitCount <= 0 || digitCount > 8)
            throw new IllegalArgumentException();
        engine = new GuessEngine(digitCount, symbolCount, chooser);
    }

    /**
     * Reset the game
     * 
     * @return the first guess number suggested by the chooser
     */
    public int restart() {
        return getNextGuess(true);
    }

    /**
     * 
     * @return the next guess number suggested by the chooser
     */
    public int nextGuess() {
        return getNextGuess(false);
    }

    private int getNextGuess(boolean first) {

        int guess;

        if (first) {
            engine.reset();
        }

        guess = engine.nextGuess();

        return guess;
    }
    
    public int setGuessLabel(int guess, int label) {
        return engine.setLabel(guess, label);
    }
}
