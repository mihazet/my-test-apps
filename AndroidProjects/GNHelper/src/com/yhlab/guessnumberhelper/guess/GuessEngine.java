package com.yhlab.guessnumberhelper.guess;

/**
 * <p>
 * This class provides a high level interface to play the guess game. Users may
 * use this class as shown below. <blockquote>
 * 
 * <pre>
 * int digitCount = 4, target = (digitCount &lt;&lt; 4);// 4A0B
 * GuessEngine engine = new GuessEngine(digitCount, chooser);
 * while (true) {
 *     int guess = engine.nextGuess();
 *     showDigits(guess, digitCount);
 *     int label = getLabel();
 *     if (label == target)
 *         break;
 *     if (!engine.setLabel(label))
 *         System.out.println(&quot;no such number!&quot;);
 * }
 * </pre>
 * 
 * </blockquote>
 * </p>
 * 
 * <p>
 * The purpose of this class is just provide a friendly interface for clients to
 * use. The main AI part is provided by the instance of
 * <code>GuessChooser</code>.
 * </p>
 * 
 * @see GuessChooser
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class GuessEngine {

    private static final int STATE_INIT = 0;
    private static final int STATE_GUESS = 1;
    private static final int STATE_INPUT = 2;

    private int state = STATE_INIT;

    private int digitCount;
    private int symbolCount;
    private int nextGuess;
    private int guessedMask;

    private GuessTreeNode node;
    private IGuessChooser chooser;

    /**
     * Constructs a new instance of <code>GuessEngine</code>.
     * 
     * @param digitCount
     *            the number of digits in the game
     * @param chooser
     *            the guess chooser used
     */
    public GuessEngine(int digitCount,
            int symbolCount, IGuessChooser chooser) {
        if (digitCount <= 0 || digitCount > 8)
            throw new IllegalArgumentException();
        if (symbolCount < digitCount || symbolCount >= 0x10)
            throw new IllegalArgumentException();
        if (chooser == null)
            throw new NullPointerException();
        this.digitCount = digitCount;
        this.symbolCount = symbolCount;
        this.chooser = chooser;
        this.reset();
    }

    /**
     * Reset the engine in order to restart a new game.
     */
    public void reset() {
        this.state = STATE_GUESS;
        this.node = new GuessTreeNode(digitCount, symbolCount);
        this.guessedMask = 0x00;
    }

    /**
     * Gets the next guess number. The number is formated in HEX based. For
     * example, the value 0x1234 means to guess "1234". This function can only
     * be called in the following three conditions:
     * <ol>
     * <li>after the instance has been constructed
     * <li>after the function <code>reset()</code>
     * <li>after the function <code>setLabel(int)</code>.
     * <ol>
     * In other cases, an <code> IllegalStateException </code> will be thrown.
     * 
     * @return the next number to guess
     * @throws IllegalStateException
     *             if this function is not called after <code>reset()</code> or
     *             <code>setLabel( int )</code>.
     */
    public int nextGuess() {
        if (state != STATE_GUESS)
            throw new IllegalStateException();
        this.nextGuess = chooser.nextGuess(node);
        state = STATE_INPUT;
        int tmp = nextGuess;
        for (int i = 0; i < digitCount; ++i, tmp >>= 4)
            guessedMask |= (0x01 << (tmp & 0x0F));
        return nextGuess;
    }

    /**
     * Sets the <code>IGuessChooser</code> used in this engine.
     * 
     * @param chooser
     *            the chooser to be used
     */
    public void setChooser(IGuessChooser chooser) {
        if (chooser == null)
            throw new NullPointerException();
        this.chooser = chooser;
    }

    /**
     * <p>
     * Sets the label of the previous guess. This function will return
     * <code>false</code> to indicate no candidate numbers match the given label
     * or the previous guess is the correct answer. In such a case, the state is
     * not changed, i.e., one have to input another valid label or reset the
     * game. In other cases, this function will return <code>true</code> and one
     * can get the next guess nubmer by <code>nextGuess()
     * </code> again.
     * </p>
     * 
     * <p>
     * Please reference <code>Game</code> for more details about the description
     * about the label.
     * </p>
     * 
     * @param label
     *            the label of the guess
     * @return false if no candidate numbers match the label; otherwise, true.
     * @see Game
     */
    public boolean setLabel(int label) {
        if (state != STATE_INPUT)
            throw new IllegalStateException();
        node.split(nextGuess);
        GuessTreeNode next = node.findChild(label);
        if (next == null)
            return false;
        state = STATE_GUESS;
        this.node = next;
        return true;
    }

}
