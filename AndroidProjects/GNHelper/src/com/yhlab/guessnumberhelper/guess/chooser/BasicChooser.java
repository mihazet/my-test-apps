package com.yhlab.guessnumberhelper.guess.chooser;

import java.util.Random;
import com.yhlab.guessnumberhelper.guess.ChooserBuilder;
import com.yhlab.guessnumberhelper.guess.ChooserFactory;
import com.yhlab.guessnumberhelper.guess.GuessChooser;
import com.yhlab.guessnumberhelper.guess.GuessTreeNode;

/**
 * <p>
 * A <code>BasicChooser</code> randomly chooses one of the possible candidates
 * as the next guess.
 * </p>
 * 
 * <p>
 * Experiments show that it takes about 5.47 times to get the correct answer in
 * a 4-digit guess game.
 * </p>
 * 
 * @author Cheng-Ru Lin ( owenlin.twn@gmail.com )
 */
public class BasicChooser implements GuessChooser {

    static {
        ChooserFactory factory = ChooserFactory.getInstance();
        factory.registerBuilder("Basic", new Builder());
    }

    private Random random;

    public BasicChooser(Random random) {
        if (random == null)
            throw new NullPointerException();
        this.random = random;
    }

    public int nextGuess(GuessTreeNode node) {
        int index = random.nextInt(node.size());
        return node.getNumber(index);
    }

    public String toString() {
        String name = this.getClass().getName();
        int index = name.lastIndexOf('.');
        return index < 0 ? name : name.substring(index + 1);
    }

    private static class Builder implements ChooserBuilder {

        public GuessChooser buildChooser(String[] argv) {
            long seed = argv.length == 0 ?
                    System.currentTimeMillis() :
                    Long.parseLong(argv[0]);
            Random random = new Random(seed);
            GuessChooser chooser = new BasicChooser(random);
            System.out.println("create "
                    + chooser + " with seed as " + seed);
            return chooser;
        }

    }
}
