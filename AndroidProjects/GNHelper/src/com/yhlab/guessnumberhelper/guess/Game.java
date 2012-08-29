package com.yhlab.guessnumberhelper.guess;

import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Game {

    private int digitCount;
    private Random random;
    private GuessEngine engine;

    /**
     * Constructs a new instance of <code>Game</code>.
     * 
     * @param digitCount
     *            the number of digits
     * @param random
     *            the random number variable used to shuffle the digits.
     * @param chooser
     *            the guess chooser adopted
     */
    public Game(int digitCount, int symbolCount, Random random,
            IGuessChooser chooser) {
        if (digitCount <= 0 || digitCount > 8)
            throw new IllegalArgumentException();
        if (random == null)
            throw new NullPointerException();
        this.digitCount = digitCount;
        this.random = random;
        this.engine = new GuessEngine(digitCount, symbolCount, chooser);
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
    
    public boolean setGuessLabel(int guess, int label) {
        return engine.setLabel(label);
    }
}
