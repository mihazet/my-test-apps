package com.yhlab.guessnumberhelper.guess;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * <p>
 * This java application play the guess game with users. It will guess the
 * passcode in the mind of the user. This game is also named as "MasterMind" in
 * other countries.
 * </p>
 * 
 * <p>
 * Here is the rule of the game. At the beginning, user chooses one 4-digit
 * passcode in his mind. This program will then try to probe the passcode and
 * finally get the correct one. It will shoot several guesses. For each guess,
 * the user responses the relation ship between the guess and the correct
 * passcode. Here we define it as the labels of the guess. These labels are the
 * clues of the passcode. Based on these clues, this application is able to
 * guess the passcode.
 * </p>
 * 
 * <p>
 * As mentioned above, the label is the relationship between the digits of the
 * passcode and those of the guess number. If a digit appears in both of the
 * passcode and the guess number and is in the same position of the two numbers,
 * then we say it's an "A". If the digit appears in both numbers but is in
 * different positions, then we say it's a "B" The label is the information of
 * how many "A"'s and "B"'s the guess number has. In this application, user can
 * simply type 01 to indicates 0A1B, alternatively, he can types "0A1B" or
 * "0a1b" for the same information.
 * </p>
 * 
 * <p>
 * Many implementations of the <code>IGuessChooser</code> will choose the same
 * number for the first guess. This could be a weakness of this application.
 * Thus, in this application, the digits is shuffled at the beginning of the
 * game. Which means we use a one-to-one and on-to function to map the original
 * digits. By doing so, the users is hard to see the rule of the guessing
 * sequence.
 * </p>
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class ConsoleGame {
    private static final Pattern pattern = Pattern
            .compile("\\s*(\\d)[aA]?\\s*(\\d)[bB]?\\s*");

    private Random random;
    private int digits[];
    private int rotate[];
    private GuessEngine engine;

    /**
     * Constructs a new instance of <code>ConsoleGame</code>.
     * 
     * @param digitCount
     *            the number of digits
     * @param random
     *            the random number variable used to shuffle the digits.
     * @param chooser
     *            the guess chooser adopted
     */
    public ConsoleGame(int digitCount, int symbolCount,
            Random random, IGuessChooser chooser) {
        if (digitCount <= 0 || digitCount > 8)
            throw new IllegalArgumentException();
        if (random == null)
            throw new NullPointerException();
        this.random = random;
        this.engine = new GuessEngine(
                digitCount, symbolCount, chooser);

        this.rotate = new int[digitCount];
        for (int i = 0; i < digitCount; ++i)
            rotate[i] = i;

        this.digits = new int[symbolCount];
        for (int i = 0; i < symbolCount; ++i)
            digits[i] = i;
    }

    /**
     * Plays the game.
     * 
     * @param is
     *            the input stream where the users responses come from
     * @param os
     *            the output stream where to output the guess numbers
     * @throws IOException
     *             if I/O error
     */
    public void play(InputStream is, PrintStream os)
            throws IOException {
        Utility.randomize(digits, random);
        Utility.randomize(rotate, random);
        int target = rotate.length << 4;
        engine.reset();
        BufferedReader reader =
                new BufferedReader(new InputStreamReader(is));
        while (true) {
            os.print("guess: " +
                    toString(engine.nextGuess()));
            os.print(" ?A?B: ");
            int label = parseLabel(reader.readLine());
            while (label < 0) {
                os.println("Error: invalid input.");
                os.print("?A?B: ");
                label = parseLabel(reader.readLine());
            }
            if (label == target)
                break;
            if (engine.setLabel(label))
                continue;
            os.println("Error: no such number!!");
            break;
        }
    }

    private int parseLabel(String line) {
        Matcher matcher = pattern.matcher(line);
        if (!matcher.matches())
            return -1;
        int a = matcher.group(1).charAt(0) - '0';
        int b = matcher.group(2).charAt(0) - '0';
        int d = rotate.length;
        if (a > d || b > d ||
                a + b > d || (a == d - 1 && b == 1))
            return -1;
        return (a << 4) | b;
    }

    private String toString(int number) {
        int digitCount = rotate.length;
        char buffer[] = new char[digitCount];
        for (int i = digitCount - 1; i >= 0; --i) {
            int x = digits[number & 0x0F];
            buffer[rotate[i]] =
                    (char) (x < 10 ? '0' + x : 'A' + (x - 10));
            number >>= 4;
        }
        return new String(buffer);
    }

    /**
     * The entry point of the java application.
     * 
     * @param argv
     *            the argument vector
     * @throws IOException
     *             for I/O errors
     */
    static public void main(String argv[]) throws IOException {
        String name, args[];
        if (argv.length == 0) {
            name = "Basic";
            args = new String[0];
        } else {
            name = argv[0];
            args = new String[argv.length - 1];
            System.arraycopy(argv, 1, args, 0, args.length);
        }
        ChooserFactory factory = ChooserFactory.getInstance();
        IGuessChooser chooser = factory.buildChooser(name, args);
        if (chooser instanceof ScoredChooser) {
            ((ScoredChooser) chooser).setTimeout(5000);
        }
        final int digitCount = 4;
        final int symbolCount = 10;
        long seed = System.currentTimeMillis();
        System.out.println(
                "start game with random seed as " + seed);
        System.out.println("using chooser: " + chooser);
        ConsoleGame game = new ConsoleGame(digitCount,
                symbolCount, new Random(seed), chooser);
        game.play(System.in, System.out);
    }
}
