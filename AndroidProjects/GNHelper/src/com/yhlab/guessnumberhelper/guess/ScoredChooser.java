package com.yhlab.guessnumberhelper.guess;

import java.util.Random;

/**
 * This class provides an abstract level for the interface
 * <code>GuessChooser</code>, so that the descendent of this class can simply
 * provide the function <code>score(int)</code> to score the possible guesses.
 * The guess with the highest score will be choosed.
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public abstract class ScoredChooser implements GuessChooser {

    /**
     * The guess tree node which represents the current state of the game.
     */
    protected GuessTreeNode node;

    /**
     * The random number used in this scored chooser
     */
    protected Random random;

    private long timeUp;
    private long timeout;

    /**
     * Evaluates the given guess number and gives it a score. This function will
     * be called in <code>nextGuess()</code> for all possible guesses in order
     * to find the best guess. If two or more guess have the same score, one of
     * these guesses will be choosed evenly.
     * 
     * @param guess
     *            the guess to be scored
     * @return the socre of the guess
     */
    abstract public double score(int guess);

    /**
     * Constructs a scored chooser which uses the given random variable to
     * select the guess
     * 
     * @param random
     *            the random variable used to choose the best guess
     */
    protected ScoredChooser(Random random) {
        if (random == null)
            throw new NullPointerException();
        this.random = random;
    }

    public String toString() {
        String name = getClass().getName();
        int index = name.lastIndexOf('.');
        return index < 0 ? name : name.substring(index + 1);
    }

    /**
     * Setups before score the guesses for the given node. This function will be
     * called at the begining of the function <code>nextGuess()</code>. The sub
     * classes can overrides this function to perform some preparations before
     * evaluate the guesses of the same node.
     * 
     * @param node
     *            the state of the game where the guess is evaluated based on
     * @see #nextGuess(GuessTreeNode)
     */
    protected void setup(GuessTreeNode node) {
    }

    /**
     * Cleans up after score the guesses for the given node. This function will
     * be called at the end of the funciton <code>nextGuess()</code>.
     * 
     * @see #nextGuess(GuessTreeNode)
     */
    protected void cleanUp() {
    }

    /**
     * Tests if the scoring has been timeouted.
     * 
     * @return <code>true</code> if it is timeouted; <code>false</code>,
     *         otherwise
     */
    protected final boolean isTimeUp() {
        return timeUp > 0 && System.currentTimeMillis() > timeUp;
    }

    /**
     * Chooses the next guesses by their scores. In this class, it generates all
     * the possible guesses and then score these guesses by performing the
     * function <code>score(int)</code>. Finnaly, it will choose one of the best
     * scored. If two or more guesses have the same score, one of them will be
     * choosed ramdomly.
     * 
     * @return the next guess
     * @see #score(int )
     */
    public int nextGuess(GuessTreeNode node) {
        this.node = node;
        this.timeUp = timeout == 0 ? 0 :
                System.currentTimeMillis() + timeout;
        setup(node);
        int guess[] = generateGuess(node);
        Utility.randomize(guess, random);
        double maxScore = Double.NEGATIVE_INFINITY;
        int nextGuess = guess[0];
        if (guess.length < 2)
            return nextGuess;
        for (int i = 0, n = guess.length; i < n; ++i) {
            double score = score(guess[i]);
            if (score > maxScore) {
                maxScore = score;
                nextGuess = guess[i];
            }
            if (isTimeUp())
                break;
        }
        cleanUp();
        this.node = null;
        return nextGuess;
    }

    private int[] generateGuess(GuessTreeNode node) {
        GuessGenerator gen = new GuessGenerator(node);
        return gen.generate();
    }

    /**
     * Sets the computation timeout when choosing the best guess. If the
     * calculation is timeouted, the current best guess will be returned in
     * function <code>nextGuess()</code>
     * 
     * @param timeout
     *            the computation timeout in milliseconds.
     */
    public void setTimeout(long timeout) {
        if (timeout < 0)
            throw new IllegalArgumentException();
        this.timeout = timeout;
    }

}
